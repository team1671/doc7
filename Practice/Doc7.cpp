#include "Doc7.h"

#define K 57.295779513082320876798154814105
#define IO (DriverStation::GetInstance()->GetEnhancedIO())

class Doc7 : public SimpleRobot
{
	CustomDrive *cd;
	Joystick *controller, *stick;
	Arm *arm;
	Manipulator *man;
	Compressor *cmp;
	Tracking *track;
	
	Relay *red_white, *blue;
	
	DoubleSolenoid *minibot;

public:
	Doc7(void)
	{
		cd = new CustomDrive(NUM_JAGS);
		controller = new Joystick(DOC7_GAMEPAD);
		stick = new Joystick(DOC7_ARMPAD);
		
		minibot = new DoubleSolenoid(3, 4);
		
		arm = new Arm(DOC7_ARM_JAG, DOC7_ARM_A_CHANNEL, DOC7_ARM_B_CHANNEL, DOC7_LIMIT_SWITCH_PORT);
		man = new Manipulator(DOC7_MAN_JAG, DOC7_MAN_A_CHANNEL, DOC7_MAN_B_CHANNEL, DOC7_SOL_OPEN, DOC7_SOL_CLOSE);
		cmp = new Compressor(DOC7_RELAY_PORT, DOC7_COMPRESSOR_PORT);
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
		cmp->Start();
		cd->MecanumDrive(0, 0, 0);
		minibot->Set(DoubleSolenoid::kForward);
		man->Close();
//		while(IsAutonomous() && IsEnabled())
//		{
//			GetWatchdog().Kill();
//			
//			arm->GoTo(90.0f);
//			man->GoTo(130.0f);
//			
//			cd->MecanumDrive(0, -.4, 0);
//			Wait(.15f);
//			
//			if(IO.GetDigital(11))
//			{
//				while(!track->TrackLine(-1))
//				{
//					if(!IsEnabled()) break;
//				}
//				while(!track->TrackLine(0))
//				{
//					if(!IsEnabled()) break;
//				}
//			};
//			if(IO.GetDigital(13))
//			{
//				while(!track->TrackLine(0))
//				{
//					if(!IsEnabled()) break;
//				}
//			};
//			if(IO.GetDigital(15))
//			{
//				while(!track->TrackLine(1))
//				{
//					if(!IsEnabled()) break;
//				}
//				while(!track->TrackLine(0))
//				{
//					if(!IsEnabled()) break;
//				}
//			};
//			break;
//		};
//		A
//		man->Open();
//		man->GoTo(180.0);
//		cd->MecanumDrive(0, .35, 0);
//		Wait(1.0f);
		
//		man->_bGotoMan = false;
//		arm->_bGoto = false;
//		arm->GoTo(10.0);					//New
//		man->GoTo(10.0);					//New
//		cd->MecanumDrive(0, .35, 0);		//New
//		Wait(.8f);							//New
		cd->MecanumDrive(0, 0, 0);
		cmp->Stop();
		
	};
	
	void OperatorControl(void)
	{
		GetWatchdog().Kill();
		cmp->Start();
		cd->MecanumDrive(0, 0, 0);
		minibot->Set(DoubleSolenoid::kForward);
		while(IsOperatorControl() && IsEnabled())
		{
			GetWatchdog().Kill();
			if(IO.GetDigital(12)) 
			{
				red_white->Set(Relay::kReverse);
			} else if(IO.GetDigital(14))
			{
				red_white->Set(Relay::kForward);
			} else
			{
				red_white->Set(Relay::kOff);
			}
			if(IO.GetDigital(16)) 
			{
				blue->Set(Relay::kReverse);
			}
			else
			{
				blue->Set(Relay::kOff);
			}
			
			if(IO.GetDigital(10)) minibot->Set(DoubleSolenoid::kReverse);
			else if(!IO.GetDigital(10)) minibot->Set(DoubleSolenoid::kForward);
			//cd->MecanumDrive(controller);
			
			cd->MecanumDrive(controller);
			if(stick->GetRawButton(1))
			{
	 			man->GoTo(0.0f);
				arm->GoTo(0.0f);
			}
			else if(stick->GetRawButton(2))
			{
				arm->GoTo(42.0f);
				man->GoTo(39.5f);
				
			}
			else if(stick->GetRawButton(3))
			{
				arm->GoTo(90.0f);
				man->GoTo(75.0f);
			}
			else if(stick->GetRawButton(4))
			{
				printf(" Man: %f \n Arm: %f \n", (man->m_pManEnc->Get() * -1.4), (arm->a_pArmEnc->Get() * 1.4));
				Wait(.2f);
			}
			else
			{
				arm->ArmControl(stick);
				if (stick->GetRawButton(8)) arm->NoPIDArmControl(stick);
				//man->UseJag(stick->GetTwist());
				man->ManipulatorControl(stick);
			}
		};
		cmp->Stop();
	};
};

START_ROBOT_CLASS(Doc7);