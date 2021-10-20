//In this exercise, you will be measuring the analog voltage
//emitted by the potentiometer and displaying that value
//on the terminal screen.

//Because the Raspberry Pi lacks an internal ADC, we will use
//the Adafruit ADS1015 12 Bit ADC.

//The ADS1015 communicates with the Raspberry Pi through
//I2C communication protocol.

//Complete the given function and use it to measure the
//voltage emitted by the potentiometer.

//Reference the lab supplement ads1015.pdf and I2CProtocol.pdf
//for more details on the Adafruit ADS1015 12 Bit converter
//and I2C protocol.

// single-end, 0-6.144V

//Use g++ -std=c++11 -o Lab1EX4 Lab1EX4.cpp -lwiringPi

#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <signal.h>
#include <stdlib.h>
using namespace std;



int adcVal();
int adc;

int main(){
	//Initialize the wiringPi Library.
	wiringPiSetup();

	while(true){
		/*Read value from ADC*/
		double value = adcVal();	

		/*Convert data to milliVolts.*/
		value = ((value) / 2047) * 6.144;			
		/*Print value to screen.*/
		cout << "The Analog Input Voltage Is: " << value << endl;

		/*Slow the script speed down to gain better measurements.*/
		for(int i = 0; i < 250000000; i++){
			continue;
		}
	}
}

//This function is used to read data from ADC1015
int adcVal(){

	uint16_t low, high, value;
	// Refer to the supplemental documents to find the parameters. In this lab, the ADS1015
	// needs to be set in single-end mode, FSR (full-scale range)is 6.144, you can choose 
	// any input pin (A0, A1, A2, A3) you like.
	adc = wiringPiI2CSetup(0x48);
	wiringPiI2CWriteReg16(adc, 0x01, 0xC080);
	usleep(1000);
    uint16_t data = wiringPiI2CReadReg16(adc, 0b00);


    low = (data & 0xFF00) >> 8;
    high = (data & 0x00FF) << 8;
    value = (high | low)>>4;
	return value;
}
