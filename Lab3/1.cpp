//Use g++ -std=c++11 -o Lab3EX1 Lab3EX1.cpp -lwiringPi

#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <wiringPi.h>
#include <ctime>
#include <ratio>
#include <chrono>
using namespace std;
using namespace std::chrono;

// GPIO 12 is in use. Get pin number from RPi
#define sig 1

int main(){
	//Set up wiringPi
	wiringPiSetup();

	
	// Times are all in us. 18500 us = 18.5 ms
	long t_LH = 2;
	long t_HL = 5;
	long t_holdoff = 750;
	long t_burst = 200;
	long t_inMin = 115;
	long t_inMax = 18500;
	
	while(true){
		/*Set the pinMode to output and generate a LOW-HIGH-LOW signal to trigger the sensor. 
		Use a 2 us delay between a LOW-HIGH and then a 5 us delay between HIGH-LOW*/
		pinMode(sig, OUTPUT);
		digitalWrite(sig, LOW);
		delayMicroseconds(t_LH);
		digitalWrite(sig, HIGH);
		delayMicroseconds(t_HL);
		digitalWrite(sig, LOW);

		/*Echo holdoff delay 750 us*/
		delayMicroseconds(t_holdoff);

		/*Switch the pinMode to input and read the echo return pulse width*/
		pinMode(sig, INPUT);
		high_resolution_clock::time_point t1 = high_resolution_clock::now();
		high_resolution_clock::time_point t2;
		int time_span;
		while(digitalRead(sig))
		{
				t2 = high_resolution_clock::now();
				time_span = duration_cast<microseconds>(t2-t1).count();
				if(time_span >= t_inMax) break;
					
		}

		/*Calculate the distance.*/ //Speed of sound is 340m/s
		
		double speed = .0340;
		
		double distance = ((time_span/2)*speed);
		
			// 340 m/s -> 340000 m/ms -> 340000000 m/us

		/*Print the distance.*/
		cout << "Distance = " << distance << " centimeters." << endl;

		/*Delay before next measurement. The actual delay may be a little longer than what is shown is the datasheet.*/
		delayMicroseconds(200);
		delay(50);
	}
}
