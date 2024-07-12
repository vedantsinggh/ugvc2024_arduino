import serial
import time
import atexit
import serial.tools.list_ports

from flask import Flask

app = Flask(__name__)

baud_rate = 115200

def find_arduino_port(serial_number):
    ports = serial.tools.list_ports.comports()
    for p in ports:
        if p.serial_number == serial_number:
            return p.device
    return None

# Serial Numbers
LF_serial = '343313236353516030E1'
RF_serial = '34331323635351806012'
RB_serial = '3433132363535160C0F0'
LB_serial = '34331323635351208252'

LF_port = find_arduino_port(LF_serial)
RF_port = find_arduino_port(RF_serial)
RB_port = find_arduino_port(RB_serial)
LB_port = find_arduino_port(LB_serial)

if LF_port:
    LF = serial.Serial(LF_port, baud_rate)
else:
    raise Exception("Left front Arduino not found")

if RF_port:
    RF = serial.Serial(RF_port, baud_rate)
else:
    raise Exception("Right front Arduino not found")

if RB_port:
    RB = serial.Serial(RB_port, baud_rate)
else:
    raise Exception("Right back Arduino not found")

if LB_port:
    LB = serial.Serial(LB_port, baud_rate)
else:
    raise Exception("Left back Arduino not found")

time.sleep(2)
speed = 30
direction = "stop"
turn  = 1000

@app.route("/")
def home():
	data = "Welcome to AGNI!"
	return data

@app.route("/start")
def start():
	global direction
	direction = "start"
	LF.write(direction.encode())
	LF.write(b'\n')
	LB.write(direction.encode())
	LB.write(b'\n')
	RF.write(direction.encode())
	RF.write(b'\n')
	RB.write(direction.encode())
	RB.write(b'\n')
	return direction

@app.route("/stop")
def stop():
	global direction
	direction = "stop"
	LF.write(direction.encode())
	LF.write(b'\n')
	LB.write(direction.encode())
	LB.write(b'\n')
	RF.write(direction.encode())
	RF.write(b'\n')
	RB.write(direction.encode())
	RB.write(b'\n')
	return direction

@app.route("/right")
def right():
	global direction
	direction = "right"
	LF.write(direction.encode())
	LF.write(b'\n')
	LB.write(direction.encode())
	LB.write(b'\n')
	RF.write(direction.encode())
	RF.write(b'\n')
	RB.write(direction.encode())
	RB.write(b'\n')
	return direction

@app.route("/left")
def left():
	global direction
	direction = "left"
	LF.write(direction.encode())
	LF.write(b'\n')
	LB.write(direction.encode())
	LB.write(b'\n')
	RF.write(direction.encode())
	RF.write(b'\n')
	RB.write(direction.encode())
	RB.write(b'\n')
	return direction

@app.route("/speed/<id>")
def speed(id):
	global speed
	data = f"Speed changed to {id}"
	speed = (int)(id)
	LF.write(f"speed{id}".encode())
	LF.write(b'\n')
	LB.write(f"speed{id}".encode())
	LB.write(b'\n')
	RF.write(f"speed{id}".encode())
	RF.write(b'\n')
	RB.write(f"speed{id}".encode())
	RB.write(b'\n')
	return data

@app.route("/turn/<id>")
def turn(id):
	global turn
	data = f"Turn changed to {id}"
	turn = (int)(id)
	LF.write(f"turn{id}".encode())
	LF.write(b'\n')
	LB.write(f"turn{id}".encode())
	LB.write(b'\n')
	RF.write(f"turn{id}".encode())
	RF.write(b'\n')
	RB.write(f"turn{id}".encode())
	RB.write(b'\n')
	return data

@app.route("/getspeed")
def getspeed():
	return speed

@app.route("/getdirection")
def getdirection():
	return direction

if __name__ == "__main__":
	app.run(host='0.0.0.0')

def exit_handler():
	LF.close()
	LB.close()
	RF.close()
	RB.close()

atexit.register(exit_handler)
