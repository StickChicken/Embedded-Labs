//Use g++ joystick.cc -std=c++11 -o 2 2.cpp -lwiringPi

#include <iostream>
#include <wiringPi.h>
#include <wiringSerial.h>
#include "joystick.hh"
#include <unistd.h>
#include <cstdlib>
using namespace std;

//bool checkButton(Joystick, JoystickEvent, unsigned int);
void movement(int, int);
int kobuki;

int main(){

	wiringPiSetup();
	kobuki = serialOpen("/dev/kobuki", 115200);
	Joystick joystick("/dev/input/js0");
	JoystickEvent event;
	unsigned int button;

	//The joystick creates events when a button or axis changes value.
	//Sample event from the joystick: joystick.sample(&event)

	//You can interpret these by sampling the events.
	//Each event has three parameters.
	//A type, axis or button,
	//judge if the event is button: event.isButton()
	//judge if the event is axis: event.isAxis()
	//A number corresponding to the axis or button pressed: event.number
	//And a value, Buttons: 0-unpressed, 1-pressed, Axis: -32767 to 0 to 32767: event.value

	while(true){
		/*Create a series of commands to interpret the
		joystick input and use that input to move the Kobuki*/

		//Use the following Key Map:
		//Up     - move the Kobuki forward
		//Down   - move the Kobuki backward
		//Left   - rotate the Kobuki 90 degrees counterclockwise
		//Right  - rotate the Kobuki 90 degrees clockwise
		//Start  - immediately stop the Kobuki's movement
		//Select - exit the script and close the Kobuki's connection cleanly
		
		if (joystick.sample(&event))
		{
			if (event.isButton())
			{
				printf("isButton: %u | Value: %d\n", event.number, event.value);
				/*Interpret the joystick input and use that input to move the Kobuki*/
				switch(event.number){
					case 7:
							movement(0,0);
					break;
					
					case 8:
						if(event.value) serialClose(kobuki);
						else continue;	
					break;
				}
				movement(0,0);

			}
			if (event.isAxis())
			{
				printf("isAxis: %u | Value: %d\n", event.number, event.value);
				/*Interpret the joystick input and use that input to move the Kobuki*/

				switch(event.number){
					case 7:
						if(event.value == -32767){
								movement(250, 0);
							
						}
						
						else if(event.value == 32767){
								movement(-250, 0);								
						}
						
						
					break;
					
					case 6:
						if(event.value > 0){
							for(int i = 0; i < 75; i++){
								movement(181, -1);
								//if(checkButton(joystick, event, button)) break;
							}
						}
						
						else if(event.value < 0){
							for(int i = 0; i < 75; i++){
								movement(181, 1);
								//if(checkButton(joystick, event, button)) break;
							}
						}
					break;
					}
					
				}
				
			}
		}

	

	return 0;
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

/*bool checkButton(Joystick, JoystickEvent, unsigned int button){
if (joystick.sample(&event)){
                        if (event.isButton())
                        {
                                printf("isButton: %u | Value: %d\n", event.number, event.value);
                                Interpret the joystick input and use that input to move the Kobuki
                                switch(event.number){
                                        case 7:
                                                if(event.value) return 1;
                                        break;

                                        case 8:
                                                if(event.value) serialClose(kobuki);
                                                return 1;
                                        break;
                                        
                                }
                                

                        }
		}
		return 0;
}
*/
