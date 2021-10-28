//Use g++ joystick.cc -std=c++11 -o 3b 3b.cpp

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
#define  PORT 8080
using namespace std;

int createSocket();

int sock = 0, valread;


int main(int argc, char const *argv[])
{

    //char *hello = "Hello from client"; 
	char buffer[1024] = {0};

	//Open the file stream for the joystick
	Joystick joystick("/dev/input/js0");
	JoystickEvent event;

	if(!joystick.isFound()){
		cout << "Error opening joystick" << endl;
		exit(1);
	}


	//Create the connection to the server
	createSocket();

	while(true)
	{

		/*Sample the events from the joystick*/
		if (joystick.sample(&event))
		{
			buffer[] = 'S';
			if (event.isButton())
			{
				printf("isButton: %u | Value: %d\n", event.number, event.value);
				
				/*Interpret the joystick input and use that input to move the Kobuki*/
				switch(event.number)
				{
					case 7:
							//movement(0,0);
							
					break;
					
					case 8:
						if(event.value) cout << ""; //send close command 
						else continue;	
					break;
				}
				//movement(0,0);

			}
			if (event.isAxis())
			{
				buffer[] = 'S';
				printf("isAxis: %u | Value: %d\n", event.number, event.value);
				/*Interpret the joystick input and use that input to move the Kobuki*/
				
				switch(event.number)
				{
					case 7:
						if(event.value < 0)
						{
							//movement(250, 0);
							
						}
						else if(event.value > 0)
						{
							//movement(-250, 0);
						}
						
					break;
					
					case 6:
						if(event.value > 0)
						{
							//movement(181, -1);
						}
						
						else if(event.value > 0)
						{
							//movement(181, 1);
						}
					break;
				}
					
			}
		}
				
		/*Convert the event to a useable data type so it can be sent*/
		//valread = read(sock, buffer, 1024);

		/*Print the data stream to the terminal*/
		//printf("%s\n", buffer);
		
		/*Send the data to the server*/
			send(sock , buffer , strlen(buffer) , 0 );
		//printf("Hello message sent\n");
		// should sock be valread? what is valread doing? if /n is new line, is %s a pointer to the thing above it? In this case my valread?
		
		
		// I have no idea what this if clause should be.
		if(1) 
		{
		//Closes out of all connections cleanly
		//When you need to close out of the connection, please
		//close TTP/IP data streams.
		//Not doing so will result in the need to restart
		//the raspberry pi and Kobuki
			close(sock);
			exit(0);

		//Set a delay
		delay(50);
		}

	return 0;
	}
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
	if(inet_pton(AF_INET, "10.227.59.225" , &serv_addr.sin_addr) <= 0){
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
		printf("\nConnection Failed \n");
		return -1;
	}

	return 0;
}
