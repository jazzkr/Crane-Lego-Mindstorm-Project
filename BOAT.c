/*
Lego NXT Mindstorm Project
By: Krisztian K., Anton D., Alex M., and Marco D.

"NXT" SETUP:
	Motor A: Right Motor
	Motor B: Left Motor
	S1: Ultrasonic Sensor

This is the code that will control the base functions, including moving and scanning with the ultrasonic sensor.

*/

//This function is to make the robot go in a full 360 circle
//which is necessary when it is scanning with the ultrasonic sensor
//WRITTEN BY: Marco D.
int motorScan()
{
     motor[motorA] = 25;
     motor[motorB] = -25;
     nMotorEncoder[motorA] = 0;

     // Minimum distance initialization
     int minimumDistance = 999;
     int sonarValue = 0;

     while(nMotorEncoder[motorA] < 3650) {
         sonarValue = SensorValue[S1];
         nxtDisplayString(1, "%d", sonarValue);
         if (sonarValue < minimumDistance + 15) {
            minimumDistance = sonarValue;
            PlaySound(soundShortBlip);
         }

         if(minimumDistance >= 255 && nMotorEncoder[motorA] > 3400) nMotorEncoder[motorA] = 0;

     }

     motor[motorA] = 0;
     motor[motorB] = 0;

     return minimumDistance;
}

// This function is to make the robot align itself with the obstacle
// determined to be the shortest distance from the robot

void alignRobot(int & distanceValue, int & turnValue) {

		 PlaySound(soundBeepBeep);
		 //start spinning
		 nMotorEncoder[motorA] = 0;
     motor[motorA] = 35;
     motor[motorB] = -35;

     bool foundObject = false;
     int sonarValue = 0;


     while (!foundObject) {

       sonarValue = SensorValue[S1];

       if (sonarValue < distanceValue + 15 && sonarValue > distanceValue - 15) {
             foundObject = true;
       }
     }

     motor[motorA] = 0;
     motor[motorB] = 0;

     //PlaySound(soundBeepBeep);
     //wait10Msec(400);
     //It has now found the object

     turnValue += nMotorEncoder[motorA];
		 nMotorEncoder[motorA] = 0;

     //KRISZTIANS ADDITION (TEST)
     /*
     motor[motorA] = 10;
     motor[motorB] = -10;

     while(SensorValue[S1] < distanceValue + 10 && SensorValue[S1] > distanceValue - 10);

     motor[motorA] = 0;
     motor[motorB] = 0;
     wait10Msec(200);

     int encoderWidthOfObject = nMotorEncoder[motorA];
     int centreOfObject = encoderWidthOfObject/3;

     motor[motorA] = -10;
     motor[motorB] = 10;

     while(nMotorEncoder[motorA] > centreOfObject);

     motor[motorA] = 0;
     motor[motorB] = 0;
     */
     //KRISZTIANS ADDITION END (TEST)

     //MARCO'S INITIAL ADJUSTMENT
     //Small adjustment to centre robot with object (hardcoded...)
     motor[motorA] = 10;
     motor[motorB] = -10;
     while(nMotorEncoder[motorA] < 130);

     // Possibly add fail-safe if the object is removed?

     //turnValue += nMotorEncoder[motorA];
     //return sonarValue;
}

void moveToObject(int distanceValue) {

		nMotorEncoder[motorA] = 0;

		motor[motorA] = 35;
		motor[motorB] = 35;

		//Move forward a set amount depending on how far away the object is
		while (nMotorEncoder[motorA] < ((distanceValue-10) * 120) / PI);

		motor[motorA] = 0;
		motor[motorB] = 0;
}


void returnToBase(int distanceToObject, int spinValue)
{
		//Spin
		nMotorEncoder[motorA] = 0;
		motor[motorA] = 35;
		motor[motorB] = -35;

		while(nMotorEncoder[motorA] < 1890);

		nMotorEncoder[motorA] = 0;

		motor[motorA] = 35;
		motor[motorB] = 35;

		int distanceToTravel = (120 * distanceToObject) / PI;

		while(nMotorEncoder[motorA] < distanceToTravel);

		motor[motorA] = 0;
		motor[motorB] = 0;
}

void moveBack(int timems)
{
		motor[motorA] = -25;
		motor[motorB] = -25;

		wait1Msec(timems);

		motor[motorA] = 0;
		motor[motorB] = 0;
}

task main()
{
  SensorType[S1] = sensorSONAR;

	int closestObstacleValue = motorScan();

	int returnValue = 0;

	nxtDisplayString(3, "%d", closestObstacleValue);

	wait1Msec(1000);

	alignRobot(closestObstacleValue, returnValue);

	moveToObject(closestObstacleValue);

	ubyte OutgoingMessage[1] = {1};

	//Connect bluetooth
	btConnect(1, "Marco");

	//Send bluetooth command to claw
	cCmdMessageWriteToBluetooth(OutgoingMessage, 1, mailbox1);
	wait1Msec(5);

	//Wait until claw sequence is done
	wait1Msec(8000);

	//Return to base
	returnToBase(closestObstacleValue, returnValue);

	//Send message to lower the object
	OutgoingMessage[0] = 2;
	cCmdMessageWriteToBluetooth(OutgoingMessage, 1, mailbox1);
	wait1Msec(5);

	//Wait until drop sequence is done
	wait1Msec(5000);
	moveBack(1000);

	//End Program
	OutgoingMessage[0] = 3;
	cCmdMessageWriteToBluetooth(OutgoingMessage, 1, mailbox1);
	wait1Msec(5);
}
