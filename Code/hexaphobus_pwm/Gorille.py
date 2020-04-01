import serial
import time

# check the connection between Arduino and Raspberry pi by type "ls /dev/tty*"

def setup():
    port = "/dev/ttyACM0"
    global ser
    ser = serial.Serial(port,500000)
    ser.baudrate = 500000
     
def write_to_ard(etat):
    ser.write(etat.encode())

    read_to_ard()

def read_to_ard():
    response = ""
    while response == "":
        ser.flushInput()
        response = ser.readline()
        if len(response)>0:
            response = response.decode().strip()
            print(response)
    
    return response

def main():
    setup()
    while True:
        write_to_ard("Right|")
        read_to_ard()
        time.sleep(0.2)
        read_to_ard()
        time.sleep(0.2)
        read_to_ard()
        time.sleep(0.2)
        read_to_ard()
        time.sleep(0.2)
        read_to_ard()
        time.sleep(0.2)
        read_to_ard()
        time.sleep(0.2)
        #write_to_ard("Forward")
        #read_to_ard()
        #time.sleep(2)
        #read_to_ard()
        #time.sleep(2)
        #read_to_ard()
        #time.sleep(1)
        #write_to_ard("Right")
        #write_to_ard("Forward")
        #time.sleep(5)
        #write_to_ard("Backward")
        #time.sleep(5)
        #write_to_ard("")
        #time.sleep(10)
        #a = ser.readline().decode().strip()  

        


if __name__ == "__main__":
    main()
