from uart_proto import UARTClient, CMD


class DeviceServiceError(Exception):
    pass


class DeviceService:
    def __init__(self, port="/dev/serial0", baud=115200):
        self.client = UARTClient(port=port, baud=baud)

    def close(self):
        self.client.close()

    def _validate_byte(self, value: int, name: str):
        if not isinstance(value, int):
            raise ValueError(f"{name} must be an integer")

        if not 0 <= value <= 255:
            raise ValueError(f"{name} must be between 0 and 255")

    def _validate_percent(self, value: int, name: str):
        if not isinstance(value, int):
            raise ValueError(f"{name} must be an integer")

        if not 0 <= value <= 100:
            raise ValueError(f"{name} must be between 0 and 100")

    def _request(self, cmd: int, payload: bytes = b""):
        self.client.send(cmd, payload)
        response = self.client.read()

        if response is None:
            raise DeviceServiceError("No response from device")

        return response

    def _expect_ack(self, cmd: int, payload: bytes = b"") -> bool:
        resp_cmd, _ = self._request(cmd, payload)

        if resp_cmd == CMD.ACK:
            return True

        if resp_cmd == CMD.NACK:
            return False

        raise DeviceServiceError(f"Unexpected response CMD=0x{resp_cmd:02X}")

    def ping(self) -> bool:
        return self._expect_ack(CMD.PING)

    def get_status(self) -> dict:
        resp_cmd, payload = self._request(CMD.STAT)

        if resp_cmd != CMD.STAT:
            raise DeviceServiceError(f"Unexpected response CMD=0x{resp_cmd:02X}")

        if len(payload) < 1:
            raise DeviceServiceError("Invalid status payload")

        return {
            "flags": payload[0],
            "flags_hex": f"0x{payload[0]:02X}",
        }

    def set_f1(self, fan_id: int, duty: int) -> bool:
        self._validate_byte(fan_id, "fan_id")
        self._validate_percent(duty, "duty")

        return self._expect_ack(CMD.SET_F1, bytes([fan_id, duty]))

    def set_f2(self, fan_id: int, duty: int) -> bool:
        self._validate_byte(fan_id, "fan_id")
        self._validate_percent(duty, "duty")

        return self._expect_ack(CMD.SET_F2, bytes([fan_id, duty]))

    def get_f1(self, fan_id: int) -> dict:
        self._validate_byte(fan_id, "fan_id")

        resp_cmd, payload = self._request(CMD.GET_F1, bytes([fan_id]))

        if resp_cmd != CMD.GET_F1:
            raise DeviceServiceError(f"Unexpected response CMD=0x{resp_cmd:02X}")

        if len(payload) < 3:
            raise DeviceServiceError("Invalid FAN1 payload")

        duty = payload[0]
        rpm = (payload[1] << 8) | payload[2]

        return {
            "fan_id": fan_id,
            "duty": duty,
            "rpm": rpm,
        }

    def get_f2(self, fan_id: int) -> dict:
        self._validate_byte(fan_id, "fan_id")

        resp_cmd, payload = self._request(CMD.GET_F2, bytes([fan_id]))

        if resp_cmd != CMD.GET_F2:
            raise DeviceServiceError(f"Unexpected response CMD=0x{resp_cmd:02X}")

        if len(payload) < 3:
            raise DeviceServiceError("Invalid FAN2 payload")

        duty = payload[0]
        rpm = (payload[1] << 8) | payload[2]

        return {
            "fan_id": fan_id,
            "duty": duty,
            "rpm": rpm,
        }

    def get_sensor(self, sensor_id: int) -> dict:
        self._validate_byte(sensor_id, "sensor_id")

        resp_cmd, payload = self._request(CMD.GET_SENSOR, bytes([sensor_id]))

        if resp_cmd != CMD.GET_SENSOR:
            raise DeviceServiceError(f"Unexpected response CMD=0x{resp_cmd:02X}")

        return {
            "sensor_id": sensor_id,
            "raw_payload": payload.hex(" "),
            "payload": list(payload),
        }
