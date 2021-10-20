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

/*set your pin numbers and pid values*/
int motor_pin = ;
int sonar_pin = ;
float kp= ; 
float ki= ; 
float kd= ;


// Find real GPIO pin out from RPi
#define sig GPIO18
#define PWM GPIO12
#define SDA GPIO2
#define SCL GPIO3


int main(){
	wiringPiSetup();
    adc = wiringPiI2CSetup(0x48);

    /*Set the pinMode (sonar and fan pins)*/
    pinMode(sig, OUTPUT);
    pinMode(SDA, OUTPUT);
    pinMode(SCL, OUTPUT);
    pinMode(PWM, PWM_OUTPUT);
    // Below is from the SOFT_PWM pdf, is it necessary?
    softPwmCreate(PWM, 0, 100);
    while(1){
        softPwmWrite(PWM, 50);
        cout<<"in while loop"<<endl;
        usleep(100000);
    }
    return 0;

    
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
        cout<<"obj_value: "<<obj_value<<" measured_value: "<<measured_value<<endl;
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
    double objective = 90;

    /*read the measured position/distance of the ball*/
    pinMode(sig, INPUT);
	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	high_resolution_clock::time_point t2;
	int time_span;
	while(digitalRead(sig))
	{
		t2 = high_resolution_clock::now();
		time_span = duration_cast<microseconds>(t2-t1).count();
		if(time_span >= t_inMax) 
        break;			
	}
	double speed = .000340;
	double distance = ((time_span/2)*speed);

    /*calculate the distance error between the obj and measured distance */
    double error = objective - distance;

    /*calculate the proportional, integral and derivative output */
    PID_p = ;
    PID_i = ;
    PID_d = ;
    PID_total = PID_p + PID_d + PID_i; 

    /*assign distance_error to distance_previous_error*/


    /*use PID_total to control your fan: pwmWrite*/
    
}


/* use a sonar sensor to measure the position of the Ping-Pang ball. you may reuse
your code in EX1.*/
float read_sonar()
{

}

/* use a potentiometer to set an objective position (10 - 90 cm) of the Ping-Pang ball, varying the potentiometer
can change the objective distance. you may reuse your code in Lab 1.*/
float read_potentionmeter()
{

}

