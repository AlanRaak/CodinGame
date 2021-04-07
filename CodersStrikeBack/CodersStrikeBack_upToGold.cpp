#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <cmath>

#define PI 3.14159265

using namespace std; // sest nii on mugavam

struct Vector{
    int x;
    int y;
};

int collidesWithEnemy(Vector *speed, Vector *position, Vector *opponentSpeed, Vector *opponentPosition);

int main()
{

    // game loop
    int thrust = 100;

    struct Vector prevCords; // should initialise from robot start position

    struct Vector position;

    float speed;
    float prevSpeed;
    struct Vector speedVector = {0, 0};
    struct Vector prevSpeedVector = {0, 0};
    struct Vector accelerationVector = {0, 0};
    float acceleration;

    struct Vector opponentPosition;
    struct Vector prevOpponentPosition;
    struct Vector opponentSpeedVector;

    struct Vector nextCheckPoint;

    struct Vector toTargetVector;
    while (1) {

        int nextCheckpointDist; // distance to the next checkpoint
        int nextCheckpointAngle; // angle between your pod orientation and the direction of the next checkpoint
        cin >> position.x >> position.y >> nextCheckPoint.x >> nextCheckPoint.y >> nextCheckpointDist >> nextCheckpointAngle; cin.ignore();
        toTargetVector = {nextCheckPoint.x - position.x, nextCheckPoint.y - nextCheckPoint.y};

        cin >> opponentPosition.x >> opponentPosition.y; cin.ignore();

        opponentSpeedVector.x = opponentPosition.x - prevOpponentPosition.x; // check oponent movement path for collisions
        opponentSpeedVector.y = opponentPosition.y - prevOpponentPosition.y;

        speedVector.x = position.x - prevCords.x; // divide with delta time perhaps
        speedVector.y = position.y - prevCords.y;

         // change speed vector orientation for better target 

        nextCheckPoint.x -= speedVector.x*2; // take target point so that trajectori is as straight as possible
        nextCheckPoint.y -= speedVector.y*2; // i.e subtract speed vector from target point

        speed = sqrt(pow(speedVector.x, 2) + pow(speedVector.y, 2));

        acceleration = speed - prevSpeed; // divide by dt perhaps

        cerr << "nextdist - " << nextCheckpointDist 
            << "  speed - " << speed 
            << endl
            << "accel - " << acceleration 
            << "  anglerror - " << abs(nextCheckpointAngle)
            << endl; // print debugging values

        if(abs(nextCheckpointAngle) > 90 || nextCheckpointDist / speed < 2){ // thrust and brake  if needed
            thrust = 0;
        }else{
            thrust = 100;
        }

        if(collidesWithEnemy(&speedVector, &position, &opponentSpeedVector, &opponentPosition)){
            cout << nextCheckPoint.x << " " << nextCheckPoint.y << " " << "SHIELD" << endl;
            cerr << "shielding!" << endl; // debugging
        }else if(nextCheckpointDist > 3000 && abs(nextCheckpointAngle) < 5){
            cout << nextCheckPoint.x << " " << nextCheckPoint.y << " " << "BOOST" << endl;
            cerr << "boosting!" << endl; // debugging
        }else{
            cout << nextCheckPoint.x << " " << nextCheckPoint.y << " " << thrust << endl;
        }

        prevSpeed = speed;
        prevCords.x = position.x;
        prevCords.y = position.y;
        prevSpeedVector.x = speedVector.x;
        prevSpeedVector.y = speedVector.y;
        prevOpponentPosition.x = opponentPosition.x;
        prevOpponentPosition.y = opponentPosition.y;

        // Write an action using cout. DON'T FORGET THE "<< endl"
        // To debug: cerr << "Debug messages..." << endl;


        // You have to output the target position
        // followed by the power (0 <= thrust <= 100)
        // i.e.: "x y thrust"
        
    }
}

int collidesWithEnemy(Vector *speed, Vector *position, Vector *opponentSpeed, Vector *opponentPosition){
    struct Vector predictedLocation = *position;
    struct Vector opponentPredictedLocation = *opponentPosition;
   
    int distanceBetweenBots;

    predictedLocation.x += (*speed).x; // would do speed*time, but since speed is positoon change in one cycle then it isnt necessary
    predictedLocation.y += (*speed).y;

    opponentPredictedLocation.x += (*opponentSpeed).x;
    opponentPredictedLocation.y += (*opponentSpeed).y;

    distanceBetweenBots = sqrt(pow((predictedLocation.x - opponentPredictedLocation.x), 2) 
                            + pow((predictedLocation.y - opponentPredictedLocation.y), 2));

    if(distanceBetweenBots < 800){ // checks if collision happens next cycle
        return 1;
    }else{
        return 0;
    }
}