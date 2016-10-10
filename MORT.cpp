//#include "CDriverStation.h"
#include "MORT_defines.h"
#include "MORT_includes.h"

float ServiceJoysticks(int choice)
{
	float left_joystick;
	float right_joystick;
	static int left_flag = 0;
	static int right_flag = 0;
	static float old_left_joy;
	static float old_right_joy;
	
	// get joystick values to pass in for ramps and set to motors
	
	left_joystick = GetY(LEFT_JOYSTICK);
    right_joystick = GetY(RIGHT_JOYSTICK);
    // if the trigger is on for brakes or the top button for ramping, then call ramp function and set output to a ramped joystick value    
    
    if((GetTop(LEFT_JOYSTICK) || GetTrigger(LEFT_JOYSTICK)) && (left_flag != 1))
    {
        left_flag = 1;
    	old_left_joy = left_joystick; 
    }
    
    // clear the flag once the top has been released to then repeat
    if(GetTop(LEFT_JOYSTICK) && (left_flag == 1))
    {	
    	old_left_joy += (left_joystick - old_left_joy )/(RampUpSpeed);
    	left_joystick = old_left_joy; 
    }
    if(GetTrigger(LEFT_JOYSTICK) && (left_flag == 1))
    {
    	old_left_joy /= BrakingConstant;
    	left_joystick = old_left_joy; 
    }
    if(!GetTrigger(LEFT_JOYSTICK) && !GetTop(LEFT_JOYSTICK))
    {
    	left_flag = 0;
    }
    // if the cout is used it slows down the loop so the ramp constant needs to be changed
    // same logic as above for left
    if((GetTop(RIGHT_JOYSTICK) || GetTrigger(RIGHT_JOYSTICK)) && (right_flag != 1))
    {
        right_flag = 1;
    	old_right_joy = right_joystick; 
    }
    
    // clear the flag once the top has been released to then repeat
    if(GetTop(RIGHT_JOYSTICK) && (right_flag == 1))
    {
    	old_right_joy += (right_joystick - old_right_joy )/(RampUpSpeed);
    	right_joystick = old_right_joy; 
    }
    if(GetTrigger(RIGHT_JOYSTICK) && (right_flag == 1))
    {
    	old_right_joy /= BrakingConstant;
    	right_joystick = old_right_joy; 
    }
    
    if(!GetTrigger(RIGHT_JOYSTICK) && !GetTop(RIGHT_JOYSTICK))
    {
    	right_flag = 0;
    }
   
    //set joystick values and reverse them for each motor
    
    right_joystick *= -1;
    
    //max was here..//
    
    if(choice == 1)
    	return limit(-0.999, 0.999, left_joystick);
    else
    	return limit(-0.999, 0.999, right_joystick);
    
    // decide which motor value to return, so they can all be set individually
}

void DriveTrain(float lside, float rside)
{
	// set all the drive train motors
	
	SetVictorSpeed(LEFT_FRONT_MOTOR, lside);
    SetVictorSpeed(LEFT_BACK_MOTOR, lside); 
    SetVictorSpeed(RIGHT_FRONT_MOTOR, rside);
    SetVictorSpeed(RIGHT_BACK_MOTOR, rside);
}

void ServiceTower()
{
	static float tower_motor;
	static float tower_joystick;
	static float roller_motor;
	
	static int feedIn= 0;
	static int feedOut= 0;
	static int tower_trigger = 0;
	static int tower_trig_check = 0;
	
	tower_joystick = GetY(TOWER_JOYSTICK);

	feedOut= GetTop(TOWER_JOYSTICK);
	feedIn= GetRawButton(TOWER_JOYSTICK, 3);
	tower_trigger = GetTrigger(TOWER_JOYSTICK);
	tower_trig_check = GetRawButton(TOWER_JOYSTICK, 4);

	if(GetDigitalInput(PUMP_SENSOR))
	{
		SetRelay(COMPRESSOR_RELAY , kOff);
	}
	else if (!GetDigitalInput(PUMP_SENSOR))
	{
		SetRelay(COMPRESSOR_RELAY , kForward);
	}
	
	if (feedIn == 1)
	{
		roller_motor = 1.0;
	}
	else if (feedOut == 1)
	{
		roller_motor = -1.0;
	}
	else
	{
		roller_motor = 0.0;
	}
	
	if(tower_trigger && tower_trig_check && GetDigitalInput(PUMP_SENSOR))
	{
		SetSolenoid(SOLENOID, true);
		
		tower_motor= 0;
		roller_motor = 0;
	}
	else if (!(tower_trigger || tower_trig_check))
	{
		SetSolenoid(SOLENOID, false);
		tower_motor = -1 * GetY(TOWER_JOYSTICK);
	}
	
	SetVictorSpeed(TOWER_MOTOR, tower_motor);
	SetVictorSpeed(ROLLER_MOTOR, roller_motor);
}

void SetLED()
{
	 if((GetUltrasonicInches(ULTRASONIC_TRIGGER, ULTRASONIC_ECHO) <= 14)
		 && (GetUltrasonicInches(ULTRASONIC_TRIGGER, ULTRASONIC_ECHO) > 10))
	 {
		 SetDSDigitalOut(RED_LED, true);
	 }
	 else if(GetUltrasonicInches(ULTRASONIC_TRIGGER, ULTRASONIC_ECHO) <= 10)
	 {
		 SetDSDigitalOut(GREEN_LED, true);
		 SetDSDigitalOut(RED_LED, false);
	 }
	 else
	 {
		 SetDSDigitalOut(GREEN_LED, false);
		 SetDSDigitalOut(RED_LED, false);
	 } 
}

// return absolute value
float abs(float x)
{
	if (x < 0)
		return (x * -1.0);
	else
		return x;
}

// return limits but do not use yet

float limit(float low, float high, float input)
{
	if (input < low)
		return low;
	if(input > high)
		return high;
	else
		return input;
}

float absmax(float a, float b)
{
	if (a < 0)
        a = a * -1;
    if (b < 0)
        b = b * -1;
//    printf("\n abs %f %f \n", a, b);

    if(a > b)
        return a;
    else
        return b;
}
