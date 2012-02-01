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
		
		
		
		
		
		GetWatchdog().Kill();
	};

	
	void Autonomous(void)
	{
		GetWatchdog().Kill();
		cmp->Start();
		float height = 45.0;
		cd->MecanumDrive(0, 0, 0);
		minibot->Set(DoubleSolenoid::kForward);
		while(IsAutonomous() && IsEnabled())
		{
			GetWatchdog().Kill();
			arm->GoTo(100.0);
			
			cd->MecanumDrive(0, -.4, 0);
			Wait(.15f);
			while(!track->TrackLine(1, height));
			cd->MecanumDrive(0, 0.4, 0);
			Wait(0.15);
			break;
		};
		
		cd->MecanumDrive(0, 0, 0);
//		
		man->GoTo(120.0);
		//while(man->m_pManEnc->GetDistance() * 1.4 < 42.0);
//		cd->MecanumDrive(0, 0.3, 0);
		Wait(2.5f);
		man->Open();
		man->GoTo(150.0);
		cd->MecanumDrive(0, .35, 0);
		Wait(1.5f);
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
			if(controller->GetRawButton(5)) minibot->Set(DoubleSolenoid::kForward);
			else if(controller->GetRawButton(7)) minibot->Set(DoubleSolenoid::kReverse);
			//cd->MecanumDrive(controller);
			if(controller->GetRawButton(3))	cd->TestDrive();
			
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
			else
			{
				arm->ArmControl(stick);
				man->ManipulatorControl(stick);
			}	
		};
		cmp->Stop();
	};
};

START_ROBOT_CLASS(Doc7);



