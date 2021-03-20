////////////////////////////////////////////////////////////////
// CControlPi.cpp
// Member function definitions for class CControl that serially 
//     controls an embedded system
// Modified March09, 2021 by Laurel Kinahan 
////////////////////////////////////////////////////////////////
//#include "stdafx.h"

#include "CControlPi.h"
//#include <conio.h>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <pigpio.h>
#include <opencv2/opencv.hpp>

using namespace std;

//Serial _com;
int buttonState = 0;
double lastDebounceTime = 0;
////////////////////////////////////////////////////////////////
// CControl Constructor
////////////////////////////////////////////////////////////////
CControlPi::CControlPi()
{
     
    buttonwasOFF = 0;
    press = 1;

    gpioSetMode(PINOUT_RED, PI_OUTPUT);
    gpioSetMode(PININ_PB1, PI_INPUT);
    gpioSetMode(PININ_PB2, PI_INPUT);

    gpioSetMode(PINOUT_SERVO1, PI_OUTPUT);
    //gpioSetMode(PINOUT_SERVO2, PI_OUTPUT);
}

////////////////////////////////////////////////////////////////
// CControl Destructor
////////////////////////////////////////////////////////////////
CControlPi::~CControlPi()
{
    gpioTerminate();
}

////////////////////////////////////////////////////////////////
// Print Menu
////////////////////////////////////////////////////////////////
void CControlPi::print_menu(void)
{
    cout << "\n**************************************";
    cout << "\n*          ELEX4618 Lab 7            *";
    cout << "\n* Select a test operation to perform *";
    cout << "\n**************************************";
    cout << "\n(A) Analog Test - test joystick outputs";
    cout << "\n(D) Digital I/O Test - test LED status";
    cout << "\n(B) Button Test - test counter debounce";
    cout << "\n(S) Servo Test - test servo position";
    cout << "\n(Q) Quit";
    cout << "\nCMD> ";
}

////////////////////////////////////////////////////////////////
// Get Analog
////////////////////////////////////////////////////////////////
int CControlPi::get_analog(int channel)
{
    int value;
    get_data(ANALOG, channel, value);
    return value;
}

////////////////////////////////////////////////////////////////
// Set Data  
////////////////////////////////////////////////////////////////
bool CControlPi::set_data(int type, int channel, int val)
{
    if (type == DIGITAL)
    {
        gpioWrite(channel, val);
    }
    else if (type == ANALOG)
    {
        gpioWrite(channel, val);
    }
    else if (type == SERVO)
    {
        gpioServo(channel, val);
        cv::waitKey(1);
    }
    return true;
}

////////////////////////////////////////////////////////////////
// Get Data
////////////////////////////////////////////////////////////////
bool CControlPi::get_data(int type, int channel, int& result)
{    
    if (type == DIGITAL)
    {
        result = gpioRead(channel);
        return true;
    }
    if (type == ANALOG)
    {
        float read_val;
        unsigned char inBuff[3];
        char cmd[3];
        cmd[0] = 1;
        cmd[1] = (0b1000 | channel) << 4;
        cmd[3] = 0;

        int handle = spiOpen(0, 200000, 3);

        spiXfer(handle, cmd, (char*)inBuff, 3);
        read_val = ((inBuff[1] & 3) << 8) | inBuff[2];
        spiClose(handle);

        result = read_val;
        return true;
    }

    else
    {
        result = 0;
        return false;
    }
}

////////////////////////////////////////////////////////////////
// Get Button
////////////////////////////////////////////////////////////////
bool CControlPi::get_button(int channel)
{
    bool pressed_twice = false;
    static bool pressed_once = false;

    if (!pressed_once)
    {
        buttonState = (gpioRead(channel));
        if (buttonState == 0)
        {
            pressed_once = true;
            lastDebounceTime = cv::getTickCount();
        }
    }
    if (pressed_once)
    {
        int lastDebounceTime = 0;
        if ((cv::getTickCount() - lastDebounceTime) / cv::getTickFrequency() > 0.1)
        {
            pressed_once = false;
            lastDebounceTime = cv::getTickCount();
            buttonState = (gpioRead(channel));
            if (!buttonState)
            {
                pressed_twice = true;
                buttonState = 1;
            }
        }
    }
    return pressed_twice;
}
