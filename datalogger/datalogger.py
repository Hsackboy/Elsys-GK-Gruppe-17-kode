#denne koden er bare for å gjøre det enkelt å hente data fra seriell, den er hentet fra:
#https://makersportal.com/blog/2018/2/25/python-datalogger-reading-the-serial-output-from-arduino-to-analyze-data-using-pyserial


import serial
import time

# Define serial port and baud rate. Change 'COM3' to your port and set correct baud rate.
ser = serial.Serial('COM6', 115200, timeout=1)  # Adjust 'COM3' for your system
time.sleep(2)  # Wait for the connection to establish

# Open a log file for writing the data
log_file = open("serial_log.txt", "w")  # 'a' means append mode

try:
    while True:
        if ser.in_waiting > 0:  # Check if there is data waiting in the serial buffer
            serial_data = ser.readline().decode('utf-8').strip()  # Read and decode the data
            print(serial_data)  # Print to the console for monitoring
            
            # Log the data into the file with a timestamp
            log_file.write(f"{time.strftime('%Y-%m-%d %H:%M:%S')} ; {serial_data}\n")
            log_file.flush()  # Ensure data is written immediately
            
except KeyboardInterrupt:
    print("Logging stopped by user.")

finally:
    log_file.close()  # Close the file when done
    ser.close()  # Close the serial connection
