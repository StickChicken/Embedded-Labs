//Use g++ -std=c++11 -o 1 1.cpp -lwiringPi

#include <string>
#include <iostream>
#include <wiringSerial.h>
#include <wiringPi.h>
#include <unistd.h>
#include <cstdlib>
#include <iomanip>
using namespace std;

int kobuki;

int main(){
	wiringPiSetup();
	kobuki = serialOpen("/dev/kobuki", 115200);
	unsigned int bumper;
	unsigned int drop;
	unsigned int cliff;
	unsigned int button;
	unsigned int read;

	while(serialDataAvail(kobuki) != -1){

		while(true){
		//If the bytes are a 1 followed by 15, then we are
		//parsing the basic sensor data packet
			read = serialGetchar(kobuki);
			if(read == 1){
				if(serialGetchar(kobuki) == 15) break;
			}
		}

		//Read past the timestamp
		serialGetchar(kobuki);
		serialGetchar(kobuki);
		

		/*Read the bytes containing the bumper, wheel drop,
			and cliff sensors. You can convert them into a usable data type.*/
			bumper = (int)serialGetchar(kobuki);
			
			drop = (int)serialGetchar(kobuki);

			cliff = (int)serialGetchar(kobuki);
			
			serialGetchar(kobuki);
			serialGetchar(kobuki);
			
			serialGetchar(kobuki);
			serialGetchar(kobuki);
			
			serialGetchar(kobuki);
			serialGetchar(kobuki);
			
			button = (int)serialGetchar(kobuki);
			serialGetchar(kobuki);
			


		/*Print the data to the screen.*/
		
		if(bumper){
			cout << "bumper = " << bumper << endl;
		}
		if(drop){
			cout << "drop = " << drop << endl;
		}
		if(cliff){
			cout << "cliff = " << cliff << endl;
		}
		if(button){
			cout << "button = " << button << endl;
		}


		/*Read through 6 bytes between the cliff sensors and
		the button sensors.*/

		/*Read the byte containing the button data.*/

		/*Close the script and the connection to the Kobuki when
		Button 1 on the Kobuki is pressed. Use serialClose(kobuki);*/
		if(button == 1) serialClose(kobuki);

		

		//Pause the script so the data read receive rate is the same as the Kobuki send rate.
		usleep(20000);
			
		
	}

	return(0);
}
