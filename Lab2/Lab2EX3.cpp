
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;


// g++ -std=c++14 Lab2EX3.cpp -o Lab2EX3 -lwiringPi

int main(void){
    
    wiringPiSetup();

    /*1. set mode for the three pins, the pin mode of the PWM pin should be PWM_OUTPUT*/
    
	/*2. write HIGH or LOW to the two I/O pins to control the rotation direction*/

    /*3. use function pwmWrite to control the motor speed. Refer to document pwm.pdf for more information.*/
   
    //run the code using sudo ./Lab2EX3

    return 0;
}

