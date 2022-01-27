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

//static const REAR_ENCODER_A = 0;
//static const REAR_ENCODER_B = 0;
//static const REAR_DRIVE_EN = 0;
//static const REAR_DRIVE_IN1 = 0;
//static const REAR_DRIVE_IN2 = 0;
//static const REAR_DRIVE_SERVO = 0;

//-------------------------------------------//

//Other Variables
static const uint8_t MAX_DRIVE_SPEED = 255;
static const;
static const;
static const;
static const;
#endif