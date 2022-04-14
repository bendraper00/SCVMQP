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
    this->distPIDF = new PIDController(PID_DIST_KP, PID_DIST_KI, PID_DIST_KD);
    this->distPIDR = new PIDController(PID_DIST_KP, PID_DIST_KI, PID_DIST_KD);

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
    Serial.println(this->scissors->rState);
    if(!digitalRead(FRONT_OPEN_ENDSTOP) && !digitalRead(REAR_OPEN_ENDSTOP)){
        return true;
    }
    else{
        this->scissors->lowerFront(MAX_DRIVE_SPEED);
        this->scissors->raiseRear(MAX_DRIVE_SPEED);
        return false;
    }
}

bool Robot::cleanStep(){
    static RobotState cleanState = IDLE;
    static RobotState tempState;
    static int cleaningSpeed = HALF_SPEED;
    static unsigned long t = 0;
    

    switch(cleanState){
        case IDLE:
            cleanState = CLEANLEFT;
            this->distPIDF->reset();
            this->distPIDR->reset();
            this->frontDrive->setWheelAngle(90);
            this->rearDrive->setWheelAngle(90);
            break;

        case CLEANLEFT:
            this->stairFollow(cleaningSpeed, DIST_TO_STEP);
            if(!digitalRead(BUMP_LEFT)){
                t = millis();
                this->pidSpeed(0);
                cleanState = WAITING;
                tempState = CLEANRIGHT;
            }
            break;

        case CLEANRIGHT:
            this->stairFollow((cleaningSpeed * -1), DIST_TO_STEP);
            if(!digitalRead(BUMP_RIGHT)){
                t = millis();
                this->pidSpeed(0);
                cleanState = WAITING;
                tempState = DRIVING;
            }
            break;

        case DRIVING:
            if(this->driveDist(50)){
                this->frontDrive->setWheelAngle(0);
                this->rearDrive->setWheelAngle(0);
                cleanState = IDLE;
                this->distPIDF->reset();
                this->distPIDR->reset();
                return true;
            }
            break;

        case WAITING:
            if(millis() - t >= delay){
                cleanState = tempState;
            }
            break;
    }
    return false;
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
            int effort = this->stagePID->calcPID(LEVEL_ANGLE, angle, MAX_DRIVE_SPEED);
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

            if(this->scissors->fState == Scissors::OPEN && abs(LEVEL_ANGLE-angle) <= 0.02){
                this->scissors->frontSpeed(0);
                this->scissors->rearSpeed(0);
                botState = IDLE;
                return true;
            }
            break;
    }
    return false;
}

bool Robot::allignStep(){
    static RobotState tempState;
    static RobotState allignState = IDLE;
    static int magnitude = 0;
    static int difference = 0;
    static bool firstPass = true;
    static unsigned long t = 0;
    sensors->getRangeData(magnitude, difference);
    
    switch(allignState){
        case IDLE:
            this->frontDrive->setWheelAngle(0);
            this->rearDrive->setWheelAngle(0);
            allignState = WAITING;
            tempState = APPROACHING;
            t = millis();
            break;

        case WAITING:
            if(millis() - t >= delay){
                allignState = tempState;
                this->allignPID->reset();
            }
            break;

        case APPROACHING:

            //Alligned
            if(magnitude == DIST_TO_STEP && difference == 0){
                this->pidSpeed(0);
                return true;
            }

            //First Pass and one of the sensors is at correct distance
            else if(firstPass == true && (this->sensors->sensor_ranges[0] == DIST_TO_STEP || this->sensors->sensor_ranges[1] == DIST_TO_STEP)){
                this->pidSpeed(0);
                firstPass = false;
                allignState = WAITING;
                tempState = ALLIGNING;
                this->rearDrive->setWheelAngle(90);
                this->allignPID->reset();
                t = millis();
            }

            //Approaching on after alligning
            else if(firstPass == false){
                //DONE
                if(magnitude == DIST_TO_STEP){
                    this->pidSpeed(0);
                    allignState = IDLE;
                    this->allignPID->reset();
                    firstPass = true;
                    return true;
                }
                //Drive forward to distance
                else{
                    this->pidSpeed(this->allignPID->calcPID(DIST_TO_STEP, magnitude, HALF_SPEED, 1000));
                }
            }

            //Drive forward
            else{
                int smallestRange = this->sensors->sensor_ranges[0];
                if(this->sensors->sensor_ranges[1] < smallestRange){smallestRange = this->sensors->sensor_ranges[1];}
                this->pidSpeed(this->allignPID->calcPID(DIST_TO_STEP, smallestRange, QUARTER_SPEED, 1000));
            }
            break;

        case ALLIGNING:
            static bool settled = false;
            static bool settling = false;
            Serial.println(settling);

            //Target 0 + ANGLE_OFFSET is to compensate for the wheel returning to 0 degrees possible shifting the robot
            this->rearDrive->pidRSpeed(this->allignPID->calcPID((0 + ANGLE_OFFSET), difference, QUARTER_SPEED, 1000));


            //Angle met and not yet settling, start timer
            if(abs((0 + ANGLE_OFFSET) - difference) <= 7 && settling == false){
                settling = true;
                t = millis();
            }

            //Angle met and settled for half a second
            else if(abs((0 + ANGLE_OFFSET) - difference) <= 7 && settling == true){
                if(millis()-t >= 500){
                    settled = true;
                    settling = false;
                }
            }

            //Angle not met or not met for long enough i.e. overshot
            else{
                settling = false;
            }


            if(settled){
                settled = false;
                this->rearDrive->pidRSpeed(0);
                this->rearDrive->setWheelAngle(0);
                allignState = WAITING;
                tempState = APPROACHING;
                this->allignPID->reset();
                t = millis();
            }

            break;
    }
    return false;
}

bool Robot::driveUpTo(float mm){
    static bool resetFlag = true;
    static int magnitude = 0;
    static int difference = 0;
    if(resetFlag){
        resetFlag = false;
        this->allignPID->reset();
    }
    sensors->getRangeData(magnitude, difference);
    if(magnitude == DIST_TO_STEP){
        this->pidSpeed(0);
        resetFlag = true;
        return true;
    }
    else{
        this->pidSpeed(this->allignPID->calcPID(DIST_TO_STEP, magnitude, QUARTER_SPEED, 1000));
        return false;
    }
}

bool Robot::driveTo(float mm){
    static bool reset = true;
    int currF = this->encoders->getFrontCounts();
    int currR = this->encoders->getRearCounts();
    static long targetF =  currF - this->encoders->distToCounts(mm);
    static long targetR =  currR - this->encoders->distToCounts(mm);
    static int effortF;
    static int effortR;
    static long prevMicros = 0;

    if(reset == true){
        reset = false;
        this->distPIDF->reset();
        this->distPIDR->reset();
        targetF =  currF - this->encoders->distToCounts(mm);
        targetR =  currR - this->encoders->distToCounts(mm);
        prevMicros = micros();
    }

    if(micros()-prevMicros >= 800){
        prevMicros = micros();
        effortF = this->distPIDF->calcPID(targetF, currF, QUARTER_SPEED, 1000);
        effortR = this->distPIDR->calcPID(targetR, currR, QUARTER_SPEED, 1000);
    }

    this->frontDrive->pidFSpeed(effortF);
    this->rearDrive->pidRSpeed(effortR);

    if(abs(targetF - currF) <= 7 && abs(targetR - currR) <= 7){
        this->pidSpeed(0);
        this->distPIDF->reset();
        this->distPIDR->reset();
        reset = true;
        return true;
    }

    Serial.print(targetR);
    Serial.print("\t");
    Serial.print(currR);
    Serial.print("\t");
    Serial.println(effortR);

    return false;
}

bool Robot::ascendStep(){
    static RobotState tempState;
    static RobotState driveState;
    static int dist;
    static unsigned long t = 0;

    switch(botState){
        case IDLE:
            botState = ALLIGNING;
            break;

        case ALLIGNING:
            if(this->allignStep()){
                botState = CLEANING;
            }
            break;
        
        case CLEANING:
            if(this->cleanStep()){
                Serial.println("CALIBRATING");
                this->sensors->gyroCalibrate();
                Serial.println("CALIBRATED");
                botState = RAISINGFRONT;
            }
            break;

        case RAISINGFRONT:
            if(this->scissors->fState == Scissors::CLOSED){
                botState = STEPREAR;
            }
            else{
                this->scissors->raiseFront(MAX_DRIVE_SPEED);
            }
            break;

        case STEPREAR:
            if(this->scissors->stepRearDown()){
                this->scissors->rearSpeed(0);
                botState = WAITING;
                tempState = DRIVING;
                dist = 65;
                driveState = LOWERINGFRONT;
                t = millis();
            }
            else{
                this->scissors->lowerRear(MAX_DRIVE_SPEED);
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

        case DRIVEUPTO:
            if(this->driveUpTo(DIST_TO_STEP)){
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
                dist = 125;
            }
            break;

        case RAISINGREAR:
            if(this->scissors->rState == Scissors::OPEN){
                tempState = DRIVEUPTO;
                botState = WAITING;
                driveState = IDLE;
                t = millis();
            }
            else{
                this->scissors->raiseRear(MAX_DRIVE_SPEED);
            }
            break;

        case WAITING:
            if(millis() - t >= delay){
                botState = tempState;
            }
            break;
    }
    return false;
}