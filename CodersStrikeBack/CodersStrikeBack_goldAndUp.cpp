#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <list>
#include <cmath>

#define PI 3.14159265

using namespace std; // sest nii on mugavam

struct My_Vector{
    int x, y;
};

My_Vector operator+( My_Vector const& vector1, My_Vector const& vector2 ){
    My_Vector addition;
    addition.x = vector1.x + vector2.x;
    addition.y = vector1.y + vector2.y;
    return addition;
};

My_Vector operator-( My_Vector const& vector1, My_Vector const& vector2 ){
    My_Vector subtraction;
    subtraction.x = vector1.x - vector2.x;
    subtraction.y = vector1.y - vector2.y;
    return subtraction;
};

My_Vector operator*( My_Vector const& vector1, int scalar ){
    My_Vector product;
    product.x = vector1.x * scalar;
    product.y = vector1.y * scalar;
    return product;
};

My_Vector operator/( My_Vector const& vector1, int divisor ){
    My_Vector quotient;
    quotient.x = vector1.x / divisor;
    quotient.y = vector1.y / divisor;
    return quotient;
};

class Pod {
    public:
        My_Vector position;
        My_Vector speed;
        int angle;
        int nextCheckPointId;
        vector<My_Vector> checkPoints; // all pods use the same checkpoints, static would be best but couldn't initialize

        int pointsPassed = 0;
        int prevNextCheckPointId;

        void SetCheckPoints(vector<My_Vector> checkPoints);
        void CountPassedPoints();
};

class AllyPod : public Pod {
    public:
        My_Vector nextTargetPoint;
        int thrust = 100;
        string thrust_str;

        void ReCalculateRacer(Pod &opponent1, Pod &opponent2);

    protected:
        void SetNextTargetPoint();
        void SetThrust(Pod &opponent1, Pod &opponent2);
        int CheckCollision(Pod const &pod) const;
        int ReachesTarget() const;
};

class AllyPodRam : public AllyPod {
    public:
        void ReCalculateRam(Pod &opponent1, Pod &opponent2);

    private:
        int CheckCollisionRam(Pod const &pod) const;
        void SetThrustRam(Pod &opponent1, Pod &opponent2);
        void SetInterceptPoint(Pod &opponent1, Pod &opponent2);
};


int main()
{

    AllyPod allyPod1;
    AllyPodRam allyPodRam;

    Pod enemyPod1;
    Pod enemyPod2;
    
    int laps;
    int checkPointCount;

    cin >> laps; cin.ignore();
    cin >> checkPointCount; cin.ignore();

    vector<My_Vector> checkPoints(checkPointCount);

    for(int i = 0; i < checkPointCount; i++){
        My_Vector cordinates;
        cin >> cordinates.x >> cordinates.y; cin.ignore();
        checkPoints[i] = cordinates;
    }
    /*vector<My_Vector> checkPoints(checkPointCount * laps);
    for(int i = 0; i < checkPointCount; i++){
        My_Vector cordinates;
        cin >> cordinates.x >> cordinates.y; cin.ignore();
        for(int u = 0; u < laps; u++){
            checkPoints[checkPointCount*u+i] = cordinates; // now next logic doesnt need to use laps
        }
    }*/

    allyPod1.checkPoints = checkPoints;
    allyPodRam.checkPoints = checkPoints;
    enemyPod1.checkPoints = checkPoints;
    enemyPod2.checkPoints = checkPoints;

    while (1) {

        cin >> allyPod1.position.x >> allyPod1.position.y >> allyPod1.speed.x >> allyPod1.speed.y >> allyPod1.angle >> allyPod1.nextCheckPointId; cin.ignore();
        cin >> allyPodRam.position.x >> allyPodRam.position.y >> allyPodRam.speed.x >> allyPodRam.speed.y >> allyPodRam.angle >> allyPodRam.nextCheckPointId; cin.ignore();
        
        cin >> enemyPod1.position.x >> enemyPod1.position.y >> enemyPod1.speed.x >> enemyPod1.speed.y >> enemyPod1.angle >> enemyPod1.nextCheckPointId; cin.ignore();
        cin >> enemyPod2.position.x >> enemyPod2.position.y >> enemyPod2.speed.x >> enemyPod2.speed.y >> enemyPod2.angle >> enemyPod2.nextCheckPointId; cin.ignore();

        allyPod1.ReCalculateRacer(enemyPod1, enemyPod2);
        allyPodRam.ReCalculateRam(enemyPod1, enemyPod2);

        enemyPod1.CountPassedPoints();
        enemyPod2.CountPassedPoints();

        if(allyPod1.thrust == 101){
            cout << allyPod1.nextTargetPoint.x << " " << allyPod1.nextTargetPoint.y << " " << allyPod1.thrust_str << endl;
        }else{
            cout << allyPod1.nextTargetPoint.x << " " << allyPod1.nextTargetPoint.y << " " << allyPod1.thrust << endl;
        }
        if(allyPodRam.thrust == 101){
            cout << allyPodRam.nextTargetPoint.x << " " << allyPodRam.nextTargetPoint.y << " " << allyPodRam.thrust_str << endl;
        }else{
            cout << allyPodRam.nextTargetPoint.x << " " << allyPodRam.nextTargetPoint.y << " " << allyPodRam.thrust << endl;
        }
    }
}

int AllyPod::CheckCollision(Pod const &pod) const {
    int distanceBetweenBots;
    My_Vector positionDif = (position + speed) - (pod.position + pod.speed);

    distanceBetweenBots = sqrt(pow(positionDif.x, 2) // predict position 
                             + pow(positionDif.y, 2)); // can compare distance as square for performance

    if(distanceBetweenBots < 800){ // checks if collision happens next cycle
        My_Vector podPositionNextTurn = position + speed;
        My_Vector enemyPositionNextTurn = pod.position + pod.speed;

        My_Vector allyToEnemyVector = enemyPositionNextTurn - podPositionNextTurn;
        My_Vector vectorToTarget = checkPoints[nextCheckPointId] - podPositionNextTurn;

        My_Vector collisionResult = vectorToTarget + allyToEnemyVector;

        int distanceToTarget = sqrt(pow(vectorToTarget.x, 2) + pow(vectorToTarget.y, 2));
        int distanceToTargetCollision = sqrt(pow(collisionResult.x, 2) + pow(collisionResult.y, 2));
        if(distanceToTarget > distanceToTargetCollision){ // check if collision is worth shielding, i.e 
            return 0;
        }
        return 1;
    }else{
        return 0;
    }
};

int AllyPodRam::CheckCollisionRam(Pod const &pod) const {
    int distanceBetweenBots;
    My_Vector positionDif = (position + speed) - (pod.position + pod.speed);

    distanceBetweenBots = sqrt(pow(positionDif.x, 2) // predict position 
                             + pow(positionDif.y, 2)); // can compare distance as square for performance

    if(distanceBetweenBots < 800){ // checks if collision happens next cycle
        return 1;
    }else{
        return 0;
    }
};

void AllyPod::SetThrust(Pod &opponent1, Pod &opponent2) {
    // ally <-> ally collison is ignored
    // ally <-> opponent collision is shielded
    // opponent <-> opponent collision is ignored
    int angleToNextPoint = (int) (atan2(nextTargetPoint.y - position.y, nextTargetPoint.x - position.x) * 180 / PI + 360) % 360;
    int angleToTurn = (angleToNextPoint - angle + 540) & 360 - 180;

    if (CheckCollision(opponent1) || CheckCollision(opponent2)){
        thrust_str = "SHIELD";
        thrust = 101; // set value to check; if 101 then use thrust_str
    }else if(abs(angleToTurn) > 90){
        thrust = 0;
    }else{
        thrust = 100;
    }
};

void AllyPodRam::SetThrustRam(Pod &opponent1, Pod &opponent2) {
    // ally <-> ally collison is ignored
    // ally <-> opponent collision is shielded
    // opponent <-> opponent collision is ignored
    int angleToNextPoint = (int) (atan2(nextTargetPoint.y - position.y, nextTargetPoint.x - position.x) * 180 / PI + 360) % 360;
    int angleToTurn = (angleToNextPoint - angle + 540) & 360 - 180;

    if (CheckCollisionRam(opponent1) || CheckCollisionRam(opponent2)){
        thrust_str = "SHIELD";
        thrust = 101; // set value to check; if 101 then use thrust_str
    }else if(abs(angleToTurn) > 90){
        thrust = 0;
    }else{
        thrust = 100;
    }
};

void AllyPod::SetNextTargetPoint(){
    if(ReachesTarget()){
        if(checkPoints.size() == nextCheckPointId){
            nextTargetPoint = checkPoints[0];
        }
        nextTargetPoint = checkPoints[nextCheckPointId + 1];
    }else{
        nextTargetPoint = checkPoints[nextCheckPointId] - (speed * 2);
    }
    My_Vector vectorToNextPoint = checkPoints[nextCheckPointId] - position;
    My_Vector vectorToAddToTarget;
    float speedPerpendicularComponent;
    
    float speedLength = sqrt(pow(speed.x, 2) + pow(speed.y, 2));
    float vectorToNextPointLength = sqrt(pow(vectorToNextPoint.x, 2) + pow(vectorToNextPoint.y, 2));
    float vectorLenProduct = speedLength * vectorToNextPointLength;

    if(vectorLenProduct == 0){
        speedPerpendicularComponent = 0;
    }else{
        float angle = acos((vectorToNextPoint.x*speed.x + vectorToNextPoint.y*speed.y) / vectorLenProduct); // angle between 2 vectors formula
        speedPerpendicularComponent = sin(angle) * speedLength;
    }

    int length = speedPerpendicularComponent*10;
    int divisor = 20;
    // make perpendicular vector x;y to -y;x or y;-x

    if(pow(speed.x - vectorToNextPoint.y, 2) + pow(speed.y + vectorToNextPoint.x, 2) < pow(speed.x + vectorToNextPoint.y, 2) + pow(speed.y - vectorToNextPoint.x, 2)){
        vectorToAddToTarget = {-(vectorToNextPoint.y+length)/divisor, (vectorToNextPoint.x+length)/divisor};
    }else{
        vectorToAddToTarget = {(vectorToNextPoint.y+length)/divisor, -(vectorToNextPoint.x+length)/divisor};
    }
    nextTargetPoint = nextTargetPoint + vectorToAddToTarget;
};

void AllyPod::ReCalculateRacer(Pod &opponent1, Pod &opponent2) {
    this->SetNextTargetPoint();
    this->SetThrust(opponent1, opponent2);
    cerr << thrust << endl;

};

void AllyPodRam::ReCalculateRam(Pod &opponent1, Pod &opponent2){
    this->SetThrust(opponent1, opponent2);
    this->SetInterceptPoint(opponent1, opponent2);
}

int AllyPod::ReachesTarget() const{
    My_Vector targetPoint = checkPoints[nextCheckPointId];
    My_Vector currentPosition = position;
    int horizon = 5;

    for(int i = 0; i < horizon; i++){ // TODO should write more logic to predict future position - simulation
        if(pow(currentPosition.x - targetPoint.x, 2) + pow(currentPosition.y - targetPoint.y, 2) < 640000){ // check if pod reaches target; 640000 = pow(800, 2), perhaps better performance
            return 1;
        }else{
            currentPosition = currentPosition + speed;
        }
    }
    return 0; // return 0 if pod doesn't reach target in predicted horizon
}

void Pod::CountPassedPoints(){
    if(prevNextCheckPointId != nextCheckPointId){
        pointsPassed += 1;
        prevNextCheckPointId = nextCheckPointId;
    }
}

void AllyPodRam::SetInterceptPoint(Pod &opponent1, Pod &opponent2){
    Pod leadingEnemy;
    My_Vector enemyTragetPoint;
    My_Vector vectorToAddToTarget;
    int distanceBetweenRamAndEnemy;

    if(opponent1.pointsPassed == opponent2.pointsPassed){ // check which of them is leading
        My_Vector equalTargetPoint = opponent1.checkPoints[opponent1.nextCheckPointId];

        if(pow(opponent1.position.x - equalTargetPoint.x, 2) + pow(opponent1.position.y - equalTargetPoint.y, 2) < pow(opponent2.position.x - equalTargetPoint.x, 2) + pow(opponent2.position.y - equalTargetPoint.y, 2)){ // check which pod is leading
            leadingEnemy = opponent1;
        }else{
            leadingEnemy = opponent2;
        }
    }else if(opponent1.pointsPassed > opponent2.pointsPassed){
        leadingEnemy = opponent1;
    }else{
        leadingEnemy = opponent2;
    }

    enemyTragetPoint = leadingEnemy.checkPoints[leadingEnemy.nextCheckPointId];

    if(pow(position.x - enemyTragetPoint.x, 2) + pow(position.y - enemyTragetPoint.y, 2) < pow(leadingEnemy.position.x - enemyTragetPoint.x, 2) + pow(leadingEnemy.position.y - enemyTragetPoint.y, 2)){
        // ram pod is closer to enemy target -> attempt to intercept enemypod
        distanceBetweenRamAndEnemy = sqrt(pow(leadingEnemy.position.x - position.x, 2) + pow(leadingEnemy.position.y - position.y, 2));

        nextTargetPoint = leadingEnemy.position + leadingEnemy.speed*(distanceBetweenRamAndEnemy/1000);
    }else{
        // ram pod is further from enemy target point -> dont attemp interception, set up ambush between target and next target point
        if(checkPoints.size() == nextCheckPointId){
            nextTargetPoint = (checkPoints[0] + checkPoints[nextCheckPointId]) / 2;
        }else{
            nextTargetPoint = (checkPoints[nextCheckPointId] + checkPoints[nextCheckPointId + 1]) / 2;
        }

        if(sqrt(pow(nextTargetPoint.x - position.x, 2) + pow(nextTargetPoint.y - position.y, 2)) < 150){
            thrust = 0;
        }
    }
}
