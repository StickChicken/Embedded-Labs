#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

#define IN3 2
#define IN4 3 
#define PWM 26 

// g++ -std=c++14 Lab2EX3.cpp -o Lab2EX3 -lwiringPi

int main(void){
    
    wiringPiSetup();

    /*1. set mode for the three pins, the pin mode of the PWM pin should be PWM_OUTPUT*/

    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(PWM, PWM_OUTPUT);
    
	/*2. write HIGH or LOW to the two I/O pins to control the rotation direction*/
    while(1){
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);

    /*3. use function pwmWrite to control the motor speed. Refer to document pwm.pdf for more information.*/

    /* Range is 0-1024. 0x400 = 1024. 0x264 = 612. Half Speed? */
    pwmWrite(PWM, 0x264);
  		
    delay(500);
    digitalWrite(IN3, 0);
    digitalWrite(IN4, 1);

    pwmWrite(PWM, 0x264); 
    delay(500);
    }
    //run the code using sudo ./Lab2EX3

    return 0;
}
