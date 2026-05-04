from device_service import DeviceService, DeviceServiceError
from mock_device_service import MockDeviceService
import sys


def print_help():
    print("UART CLI")
    print()
    print("Usage:")
    print("  python3 cli.py <command> [args] [--mock]")
    print()
    print("Commands:")
    print("  help")
    print("  ping")
    print("  stat")
    print("  set_f1 <fan_id> <duty>")
    print("  set_f2 <fan_id> <duty>")
    print("  get_f1 <fan_id>")
    print("  get_f2 <fan_id>")
    print("  get_sensor <sensor_id>")
    print()
    print("Options:")
    print("  --mock")


def require_args(args, count):
    if len(args) != count:
        print("Wrong number of arguments.\n")
        print_help()
        sys.exit(1)


def parse_int(value, name):
    try:
        return int(value)
    except ValueError:
        print(f"{name} must be a number")
        sys.exit(1)


def print_result(result):
    if isinstance(result, bool):
        print("OK" if result else "NACK")
    else:
        print(result)


def main():
    mock_mode = "--mock" in sys.argv
    args = [arg for arg in sys.argv if arg != "--mock"]

    if len(args) < 2:
        print_help()
        return

    command = args[1].lower()

    if command in ("help", "-h", "--help"):
        print_help()
        return

    service = MockDeviceService() if mock_mode else DeviceService()

    try:
        if command == "ping":
            require_args(args, 2)
            print_result(service.ping())

        elif command == "stat":
            require_args(args, 2)
            print_result(service.get_status())

        elif command == "set_f1":
            require_args(args, 4)
            fan_id = parse_int(args[2], "fan_id")
            duty = parse_int(args[3], "duty")
            print_result(service.set_f1(fan_id, duty))

        elif command == "set_f2":
            require_args(args, 4)
            fan_id = parse_int(args[2], "fan_id")
            duty = parse_int(args[3], "duty")
            print_result(service.set_f2(fan_id, duty))

        elif command == "get_f1":
            require_args(args, 3)
            fan_id = parse_int(args[2], "fan_id")
            print_result(service.get_f1(fan_id))

        elif command == "get_f2":
            require_args(args, 3)
            fan_id = parse_int(args[2], "fan_id")
            print_result(service.get_f2(fan_id))

        elif command == "get_sensor":
            require_args(args, 3)
            sensor_id = parse_int(args[2], "sensor_id")
            print_result(service.get_sensor(sensor_id))

        else:
            print(f"Unknown command: {command}\n")
            print_help()

    except (ValueError, DeviceServiceError) as err:
        print("Error:", err)

    finally:
        service.close()


if __name__ == "__main__":
    main()
