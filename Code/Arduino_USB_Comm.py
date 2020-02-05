import serial
import time

# check the connection between Arduino and Raspberry pi by type "ls /dev/tty*"

def setup():
    print()
# def write_to_ard(etat):
#     ser.write(etat.encode())
#     response = ""
#     while response == "":
#         response = ser.readline()
#         response = response.decode().strip()
#         print("boucle")
#     return response

def main():
    while True:
        port = "/dev/ttyACM0" 
        ser = serial.Serial(port,9600)
        ser.baudrate = 9600
        ser.flushInput()
        a = ser.readline().decode().strip()
        print(a)   

        


if __name__ == "__main__":
    main()
