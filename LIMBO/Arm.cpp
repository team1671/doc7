#include "Arm.h"
#include <stdio.h>

#define _PID_ARM_

bool _bGoto;
Arm::Arm(void)
{
	a_pArmJag = NULL;
	a_pArmEnc = NULL;
	a_pPosController = NULL;
	a_pLimit = NULL;
};


Arm::Arm(int _armJagPort, int _aChannel, int _bChannel, int _limitPort)
{
	
	a_pArmJag = new Jaguar(_armJagPort);
	a_pArmEnc = new Encoder(_aChannel, _bChannel);
	a_pLimit = new DigitalInput(_limitPort);
	a_pArmEnc->SetReverseDirection(true);
	a_pArmEnc->Start();
#ifdef _PID_ARM_
	a_pPosController = new PIDController(0.2f, 0.0f, 0, a_pArmEnc, a_pArmJag);
	a_pPosController->SetContinuous(true);
//	a_pPosController->SetInputRange(0, 70.0f);
	a_pPosController->SetOutputRange(-0.6f, 0.6f);
#else
	
	a_pArmEnc->Reset();
#endif
	
};
void Arm::GoTo(float _theta)
{
#ifdef _PID_ARM_
	_bGoto = true;
	if(a_pPosController == NULL) return;
	a_pArmEnc->SetPIDSourceParameter(Encoder::kDistance);
	a_pPosController->Enable(); 
	a_pPosController->SetSetpoint(_theta/1.4f);
	if(a_pLimit->Get() == 1) 
	{
		a_pArmEnc->Reset();
	}
#else
	if((a_pArmEnc->GetDistance() * 1.4f)< _theta)
	{
		while((a_pArmEnc->GetDistance() * 1.4f) < _theta)
		{
			a_pArmJag->Set(.5f);
		};
	}
	else if((a_pArmEnc->GetDistance() * 1.4f) > _theta)
	{
		while((a_pArmEnc->GetDistance() * 1.4f) > _theta)
		{
			a_pArmJag->Set(-.3f);
		};
	}
	a_pArmJag->Set(0);
#endif
	
};


void Arm::ArmControl(Joystick *_stick)
{
	if(a_pArmJag == NULL) return;
	if(a_pArmEnc == NULL) return;
	
	if(_stick->GetRawButton(4)) {printf("Arm: %f\n", a_pArmEnc->GetDistance() * 1.4f);}
	
	if(_stick->GetRawButton(10))
	{
#ifdef _PID_ARM_
		if(a_pPosController == NULL) return;
#endif
		a_pArmEnc->Reset();
		
		
	}
	else
	{
#ifdef _PID_ARM_
		if(a_pPosController == NULL) return;
		if(_stick->GetRawButton(11)) _bGoto = false;
		if(_bGoto) return;
		a_pPosController->Enable();
		float newSetPoint = a_pArmEnc->GetDistance() + _stick->GetY() * 3.0;
		a_pPosController->SetSetpoint(newSetPoint);
#else
		a_pArmJag->Set(_stick->GetY() * 0.6f);
		
#endif
	};
};

void Arm::Set(float _speed)
{
	a_pArmJag->Set(_speed);
}
void Arm::Print(void)
{
	printf("%f\n", a_pArmEnc->Get() * -1.4f);
	Wait(0.1);
}



