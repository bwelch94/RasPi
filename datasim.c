#include <wiringPi.h> 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>



// global counter - counts total number of recorded events
static volatile int globalCounter;

int setup(void);

char hex2bin(char input[]);

//int make_xy(void); this may come back eventually, but for now its out. 

void interrupt(void);

int main(void){
	double countRate = 400;
	double delayTime;
	delayTime = (1/countRate) * 1000;
	int myCount = 0;
	srand(time(0)); //seed random number generator with current time
	wiringPiSetupGpio(); // set up wiringPi to use BCM pin numbering 
	int npins = 12;
	int xpins[12] = {22,18,9,23,15,25,27,14,10,17,4,24}; //MSB to LSB order
	int ypins[12] = {21,5,26,20,7,19,16,8,13,12,11,6};
	int ackPin = 2; //assuming BCM numbering
	int reqPin = 3; //assuming BCM numbering
	for (int i; i < npins; i++){
		pinMode(xpins[i], OUTPUT);
		pinMode(ypins[i], OUTPUT);
	}
	pinMode(ackPin, INPUT);
	pinMode(reqPin, OUTPUT);
	pullUpDnControl(ackPin, PUD_UP);
	time_t now, later, endtime;
	time_t seconds = 1;
	wiringPiISR(ackPin, INT_EDGE_FALLING, &interrupt);
	for (;;) {
		int xnum, ynum;
		char xhex[16], xbin[16], yhex[16], ybin[16];
		xnum = rand() % 500; // make this a random int in some range...
		ynum = xnum;
		sprintf(xhex, "%x", xnum);
		sprintf(yhex, "%x", ynum);
		printf("xnum %i \n", xnum);
		printf("xhex %s \n", xhex);
		//printf("hex2bin %s \n", hex2bin(xhex));
		long int ix=0;
    		while(xhex[ix]){
    			printf("%li \n",ix);
         		switch(xhex[ix]){
             			case '0': strcat(xbin, "0000"); break;
             			case '1': strcat(xbin, "0001"); break;
             			case '2': strcat(xbin, "0010"); break;
             			case '3': strcat(xbin, "0011"); break;
             			case '4': strcat(xbin, "0100"); break;
             			case '5': strcat(xbin, "0101"); break;
             			case '6': strcat(xbin, "0110"); break;
             			case '7': strcat(xbin, "0111"); break;
             			case '8': strcat(xbin, "1000"); break;
             			case '9': strcat(xbin, "1001"); break;
             			case 'A': strcat(xbin, "1010"); break;
             			case 'B': strcat(xbin, "1011"); break;
             			case 'C': strcat(xbin, "1100"); break;
             			case 'D': strcat(xbin, "1101"); break;
             			case 'E': strcat(xbin, "1110"); break;
             			case 'F': strcat(xbin, "1111"); break;
             			case 'a': strcat(xbin, "1010"); break;
             			case 'b': strcat(xbin, "1011"); break;
             			case 'c': strcat(xbin, "1100"); break;
             			case 'd': strcat(xbin, "1101"); break;
             			case 'e': strcat(xbin, "1110"); break;
             			case 'f': strcat(xbin, "1111"); break;
             			default:  sprintf(xbin, "\nInvalid hexadecimal digit %c ",xhex[ix]); return 0;
         		}
         		i++;
    		}
		printf("x %s \n", xbin);
		/*long int j=0;
		printf("yhex %s \n",yhex);
    		while(yhex[j]){
    			printf("%li \n",j);
         		switch(yhex[j]){
             			case '0': strcat(ybin, "0000"); break;
             			case '1': strcat(ybin, "0001"); break;
             			case '2': strcat(ybin, "0010"); break;
             			case '3': strcat(ybin, "0011"); break;
             			case '4': strcat(ybin, "0100"); break;
             			case '5': strcat(ybin, "0101"); break;
             			case '6': strcat(ybin, "0110"); break;
             			case '7': strcat(ybin, "0111"); break;
             			case '8': strcat(ybin, "1000"); break;
             			case '9': strcat(ybin, "1001"); break;
             			case 'A': strcat(ybin, "1010"); break;
             			case 'B': strcat(ybin, "1011"); break;
             			case 'C': strcat(ybin, "1100"); break;
             			case 'D': strcat(ybin, "1101"); break;
             			case 'E': strcat(ybin, "1110"); break;
             			case 'F': strcat(ybin, "1111"); break;
             			case 'a': strcat(ybin, "1010"); break;
             			case 'b': strcat(ybin, "1011"); break;
             			case 'c': strcat(ybin, "1100"); break;
             			case 'd': strcat(ybin, "1101"); break;
             			case 'e': strcat(ybin, "1110"); break;
             			case 'f': strcat(ybin, "1111"); break;
             			default:  sprintf(ybin, "\nInvalid hexadecimal digit %c ",yhex[j]); return 0;
         		}
         		j++;
    		}*/
		printf("y \n");
		//xstate = atoi(xbin);
		//ystate = atoi(ybin);
		for (int i=0; i < npins; i++){
			printf("start for loop\n");
			int xi = atoi(xbin[i]);
			int yi = atoi(xbin[i]);
			printf("done atoi\n");
			digitalWrite(xpins[i], xi);
			digitalWrite(ypins[i], yi);
			printf("Pin %d State is %d \n", i,xi);
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

/*char hex2bin(char input[]){
//	const char input[] = "..."; // the value to be converted
	char res[13]; // the length of the output string has to be n+1 where n is the number of binary digits to show, in this case 12
	res[12] = '\0';
	int t = 2048; // set this to s^(n-1) where n is the number of binary digits to show, in this case 8
	int v = strtol(input, 0, 16); // convert the hex value to a number

	while(t) // loop till we're done
	{
    	strcat(res, t < v ? "1" : "0");
    	if(t < v)
        	v -= t;
    	t /= 2;
	}
	// res now contains the binary representation of the number
	return res;
}*/
