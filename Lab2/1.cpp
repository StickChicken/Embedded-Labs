// g++ -std=c++11 -o Lab2EX1 Lab2EX1.cpp -lwiringPi

#include <wiringPi.h>
#include <softPwm.h>
#include <stdio.h>



/* signal pin of the servo*/
#define servoPin 1 

//Specify a certain rotation angle (0-180) for the servo
void servoWrite(int pin, int angle){ 
    long time = 0;
    time = 10 * (.5 + (angle / 90)); 
    softPwmWrite(pin,time);   
}

int main(void)
{
    wiringPiSetup();    
    softPwmCreate(servoPin,  0, 200);

    while(1){
	
	servoWrite(servoPin, 180);
        /*make servo rotate from minimum angle to maximum, use the function 
        servoWrite(int pin, int angle), increase 1 degree each time. You can use for loop*/
	delay(500);
	
	servoWrite(servoPin, 0);
        /*make servo rotate from maximum angle to minimum, use the function 
        servoWrite(int pin, int angle), increase 1 degree each time*/
        delay(500);
    }
    return 0;
}


