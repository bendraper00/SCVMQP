#ifndef CONFIG_H_
#define CONFIG_H_

/*
NOTES:
- Do not use analogWrite functions on pins 2,3,5,11,12,13. They depend on Timer3 and Timer 1(ir remote)
- Encoder Channel A needs to be an interrupt
- Servo pins need to be PWM capable pins
*/

//Pins Assignments
static const uint8_t FRONT_ENCODER_A = 3; //INTERRUPT
static const uint8_t FRONT_ENCODER_B = 53; //DIGITAL
static const uint8_t FRONT_DRIVE_EN = 4; //PWM
static const uint8_t FRONT_DRIVE_IN1 = 52; //DIGITAL
static const uint8_t FRONT_DRIVE_IN2 = 51; //DIGITAL
static const uint8_t FRONT_DRIVE_SERVO = 6; //PWM
static const uint8_t FRONT_DRIVE_BUTTON = 0; //DIGITAL

static const uint8_t REAR_ENCODER_A = 18; //INTERRUPT
static const uint8_t REAR_ENCODER_B = 50; //DIGITAL
static const uint8_t REAR_DRIVE_EN = 7; //PWM
static const uint8_t REAR_DRIVE_IN1 = 49; //DIGITAL
static const uint8_t REAR_DRIVE_IN2 = 48; //DIGITAL
static const uint8_t REAR_DRIVE_SERVO = 8; //PWM

static const uint8_t FRONT_OPEN_ENDSTOP = 0; //INTERRUPT
static const uint8_t FRONT_CLOSED_ENDSTOP = 0; //INTERRUPT
static const uint8_t FRONT_SCISSOR_EN = 0; //PWM
static const uint8_t FRONT_SCISSOR_IN1 = 0; //DIGITAL
static const uint8_t FRONT_SCISSOR_IN2 = 0; //DIGITAL

static const uint8_t REAR_OPEN_ENDSTOP = 0; //INTERRUPT
static const uint8_t REAR_CLOSED_ENDSTOP = 0; //INTERRUPT
static const uint8_t REAR_SCISSOR_EN = 0; //PWM
static const uint8_t REAR_SCISSOR_IN1 = 0; //DIGITAL
static const uint8_t REAR_SCISSOR_IN2 = 0; //DIGITAL


static const int IR_REMOTE_SIGNAL = 47;

//-------------------------------------------//

//Other Variables
static const uint8_t MAX_DRIVE_SPEED = 255;
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
static const float PID_SPEED_KI = 1.0;
static const float PID_SPEED_KD = 0.0;
static const float PID_DIST_KP = 2.0;
static const float PID_DIST_KI = 0.0;
static const float PID_DIST_KD = 0.0;
static const float PID_SCISSOR_KP = 1.0;
static const float PID_SCISSOR_KI = 1.0;
static const float PID_SCISSOR_KD = 0.0;

static const int LOX1_ADDRESS = 0x30;
static const int LOX2_ADDRESS = 0x31;
static const int LOX1_SHT = 30;
static const int LOX2_SHT = 31;
static const int TOF_SENSOR_COUNT = 2;
static const int TOF_PERIOD = 10;




#endif