//use g++ -std=c++11 -o Lab5EX2 Lab5EX2.cpp -lwiringPi


#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <ctime>
#include <ratio>
#include <chrono>
using namespace std;
using namespace std::chrono;

int kobuki;
float read_sonar();
void movement(int, int);

int main(){
	wiringPiSetup();
	kobuki = serialOpen("/dev/kobuki", 115200);

	/*Move from a random point within the area designated "X" to the
	point B as shown on the diagram. Use a sonar sensor to navigate through the channel.
	You can reuse your code from Lab 3 and 4*/

	/*Note: the Kobuki must completely pass point B as shown to receive full credit*/


}


float read_sonar(){
	// you can reuse your code from Lab 3

}




void movement(int sp, int r){
	// you can reuse your code from Lab 4
	
}