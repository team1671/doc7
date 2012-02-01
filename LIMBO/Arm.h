#ifndef _ARM_H_
#define _ARM_H_

#include "WPILib.h"

class Jaguar;
class Joytick;
class Encoder;

class Arm 
{
private:
	
	Encoder *a_pArmEnc;
	bool Cliche;
	DigitalInput *a_pLimit;
	
public:
	Jaguar *a_pArmJag;
	PIDController *a_pPosController;
	
	Arm(void);
	Arm(int _armJagPort, int _aChannel, int _bChannel, int _limitPort);
	
	void Set(float);
	void Print(void);
	void ArmControl(Joystick *);
	void GoTo(float _theta);

};

#endif

