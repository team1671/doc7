#ifndef _TRACKING_H_
#define _TRACKING_H_

#include "WPILib.h"
#include "CustomDrive.h"
#include "Arm.h"

class Jaguar;
class DigitalInput;
class Encoder;
class CustomDrive;
class DriverStation;


class Tracking : public CustomDrive
{
private:
	CustomDrive *drive;
	
	DigitalInput *left;
	DigitalInput *center;
	DigitalInput *right;
	Encoder *armEnc;
	
	DriverStation *ds;
	
public:
	Tracking(void);
	Tracking(int _leftSensorPort, int _centerSensorPort, int _rightSensorPort);
	
	bool TrackLine(int _dir, float _angle);
	
};
#endif

