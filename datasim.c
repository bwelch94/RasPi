#include <wiringPi.h> 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* currently psuedo-code, so probs won't work for a while... 
Also I'm writing this on my laptop w/o testing on the Pi.
Which means it almost certainly won't work for a while
*/

// global counter - counts total number of recorded events
static volatile int globalCounter ;

int setup(void);

//int make_xy(void); this may come back eventually, but for now its out. 

void interrupt(void);

int main(void){
	int myCount = 0;
	srand(time(0)); //seed random number generator with current time
	setup();
	xpins = xpins; //make this a real thing later
	ypins = ypins;
	wiringPiISR(ackPin, INT_EDGE_FALLING, &interrupt)
	for (;;) {
		for (i; i < npins; i++){
			int xstate, ystate;
			xstate = ystate = rand() % 2; //change this eventually to do patterns etc.
			digitalWrite(xpins[i], xstate);
			digitalWrite(ypins[i], ystate);
		}
		// wait for signal from ACK line
		for (;;){
			if (globalCounter != myCount){
				printf("Event! N. %d", globalCounter);
				myCount = globalCounter;
				delay(100) // delay in milliseconds
				break;
			}
		}
	}
}


void interrupt(void) {++globalCounter;}

int setup(void){
	int npins = 12; //double check this number, should be n_each of x,y pins
	int ackPin = 2; //assuming BCM numbering
	int reqPin = 3; //assuming BCM numbering
	for (i; i < npins; i++){
		pinMode(xpins[i], OUTPUT);
		pinMode(ypins[i], OUTPUT);
	}
	pinMode(ackPin, INPUT);
	pinMode(reqPin, INPUT);
	pullUpDnControl(ackPin, PUD_UP);
	pullUpDnControl(reqPin, PUD_UP);
	return 0;
}