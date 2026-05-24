import json
import struct
import threading
import time
from dataclasses import dataclass 
from typing import Any, Dict, Optional, Tuple


import paho.mqtt.client as mqtt
import serial

BROKER_HOST = "localhost"
BROKER_PORT = 1883  #default for MQTT
 
BASE_TOPIC = "iot/pic"  #the root topic 
CMD_TOPIC = f"{BASE_TOPIC}/cmd/#"  #command extension

SERIAL_PORT = "/dev/serial0"
BAUDRATE = 9600
POLL_INTERNAL_S = 2.0  #poll frequency, in seconds
UART_TIMEOUT_S = 1.0  #time before the UART times out in seconds


#PIC UART PROTOCOL : frame format
# SOF 0x5A
# CMD  (check CMD class)
# LEN  (number of payload bytes)
# PAYLOAD (uint8_t)
# ..
#CHKSUM
# END1 0xAA
# END2 0x55   (cheeky microchip easter egg)

SOF = 0x5A
END1 = 0xAA
END2 = 0X55
MAX_PAYLOAD = 255

class CMD:
    PING = 0x01
    STAT = 0x02
    SET_F1 = 0x03
    SET_F2 = 0x04
    GET_F1 = 0x05
    GET_F2 = 0x06
    GET_SENSOR = 0x07
    ACK = 0xF0
    NACK = 0xF1


def checksum(cmd: int, payload: bytes) -> int:
    chk = cmd^len(payload)  #xor the cmd and the length of payload, to check for len corruption
    for b in payload: 
        chk ^= b #xor bytes
    return chk & 0xFF #force to stay within one byte



def build_frame(cmd: int, payload: bytes = b"")->bytes:  #construct frame
    if not 0 <= cmd <=0xFF:
        raise ValueError("CMD must be 0 .. 255")
    if len(payload) > MAX_PAYLOAD:
        raise ValueError("payload too long")
    
    chk = checksum(cmd, payload)

    return bytes([SOF, cmd, len(payload)]) + payload + bytes([chk, END1, END2])



def parse_frame(frame: bytes) ->tuple[int, bytes]:
    minimum_len = 6 # SOF, CMD, LEN, CHK, END1, END2  (there can be a payload of 0, but the len byte will still be there)
    if len(frame) < minimum_len:
        raise ValueError("frame too short")
    if frame[0] != SOF:
        raise ValueError("Invalid SOF")
    if frame[-2:] != bytes([END1,END2]):
        raise ValueError("invalid end sequence")
    
    cmd = frame[1]
    length= frame[2]
    payload = frame[3 : 3 + length]
    rx_chk = frame[3 + length]

    expected_len = minimum_len + length #check if lengths are matching
    if len(frame) != expected_len:
        raise ValueError(f"invalid frame length")
    
    expected_chk = checksum(cmd, payload) #check if checksums are matching
    if rx_chk != expected_chk:
        raise ValueError(f"checksum mismatch")
    

    return cmd, payload


#dataclasses to hold actual measurements
@dataclass
class FanMeasurement:
    fan_id: int
    duty:   int
    rpm:    int

    def as_dict(self) -> Dict[str, Any]:
        return {
            "fan_id" : self.fan_id,
            "duty" : self.duty,
            "rpm" : self.rpm
        }
    
@dataclass
class SensorMeasurement:
    sensor_id:  int
    aqi:        int
    tvoc_ppb:   int
    eco2_ppm:   int

    def as_dict(self) -> Dict[str, Any]:
        return {
            "sensor_id" : self.sensor_id,  
            "aqi" : self.aqi,        
            "tvoc_ppb" : self.tvoc_ppb,   
            "eco2_ppm" : self.eco2_ppm,   
        }
    
    


class PICUartClient:  #class to communicate with PIC subsystem through /dev/serial0
    def __init__(self, port: str, baudrate: int, timeout_s: float = UART_TIMEOUT_S):
        self.timeout_s = timeout_s
        self.lock = threading.Lock() #mutex
        self.ser = serial.Serial(port=port, baudrate=baudrate, timeout=0.05) #serial object

    def close(self) -> None:
        self.ser.close()

    def read_frame(self, timeout_s: Optional[float] = None) -> bytes:
        deadline = time.monotonic() + (timeout_s or self.timeout_s) #set relative deadline
        buf = bytearray() #buffer to hold frame

        while time.monotonic() < deadline:  #check if deadline has not passed yet
            raw = self.ser.read(1)
            if not raw:
                continue
                
            byte = raw[0]

            #resync on SOF
            if not buf:
                if byte == SOF:
                    buf.append(byte)
                continue
                
            buf.append(byte)

            #when SOF, CMD and LEN are available, the frame length is known

            if len(buf) >= 3:
                payload_len = buf[2]
                expected_len = 6 + payload_len
                if len(buf)==expected_len:
                    return bytes(buf)
                if len(buf)>expected_len:
                    raise ValueError("received oversized frame while reading from PIC")
        
        raise TimeoutError("no response from PIC subsystem")

    
    def transaction(self, cmd:int, payload:bytes  = b"") -> Tuple[int, bytes]:
        with self.lock:
            self.ser.reset_input_buffer()
            self.ser.write(build_frame(cmd, payload))
            self.ser.flush()
            return parse_frame(self.read_frame())
    
    def ping(self) -> bool:
        rx_cmd, _ = self.transaction(CMD.PING)
        return rx_cmd == CMD.ACK
    
    def set_fan_duty(self, fan_id: int, duty: int) -> None:
        if not 0<=duty<=100:
            raise ValueError("duty must be between 0 and 100")
        
        #only two fan ids are implemented in controller (only to pins allocated)
        if fan_id == 1:
            cmd = CMD.SET_F1
        elif fan_id == 2:
            cmd = CMD.SET_F2
        else:
            raise ValueError("Fan_id must be 1 or 2")
    
        payload = bytes([duty])
        rx_cmd, rx_payload = self.transaction(cmd, payload)
        if rx_cmd == CMD.NACK:
            detail  = rx_payload.hex(" ") if rx_payload else "no detail"
            raise RuntimeError(f"PIC rejected fan duty cmd: {detail}")
        if rx_cmd != CMD.ACK:
            raise RuntimeError(f"unexpected resposne CMD.")
        
    def get_fan(self, fan_id: int) -> FanMeasurement:
        if fan_id == 1:
            cmd = CMD.GET_F1
        elif fan_id == 2:
            cmd = CMD.GET_F2
        else:
            raise ValueError("fan id must be 1 or 2")
        
        rx_cmd, payload = self.transaction(cmd)
        if rx_cmd == CMD.NACK:
            raise RuntimeError(f"PIC rejected reading fan")
            
        if len(payload):
            raise ValueError(f"fan payload too short: {payload.hex(' ')}")
        
        duty = payload[0] #duty is first byte in payload 
        rpm = struct.unpack_from(">H", payload, 1)[0] #ensure correct endianess
        return FanMeasurement(fan_id=fan_id, duty=duty, rpm=rpm)
    




    def get_sensor(self, sensor_id: int = 1) -> SensorMeasurement:
        rx_cmd, payload = self.transaction(CMD.GET_SENSOR, bytes([sensor_id]))
        
        if rx_cmd == CMD.NACK:
            raise RuntimeError("PIC returned NACK while reading")
        if len(payload) < 5: #sensor payload is 1 + 2 + 2 bits
            raise ValueError(f"sensor payload too short: {payload.hex(' ')}")

        aqi = payload[0]
        tvoc_ppb = struct.unpack_from(">H", payload, 1)[0] #bytes 1 and 2
        eco2_ppm = struct.unpack_from(">H", payload, 3)[0] #bytes 3 and 4
        return SensorMeasurement(sensor_id=sensor_id, aqi=aqi, tvoc_ppb=tvoc_ppb, eco2_ppm=eco2_ppm)
    

    #def get_status(): #not implemented

#main gateway into MQTT
class MQTTgateway:
    def __init__(self, PIC: PICUartClient):
        self.PIC = PIC
        self.client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
        self.client.on_connect = self.on_connect
        self.client.on_message = self.on_message

    def connect(self) -> None:
        self.client.connect(BROKER_HOST, BROKER_PORT, 60)
        self.client.loop_start()

    def close(self) -> None:
        self.client.loop_stop()
        self.client.disconnect()

    def publish_json(self, topic: str, payload: Dict[str, Any], retain: bool = False) -> None:
        payload = Dict(payload)
        payload["timestamp"] = int(time.time())
        message = json.dumps(payload)
        print(f"PUB {topic}: {message}")
        self.client.publish(topic,message,retain=retain)

    def publish_error(self, error: Exception, source: str = "gateway")->None:
        self.publish_json(
            f"{BASE_TOPIC}/error",
            {
                "ok" : False,
                "source" : source,
                "error" : str(error),
            },
        )
    def on_connect(self, client, userdata, flags, reason_code, properties=None) -> None:
        print(f"Connected to MQTT broker with reasoin code: {reason_code}")
        client.subscribe(CMD_TOPIC)
        print(f"subscribed to {CMD_TOPIC}")

    def on_message(self, client, userdata, msg) -> None:
        topic = msg.topic

        try:
            payload = json.loads(msg.payload.decode()) if msg.payload else {}
            print(f"CMD {topic} : {payload}")

            if topic == f"{BASE_TOPIC}/cmd/fans/1/duty":
                self.handle_set_fan_duty(fan_id=1, payload=payload)
            elif topic == f"{BASE_TOPIC}/cmd/fans/2/duty":
                self.handle_set_fan_duty(fan_id=1, payload=payload)
            elif topic == f"{BASE_TOPIC}/CMD/PING":
                ok = self.PIC.ping()
                self.publish_json(f"{BASE_TOPIC}/ack/ping", {"ok" : ok})
            else:
                raise ValueError(f"unknown command topic: {topic}")
            
        except Exception as err:
            print("Command error:", err)
            self.publish_json(
                f"{BASE_TOPIC}/ack/error",{
                    "ok" : False,
                    "topic" : topic,
                    "error" : str(err)
                },
            )

    def handle_set_fan_duty(self, fan_id: int, payload: Dict[str, Any]) -> None:
        duty = int(payload.get("duty", payload.get("value")))
        self.PIC.set_fan_duty(fan_id, duty)

        self.publish_json(
            f"{BASE_TOPIC}/ack/fans/{fan_id}/duty",
            {
                "ok" : True,
                "fan_id" : fan_id,
                "duty" : duty,
            },
        )

    def poll_once(self)->None:
        self.publish_json(f"{BASE_TOPIC}/fans/1", self.PIC.get_fan(1).as_dict(), retain=True)
        self.publish_json(f"{BASE_TOPIC}/fans/2", self.PIC.get_fan(2).as_dict(), retain=True)
        self.publish_json(f"{BASE_TOPIC}/sensors/1", self.PIC.get_sensor(1).as_dict(), retain=True)


    def run_forever(self)->None:
        while True:
            try:
                self.poll_once()
            except Exception as err:
                print("polling error:", err)
                self.publish_error(err, source="poll")

            time.sleep(POLL_INTERNAL_S)



def main():
    PIC = PICUartClient(SERIAL_PORT, BAUDRATE)
    gateway = MQTTgateway(PIC)

    try:
        gateway.connect()
        gateway.run_forever()
    except KeyboardInterrupt:
        print("Stopping MQTTgateway")
    finally:
        gateway.close()
        PIC.close()

if __name__ == "__main__":
    main()

