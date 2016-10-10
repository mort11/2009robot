#ifndef _MORT__DEFINES_H_
#define _MORT__DEFINES_H_

#include "MORT_includes.h"
//#include "Cwrappers.h"

/***** Constants******/
static const float e = 2.718; 
static const float RampUpSpeed = 1000; //15000;  //90000;
static const float BrakingConstant = e;

/***** Motor and Joystick Assignments******/

static const UINT32 LEFT_BACK_MOTOR = 1;
static const UINT32 RIGHT_FRONT_MOTOR = 2;
static const UINT32 LEFT_FRONT_MOTOR = 3;
static const UINT32 RIGHT_BACK_MOTOR = 7;

static const UINT32 ROLLER_MOTOR = 5;
static const UINT32 TOWER_MOTOR = 6;

static const UINT32 LEFT_JOYSTICK = 2;
static const UINT32 RIGHT_JOYSTICK = 3;
static const UINT32 TOWER_JOYSTICK = 4;

/******Solenoid *******/
static const UINT32 SOLENOID = 1;

/*****Digital Inputs*****/
static const UINT32 PUMP_SENSOR= 1; 
static const UINT32 AUTO_SWITCH = 2;

static const UINT32 ULTRASONIC_TRIGGER = 3;
static const UINT32 ULTRASONIC_ECHO = 4;

static const UINT32 US_FRONT = 1;
static const UINT32 US_LSIDE = 2;
static const UINT32 US_RSIDE = 3;

/*******LED Outputs*******/
static const UINT32 RED_LED = 8;
static const UINT32 GREEN_LED = 7;

/*****Relay******/
static const UINT32 COMPRESSOR_RELAY = 1;

/******Autonomous Constants*****/

static const int PARTICLE_PERCENT_MIN = 1;
static const int PARTICLE_PERCENT_MAX = 40;
static const float MIN_OFFSET = 0.7;

static const float LEFT = -1.0;
static const float RIGHT =  1.0;

static const int BL = 1;
static const int RD = 2;

/***** Robot Function Prototypes*****/
float ServiceJoysticks(int);
void DriveTrain(float, float);
void ServiceTower(void);
void SingleDrive(float, float);
void SetLED(void);

/***** Autonomous Prototypes *****/
void FollowTarget(float, float, int);
void TrackTarget(int);
int GetAutoMode();
void AutonomousPump();

/***** Report Prototypes *****/
void ColPrintReport(ColorReport*);
void ParPrintReport(ParticleAnalysisReport*);
void ThreshPrintReport(TrackingThreshold*);

/***** I/O Handlers *****/
void SetLED();


/***** Math Prototypes*****/
float abs(float);
float limit(float, float, float);
float absmax(float, float);

#endif
