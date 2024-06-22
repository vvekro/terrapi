import os

import serial
from influxdb_client import InfluxDBClient, Point
from influxdb_client.client.write_api import SYNCHRONOUS

from dotenv import load_dotenv
load_dotenv()


def send_to_influx(bucket, sensor, value):
    point = Point(sensor).field("value", value)
    write_api.write(bucket=bucket, org="iot6", record=point)


if __name__ == "__main__":
    ser = serial.Serial('/dev/ttyACM0', 9600)
    
    influx_client = InfluxDBClient(url="http://localhost:8086", token=os.getenv("INFLUX_GRAFANA_API_KEY"), org="iot6")
    write_api = influx_client.write_api(write_options=SYNCHRONOUS)

    bucket = "sensor_data"

    while True:
        try:
            serial_data = ser.readline()
            print(serial_data)

            if not serial_data:
                continue

            line = serial_data.decode('utf-8').strip()
        
            # Example line: "Moisture:500,Light:300,Temperature:200"
            parts = line.split(',')
            moisture = int(parts[0].split(':')[1])
            light = float(parts[1].split(':')[1])
            temperature = float(parts[2].split(':')[1])
        
            # Send data to InfluxDB
            send_to_influx('soil_moisture', moisture)
            send_to_influx('light_level', light)
            send_to_influx('ambient_temperature', temperature)
        
            print(f"Moisture: {moisture}, Light: {light}, Temperature: {temperature}")

        except Exception as e:
            print(f"\nERROR:\n {e}")

