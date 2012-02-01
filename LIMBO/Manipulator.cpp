#include "Manipulator.h"
#include <stdio.h>

#define _PID_MAN_

bool isOpen;
bool _bGotoMan;

class Jaguar;
class Joystick;
class Encoder;

Manipulator::Manipulator(void)
{
	m_pManJag = NULL;
	m_pManEnc = NULL;
	m_pSolOpen = NULL;
	m_pSolClose = NULL;
	isOpen = false;
	
	m_pPosController = NULL;
};

Manipulator::Manipulator(int _manJagPort, int _aChannel, int _bChannel, int _solOpenPort, int _solClosePort)
{
	m_pManJag = new Jaguar(_manJagPort);
	m_pManEnc = new Encoder(_aChannel, _bChannel);
	m_pSolOpen = new Solenoid(8, _solOpenPort);
	m_pSolClose = new Solenoid(8, _solClosePort);
	isOpen = false;
	
	m_pManEnc->SetReverseDirection(true);
	m_pManEnc->Start();
	m_pManEnc->Reset();
	m_pSolOpen->Set(false);
	m_pSolClose->Set(true);
	
#ifdef _PID_MAN_
	m_pPosController = new PIDController(0.2f, 0.0f, 0, m_pManEnc, m_pManJag);
	m_pPosController->SetContinuous(true);
//	m_pPosController->SetInputRange(0, 70.0f);
	m_pPosController->SetOutputRange(-0.5f, 0.5f);
#else
#endif
};

void Manipulator::GoTo(float _theta)
{
#ifdef _PID_MAN_
	if(m_pPosController == NULL) return;
	_bGotoMan = true;
	m_pManEnc->SetPIDSourceParameter(Encoder::kDistance);
	m_pPosController->Enable(); 
	m_pPosController->SetSetpoint(-_theta/1.4f);
#else
	_theta = -_theta;
	if((m_pManEnc->GetDistance() * 1.4f) > _theta)
	{
		while((m_pManEnc->GetDistance() * 1.4f) > _theta)
		{
			m_pManJag->Set(-.5f);
		};
	}
	else if((m_pManEnc->GetDistance() * 1.4f) < _theta)
	{
		while((m_pManEnc->GetDistance() * 1.4f) < _theta)
		{
			m_pManJag->Set(.5f);
		};
	}
	m_pManJag->Set(0);
#endif
};

void Manipulator::Open(void)
{
	if(isOpen) return;
	m_pSolOpen->Set(true);
	m_pSolClose->Set(false);
	isOpen = true;
};

void Manipulator::Close(void)
{
	if(!isOpen) return;
	m_pSolOpen->Set(false);
	m_pSolClose->Set(true);
	isOpen = false;
};

void Manipulator::ManipulatorControl(Joystick *_stick)
{
	if(m_pManJag == NULL) return;
	
#ifdef _PID_MAN_
		if(m_pPosController == NULL) return;
		if(_stick->GetRawButton(11)) _bGotoMan = false;
		if(_bGotoMan) return;
		m_pManEnc->SetPIDSourceParameter(Encoder::kDistance);
		m_pPosController->Enable();
		float newSetPoint = m_pManEnc->GetDistance() + 3 * _stick->GetTwist();
		m_pPosController->SetSetpoint(newSetPoint);
		
		
#else
	m_pManJag->Set(_stick->GetTwist());
#endif

	if(_stick->GetRawButton(4)) {printf("Man: %f\n", m_pManEnc->GetDistance() * 1.4f); Wait(0.5);}
	if(m_pSolOpen == NULL || m_pSolClose == NULL) return;
	
	if(_stick->GetRawButton(6))
	{
		Open();
	}
	else if(_stick->GetRawButton(8))
	{
		Close();
	}
	
	if(_stick->GetRawButton(10)) m_pManEnc->Reset();
	
	
};

