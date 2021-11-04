//use g++ -std=c++11 -o 2 .cpp -lwiringPi


#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <ctime>
#include <ratio>
#include <chrono>
#define sonar_pin 1  //gpio 18

using namespace std;
using namespace std::chrono;


int kobuki;
float read_sonar();
void movement(int, int);
void turn(bool);

int main(){
	wiringPiSetup();
	kobuki = serialOpen("/dev/kobuki", 115200);
	bool turnCount = 0;

	/*Move from a random point within the area designated "X" to the
	point B as shown on the diagram. Use a sonar sensor to navigate through the channel.
	You can reuse your code from Lab 3 and 4*/
	while(1){
		if(read_sonar() < 22.5){
			movement(0, 0);
			if(turnCount) turn(1);
			else turn(0);
			turnCount = !turnCount;
			
		}
		movement(300, 0);
		
	}
	/*Note: the Kobuki must completely pass point B as shown to receive full credit*/
return(0);

}


float read_sonar(){
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




void movement(int sp, int r){
	//Create the byte stream packet with the following format:
	unsigned char b_0 = 0xAA; /*Byte 0: Kobuki Header 0*/
	unsigned char b_1 = 0x55; /*Byte 1: Kobuki Header 1*/
	unsigned char b_2 = 0x06; /*Byte 2: Length of Payload*/
	unsigned char b_3 = 0x01; /*Byte 3: Sub-Payload Header*/
	unsigned char b_4 = 0x04; /*Byte 4: Length of Sub-Payload*/

	unsigned char b_5 = sp & 0xff;	//Byte 5: Payload Data: Speed(mm/s)
	unsigned char b_6 = (sp >> 8) & 0xff; //Byte 6: Payload Data: Speed(mm/s)
	unsigned char b_7 = r & 0xff;	//Byte 7: Payload Data: Radius(mm)
	unsigned char b_8 = (r >> 8) & 0xff;	//Byte 8: Payload Data: Radius(mm)
	unsigned char checksum = 0;		//Byte 9: Checksum
	
	//Checksum all of the data
	char packet[] = {b_0,b_1,b_2,b_3,b_4,b_5,b_6,b_7,b_8};
	for (unsigned int i = 2; i < 9; i++)
		checksum ^= packet[i];

	/*Send the data (Byte 1 - Byte 9) to Kobuki using serialPutchar (kobuki, );*/
	for(unsigned int i = 0; i <= 8; i++){
		serialPutchar(kobuki, packet[i]);
	}
	serialPutchar(kobuki, checksum);
	/*Pause the script so the data send rate is the
	same as the Kobuki data receive rate*/
	delay(20);
}

void turn(bool t){
	if(t){
		for(int i = 0; i < 72; i++){
		movement(181, 1);
		}
		movement(0,0);
	}
	else{
		for(int i = 0; i < 72; i++){
		movement(181, -1);
		}
		movement(0, 0);
	}
	
}
