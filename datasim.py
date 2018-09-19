import numpy as np
import RPi.GPIO GPIO

def setup(inpin,outpin,mode=GPIO.BCM):
	'''Setup for RasPi GPIO pins. 
	GPIO.BCM uses chip nmbering (labeled GPIOnn in pinout command output)
	GPIO.BOARD uses physical pin numbers
	Inputs set up with pull-up-down resistors set to pull up. 
	Pull up because handshake reads negative signals
	'''
	GPIO.setmode(mode)
	GPIO.setup(inpin, GPIO.IN, pull_up_down = GPIO.PUD_UP)
	GPIO.setup(outpin, GPIO.OUT)

def default_pins():
	##### change these later to be optimal ####
	#input pins: ACK in DSTDC-F manual
	ack = 2 
	#output pins: x/y words, REQ signal in DSTDC-F
	req = 3
	xpins = np.arange(4,15).tolist()
	ypins = np.arange(16,27).tolist()

	return ack, req, xpins, ypins

def make_xy():
	#random 0 or 1 choice for now
	#maybe try to follow some pattern later?
	choices = [0,1]
	xstates = np.random.choice(choices,size=12)
	ystates = np.random.choice(choices,size=12)

def main():
	# Flesh out later...
	ack, req, xpins, ypins = default_pins()
	outpins = req + xpins + ypins
	setup(ack, outpins)
	GPIO.output(req, 1)
	# main loop. Maybe change this to only run when a switch is thrown?
	while True:
		xstates, ystates = make_xy()
		GPIO.output(xpins, xstates)
		GPIO.output(ypins, ystates)
		GPIO.output(req, 0)
		#wait for rising edge at end of ACK pulse
		GPIO.wait_for_edge(ack, GPIO.RISING) 
		GPIO.output(req, 1)
		continue