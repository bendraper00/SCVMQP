#ifndef CONFIG_H_
#define CONFIG_H_

/*
NOTES:
- Do not use analogWrite functions on pins 2,3, or 5. They depend on Timer3
- Encoder Channel A needs to be an interrupt
- Servo pins need to be PWM capable pins
*/

//Pins Assignments
static const uint8_t FRONT_ENCODER_A = 2;
static const uint8_t FRONT_ENCODER_B = 51;
static const uint8_t FRONT_DRIVE_EN = 11;
static const uint8_t FRONT_DRIVE_IN1 = 53;
static const uint8_t FRONT_DRIVE_IN2 = 52;
static const uint8_t FRONT_DRIVE_SERVO = 9;
static const uint8_t FRONT_DRIVE_BUTTON = 37;

static const REAR_ENCODER_A = 0;
static const REAR_ENCODER_B = 0;
static const REAR_DRIVE_EN = 0;
static const REAR_DRIVE_IN1 = 0;
static const REAR_DRIVE_IN2 = 0;
static const REAR_DRIVE_SERVO = 0;

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
static const float PID_SPEED_KP = 3.0;
static const float PID_SPEED_KI = 0.5;
static const float PID_SPEED_KD = 0.0;
static const float PID_DIST_KP = 2.0;
static const float PID_DIST_KI = 0.0;
static const float PID_DIST_KD = 0.0;
#endif