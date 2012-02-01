#include "Tracking.h"

  
  
Tracking::Tracking(void)
{
	drive = NULL;
	armEnc = NULL;
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

bool Tracking::TrackLine(int _dir, float _angle)
{
	float _speed = (_dir != 0) ? .5f : .35f;
	
		if(!(left->Get()) && center->Get() && !(right->Get()))
			{
				drive->MecanumDrive(0, -_speed, 0);
				return false;
			}
		else if((left->Get()) && !(center->Get()) && (right->Get()))
			{
				if(_dir == 0) 
					{
					drive->MecanumDrive(0, 0, 0);
					}
				else if(_dir == 1)
					{
					drive->MecanumDrive(0, -0.3f, 0);
					Wait(0.2f);
					while(!(right->Get())) 
						{
						drive->MecanumDrive(-0.6, 0, -0.3);
						};
					drive->MecanumDrive(0, -.5, 0);
					Wait(.7f);
					}
				else if(_dir == -1) 
					{
					drive->MecanumDrive(0, -0.3, 0);
					Wait(0.2f);
					while(!(left->Get()))
						{
						drive->MecanumDrive(0.6, 0, 0.3);
						};
					drive->MecanumDrive(0, -.5, 0);
					Wait(.6f);
					}
				return true;
			}
		else if(left->Get())
			{
				drive->MecanumDrive(_speed, -(_speed - .1), 0);
				return false;
			}
		else if(right->Get())
			{
				drive->MecanumDrive(-_speed, -(_speed -.1), 0);
				return false;
			}
	return false;
	
	
};





