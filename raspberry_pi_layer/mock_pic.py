from uart_proto import build_frame, CMD, END1, END2, parse_frame





def handle_frame(data: bytes) -> bytes:

	try:
		cmd, payload = parse_frame(data)
	except ValueError:
		return build_frame(CMD.NACK)


	



	

	if cmd == CMD.PING:
		return build_frame(CMD.ACK)
	
	if cmd == CMD.STAT:
		fake_status_flags = 0x00
		return build_frame(CMD.STAT, bytes([fake_status_flags]))



	if cmd == CMD.SET_F1:
		if len(payload) != 2:
			return build_frame(CMD.NACK)

		fan_id = payload[0]
		duty = payload[1]
		if fan_id != 1 or duty >100:
			return build_frame(CMD.NACK)
		
		return build_frame(CMD.ACK)
	
	
	if cmd == CMD.SET_F2:
		if len(payload) != 2:
			return build_frame(CMD.NACK)

		fan_id = payload[0]
		duty = payload[1]
		if fan_id != 1 or duty >100:
			return build_frame(CMD.NACK)
		
		return build_frame(CMD.ACK)
	




	if cmd == CMD.GET_F1:
		if len(payload) != 1:
			return build_frame(CMD.NACK)


		fake_duty = 50
		fake_rpm = 1234
		return build_frame(CMD.GET_F1, bytes([fake_duty, (fake_rpm >> 8) & 0xff, fake_rpm & 0xff]))

	
	if cmd == CMD.GET_F2:
		if len(payload) != 1:
			return build_frame(CMD.NACK)

		fake_duty = 75
		fake_rpm = 879
		return build_frame(CMD.GET_F2, bytes([fake_duty,(fake_rpm >> 8) & 0xff, fake_rpm & 0xff]))
	
	
	if cmd == CMD.GET_SENSOR:
		if len(payload) != 1:
			return build_frame(CMD.NACK)
		sensor_id = payload[0]
		fake_sensor_value = 42

		return build_frame(CMD.GET_SENSOR, bytes([fake_sensor_value]))

	return build_frame(CMD.NACK)


