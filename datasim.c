#include <wiringPi.h> 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>



// global counter - counts total number of recorded events
static volatile int globalCounter;

int setup(void);

//int make_xy(void); this may come back eventually, but for now its out. 

void interrupt(void);

int main(void){
	double countRate = 400;
	double delayTime;
	delayTime = (1/countRate) * 1000;
	int myCount = 0;
	srand(time(0)); //seed random number generator with current time
	setup();
	int npins = 12;
	int xpins[12] = {22,18,9,23,15,25,27,14,10,17,4,24}; //MSB to LSB order
	int ypins[12] = {21,5,26,20,7,19,16,8,13,12,11,6};
	int ackPin = 2; //assuming BCM numbering
	int reqPin = 3; //assuming BCM numbering
	time_t now, later, endtime;
	time_t seconds = 1;
	wiringPiISR(ackPin, INT_EDGE_FALLING, &interrupt);
	for (;;) {
		for (int i=0; i < npins; i++){
			int xstate, ystate;
			xstate = ystate = rand() % 2; //change this eventually to do patterns etc.
			digitalWrite(xpins[i], xstate);
			digitalWrite(ypins[i], ystate);
			//printf("Pin %d State is %d \n", i,xstate);
		}
		digitalWrite(reqPin, LOW); // data available
		// wait for signal from ACK line
		now = time(NULL);
		endtime = now + seconds;
		for (;;){
			if (globalCounter != myCount){
				printf("Event! N. %d \n", globalCounter);
				myCount = globalCounter;
				delay(delayTime); // delay in milliseconds
				break;
			}
			later = time(NULL);
			if (later > endtime){
				printf("Nada... \n");
				break;
			}
		}
		digitalWrite(reqPin, HIGH);
	}
	return(1);
}


void interrupt(void) {++globalCounter;}

int setup(void){
	wiringPiSetupGpio(); // set up wiringPi to use BCM pin numbering
	int npins = 12; //double check this number, should be n_each of x,y pins
	int ackPin = 2; //assuming BCM numbering
	int reqPin = 3; //assuming BCM numbering
	int xpins[12] = {22,18,9,23,15,25,27,14,10,17,4,24}; //MSB to LSB order
	int ypins[12] = {21,5,26,20,7,19,16,8,13,12,11,6};
	for (int i; i < npins; i++){
		pinMode(xpins[i], OUTPUT);
		pinMode(ypins[i], OUTPUT);
	}
	pinMode(ackPin, INPUT);
	pinMode(reqPin, OUTPUT);
	pullUpDnControl(ackPin, PUD_UP);
	return 0;
}