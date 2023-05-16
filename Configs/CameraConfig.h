// Настройки камер
#define CAMERA_MAX_COUNT  6
#define CAMERA_IP_ADDRESS "192.168.1.11"

#define CAMERA1_IP_ADDRESS "192.168.1.11"
#define CAMERA2_IP_ADDRESS "192.168.1.12"
#define CAMERA3_IP_ADDRESS "192.168.1.13"
#define CAMERA4_IP_ADDRESS "192.168.1.14"
#define CAMERA5_IP_ADDRESS "192.168.1.15"
#define CAMERA6_IP_ADDRESS "192.168.1.16"

#define CAMERA_IP_PORT            1000
#define CAMERA_COMMAND_TERMINATOR "\r\n"

// Настройки выводов тестируемой камеры
#define ZOOM_MOTOR    19
#define FOCUS_MOTOR   22
#define IRC           21
#define IR_BL         20
#define CAMERA_I2C    1
#define IRIS_PWM      14
#define AUDIO_DAC     12
#define AUDIO_ADC     13
#define CAMERA_UART   2
#define ALARM_PIN_OUT 10
#define ALARM_PIN_IN  11
#define LED1_PIN      12

// Значения, выставляемые на ножках для проверки
#define TEST_AUDIO_DAC_VALUE 512
