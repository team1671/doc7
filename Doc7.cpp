#include "Doc7.h"
#include "WPILib.h"
#include "Joystick.h"
#include "CustomDrive.h"
#include <IOstream.h>

#define K 57.295779513082320876798154814105
#define IO (DriverStation::GetInstance()->GetEnhancedIO())


//                                        WARNING
//!!!!!!!!!!!!!!!!!!                !!!!!!!!!!!!!!!!!!!!             !!!!!!!!!!!!!!!!!!!!!
//before use, see line 41
//		game = false;//set to true or false before use- true for auton & tele-op, false for just 1




class Doc7_2012Season : public SimpleRobot
{

	///declaritories
	
	DriverStationLCD *driverOut;
	
	CustomDrive *cd;
	Joystick *stick, *controller;
	
	KinectStick *leftarm, *rightarm;
	
	Compressor *cmp;

	Relay *red_white, *blue;

	Jaguar *manJag, *ShoulderJag;
	Encoder *manEnc;

	DoubleSolenoid *minibot, *claw;
public:
	Doc7_2012Season(void)
	{	
		
		///Constructs
		driverOut = DriverStationLCD::GetInstance();
		
		cd = new CustomDrive(NUM_JAGS);

		stick = new Joystick(DOC7_STICK_MANIPULATOR);// arm controll
		controller = new Joystick(DOC7_STICK_DRIVE);// base
		
		leftarm = new KinectStick (1);
		rightarm = new KinectStick (2);
		
		manEnc = new Encoder(DOC7_MAN_A_CHANNEL, DOC7_MAN_B_CHANNEL);
				
		manJag = new Jaguar(DOC7_MAN_PWM_PORT);//cons manip Jag
		ShoulderJag = new Jaguar(DOC7_SHOULDER_PWM_PORT);
		
		minibot = new DoubleSolenoid(DOC7_DOUBLE_SOL_A_PORT, DOC7_DOUBLE_SOL_B_PORT);
		claw = new DoubleSolenoid(1,2);
		
		cmp = new Compressor(DOC7_RELAY_PORT, DOC7_COMPRESSOR_PORT);
		
		red_white = new Relay(DOC7_RED_WHITE_SPIKE,Relay::kBothDirections);		
		red_white->Set(Relay::kOff);
		
		blue = new Relay(DOC7_BLUE_SPIKE,Relay::kBothDirections);
		blue->Set(Relay::kOff);

		GetWatchdog().Kill();
	};
	
	void Autonomous(void)
	{
		GetWatchdog().Kill();
		
		cmp->Start();//compressor
		cd->MecanumDrive(0, 0, 0);
		minibot->Set(DoubleSolenoid::kForward);
		
		while(IsAutonomous() && IsEnabled())
		{
		}
		cmp->Stop();
	}

	void OperatorControl(void)
	{
		// feed watchdog ---    TheVessel.SetSafetyEnabled(true);
		GetWatchdog().Kill();
		
		cmp->Start();
		cd->MecanumDrive(0, 0, 0);
		minibot->Set(DoubleSolenoid::kForward);
		while(IsOperatorControl() && IsEnabled())
		{
			GetWatchdog().Kill();

			cd->MecanumDrive(controller);//passes joystick controller's input to cd mechdrv
			
			manJag->Set(stick->GetTwist() * 0.5);
			ShoulderJag->Set(stick->GetY() * 0.5);
			
			if(stick->GetRawButton(1))
			{
			}
			else if(stick->GetRawButton(2))
			{
			}
			else if(stick->GetRawButton(3))
			{
			}
			else if(stick->GetRawButton(4))
			{
			}
			else if(stick->GetRawButton(5))
			{
				minibot->Set(DoubleSolenoid::kReverse);
			}
			else if(stick->GetRawButton(6))
			{
				claw->Set(DoubleSolenoid::kReverse);
			}
			else if(stick->GetRawButton(7))
			{
			}
			else if(stick->GetRawButton(8))
			{
				claw->Set(DoubleSolenoid::kForward);
			}
			else if(stick->GetRawButton(9))
			{
			}
			else if(stick->GetRawButton(10))
			{
			}
			else if(stick->GetRawButton(11))
			{
			}
			else if(stick->GetRawButton(12))
			{
			}
			else 
			{
				minibot->Set(DoubleSolenoid::kForward);
			}
		}
		cmp->Stop();//stops compressor
	}
};

START_ROBOT_CLASS(Doc7_2012Season);
