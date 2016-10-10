#include "MORT_defines.h"
#include "MORT_includes.h"

int GetAutoMode()
{
	static int mode;
	
	if(GetDigitalInput(AUTO_SWITCH))
		mode = 1;
	else
		mode = 0;

	if(mode == 1)
		return 1; //WE ARE BLUE
	else
		return 2; //WE ARE RED

}


void AutonomousPump()
{
	if(GetDigitalInput(PUMP_SENSOR))
	{
		SetRelay(COMPRESSOR_RELAY , kOff);
	}
	else if (!GetDigitalInput(PUMP_SENSOR))
	{
		SetRelay(COMPRESSOR_RELAY , kForward);
	}
//	SetVictorSpeed(ROLLER_MOTOR, -1.0);
}


void FollowTarget(float x_offset, float particle_percent, float y_offset, int color_check)
{	

	static float x_val;
	static float y_val;
	static int direction = 0;
	static int count = 0;
	
	if(color_check)
	{
		if(x_offset < -0.1)
		{
			direction = (int) LEFT;
		}
		if(x_offset > 0.1)
		{
			direction = (int) RIGHT;
		}

		if((particle_percent < 40)&& (particle_percent > 1)) //) //(temp_check &&  ...
		{
			y_val = 0.8; //0.6
			x_val = x_offset/1.3;
		}

		else if (((particle_percent > PARTICLE_PERCENT_MAX)
				|| (-1 * y_offset > (MIN_OFFSET))) 
				&& (GetUltrasonicInches(ULTRASONIC_TRIGGER, ULTRASONIC_ECHO) < 15.2)) // && sensor is right
		{
			y_val = 0.8; //0.4
			x_val = 0;
			count+= 2; //1
		}
		if (GetUltrasonicInches(ULTRASONIC_TRIGGER, ULTRASONIC_ECHO)< 12)
		{
			y_val = 0.8; //0.4
			x_val = 0;
			count+=4;
		}
		if(GetUltrasonicInches(ULTRASONIC_TRIGGER, ULTRASONIC_ECHO) > 15)
		{
			count = 0;
		}
		if(count > 160) //200
		{
			SetSolenoid(SOLENOID, true);
		}
	}
	else
	{
		if(direction == LEFT)
		{
	//		y_val = 0.26;
			x_val = - 0.175;
		}
		else
		{
//			y_val = 0.26;
			x_val = 0.175;
		}
		
		if((GetUltrasonicInches(ULTRASONIC_TRIGGER, ULTRASONIC_ECHO) < 15) 
			|| (GetAnalogVoltage(US_FRONT)/(9.8/1000) < 17))
		{
			y_val = -0.3;
		}
		else
		{
			y_val = 0.3; //0.26;
		}
		
		if((GetAnalogVoltage(US_LSIDE)/(9.8/1000) < 12))
		{
			x_val = -0.4;
		}
		if((GetAnalogVoltage(US_RSIDE)/(9.8/1000) < 12))
		{
			x_val = 0.4;
		}
		else
		{
			x_val = 0.0;
		}
	}

	x_val = limit(-0.999, 0.999, x_val);
	y_val = limit(-0.999, 0.999, y_val);
	
	SingleDrive(x_val, y_val);
}

void TrackTarget(int auto_mode)
{ 
	
	static float green_y_offset;
	static float red_y_offset;
	static float temp_x_offset;
	static float particle_percent;
	static float temp_y_offset;
	
	
	static int green_check = 0; 
	static int red_check = 0;
//	static int temp_check;
	static int color_check;

	Range g_hue;		Range r_hue;
	Range g_sat;		Range r_sat;
	Range g_lum;		Range r_lum;
	
	ParticleAnalysisReport green_par;
	ParticleAnalysisReport red_par;

/*
	r_hue.minValue = 55; 			r_hue.maxValue = 125;
	r_sat.minValue = 32; 			r_sat.maxValue = 255;
	r_lum.minValue = 50; 			r_lum.maxValue = 255;

	g_hue.minValue = 220; 			g_hue.maxValue = 255;
	g_sat.minValue = 50; 			g_sat.maxValue = 255;
	g_lum.minValue = 50; 			g_lum.maxValue = 255;
*/	

	g_hue.minValue = 57; 			g_hue.maxValue = 83;
	g_sat.minValue = 62; 			g_sat.maxValue = 205;
	g_lum.minValue = 52; 			g_lum.maxValue = 85;

	r_hue.minValue = 166; 			r_hue.maxValue = 247;
	r_sat.minValue = 128; 			r_sat.maxValue = 255;
	r_lum.minValue = 42; 			r_lum.maxValue = 64;
	
	green_check = FindColor(IMAQ_HSL, &g_hue, &g_sat, &g_lum, &green_par);
	red_check = FindColor(IMAQ_HSL, &r_hue, &r_sat, &r_lum, &red_par);
	
	green_y_offset = green_par.center_mass_y_normalized;
	red_y_offset = red_par.center_mass_y_normalized;
	
	if (auto_mode == RD) //BL
	{
		particle_percent = 100 * green_par.particleToImagePercent;
//		temp_check = green_check;
		temp_x_offset = green_par.center_mass_x_normalized;
		temp_y_offset = green_y_offset;
		
		if((red_y_offset > green_y_offset) && (green_check))
		{
			color_check = 1;
		}
		
		else
		{
			color_check = 0;
		}
	}
	else //then auto_mode is RED // BLUE
	{
		particle_percent = 100 * green_par.particleToImagePercent;
//		temp_check = red_check;	
		temp_x_offset = green_par.center_mass_x_normalized;
		temp_y_offset = green_y_offset;
		
	//	particle_percent = 100 * red_par.particleToImagePercent;
	//	temp_check = red_check;	
	//	temp_x_offset = red_par.center_mass_x_normalized;
	//	temp_y_offset = red_y_offset;
		
		if((green_y_offset > red_y_offset) && (red_check))
		{
			color_check = 1;
		}
		else
		{
			color_check = 0;
		}
	}
	
	if((red_y_offset > green_y_offset))
	{
		SetDSDigitalOut(GREEN_LED, true);
		SetDSDigitalOut(RED_LED, false);
	}
	if((red_y_offset < green_y_offset))
	{
		SetDSDigitalOut(RED_LED, true);
		SetDSDigitalOut(GREEN_LED, false);
	}
		
	FollowTarget(temp_x_offset, particle_percent, temp_y_offset, color_check);
//	cout << "r: " << red_y_offset << " g: " << green_y_offset <<endl;
}

void SingleDrive(float x_val, float y_val)
{
	float leftMotorSpeed;
	float rightMotorSpeed;
	double reduction;	

//	y_val *= -1;
		
	if (y_val < 0)
	{
		x_val *= -1;
	}

	
	leftMotorSpeed =  x_val + y_val;
	rightMotorSpeed =  x_val - y_val;

    reduction = absmax(leftMotorSpeed , rightMotorSpeed);

    if (reduction == 0)
    {
        reduction = 1;
    }
    
    if (abs(leftMotorSpeed) > 1.0)
        leftMotorSpeed *= reduction;
    if (abs(rightMotorSpeed) > 1.0)
        rightMotorSpeed *= reduction;
    
    leftMotorSpeed *= -1;
    rightMotorSpeed *= -1;
    
    leftMotorSpeed= limit(-1.0, 1.0, leftMotorSpeed);
    rightMotorSpeed = limit(-1.0, 1.0, rightMotorSpeed);
    
    DriveTrain (leftMotorSpeed, rightMotorSpeed);
}
