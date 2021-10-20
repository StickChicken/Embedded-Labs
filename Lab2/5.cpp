// g++ -std=c++14 Lab2EX4.cpp -o Lab2EX4 -lwiringPi


#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <string.h>
#include <iostream>

#define IN3 2
#define IN4 3
#define PWM 26

using namespace std;

/* Complete the code: I2c address of LCD, some LCD i2c address might be 0x27 */
int LCDAddr = 0x27;  


int BLEN = 1; // 1--open backlight.0--close backlight
int fd; // Linux file descriptor
int button_state = LOW;
bool changed = false;


// Send an 16 bits data to LCD buffer
void write_word(int data){
    int temp = data;
    if ( BLEN == 1 )
        temp |= 0x08;
    else
        temp &= 0xF7;
    wiringPiI2CWrite(fd,temp);

}

// Send control command to lcd
void send_command(int comm){
    int buf;
    // Send bit7-4 firstly
    buf = comm & 0xF0;
    buf |= 0x04;            // RS = 0, RW = 0, EN = 1
    write_word(buf);
    delay(2);
    buf &= 0xFB;            // Make EN = 0
    write_word(buf);
    // Send bit3-0 secondly
    buf = (comm & 0x0F) << 4;
    buf |= 0x04;            // RS = 0, RW = 0, EN = 1
    write_word(buf);
    delay(2);
    buf &= 0xFB;            // Make EN = 0
    write_word(buf);
}

// Send character to lcd
void send_data(int data){
    int buf;
    // Send bit7-4 firstly
    buf = data & 0xF0;
    buf |= 0x05;            // RS = 1, RW = 0, EN = 1
    write_word(buf);
    delay(2);
    buf &= 0xFB;            // Make EN = 0
    write_word(buf);
    // Send bit3-0 secondly
    buf = (data & 0x0F) << 4;
    buf |= 0x05;            // RS = 1, RW = 0, EN = 1
    write_word(buf);
    delay(2);
    buf &= 0xFB;            // Make EN = 0
    write_word(buf);
}

// Initialize the lcd
void init(){
    send_command(0x33); // Initialize 
    delay(5);
    send_command(0x32); // Initialize 
    delay(5);

    /* Complete the code: 4bit mode 2 Lines & 5*8 dots */
    send_command(0x28);
    delay(5);

    send_command(0x0C); // Enable display without cursor
    delay(5);

    /* Complete the code: Clear screen */
    send_command(0x01); 
    
}

// Clear screen
void clear(){
    /* Complete the code:clear Screen */
    send_command(0x01);

}

// Print the message on the lcd
void write(int x, int y, const char data[]){
    int addr, i;
    int tmp;
    if (x < 0) x = 0; if (x > 15) x = 15;
    if (y < 0) y = 0; if (y > 1)  y = 1;

    /* Complete the code: Target address, Move cursor */
    addr = 0x80 + 0x40 * y + x;

    // Set the address
    send_command(addr);
    
    tmp = strlen(data);
    for (i = 0; i < tmp; i++){
        send_data(data[i]);
    }
}

void print_info()
{
    printf("\n");
    printf("|***************************|\n");
    printf("|    IIC 1602 LCD test      |\n");
    printf("| --------------------------|\n");
    printf("| | LCD |            | Pi   |\n");
    printf("| --------------------------|\n");
    printf("| | GND | connect to | GND  |\n");
    printf("| | VCC | connect to | 5V   |\n");
    printf("| | SDA | connect to | SDA.1|\n");
    printf("| | SCL | connect to | SCL.1|\n");
    printf("| --------------------------|\n");
    printf("|                   ECEN4213|\n");
    printf("|***************************|\n");
    printf("Program is running...\n");
    printf("Press Ctrl+C to end the program\n");
}


bool direction = 0; //0 is counterclockwise
bool speed = 1; //0 is not running the spinup / spindown

void checkDirection(){
    
	if(direction){
	  digitalWrite(IN3, HIGH);
	  digitalWrite(IN4, LOW);
	  write(0,0,"DR: Counter-Clockwise");
	}
	else {
	  digitalWrite(IN3, LOW);
	  digitalWrite(IN4, HIGH);
	  write(0,0,"DR: Clockwise        ");
}
}

void speedSet(){
    if(digitalRead(4)) speed = !speed;
}

void directionSet(){
    if(digitalRead(1)) direction = !direction;
}


int main(){

    // Set up WiringPi
    if (wiringPiSetup () == -1) exit (1);    
    
    // Init I2C, assign a buffer handler to variable fd
    fd = wiringPiI2CSetup(LCDAddr);

    /* Complete the code: in the init function*/
    init();


    print_info();
    pinMode(1, PUD_DOWN);
    pinMode(4, PUD_DOWN);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(PWM, PWM_OUTPUT);
    wiringPiISR(1, INT_EDGE_RISING, directionSet);
    wiringPiISR(4, INT_EDGE_RISING, speedSet);

    /* Set up two interrupts using wiringPiISR() for two buttons. You can refer to the lab documents.
    You can use some global variables which can be used in the following while loop*/


    while(true) {
        /*According to the global variables, the LCD display and DC motor speed and direction can be controlled*/
	checkDirection();
	
    bool temp = speed;
	if(speed){
	  for(int i = 100; i < 1024; i++){
        if(speed != temp) break;
        checkDirection();
		pwmWrite(PWM, i);
        
        char buffer[10];
        sprintf(buffer,"%d",i);
		write(0,1,buffer);
        
		delay(5);		
	  }
	}
    else pwmWrite(PWM, 600);
    

    }

    return 0;
}
