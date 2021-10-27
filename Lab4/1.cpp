//use g++ -std=c++11 -o 1 1.cpp -lwiringPi

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

	 /*
	  The Kobuki accepts data packets at a rate of 20 ms.
	  To continually move, data needs to be sent continuously

	  Due to machine error, the calculated value of the time needed
	  will not be exact, but can give you a rough starting value.
	 */
	 
	 //negative numbers are clockwise

	/*Starting at bottom right corner, travel
	along bottom side. (You can stop Kobuki before running the very next segment.)*/
	for(int i = 0; i < 280; i++){
		movement(250, 0);
	}
	for(int i = 0; i < 50; i++) {
		movement(0, 0);
	}
	/*Rotate the Kobuki 90 degrees clockwise*/
	for(int i = 0; i < 72; i++){
		movement(181, -1);
	}
	for(int i = 0; i < 50; i++) {
		movement(0, 0);
	}
	/*Move along left side*/
	for(int i = 0; i < 230; i++){
		movement(250, 0);
	}
	for(int i = 0; i < 50; i++){
		movement(0,0);
	}
	/*Move along semicircle*/
	for(int i = 0; i < 545; i++){
		movement(193, -500);
	}
	for(int i = 0; i < 50; i++){
		movement(0,0);
	}
	/*Move along right side*/
	for(int i = 0; i < 280; i++){
		movement(200, 0);
	}
	/*Rotate the Kobuki 90 degrees clockwise*/
	for(int i = 0; i < 67; i++){
		movement(181, -1);
	}
	for(int i = 0; i < 50; i++){
		movement(0,0);
	}
	//Close the serial stream to the Kobuki
	//Not doing this will result in an unclean disconnect
	//resulting in the need to restart the Kobuki
	//after every run.
	serialClose(kobuki);
	return(0);
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

