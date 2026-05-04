import serial
import time 



END1 = 0xAA
END2 = 0x55


class CMD:
	PING		= 0x01
	STAT		= 0x02
	SET_F1		= 0x03
	SET_F2		= 0x04
	GET_F1		= 0x05
	GET_F2		= 0x06
	GET_SENSOR	= 0x07
	ACK		= 0xF0
	NACK		= 0xF1
	




def build_frame(cmd: int, payload: bytes = b"") -> bytes:
	if len(payload) > 255: 
		raise ValueError("payload too long")
	return bytes([cmd, len(payload)]) + payload + bytes([END1,END2])



def parse_frame(data: bytes):
	if len(data) < 4:
		raise ValueError("frame too short")

	
	if data[-2:] != bytes([END1, END2]):
		raise ValueError("invalid frame ending")

	cmd = data[0]
	length = data[1]
	payload = data[2:2+length]
	
	expected_len = 2 + length + 2

	if len(data) != expected_len:
		raise ValueError("fame len mismatch")


	return cmd , payload


def describe_response(cmd: int, payload: bytes) ->str:
	if cmd == CMD.ACK:
		return "ACK"

	if cmd == CMD.NACK:
		return "NACK"

	if cmd == CMD.STAT:
		if len(payload) >= 1:
			return f"STATUS flags=0x{payload[0]:02X}"

		return "STATUS: invalid payload"
		
	if cmd == CMD.GET_F1:
		if len(payload) >= 3:
			duty = payload[0]
			rpm = (payload[1] << 8) | payload[2]
			return f"FAN1 duty={duty}, rpm={rpm}"
		return "FAN1: invalid payload"


	if cmd == CMD.GET_F2:
		if len(payload) >= 3:
			duty = payload[0]
			rpm = (payload[1] << 8) | payload[2]
			return f"FAN2 duty={duty}, rpm={rpm}"
		return "FAN2: invalid payload"

		
	if cmd == CMD.GET_SENSOR:
		return f"SENSOR payload={payload.hex(' ')}"
	
	return f"CMD=0x{cmd:02X}, payload={payload.hex(' ')}"







class UARTClient:
	def __init__(self, port="/dev/serial0", baud=115200, timeout=1.0):
		self.ser = serial.Serial(port, baud, timeout=0.05)
		self.timeout = timeout
		


	def send(self, cmd, payload=b""):
		frame = build_frame(cmd, payload)
		print("TX:", frame.hex(" "))
		self.ser.write(frame)
		return frame





	def read_frame(self):
		#time.sleep(0.1)
		buf = bytearray()
		deadline = time.monotonic() + self.timeout
		
		while time.monotonic() < deadline:
			byte = self.ser.read(1)

			if not byte:
				continue

			buf += byte

			if len(buf) >= 2 and buf[-2] == END1 and buf[-1] == END2:
				return bytes(buf)

		return None

		
	def read(self):
		frame = self.read_frame()
		if frame is None:
			print("RX: <timeout>")
			return None

		
		print("RX:", frame.hex(" "))

		try:
			cmd, payload = parse_frame(frame)
			print("decoded:", describe_response(cmd, payload))
			return cmd, payload

		except ValueError as err:
			print("RX parse error:", err)
			return None


	def close(self):
		self.ser.close()


