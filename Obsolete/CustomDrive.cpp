 #include "CustomDrive.h"

//When assigning motors, ODD is LEFT; EVEN is RIGHT

CustomDrive::CustomDrive(void)
{
	//By default, the array is set to all 0's
	cd_nJags = 0;
	for(register int i = 0; i < MAX_MOTORS; ++i)
	{
		cd_pMotors[i] = 0;
	};
	gyro = NULL;
};

CustomDrive::CustomDrive(int _nJags)
{
	//Assigns the number of jags to the ports.
	cd_nJags = _nJags;
	for(register int i = 0; i < _nJags; ++i)
	{
		cd_pMotors[i] = new Jaguar(i+1);
	};

};


CustomDrive::CustomDrive(int _nJags, int _gyroPort)
{
	//Assigns the number of jags to the ports.
	cd_nJags = _nJags;
	for(register int i = 0; i < _nJags; ++i)
	{
		cd_pMotors[i] = new Jaguar(i+1);
	};
	gyro = new Gyro(_gyroPort);
};

//*****TANK DRIVE*****

inline void CustomDrive::TankDrive(float _lSpeed, float _rSpeed)
{
	//Sets motor speeds the the arguments in the function
	for(register int i = 0; i < cd_nJags; i+=2)
	{
		cd_pMotors[i]->Set(_rSpeed);
		cd_pMotors[i+1]->Set(_lSpeed);
	};
};

inline void CustomDrive::TankDrive(Joystick* _lStick, Joystick* _rStick)
{
	//Calls the previous function and inserts the Joystick pointers as the arguments for the function
	TankDrive(_lStick->GetY(), _rStick->GetY());
};

inline void CustomDrive::WiiTawdDrive(DriverStationEnhancedIO& _eds)
{
	(_eds.GetAcceleration(DriverStationEnhancedIO::kAccelX), _eds.GetAcceleration(DriverStationEnhancedIO::kAccelY), 0);
};

//*****ARCADE DRIVE*****

inline void CustomDrive::ArcadeDrive(float _speed, float _heading)
{
	for(register int i = 0; i < cd_nJags; i += 2)
		{

			cd_pMotors[i]->Set(_speed*(cos(_heading)+sin(_heading)));
			cd_pMotors[i+1]->Set(_speed*(-cos(_heading)+sin(_heading)));
		};
};

inline void CustomDrive::ArcadeDrive(Joystick* _stick)
{
	//Declare right and left speed floats, uses math to assign the values
	float right, left;
			right = -(_stick->GetY() + _stick->GetX());
			left = -(_stick->GetY() - _stick->GetX());

	for(register int i = 0; i < cd_nJags; i += 2)
	{
		//Assigns even jags to right and odd jags to left
		cd_pMotors[i]->Set(right* mot_dir[i]);
		cd_pMotors[i+1]->Set(left* mot_dir[i]);


	};

};

//*****MECANUM DRIVE*****

void CustomDrive::MecanumDrive(Joystick* _stick)
{
	float x,y,z;
	if(_stick->GetRawButton(8)) {
		x = _stick->GetX() * _stick->GetX() * _stick->GetX() *.6;		//Ghetto acceleration curve
		y = _stick->GetY() * _stick->GetY() * _stick->GetY() *.6;
		z = _stick->GetZ() * _stick->GetZ() * _stick->GetZ() *.6;
	}

	else {
		x = _stick->GetX() * _stick->GetX() * _stick->GetX();		//Ghetto acceleration curve
		y = _stick->GetY() * _stick->GetY() * _stick->GetY();
		z = _stick->GetZ() * _stick->GetZ() * _stick->GetZ();
	}

    MecanumDrive(x, y, z);

};

void CustomDrive::MecanumDrive(float _x, float _y, float _z)
{
	if(cd_nJags < 4) return;

	float _l1, _l2, _r1, _r2;
	_l1 = _l2 = _r1 = _r2 = _y;

	_l1 = -_x + _y -_z;
	_r1 = _x + _y + _z;
	_l2 = _x +_y - _z;
	_r2 = -_x + _y + _z;

	cd_pMotors[0]->Set((-_l1) * 1.3);//1.3
	cd_pMotors[1]->Set((-_r1) * 1.3);
	cd_pMotors[2]->Set((-_r2) * 1.3);
	cd_pMotors[3]->Set((-_l2) * 1.3);

};


//*****MECANUM TANK*****


void CustomDrive::MecanumTank(float _lx, float _ly, float _rx, float _ry)
{
	if(cd_nJags < 4) return;

	float _l1, _l2, _r1, _r2;

	_l1 = _l2 = _ly;
	_r1 = _r2 = _ry;

	_l1 = -_lx + _ly;
	_l2 = _lx + _ly;
	_r1 = -_rx + _ry;
	_r2 = _rx + _ry;

	cd_pMotors[0]->Set((-_l1));
	cd_pMotors[1]->Set((-_r1));
	cd_pMotors[2]->Set((-_r2));
	cd_pMotors[3]->Set((-_l2));

};

void CustomDrive::MecanumTank(Joystick* _stick)
{
	if(_stick->GetRawButton(6))
		{
		MecanumTank((_stick->GetX()), (_stick->GetY()), (_stick->GetZ()), (_stick->GetTwist()));
		}
	else
		{
		MecanumTank((_stick->GetX() * .5), (_stick->GetY() * .5), (_stick->GetZ() * .5), (_stick->GetTwist() * .5));
		};
};
