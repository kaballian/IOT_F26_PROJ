from flask import Flask, request, jsonify, render_template
from device_service import DeviceService, DeviceServiceError
from mock_device_service import MockDeviceService
import sys

MOCK_MODE = "--mock" in sys.argv

app = Flask(__name__)
service = MockDeviceService() if MOCK_MODE else DeviceService()


def error_response(message, status=500):
    return jsonify({"ok": False, "error": str(message)}), status


@app.route("/", methods=["GET"])
def root():
    return jsonify({
        "service": "PIC UART REST API",
        "mode": "mock" if MOCK_MODE else "uart",
        "endpoints": [
            "GET /ping",
            "GET /status",
            "GET /fans/1",
            "GET /fans/2",
            "PUT /fans/1/duty",
            "PUT /fans/2/duty",
            "GET /sensors/<sensor_id>",
        ]
    })


@app.route("/ping", methods=["GET"])
def ping():
    try:
        return jsonify({"ok": service.ping()})
    except DeviceServiceError as err:
        return error_response(err)


@app.route("/status", methods=["GET"])
def status():
    try:
        return jsonify({
            "ok": True,
            "status": service.get_status()
        })
    except DeviceServiceError as err:
        return error_response(err)


@app.route("/fans/<int:fan_id>", methods=["GET"])
def get_fan(fan_id):
    try:
        if fan_id == 1:
            data = service.get_f1(fan_id)
        elif fan_id == 2:
            data = service.get_f2(fan_id)
        else:
            return error_response("fan_id must be 1 or 2", 400)

        return jsonify({
            "ok": True,
            "fan": data
        })

    except (ValueError, DeviceServiceError) as err:
        return error_response(err)


@app.route("/fans/<int:fan_id>/duty", methods=["PUT"])
def set_fan_duty(fan_id):
    try:
        data = request.get_json()

        if data is None or "value" not in data:
            return error_response("JSON body must contain 'value'", 400)

        duty = int(data["value"])

        if fan_id == 1:
            result = service.set_f1(fan_id, duty)
        elif fan_id == 2:
            result = service.set_f2(fan_id, duty)
        else:
            return error_response("fan_id must be 1 or 2", 400)

        return jsonify({
            "ok": result,
            "fan_id": fan_id,
            "duty": duty
        })

    except (ValueError, DeviceServiceError) as err:
        return error_response(err, 400)


@app.route("/sensors/<int:sensor_id>", methods=["GET"])
def get_sensor(sensor_id):
    try:
        return jsonify({
            "ok": True,
            "sensor": service.get_sensor(sensor_id)
        })
    except (ValueError, DeviceServiceError) as err:
        return error_response(err)



@app.route("/dashboard", methods=["GET"])
def dashboard():
	return render_template("index.html")



if __name__ == "__main__":
    try:
        print("Starting REST API")
        print("Mode:", "MOCK" if MOCK_MODE else "UART")
        app.run(host="0.0.0.0", port=5000)

    finally:
        service.close()




