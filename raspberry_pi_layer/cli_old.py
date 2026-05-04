from uart_proto import UARTClient, CMD, build_frame, parse_frame, describe_response
from mock_pic import handle_frame
import sys



def print_help():
	print("UART CLI")
	print()
	print("usage:")
	print("python3 cli.py <command>[args]")
	print("commands:")
	print("ping")
	print("stat")
	print("set_f1 <fan_id> <duty>")
	print("set_f2 <fan_id> <duty>")
	print("get_f1 <fan_id>")
	print("get_f2 <fan_id>")
	print("get_sensor")

def require_args(args, min_count):
	if len(args) < min_count:
		print("wrong number of args.\n")
		print_help()
		sys.exit(1)




def parse_byte(value, name):
	try:
		number = int(value)
	except:
		print(f"{name} must be a number")
		sys.exit(1)

	if not 0 <= number <= 255:
		print(f"{name} must be between 0 and 255")
		sys.exit(1)

	return number

def decode_and_print_response(frame: bytes):
	print("RX:", frame.hex(" "))

	try:
		cmd, payload = parse_frame(frame)
		print("decoded:", describe_response(cmd, payload))
	except ValueError as err:
		print("RX parse error:" ,err)



def main():


	#detect mock mode

	mock_mode = "--mock" in sys.argv
	args = [arg for arg in sys.argv if arg != "--mock"]


	if len(args) < 2:
		print_help()
		return
	
	command = sys.argv[1].lower()
	
	if command in ("help", "-h", "--help"):
		print_help()
		return
	

	cmd_id = None
	payload = b""

	

	#parsing
	if command == "ping":
		require_args(args,2)
		cmd_id = CMD.PING


	elif command == "stat":
		require_args(args,2)
		cmd_id = CMD.STAT


	elif command == "set_f1":
		require_args(sys.argv,4)
		fan_id = parse_byte(args[2], "fan_id")
		duty = parse_byte(args[3], "duty")
		cmd_id = CMD.SET_F1
		payload = bytes([fan_id, duty])




	elif command == "set_f2":
		require_args(sys.argv,4)
		fan_id = parse_byte(args[2], "fan_id")
		duty = parse_byte(args[3], "duty")
		cmd_id = CMD.SET_F2
		payload = bytes([fan_id, duty])


	elif command == "get_f1":
		require_args(sys.argv,3)
		fan_id = parse_byte(args[2], "fan_id")
		cmd_id = CMD.GET_F1
		payload = bytes([fan_id])


	elif command == "get_f2":
		require_args(sys.argv,3)
		fan_id = parse_byte(args[2], "fan_id")
		cmd_id = CMD.GET_F2
		payload = bytes([fan_id])




	elif command == "get_sensor":
		require_args(sys.argv,3)
		sensor_id = parse_byte(args[2], "sensor_id")
		cmd_id = CMD.GET_SENSOR
		payload = bytes([sensor_id])

	else:
		print(f"unknown command: {command}\n")
		print_help()
		return



	
	#execution
	if mock_mode:
		frame = build_frame(cmd_id, payload)
		print("TX:", frame.hex(" "))
		response = handle_frame(frame)
		decode_and_print_response(response)

	else:
		client = UARTClient()
		try:
			client.send(cmd_id, payload)
			client.read()
		finally:
			client.close()






if __name__ == "__main__":
	main()
