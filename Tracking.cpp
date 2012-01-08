#include "Tracking.h"



Tracking::Tracking(void)
{
	drive = NULL;
	
	left = NULL;
	center = NULL;
	right = NULL;
	ds = NULL;
};

Tracking::Tracking(int _leftPort, int _centerPort, int _rightPort)
{
	drive = new CustomDrive(4, 1);
	left = new DigitalInput(6);
	center = new DigitalInput(7);
	right = new DigitalInput(8);
	ds = DriverStation::GetInstance();
};

bool Tracking::TrackLine(int _dir)
{
	float _speed = (_dir != 0) ? .5f : .35f;

		if(!(left->Get()) && center->Get() && !(right->Get()))
			{
				drive->MecanumDrive(0, -_speed, 0);
				Wait(0.05f);
				return false;
			}
		else if((left->Get()) && (center->Get()) && (right->Get()))
			{
				if(_dir == 0)
					{
					drive->MecanumDrive(0, 0, 0);
					}
				else if(_dir == 1)
					{
					drive->MecanumDrive(0, 0, 0);
					}
				else if(_dir == -1)
					{
					drive->MecanumDrive(0, 0, 0);
					}
				return true;
			}
		else if(left->Get() && !(right->Get()))
			{
				drive->MecanumDrive(-_speed, -_speed, 0);
				Wait(0.05f);
				return false;
			}
		else if(right->Get() && !(left->Get()))
			{
				drive->MecanumDrive(_speed, -_speed, 0);
				Wait(0.05f);
				return false;
			}
		else if(!(left->Get()) && !(center->Get()) && !(right->Get()))
			{
				if(_dir == 0)
					{
					drive->MecanumDrive(0, 0, 0);
					}
				else if(_dir == 1)
					{
					drive->MecanumDrive(0, 0, 0);
					}
				else if(_dir == -1)
					{
					drive->MecanumDrive(0, 0, 0);
					}
				return true;
			}
	return false;


};
