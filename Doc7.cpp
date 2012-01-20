#include "Doc7.h"
#include "WPILib.h"
#include "Joystick.h"
#include <IOstream.h>

#define K 57.295779513082320876798154814105
#define IO (DriverStation::GetInstance()->GetEnhancedIO())


//                                        WARNING
//!!!!!!!!!!!!!!!!!!                !!!!!!!!!!!!!!!!!!!!             !!!!!!!!!!!!!!!!!!!!!
//before use, see line 41
//		game = false;//set to true or false before use- true for auton & tele-op, false for just 1




class RobotDemo : public SimpleRobot
{

	///declaritories
	
	CustomDrive *cd;
	Joystick *stick, *controller;
	Compressor *cmp;
	Tracking *track;

	Relay *red_white, *blue;

	Jaguar *manJag, *ShoulderJag;
	Solenoid *open , *closed;
	Encoder *manEnc;
	bool reset, game;
	int  output;
	bool AlmostZeroPi;
	DoubleSolenoid *minibot;
public:
	RobotDemo(void)
	{	
		
		///constructs
		game = false;//set to true or false before use- true for auton & tele-op, false for just 1
		
		cd = new CustomDrive(NUM_JAGS);
		
		closed = new Solenoid(8, 1);//true if closed
		open = new Solenoid(8, 2);

		stick = new Joystick(2);// arm controll
		controller = new Joystick(1);// base
		
		manEnc = new Encoder(4,5);
		
		reset = false;//if button 
		
		manJag = new Jaguar(6);//cons manip Jag
		ShoulderJag = new Jaguar(5);
		
		
		minibot = new DoubleSolenoid(3, 4);cmp = new Compressor(DOC7_RELAY_PORT, DOC7_COMPRESSOR_PORT);
		track = new Tracking(DOC7_LEFT_LINE_PORT, DOC7_CENTER_LINE_PORT, DOC7_RIGHT_LINE_PORT);

		red_white = new Relay(DOC7_RED_WHITE_SPIKE);
		red_white->SetDirection(Relay::kBothDirections);
		red_white->Set(Relay::kOff);
		blue = new Relay(DOC7_BLUE_SPIKE);
		blue->SetDirection(Relay::kBothDirections);
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
			GetWatchdog().Kill();
			
			AlmostZeroPi = manEnc->GetDistance() -1;//Zeros encoder on manipulator with margin for error

/*    Before including below, check line tracking is proper, then fix below code for bottom scoring
			cd->MecanumDrive(0, -.4, 0);
			Wait(.15f);
			if(IO.GetDigital(11))
			{
				while(!track->TrackLine(-1))
				{
					if(!IsEnabled()) break;
				}
				while(!track->TrackLine(0))
				{
					if(!IsEnabled()) break;
				}
			};
			if(IO.GetDigital(13))
			{
				while(!track->TrackLine(0))
				{
					if(!IsEnabled()) break;
				}
			};
			if(IO.GetDigital(15))
			{
				while(!track->TrackLine(1))
				{
					if(!IsEnabled()) break;
				}
				while(!track->TrackLine(0))
				{
					if(!IsEnabled()) break;
				}
			*/};/*
			break;
		};
		//man open && score here
		cd->MecanumDrive(0, .35, 0);
		Wait(1.0f);
		//move manipulator to 0
		cd->MecanumDrive(0, .35, 0);	
		Wait(.8f);
		cd->MecanumDrive(0, 0, 0);*/
		
		if (!game){//if you arent in the game, kill compressor and brings manipulator to 0 so you dont need to reset
			cmp->Stop();
			while (manEnc->GetDistance() < AlmostZeroPi)
			{
				manJag->Set(.1);
			}
		}
	}

	void OperatorControl(void)
	{
		// feed watchdog ---    TheVessel.SetSafetyEnabled(true);
		GetWatchdog().Kill();
		
		if (!game){
		AlmostZeroPi = manEnc->GetDistance() -1;//Zeros manipulator encoder-- only here for tele op practice w/out auto
		cmp->Start();
		}
		
		cd->MecanumDrive(0, 0, 0);
		minibot->Set(DoubleSolenoid::kForward);
		while(IsOperatorControl() && IsEnabled())
		{
			GetWatchdog().Kill();
			
			if(IO.GetDigital(10)) minibot->Set(DoubleSolenoid::kReverse);
			else if(!IO.GetDigital(10)) minibot->Set(DoubleSolenoid::kForward);
		
			cd->MecanumDrive(controller);//passes joystick controller's input to cd mechdrv
			
			if(stick->GetRawButton(1))
			{
			}
			else if(stick->GetRawButton(2))
			{
			}
			else if(stick->GetRawButton(3))
			{
				reset = true;//              ENABLES NO RESTRICTION MANIPULATOR MOVEMENT
			}
			else if(stick->GetRawButton(4))
			{
				cout << "Manipulator Encoder Value:   " << manEnc->Get() << "/n";//prints manip encoder to console if open;
				Wait(.1f);//pause so not above a billion times
			}
			else if(stick->GetRawButton(6))
			{
				open->Set(true);//self explanatory, eote pnuematics
				closed->Set(false);
			}
			else if(stick->GetRawButton(8))
			{
				closed->Set(true);
				open->Set(false);
			}
			
			

			ShoulderJag->Set(stick->GetY());//shoulder moves with stick->GetY()
			if (stick->GetTwist() < 0 || manEnc->GetDistance() < AlmostZeroPi || reset)//man extending, enc not at 0, or reset true
			{
				manJag->Set(stick->GetTwist()*.3);//shoulder above but for wrist, also, only 3/10ths power
			}

		
		}
		cmp->Stop();//stops compressor
		while (manEnc->GetDistance() < AlmostZeroPi)//brings manipulator back to 0 so not need reset
		{
			manJag->Set(.1);
		}
	}
};

START_ROBOT_CLASS(RobotDemo);
