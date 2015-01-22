/*
Lego NXT Mindstorm Project
By: Krisztian K., Anton D., Alex M., and Marco D.

"Marco" SETUP:
	Motor A: Vertical Pulley Motor
	Motor B: Claw Motor
	S1: Touch Sensor 1 (On Claw)
	S2: Touch Sensor 2 (On Claw)

This is the code which will control the tower and claw motors.

*/

void grasp(int & encoderValue)
{
		//Lower the grasper
		motor[motorA] = 40;

		//Wait until touch sensor activates (or put in safety threshold)
		while(SensorValue[S1] == 0 && SensorValue[S2] == 0);

		motor[motorA] = 0;

		//Save the encoder value and reset
		encoderValue = nMotorEncoder[motorA];
		nMotorEncoder[motorA] = 0;

		//Close the grasper
		motor[motorB] = 10;
		time1[0] = 0;
		while(time1[0] < 1500);

		//Apply constant pressure
		motor[motorB] = 10;

		//Raise claw and stop at top
		motor[motorA] = -40;
		while(nMotorEncoder[motorA] > -encoderValue);
		motor[motorA] = 0;

		//Reset encoder
		nMotorEncoder[motorA] = 0;
}

void release(int lowerValue)
{

		//Lower the grasper
		nMotorEncoder[motorA] = 0;
		motor[motorA] = 40;
		//Wait until the lower value is reached
		while(nMotorEncoder[motorA] < lowerValue);
		motor[motorA] = 0;

		//Open claw
		motor[motorB] = -20;
		time1[0] = 0;
		while(time1[0] < 1200);
		motor[motorB] = 0;

		//Raise claw
		nMotorEncoder[motorA] = 0;
		motor[motorA] = -40;
		//Wait till it is fully raised
		while(nMotorEncoder[motorA] > -lowerValue);
		motor[motorA] = 0;

		//Play sound once finished
		PlaySound(soundBeepBeep);

		//Reset encoder
		nMotorEncoder[motorA] = 0;
}

task main()
{

	//Initialize touch sensors
	SensorType[S1] = sensorTouch;
	SensorType[S2] = sensorTouch;

	//The value the encoder calculates when it lowers
	int encoderValue = 0;

	nMotorEncoder[motorA] = 0;
	nMotorEncoder[motorB] = 0;

	ubyte IncomingMessage[1] = {0};

	bool check = true;

	while(check)
	{

		cCmdMessageRead(IncomingMessage, 1, mailbox1);
		wait1Msec(3);

		//If the message is to lower the grasper
		if(IncomingMessage[0] == 1)
		{
				grasp(encoderValue);
				IncomingMessage[0] = 0;
		}

		if(IncomingMessage[0] == 2)
		{
				release(encoderValue);
				IncomingMessage[0] = 0;
		}

		if(IncomingMessage[0] == 3)
		{
				check = false;
		}
	}
}
