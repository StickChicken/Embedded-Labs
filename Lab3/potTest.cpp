//Use g++ -std=c++11 -o Lab3EX2 Lab3EX2.cpp -lwiringPi

#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <chrono>
#include <cmath>


using namespace std::chrono;
using namespace std;

// functions
void sigroutine(int);
int adcVal();
void PID(float, float, float);
float read_potentionmeter();
float read_sonar();


// variables
float distance_previous_error, distance_error;
float obj_value, measured_value; // potentionmeter reading, sonar reading
int adc;
float PID_p,  PID_d, PID_total, PID_i = 0;
int time_inter_ms = 23; // time interval, you can use different time interval


int main(){
	wiringPiSetup();
    adc = wiringPiI2CSetup(0x48);
    while(1){
	float a = read_potentionmeter();
	cout << a << "\n";
	delay(500);
    }
   
}






/* use a potentiometer to set an objective position (10 - 90 cm) of the Ping-Pang ball, varying the potentiometer
can change the objective distance. you may reuse your code in Lab 1.*/
float read_potentionmeter(){
    uint16_t low, high, value;
    
    wiringPiI2CWriteReg16(adc, 0x01, 0xC080);
    usleep(1000);
    uint16_t data = wiringPiI2CReadReg16(adc, 0b00);
    
    low = (data & 0xFF00) >> 8;
    high = (data & 0x00FF) << 8;
    value = (high | low) >> 4;
    
    value = (value / 4096) * 100;
    
    return value;
}

