#ifndef _MANIPULATOR_H_
#define _MANIPULATOR_H_


#include "WPILib.h"

class Jaguar;
class Joystick;
class Encoder;

class Manipulator
{
private:
	Jaguar *m_pManJag;
	
	Solenoid *m_pSolOpen, *m_pSolClose;
	PIDController *m_pPosController;
public:
	Manipulator(void);
	Manipulator(int _manJagPort, int _aChannel, int _bChannel, int _solOpenPort, int _solClosePort);
	Encoder *m_pManEnc;
	void Open(void);
	void Close(void);
	void GoTo(float _theta);
	void ManipulatorControl(Joystick *);
		
};

#endif

