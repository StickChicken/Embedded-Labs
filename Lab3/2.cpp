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
#include <softPwm.h>

#define sonar_pin 1  //gpio 18
#define motor_pin 4 //gpio 12

using namespace std::chrono;
using namespace std;

// functions
void sigroutine(int);
int adcVal();
void PID(float, float, float);
float read_potentionmeter();
float read_sonar();


// variables
float previousError, error;
float objective, distance_p; // potentionmeter reading, sonar reading
int adc;
float PID_p,  PID_d, PID_total, PID_i = 0;
int time_inter_ms = 23; // time interval, you can use different time interval

/*set your pin numbers and pid values*/
float kp= 2; 
float ki= 0.001; 
float kd= 5;




int main(){
	wiringPiSetup();
    adc = wiringPiI2CSetup(0x48);

    /*Set the pinMode (sonar and fan pins)*/
    pinMode(sonar_pin, OUTPUT);
    pinMode(motor_pin, PWM_OUTPUT);

    softPwmCreate(motor_pin,0, 100);
    // This part is to set a system timer, the function "sigroutine" will be triggered  
    // every time_inter_ms milliseconds. 
    struct itimerval value, ovalue;
    signal(SIGALRM, sigroutine);
    value.it_value.tv_sec = 0;
    value.it_value.tv_usec = time_inter_ms*1000;
    value.it_interval.tv_sec = 0;
    value.it_interval.tv_usec = time_inter_ms*1000;
    setitimer(ITIMER_REAL, &value, &ovalue);    


	while(true){
        cout<<"obj_value: "<< objective <<" measured_value: "<< distance_p <<endl;
        cout<<"PID_p: "<<PID_p<<endl;
        cout<<"PID_i: "<<PID_i<<endl;
        cout<<"PID_d: "<<PID_d<<endl;
        cout<<"PID_total: "<<PID_total<<endl;
        delay(20);
	}
}


void sigroutine(int signo){
    PID(kp, ki, kd);
    return;
}


/* based on the obj distance and measured distance, implement a PID control algorithm to 
the speed of the fan so that the Ping-Pang ball can stay in the obj position*/
void PID(float kp, float ki, float kd){
    /*read the objective position/distance of the ball*/
    // 90 cm
    objective = read_potentionmeter();
    static float previousT{};

    /*read the measured position/distance of the ball*/
    distance_p = read_sonar();
    /*calculate the distance error between the obj and measured distance */
    error = distance_p - objective;

    /*calculate the proportional, integral and derivative output */
    float currentT = millis();
    float elapsedT = currentT - previousT;
    PID_p = error;
    PID_i += error * time_inter_ms;
    PID_d = (error - previousError) / time_inter_ms;
    
    
    PID_total = (kp * PID_p + kd * PID_d + ki * PID_i)/10; 

    /*assign distance_error to distance_previous_error*/
    previousError = error;
    previousT = millis();
    if(PID_total<0)
    PID_total= 0;

    /*use PID_total to control your fan: pwmWrite*/
    //pwmWrite(motor_pin, PID_total);
    softPwmWrite(motor_pin, PID_total);
    
}


/* use a sonar sensor to measure the position of the Ping-Pang ball. you may reuse
your code in EX1.*/

float read_sonar()
{
    // Times are all in us. 18500 us = 18.5 ms
        long t_LH = 2;
        long t_HL = 5;
        long t_holdoff = 750;
        long t_burst = 200;
        long t_inMin = 115;
        long t_inMax = 18500;
	
    /*Set the pinMode to output and generate a LOW-HIGH-LOW signal to trigger the sensor. 
    Use a 2 us delay between a LOW-HIGH and then a 5 us delay between HIGH-LOW */
		pinMode(sonar_pin, OUTPUT);
		digitalWrite(sonar_pin, LOW);
		delayMicroseconds(t_LH);
		digitalWrite(sonar_pin, HIGH);
		delayMicroseconds(t_HL);
		digitalWrite(sonar_pin, LOW);

		/*Echo holdoff delay 750 us*/
		delayMicroseconds(t_holdoff);

		/*Switch the pinMode to input and read the echo return pulse width*/
		pinMode(sonar_pin, INPUT);
		high_resolution_clock::time_point t1 = high_resolution_clock::now();
		high_resolution_clock::time_point t2;
		int time_span;
		while(digitalRead(sonar_pin))
		{
				t2 = high_resolution_clock::now();
				time_span = duration_cast<microseconds>(t2-t1).count();
				if(time_span >= t_inMax) break;
					
		}

		/*Calculate the distance.*/ //Speed of sound is 340m/s
		
		double speed = .0340;
		
		double distance_l = ((time_span/2)*speed);
		

		return distance_l;

		/*Delay before next measurement. The actual delay may be a little longer than what is shown is the datasheet.*/
		delayMicroseconds(200);

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
    
    value = (value * 100 / 1420);
    cout << "value " << value << "\n";
    
    return value;
}




