import xbox_read
import os 
import serial
import time

ser = serial.Serial('/dev/ttyUSB0', 57600)
ser.write('test')


LALOG = 0
RALOG = 0
BOOM = 0
STAB = 0
rate = 331

boolh = True
booll = False
boolg = True
faster = True

for event in xbox_read.event_stream(deadzone=6000):
#    print(ser.read())
    # Speed and direction
    if event.key == 'Y1':
	ldata = 'L'
	LALOG = event.value
        if LALOG >= 0:
	    ldata += '+'
        else:
	    ldata += '-'
	    LALOG *= -1
        LALOG = int(LALOG / rate)
        ldata += str(LALOG)
        ser.write(ldata + '\n')
	print(ldata)

    if event.key == 'Y2':
	rdata = 'R'
	RALOG = event.value
        if RALOG >= 0:
	    rdata += '+'
        else:
             rdata += '-'
	     RALOG *= -1
	RALOG = int(RALOG / rate)
	rdata += str(RALOG)
	ser.write(rdata + '\n')
	print(rdata)
   
    if event.key == "LT":
	adata = 'A' #a for axle
	adata += '-' #lower
	adata += str(int(event.value / 2.57))
	ser.write(adata + '\n')
	print(adata)

    if event.key == "LB":
	adata = 'A'
	adata += '+'
	adata += str(event.value)
	ser.write(adata + '\n')
	print(adata)

    if event.key == "RT":
	bdata = 'B' #b for boom
	bdata += '-' #lower
	bdata += str(int(event.value / 2.57))
	ser.write(bdata + '\n')
	print(bdata)

    if event.key == "RB":
	bdata = 'B'
	bdata += '+'
	bdata += str(event.value)
	ser.write(bdata + '\n')
	print(bdata)
	
       
"""   
    if event.key=='X' and event.is_press():
	pwm.setPWM(11, 0, 1100)
    if event.key=='Y' and event.is_press():
        pwm.setPWM(11, 0, 300) 
    if event.key=='RT':
	rt_intensity = event.value
	pwm.setPWM(0, 0, (servoMid + (servoWidth * event.value / 255)))
    else: pwm.setPWM(0, 0, servoMid)		
    if event.key=='B' and event.is_press():
	servoMid = steer
    if rt_intensity < 50:
	pwm.setPWM(0, 0, servoMid)
    # Speeds    
    if event.key == 'du' and booll == True:
	booll = False
	boolh = True
	print "High"
	os.system('mpg321 High.mp3 &')
	rate = 8.01
    if event.key == 'dd' and boolh == True:
	boolh = False
	booll = True
	print "Low"
	os.system('mpg321 Low.mp3 &')
	rate = 12    

    # Systems check
    if event.key == 'guide' and boolg == True:
	boolg = False
	os.system('mpg321 Startup.mp3 &')
	sleep(2.5)
	backwardL()
	backwardR()
	pwm.setPWM(3, 0, 4000)
	pwm.setPWM(0, 0, 4000)
	sleep(.3)
	pwm.setPWM(3, 0, 0)
	pwm.setPWM(0, 0, 0)
	sleep(.3) 
	forwardL()
	backwardR()
	pwm.setPWM(3, 0, 4000)
	pwm.setPWM(0, 0, 4000)
	sleep(.3)
	forwardR()
	backwardL()
	pwm.setPWM(3, 0, 4000)
	pwm.setPWM(0, 0, 4000)
	sleep(.7)
	forwardL()
	backwardR()
	pwm.setPWM(3, 0, 4000)
	pwm.setPWM(0, 0, 4000)
	sleep(.3)
	pwm.setPWM(3, 0, 0)
	pwm.setPWM(0, 0, 0)
	sleep(.3)
	forwardL()
	forwardR()
	pwm.setPWM(3, 0, 4000)
	pwm.setPWM(0, 0, 4000)
	sleep(.3)
	pwm.setPWM(3, 0, 0)
	pwm.setPWM(0, 0, 0)
	sleep(.5)	 
	os.system('mpg321 MotorSystems.mp3 &')
"""
