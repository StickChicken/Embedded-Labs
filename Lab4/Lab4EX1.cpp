//use g++ -std=c++11 -o Lab4EX1 Lab4EX1.cpp -lwiringPi

#include <iostream>
#include <unistd.h>
#include <wiringSerial.h>
#include <wiringPi.h>
using namespace std;

void movement(int, int);
int kobuki;

int main(){
	wiringPiSetup();
	kobuki = serialOpen("/dev/kobuki", 115200);

	//The Kobuki accepts data packets at a rate of 20 ms.
	//To continually move, data needs to be sent continuously

	//Due to machine error, the calculated value of the time needed
	//will not be exact, but can give you a rough starting value.

	/*Starting at bottom right corner, travel
	along bottom side. (You can stop Kobuki before running the every next segment.)*/

	/*Rotate the Kobuki 90 degrees clockwise*/

	/*Move along left side*/

	/*Move along semicircle*/

	/*Move along right side*/

	/*Rotate the Kobuki 90 degrees clockwise*/

	//Close the serial stream to the Kobuki
	//Not doing this will result in an unclean disconnect
	//resulting in the need to restart the Kobuki
	//after every run.
	serialClose(kobuki);
	return(0);
}

void movement(int sp, int r){

	//Create the byte stream packet with the following format:
	unsigned char b_0 = ; /*Byte 0: Kobuki Header 0*/
	unsigned char b_1 = ; /*Byte 1: Kobuki Header 1*/
	unsigned char b_2 = ; /*Byte 2: Length of Payload*/
	unsigned char b_3 = ; /*Byte 3: Sub-Payload Header*/
	unsigned char b_4 = ; /*Byte 4: Length of Sub-Payload*/

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


	/*Pause the script so the data send rate is the
	same as the Kobuki data receive rate*/
	
}

