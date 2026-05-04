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
	return bytes([cmd, len(payload)]) + payload + bytes([END1,END2])


def send_frame(ser, cmd: int, payload: bytes = b""):
	frame = build_frame(cmd, payload)
	print("TX:", frame.hex(" "))
	ser.write(frame)


def read_all(ser):
	time.sleep(0.1)
	data = ser.read(ser.in_waiting or 1)
	if data:
		print("RX:", data.hex(" "))
	else:
		print("RX: <nothing>")


with serial.Serial("/dev/serial0", 115200, timeout=0.5) as ser:
	# set fan 1 to 50% duty
	send_frame(ser, CMD_SET_F1_DUTY, bytes([50]))
	read_available(ser)

	# ask for fan 1 status
	send_frame(ser, CMD_GET_F1)
	read_available(ser)

