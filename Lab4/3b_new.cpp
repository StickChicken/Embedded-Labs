//Use g++ joystick.cc -std=c++11 -o 3b_new 3b_new.cpp

#include <wiringPi.h>
#include <stdio.h>
#include <iostream>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include "joystick.hh"
#include <cstdlib>
#include <iostream>
#include <string>
#include <cstring>
#define  PORT 8080
using namespace std;

int createSocket();

int sock = 0;

int main(int argc, char const *argv[]){
    char buffer[1024] = {0};
    string s = "empty";
    int n = 0;

	//Open the file stream for the joystick
	Joystick joystick("/dev/input/js0");
	JoystickEvent event;
	if(!joystick.isFound()){
		cout << "Error opening joystick" << endl;
		exit(1);
	}

	//Create the connection to the server
	createSocket();

	while(true){

		/*Sample the events from the joystick*/
        if (joystick.sample(&event))
		{
			if (event.isButton())
			{
				printf("isButton: %u | Value: %d\n", event.number, event.value);
				/*Interpret the joystick input and use that input to move the Kobuki*/
				switch(event.number){
					case 7:
							//movement(0,0);
					break;

					case 8:
						if(event.value){
                        s = "1111";
                        }
						else continue;
					break;
				}
				//movement(0,0);

			}
			if (event.isAxis())
			{
				printf("isAxis: %u | Value: %d\n", event.number, event.value);
				/*Interpret the joystick input and use that input to move the Kobuki*/

				switch(event.number){
					case 7:
						if(event.value == -32767){
								//movement(250, 0);

						}

						else if(event.value == 32767){
								//movement(-250, 0);
						}


					break;

					case 6:
						if(event.value > 0){
							for(int i = 0; i < 75; i++){
								//movement(181, -1);
								//if(checkButton(joystick, event, button)) break;
							}
						}

						else if(event.value < 0){
							for(int i = 0; i < 75; i++){
								//movement(181, 1);
								//if(checkButton(joystick, event, button)) break;
							}
						}
					break;
					}
				}
			}

		/*Convert the event to a useable data type so it can be sent*/
		/*Print the data stream to the terminal*/

		/*Send the data to the server*/
        strcpy(buffer, s.c_str());
        send(sock, buffer, 1024, 0);
        if(s == "1111")
		{
			close(sock);
			exit(0);

		//Set a delay
		delay(50);
		}

    }
    return 0;
}

//Creates the connection between the client and
//the server with the controller being the client
int createSocket(){
	struct sockaddr_in address;
	struct sockaddr_in serv_addr;


	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		printf("\nSocket creation error \n");
		return -1;
	}

	memset(&serv_addr, '0', sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port   = htons(PORT);

	/*Use the IP address of the server you are connecting to*/
	if(inet_pton(AF_INET, "192.168.1.56" , &serv_addr.sin_addr) <= 0){
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
		printf("\nConnection Failed \n");
		return -1;
	}
	return 0;
}
