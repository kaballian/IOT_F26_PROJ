class MockDeviceService:
    def close(self):
        pass

    def ping(self) -> bool:
        return True

    def get_status(self) -> dict:
        return {
            "flags": 0,
            "flags_hex": "0x00",
        }

    def set_f1(self, fan_id: int, duty: int) -> bool:
        return fan_id == 1 and 0 <= duty <= 100

    def set_f2(self, fan_id: int, duty: int) -> bool:
        return fan_id == 2 and 0 <= duty <= 100

    def get_f1(self, fan_id: int) -> dict:
        return {
            "fan_id": fan_id,
            "duty": 50,
            "rpm": 1234,
        }

    def get_f2(self, fan_id: int) -> dict:
        return {
            "fan_id": fan_id,
            "duty": 75,
            "rpm": 980,
        }

    def get_sensor(self, sensor_id: int) -> dict:
        return {
            "sensor_id": sensor_id,
            "raw_payload": "01 2a",
            "payload": [sensor_id, 42],
        }
