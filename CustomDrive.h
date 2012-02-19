#ifndef _CUSTOMDRIVE_H_
#define _CUSTOMDRIVE_H_

#define MAX_MOTORS 10

#include "WPILib.h"
#include "math.h"


class CustomDrive
{
private:
	int cd_nJags;
	Jaguar *cd_pMotors[MAX_MOTORS];
	PIDController *cd_pPID[MAX_MOTORS];
	float mot_dir[MAX_MOTORS];
	Encoder *cd_pEncoders[MAX_MOTORS];
	Gyro *gyro;

public:
	CustomDrive(void);
	CustomDrive(int _nJags);
	CustomDrive(int _nJags, int _gyroPort);

	void TankDrive(float _lSpeed, float _rSpeed);
	void TankDrive(Joystick* _lStick, Joystick* _rStick);
	void TankDrive(KinectStick* _lStick, KinectStick* _rStick);

	void WiiTawdDrive(DriverStationEnhancedIO&);

	void ArcadeDrive(float _speed, float _heading);
	void ArcadeDrive(Joystick* _stick);

	void MecanumDrive(float _x, float _y, float _z);
	void MecanumDrive(Joystick* _stick);

	void MecanumTank(float _lx, float _ly, float _rx, float _ry);
	void MecanumTank(Joystick* _stick);

};


#endif
