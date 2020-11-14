//*** motor_detection_test_1.1.c ***
/*************************************************************************
    File Name:  motor_detection_test_1.1.c
    Author:     CSrinivasan
    Date:       Mar 18th, 2020
    Modified:   None
    ©Fanshawe College, 2020

    Description: This program will rotate the camera body based on motion dection i.e. control stepper motor based on sensor's output.
                    Stepper motor used: 28BYJ - 48 (5VDC)
                    Motion Sensor used: HCSR501 (3-20VDC)
    
*************************************************************************/

// Preprocessor ============================================================
#include "mbed.h"
#include <stdio.h>
#include <stdlib.h>

// Constants ===============================================================
#define TRUE            1
#define FALSE           0
#define LIMIT           4
#define MAXFRWDANGLE    90
#define MAXREVERSEANGLE -90
#define MAXSTEPS        2048
#define OUTOFRANGE      ((sensor1 == TRUE && sensor2 == TRUE) || (sensor1 == FALSE && sensor2 == FALSE))

// Objects ===============================================================
DigitalIn  sensor1(p26);                    
DigitalIn  sensor2(p30);
DigitalIn  limitSW(p25,PullUp);             //limit switch will define motor home position to 0

DigitalOut in1(p20);                        //Motor control digital input1 for Horizontal rotation
DigitalOut in2(p19);                        //Motor control digital input2 for Horizontal rotation
DigitalOut in3(p18);                        //Motor control digital input3 for Horizontal rotation
DigitalOut in4(p17);                        //Motor control digital input4 for Horizontal rotation

DigitalOut in5(p16);                        //Motor control digital input1 for Vertical rotation
DigitalOut in6(p15);                        //Motor control digital input2 for Vertical rotation
DigitalOut in7(p14);                        //Motor control digital input3 for Vertical rotation
DigitalOut in8(p13);                        //Motor control digital input4 for Vertical rotation

DigitalOut led1(LED1);                      //on-Board LED used for debugging purpose
DigitalOut led4(LED4);                      //on-Board LED used for debugging purpose

Serial pc(USBTX, USBRX);                    //Serial communication used with 9600 Baud rate
Timer timer1sec;                            //Timer is configured to 1 sec

// Global Variables ========================================================
typedef struct stepperMotor
{
    float frwdposition;
    float reversePosition;
    float currentPosition;
    int pattern;
    int patternCounter;
}motor_t;
motor_t motor = {90,-90,-180,0,0};
char motorPattern[LIMIT] = {0x0C,0x06,0x03,0x09};       //Full Drive mode
int i=0;
char homeFlag = FALSE;

// Functions ==============================================================

/*** initialDelaySensors: ************************************************
Author:     Csrinivasan
Date:       Mar 18th, 2020      
Modified:   None
Desc:       This function will wait for 30 seconds until the PIR sensors
            becomes stable.
            PIR sensors used: HC-SR501(30 seconds propagation delay)
Input:      None
Returns:    None
*************************************************************************/
void initialDelaySensors(void)
{
    wait(30);
}
//eo initialDelaySensors:: ***********************************************

/*** initializeSwitch: ******************************************************
Author:     Csrinivasan
Date:       Mar 18th, 2020      
Modified:   None
Desc:       When the motor hits the switch, the home position(0 degree) is defiend.
Input:      None
Returns:    int
****************************************************************************/
int initializeSwitch(void)
{
    if(!limitSW)
    {
        wait(0.5);             //Eliminating switch bounce
        if(!limitSW)
        {
            return TRUE;
        }
    }
    return FALSE;
}
//eo initializeSwitch:: ******************************************************

/*** motorOperation: ******************************************************
Author:     Csrinivasan
Date:       Mar 18th, 2020      
Modified:   None
Desc:       Motor rotates based upon the given pattern.
Input:      int
Returns:    None
****************************************************************************/
void motorOperation(int pattern)
{
    switch(pattern)
    {
        case 0x0C: in1 = 1;in2 = 1;in3 = 0;in4 = 0;
                break;
        case 0x06: in1 = 0;in2 = 1;in3 = 1;in4 = 0;
                break;
        case 0x03: in1 = 0;in2 = 0;in3 = 1;in4 = 1;
                break;
        case 0x09: in1 = 1;in2 = 0;in3 = 0;in4 = 1;
                break;
        default: in1 = 0;in2 = 0;in3 = 0;in4 = 0;
                break;
    }
}

/*** motorClockwise: ******************************************************
Author:     Csrinivasan
Date:       Mar 18th, 2020      
Modified:   None
Desc:       Enabling and controlling the motor in clockwise direction
Input:      None
Returns:    None
*****************************************************************************/
void motorClockwise(void)
{
    while(motor.frwdposition >= motor.currentPosition)
    {
        in1 = in2 = in3 = in4 = 0;
        motor.pattern = motorPattern[motor.patternCounter];
        motor.currentPosition += 0.175;         //32 steps * 6 = 192
        motor.patternCounter++;
        if(motor.patternCounter >= LIMIT)
        {
            motor.patternCounter = 0;
        }
        if(homeFlag)
        {
            if(motor.currentPosition >= 0)
                return; 
        }
        if(initializeSwitch())
        {
            printf("Limit switch Connected,Home position defined \n\r");
            break;
        }
        motorOperation(motor.pattern);
        wait(0.002);
    }
}
//eo motorClockwise:: ********************************************************

/*** motorAntiClockwise: ***************************************************
Author:     Csrinivasan
Date:       Mar 18th, 2020      
Modified:   None
Desc:       Enabling and controlling the motor in anti-clockwise direction
Input:      None 
Returns:    None
***************************************************************************/
void motorAntiClockwise(void)
{
    while(motor.reversePosition <= motor.currentPosition)
    {
        in1 = in2 = in3 = in4 = 0;
        motor.pattern = motorPattern[motor.patternCounter];
        motor.currentPosition -= 0.175;         //32 steps * 6 = 192
        motor.patternCounter--;
        if(motor.patternCounter < 0)
        {
            motor.patternCounter = 3;
        }
        if(homeFlag)
        {
            if(motor.currentPosition <= 0)
                return;
        }
        motorOperation(motor.pattern);
        wait(0.002);
    }
}
//eo motorAntiClockwise:: ***************************************************

/*** motorHomePosition: ******************************************************
Author:     Csrinivasan
Date:       Mar 18th, 2020      
Modified:   None
Desc:       Initializing motor current position to home position(0)
Input:      None
Returns:    None
****************************************************************************/
void motorHomePosition(void)
{
    motorClockwise();
    motor.currentPosition = 0;
    pc.printf("Motor is in home position \n\r",motor.currentPosition);
}
//eo motorHomePosition:: ***************************************************

/*** homePosition: ********************************************************
Author:     Csrinivasan
Date:       Mar 18th, 2020      
Modified:   None
Desc:       Initializing motor current position to home position(0)
Input:      None
Returns:    None
****************************************************************************/
void homePosition(void)
{
    if(motor.currentPosition >= MAXFRWDANGLE)
    {
        motorAntiClockwise();       //Current position must be 0 degree
    }
    else if(motor.currentPosition <= MAXREVERSEANGLE)
    {
        motorClockwise();           //Current position must be 0 degree
    }
    else
    {           
        return;                      //Current position must be 0 degree
    }
}
//eo homePosition:: ********************************************************

/*** userMotorCtrlOperation: ************************************************
Author:     Csrinivasan
Date:       Mar 18th, 2020
Modified:   None
Desc:       Motor moves clockwise and anticlockwise based upon user input.
Input:      None
Returns:    None
***************************************************************************/
void userMotorCtrlOperation(void)
{   
    if(!OUTOFRANGE)                 //Then movement is detected
    {
        homeFlag = FALSE;
        if(sensor1 == TRUE)
        {
            led1 = TRUE;            //led1 indicator for sensor1 detection
            pc.printf("Motion detected in sensor1");
            motorClockwise();
            pc.printf(" eye move towards right : %i \n\r",(int)motor.currentPosition);
            //wait(5);
        }
        else
        {
            led1 = FALSE;  
        }
        if(sensor2 == TRUE)
        {
            led4 = TRUE;            //led4 indicator for sensor2 detection
            pc.printf("Motion detected in sensor2");
            motorAntiClockwise();
            pc.printf(" eye move towards left : %i \n\r",(int)motor.currentPosition);
            //wait(5);
        }
        else
        {
            led4 = FALSE;  
        }
    }
    else                            //Movement is not detected
    {
        led1 = led4 = FALSE;
        homeFlag = TRUE;
        pc.printf("Motion not detected");
        homePosition();
        pc.printf(" Eye moves to home position : %i \n\r",(int)motor.currentPosition);
    }
}   
// eo userMotorCtrlOperation::************************************************

/*****************************************************************************
***** MAIN FUNCTION *********************************************************
*****************************************************************************/
int main()
{
    motorHomePosition();
    initialDelaySensors();
    motor.currentPosition = 0;
    timer1sec.start();
    while(TRUE)
    
    {
        if((floor(timer1sec.read())>=1))
        {
        timer1sec.reset();
        userMotorCtrlOperation();
        }
    }
}
