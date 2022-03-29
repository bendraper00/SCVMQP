#ifndef CONFIG_H_
#define CONFIG_H_

/*
NOTES:
- Do not use analogWrite functions on pins 2,3,5,11,12,13. They depend on Timer3 and Timer 1(ir remote)
- Encoder Channel A needs to be an interrupt
- Servo pins need to be PWM capable pins
*/

//Pins Assignments
static const uint8_t FRONT_ENCODER_A = 18; //INTERRUPT
static const uint8_t FRONT_ENCODER_B = 36; //DIGITAL
static const uint8_t FRONT_DRIVE_EN = 10; //PWM
static const uint8_t FRONT_DRIVE_IN1 = 26; //DIGITAL
static const uint8_t FRONT_DRIVE_IN2 = 28; //DIGITAL
static const uint8_t FRONT_DRIVE_SERVO = 8; //PWM
static const uint8_t FRONT_DRIVE_BUTTON = 44; //DIGITAL

static const uint8_t REAR_ENCODER_A = 19; //INTERRUPT
static const uint8_t REAR_ENCODER_B = 37; //DIGITAL
static const uint8_t REAR_DRIVE_EN = 6; //PWM
static const uint8_t REAR_DRIVE_IN1 = 25; //DIGITAL
static const uint8_t REAR_DRIVE_IN2 = 23; //DIGITAL
static const uint8_t REAR_DRIVE_SERVO = 4; //PWM
static const uint8_t REAR_DRIVE_BUTTON = 45; //DIGITAL

static const uint8_t FRONT_OPEN_ENDSTOP = 3; //INTERRUPT
static const uint8_t FRONT_CLOSED_ENDSTOP = 48; //INTERRUPT
static const uint8_t FRONT_SCISSOR_EN = 9; //PWM
static const uint8_t FRONT_SCISSOR_IN1 = 22; //DIGITAL
static const uint8_t FRONT_SCISSOR_IN2 = 24; //DIGITAL

static const uint8_t REAR_OPEN_ENDSTOP = 2; //INTERRUPT
static const uint8_t REAR_CLOSED_ENDSTOP = 53; //INTERRUPT
static const uint8_t REAR_SCISSOR_EN = 7; //PWM
static const uint8_t REAR_SCISSOR_IN1 = 27; //DIGITAL
static const uint8_t REAR_SCISSOR_IN2 = 29; //DIGITAL

static const uint8_t FRONT_WHEEL_SWITCH = 44;
static const uint8_t REAR_WHEEL_SWITCH = 45;
static const uint8_t BUMP_LEFT = 38;
static const uint8_t BUMP_RIGHT = 39;

static const int IR_REMOTE_SIGNAL = 47;

//-------------------------------------------//

//Other Variables
static const uint8_t MAX_DRIVE_SPEED = 255;
static const uint8_t DEMO_DRIVE_SPEED = 80;
static const uint8_t CONTROLLED_DRIVE_SPEED = 100;
static const uint8_t DRIVE_DIST_TOLERANCE_ENC = 10;
static const uint8_t OFF_GROUND_BUFFER_MS = 100;
static const float ENCODER_COUNTS_TO_MM = 0.15708;
static const uint8_t SERVO_POS_1 = 0;
static const uint8_t SERVO_POS_2 = 60;
static const uint8_t SERVO_POS_3 = 90;
static const uint8_t SERVO_POS_4 = 120;

//PID VALUES
static const float PID_SPEED_KP = 1.0;
static const float PID_SPEED_KI = 0.2;
static const float PID_SPEED_KD = 10.0;

static const float PID_DIST_KP = 2.0;
static const float PID_DIST_KI = 0.0;
static const float PID_DIST_KD = 0.0;

static const float PID_SCISSOR_KP = 1.0;
static const float PID_SCISSOR_KI = 0.01;
static const float PID_SCISSOR_KD = 0.0;

static const int LOX1_ADDRESS = 0x30;
static const int LOX2_ADDRESS = 0x31;
static const int LOX1_SHT = 32;
static const int LOX2_SHT = 33;
static const int TOF_SENSOR_COUNT = 2;
static const int TOF_PERIOD = 10;

static const int GYRO_ADDRESS = 0x32;


#endif
