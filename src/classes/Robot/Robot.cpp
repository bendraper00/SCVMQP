#include "Robot.h"

Robot::Robot(){}

void Robot::init(){
    this->encoders = new Encoder(FRONT_ENCODER_A, FRONT_ENCODER_B, REAR_ENCODER_A, REAR_ENCODER_B);
    this->encoders->init();
    this->frontDrive = new DriveModule(FRONT_DRIVE_EN, 
                                       FRONT_DRIVE_IN1, 
                                       FRONT_DRIVE_IN2, 
                                       this->encoders, 
                                       FRONT_DRIVE_SERVO);
    this->rearDrive = new DriveModule(REAR_DRIVE_EN, 
                                       REAR_DRIVE_IN1, 
                                       REAR_DRIVE_IN2, 
                                       this->encoders, 
                                       REAR_DRIVE_SERVO);
    this->frontDrive->init();
    Serial.println("Front Drive Module Initialized");
    this->rearDrive->init();
    Serial.println("Rear Drive Module Initialized");
    this->sensors = new Sensors();
    this->sensors->init();
    this->scissors = new Scissors();
    this->scissors->init();
    Serial.println("Scissor Lifts Initialized");
    this->stagePID = new PIDController(PID_SCISSOR_KP, PID_SCISSOR_KI, PID_SCISSOR_KD);
    this->allignPID = new PIDController(PID_ALLIGN_KP, PID_ALLIGN_KI, PID_ALLIGN_KD);
    this->distPID = new PIDController(PID_DIST_KP, PID_DIST_KI, PID_DIST_KD);

    pinMode(FRONT_WHEEL_SWITCH, INPUT_PULLUP);
    pinMode(REAR_WHEEL_SWITCH, INPUT_PULLUP);
    pinMode(BUMP_LEFT, INPUT_PULLUP);
    pinMode(BUMP_RIGHT, INPUT_PULLUP);

}

void Robot::pidSpeed(int16_t speed){ //This may be better if the two wheels try to maintain equal counts rather than speeds. Will determine in testing
    this->frontDrive->pidFSpeed(speed);
    this->rearDrive->pidRSpeed(speed);
}

void Robot::stairFollow(int16_t speed, uint8_t dist){
    int magnitude;
    int difference;
    sensors->getRangeData(magnitude, difference);
     if(speed > 0){
        int error = magnitude - dist;
        int angle = SERVO_POS_3 + error;
        if(angle > SERVO_POS_4){angle = SERVO_POS_4;}
        if(angle < SERVO_POS_2){angle = SERVO_POS_2;}
        this->frontDrive->setWheelAngle(angle);
        this->rearDrive->setWheelAngle(angle);
    }
    else if(speed < 0){
        int error = dist - magnitude; //negative error
        int angle = SERVO_POS_3 + error;
        if(angle > SERVO_POS_4){angle = SERVO_POS_4;}
        if(angle < SERVO_POS_2){angle = SERVO_POS_2;}
        this->frontDrive->setWheelAngle(angle);
        this->rearDrive->setWheelAngle(angle);
    }
    
    int16_t frontEffort = speed + difference;
    int16_t rearEffort = speed - difference;
    
    this->frontDrive->pidFSpeed(frontEffort);
    this->rearDrive->pidRSpeed(rearEffort);
}

bool Robot::home(){
    if(!digitalRead(FRONT_OPEN_ENDSTOP) && !digitalRead(REAR_OPEN_ENDSTOP)){
        return true;
    }
    else{
        this->scissors->lowerFront(MAX_DRIVE_SPEED);
        this->scissors->raiseRear(MAX_DRIVE_SPEED);
        return false;
    }
}

bool Robot::raiseFront(int16_t speed){
    static RobotState raiseState = IDLE;
    static RobotState tempState;
    static unsigned long t = 0;

    switch(raiseState){
        case IDLE:
            raiseState = RAISINGFRONT;
            break;

        case RAISINGFRONT:
            if(this->scissors->fState == Scissors::CLOSED){
                tempState = DRIVING;
                raiseState = WAITING;
                t = millis();
            }
            else{
                this->scissors->raiseFront(speed);
            }
            break;

        //ADD CASE FOR MOVING FORWARD x MM AWAY FROM STEP
        case DRIVING:
            if(this->driveTo(80)){
                botState = LOWERINGFRONT;
            }
            break;

        case LOWERINGFRONT:
            if(digitalRead(FRONT_WHEEL_SWITCH)){
                this->scissors->frontSpeed(0);
                botState = IDLE;
                return true;
            }
            this->scissors->lowerFront(speed);
            break;

        case WAITING:
            if(millis() - t >= delay){
                botState = tempState;
            }
            break;

    }
    return false;
}

bool Robot::raiseMid(int16_t speed){
    //Use a gyro axis to keep stage level, speeding up stages to compensate
    //Only run when rear is open and front is closed or opening
    float angle;
    int rEff;
    int fEff;
    switch(botState){
        case IDLE:
            if(this->scissors->fState == Scissors::OPENING){botState = RAISINGMID;}
            break;
        case RAISINGMID:
            angle = this->sensors->getPitch();
            int effort = this->stagePID->calcPID(0.0, angle, MAX_DRIVE_SPEED);
            if(effort>=0){
                fEff = speed;
                rEff = speed-abs(effort);
            }
            else{
                fEff = speed-abs(effort);
                rEff = speed;
            }
            this->scissors->lowerFront(fEff);
            this->scissors->lowerRear(rEff);

            if(this->scissors->fState == Scissors::OPEN && abs(0.0-angle) <= 0.02){
                this->scissors->frontSpeed(0);
                this->scissors->rearSpeed(0);
                botState = IDLE;
                return true;
            }
            break;
    }
    return false;
}

bool Robot::raiseRear(){
    this->scissors->raiseRear(200);
}

bool Robot::allignStep(){
    static RobotState tempState;
    static int magnitude = 0;
    static int difference = 0;
    static bool firstPass = true;
    static unsigned long t = 0;
    
    sensors->getRangeData(magnitude, difference);

    switch(botState){
        case IDLE:
            this->frontDrive->setWheelAngle(0);
            this->rearDrive->setWheelAngle(0);
            botState = APPROACHING;
            break;

        case WAITING:
            if(millis() - t >= delay){
                botState = tempState;
                this->allignPID->reset();
            }
            break;

        case APPROACHING:
            if(magnitude == DIST_TO_STEP && difference == 0){
                this->pidSpeed(0);
                return true;
            }

            else if(firstPass == true && (this->sensors->sensor_ranges[0] == DIST_TO_STEP || this->sensors->sensor_ranges[1] == DIST_TO_STEP)){
                this->pidSpeed(0);
                firstPass = false;
                botState = WAITING;
                tempState = ALLIGNING;
                this->allignPID->reset();
                t = millis();
            }
            else if(firstPass == false){
                if(magnitude == DIST_TO_STEP){
                    this->pidSpeed(0);
                    firstPass = true;
                    return true;
                }
                else{
                    this->pidSpeed(this->allignPID->calcPID(DIST_TO_STEP, magnitude, MAX_DRIVE_SPEED));
                }
            }
            else{
                int smallestRange = this->sensors->sensor_ranges[0];
                if(this->sensors->sensor_ranges[1] < this->sensors->sensor_ranges[0]){smallestRange = this->sensors->sensor_ranges[1];}
                this->pidSpeed(this->allignPID->calcPID(DIST_TO_STEP, smallestRange, 75));
            }
            break;

        case ALLIGNING:
            this->rearDrive->setWheelAngle(90);
            this->rearDrive->pidRSpeed(this->allignPID->calcPID(10, difference, 75));
            if(difference == 10){
                this->rearDrive->setWheelAngle(0);
                this->rearDrive->pidRSpeed(0);
                botState = WAITING;
                tempState = APPROACHING;
                this->allignPID->reset();
                t = millis();
            }
            break;
    }
    return false;
}

bool Robot::driveTo(float mm){
    int curr = this->encoders->getFrontCounts();
    static long target =  curr - this->encoders->distToCounts(mm);
    Serial.println(target);
    static int effort;
    static long prevMicros = 0;

    if(micros()-prevMicros >= 800){
        prevMicros = micros();
        effort = this->distPID->calcPID(target, curr, HALF_SPEED, 20000);
    }

    this->pidSpeed(effort);

    if(curr == target){
        this->pidSpeed(0);
        this->distPID->reset();
        return true;
    }

    return false;
}

bool Robot::ascendStep(){
    static RobotState tempState;
    static RobotState driveState;
    int dist;
    static unsigned long t = 0;

    switch(botState){
        case IDLE:
            botState = RAISINGFRONT;
            Serial.println("CALIBRATING");
            this->sensors->gyroCalibrate();
            Serial.println("CALIBRATED");
            break;

        case RAISINGFRONT:
            if(this->scissors->fState == Scissors::CLOSED){
                tempState = DRIVING;
                dist = 80;
                botState = WAITING;
                driveState = LOWERINGFRONT;
                t = millis();
            }
            else{
                this->scissors->raiseFront(MAX_DRIVE_SPEED);
            }
            break;

        //ADD CASE FOR MOVING FORWARD x MM AWAY FROM STEP
        case DRIVING:
            if(this->driveTo(dist)){
                botState = driveState;
                
                if(botState == IDLE){
                    return true;
                }
            }
            break;

        case LOWERINGFRONT:
            if(digitalRead(FRONT_WHEEL_SWITCH)){
                this->scissors->frontSpeed(0);
                botState = WAITING;
                tempState = RAISINGMID;
                t = millis();
            }
            else{
                this->scissors->lowerFront(MAX_DRIVE_SPEED);
            }
            break;

        case RAISINGMID:
            if(this->raiseMid(MAX_DRIVE_SPEED)){
                botState = WAITING;
                tempState = DRIVING;
                driveState = RAISINGREAR;
                t = millis();
                dist = 150;
            }
            break;

        case RAISINGREAR:
            if(this->scissors->rState == Scissors::OPEN){
                tempState = DRIVING;
                botState = WAITING;
                driveState = IDLE;
                dist = 140;
                t = millis();
            }
            else{
                this->scissors->raiseFront(MAX_DRIVE_SPEED);
            }
            break;

        case WAITING:
            if(millis() - t >= delay){
                botState = tempState;
            }
            break;
    }

    Serial.println(botState);
    return false;
}