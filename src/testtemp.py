import os
import time
from influxdb_client import InfluxDBClient, Point, WritePrecision
from influxdb_client.client.write_api import SYNCHRONOUS
from dotenv import load_dotenv

load_dotenv()
token = os.getenv("INFLUX_API_KEY")
org = "iot6"
bucket = "sensor_data"
url = "http://localhost:8086"  # Change this if your InfluxDB server is hosted elsewhere

client = InfluxDBClient(url=url, token=token)
write_api = client.write_api(write_options=SYNCHRONOUS)

def get_cpu_temperature():
    try:
        # For Raspberry Pi, the temperature is found in this file
        with open("/sys/class/thermal/thermal_zone0/temp") as f:
            temp_str = f.read().strip()
        # Convert the temperature to Celsius
        temp_c = int(temp_str) / 1000.0
        return temp_c
    except FileNotFoundError:
        raise RuntimeError("Could not read temperature file. Are you sure this is a Raspberry Pi?")

def write_temperature_to_influxdb(temp):
    point = Point("cpu_temperature").tag("host", "raspberrypi").field("temperature", temp).time(time.time_ns(), WritePrecision.NS)
    write_api.write(bucket=bucket, org=org, record=point)

def main():
    while True:
        temp = get_cpu_temperature()
        print(f"CPU Temperature: {temp} °C")
        write_temperature_to_influxdb(temp)
        time.sleep(1)  # Wait for 60 seconds before the next reading

if __name__ == "__main__":
    main()

