#include "MORT_includes.h"
#include "MORT_defines.h"
#include "PCVideoServer.h"

void Initialize(void)
{ 
	if (StartCameraTask() == -1)
	{
		printf("Failed to spawn camera task: %s\n",
				GetVisionErrorText(GetLastVisionError()));
	}
	else
		printf("INITIALIZED\n");

	InitUltrasonic(ULTRASONIC_TRIGGER, ULTRASONIC_ECHO);
	
	SetWatchdogExpiration(100);
}

void Autonomous(void)
{
	SetWatchdogEnabled(false);

	while (IsAutonomous())
	{
		static int x;
		static int b=1;
		if (b == 1)
		{
			while(x < 40000) //40000 -- 80000
			{
				SingleDrive(0, 0.99);
				SetVictorSpeed(ROLLER_MOTOR, 0.25);
				x+= 1;
			}
			b = 0;
			SingleDrive(0, 0);
		}	
		TrackTarget(GetAutoMode());
		AutonomousPump();
		SetVictorSpeed(ROLLER_MOTOR, 0.25);
	}
}

void OperatorControl(void)
{	
	SetWatchdogEnabled(true);

	static const int LEFT_SIDE	= 1;
	static const int RIGHT_SIDE	 = 2;
	

	DriverStation *ds;
	ds = DriverStation::GetInstance();
		
	while (IsOperatorControl())
	{

		DriveTrain(ServiceJoysticks(LEFT_SIDE), ServiceJoysticks(RIGHT_SIDE));		
	    ServiceTower();
		
		if((GetUltrasonicInches(ULTRASONIC_TRIGGER, ULTRASONIC_ECHO) <= 14)
			 && (GetUltrasonicInches(ULTRASONIC_TRIGGER, ULTRASONIC_ECHO) > 10))
		 {
			 ds->SetDigitalOut(RED_LED, true);

		 }
		 else if(GetUltrasonicInches(ULTRASONIC_TRIGGER, ULTRASONIC_ECHO) <= 10)
		 {
			 ds->SetDigitalOut(GREEN_LED, true);
		 }
		 else
		 {
			 ds->SetDigitalOut(GREEN_LED, false);
			 ds->SetDigitalOut(GREEN_LED, false);
		 }
		SetLED();

//		cout << GetAnalogVoltage(US_FRONT)/(9.8/1000) << " g: " << GetUltrasonicInches(ULTRASONIC_TRIGGER, ULTRASONIC_ECHO) << endl; 
	
		WatchdogFeed();
	}
}

START_ROBOT_CLASS(SimpleCRobot);

