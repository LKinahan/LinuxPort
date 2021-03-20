////////////////////////////////////////////////////////////////
// CControl.h
// Definintion of class CControl
// Member function are defined in CControl.cpp
// Created Jan 29, 2021 by Laurel Kinahan 
////////////////////////////////////////////////////////////////
//#pragma once
#include <iostream>
#include <pigpio.h>
#include <opencv2/opencv.hpp>
//#include "Serial.h"

/**
* enum for serial communication type
*/
enum TYPE
{
	DIGITAL = 0, ///< 0
	ANALOG, ///< 1
	SERVO ///< 2
};

//#define MSP432PORT 3
//#define ADC_MAX 1024
#define JOY_X 0
#define JOY_Y 1
#define PININ_PB1 23
#define PININ_PB2 24
#define PINOUT_RED 22
#define PINOUT_BLU 17
#define PINOUT_GRN 27
#define SERVO_MIN 0
#define SERVO_MAX 180
#define SERVO_PORT 0
#define PINOUT_SERVO1 21
//#define PINOUT_SERVO2 26
#define ADC_MAX 1024


/**
*
* @brief Used to serially control an Embedded System MSP432
* Class CControl initializes the comport, sends and collects data form the MSP432
* class also debounces pushbutton and scales analog input to a percentages of its range
*
* @author Laurel Kinahan
*
*/

class CControlPi
{
private:
	//Serial _com; ///< allows for serial port communications

	long timestart;
	long time_elapsed;
	int buttonwasOFF;
	int press;

public:
	CControlPi();
	~CControlPi();
	void print_menu(void);

	/*
	* @brief initialization serial port to com port in parameter
	*
	* @param comport The com port number to comment to
	* @return none
	*/
	//void init_com(int comport);

	/*
	* @brief Gets data from channel parameter and returns data to result parameter address
	*
	* @param type Type of serial communication
	* @param Channel Desired channel on Embedded System
	* @param result Referenced address for returned result
	* @return true if successful, false if failed or timed out
	*/

	bool get_data(int type, int channel, int& result);

	/*
	* @brief Sets data to channel parameter on Embedded System
	*
	* @param type Type of serial communication
	* @param Channel Desired channel on Embedded System
	* @return true if successful, false if failed or timed out
	*/

	bool set_data(int type, int channel, int val);
	/*
	* @brief Gets data from analog channel and returns data to result parameter address as a % of range
	*
	* @param type Type of channel will always be ANALOG
	* @param Channel Desired channel number on Embedded System
	* @param raw_data Raw analog data from receive buffer
	* @param result Referenced address for returned result
	* @return percent value of A0 result
	*/

	int get_analog(int channel);

	/*
	* @brief Gets data from digital channel and returns data to result parameter address as a % of range
	*
	* @param type Type of channel will always be ANALOG
	* @param Channel Desired channel number on Embedded System
	* @param raw_data Raw analog data from receive buffer
	* @param result Referenced address for returned result
	* @return percent value of A0 result
	*/
	//void get_digital(void);

	/*
	* @brief Gets button status and returns to result address
	*
	* @param type Type of serial communication
	* @param Channel Desired channel on Embedded System
	* @param result Referenced address for returned result
	* @return true if successful, false if failed or timed out
	*/

	bool get_button(int channel);

	/**
	* @brief Enables access to the servo's angle
	*
	* @param none
	* @return none
	*/

	//void get_servo(void);
};
