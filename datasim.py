import numpy as np
import time
import RPi.GPIO as GPIO

def setup(inpin,outpin,mode=GPIO.BCM):
	'''Setup for RasPi GPIO pins. 
	GPIO.BCM uses chip nmbering (labeled GPIOnn in pinout command output)
	GPIO.BOARD uses physical pin numbers
	Inputs set up with pull-up-down resistors set to pull up. 
	Pull up because handshake reads negative signals
	'''
	GPIO.setmode(mode)
	GPIO.setup(inpin, GPIO.IN)#, pull_up_down = GPIO.PUD_UP)
	GPIO.setup(outpin, GPIO.OUT)

def default_pins():
	##### change these later to be optimal ####
	#input pins: ACK in DSTDC-F manual
	ack = 2 
	#output pins: x/y words, REQ signal in DSTDC-F
	req = [3]
	xpins = [22,18,9,23,15,25,27,14,10,17,4,24] # MSB to LSB order
	ypins = [21,5,26,20,7,19,16,8,13,12,11,6] # MSB to LSB order

	return ack, req, xpins, ypins

def make_xy():
	#random 0 or 1 choice for now
	#maybe try to follow some pattern later?
	choices = [0,1]
	#xstates = np.random.choice(choices,size=12).tolist()
	xval = np.random.randint(low=1,high=4095)
	xbin = '{0:012b}'.format(xval)
	xstates = list(map(int, xbin))
	# for now, use y=x to create easy pattern
	ystates = xstates 
	return xstates,ystates

def main(rate=10.):
	# Flesh out later...
	ack, req, xpins, ypins = default_pins()
	outpins = req + xpins + ypins
	setup(ack, outpins)
	print(req)
	GPIO.output(req, 1)
	# main loop. Maybe change this to only run when a switch is thrown?
	while True:
		xstates, ystates = make_xy()
		print(xpins,xstates)
		GPIO.output(xpins, xstates)
		GPIO.output(ypins, ystates)
		GPIO.output(req, 0)
		#wait for rising edge at end of ACK pulse
		#add pause (sim 100musec) to avoid overdoing it for the computer
		#time.sleep(1./(2*rate))
		GPIO.wait_for_edge(ack, GPIO.RISING, timeout=250) 
		GPIO.output(req, 1)
		time.sleep(1./(2*rate))
		continue
	return	



