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
#define CAMERA_WORDS_IN_COMMAND   4
#define CAMERA_WORDS_IN_RESPONSE  3

// Настройки выводов тестируемой камеры
#define ZOOM_MOTOR     0
#define FOCUS_MOTOR    1
#define IRC_MOTO_PLUS  78  // Инфракрасная шторка
#define IRC_MOTO_MINUS 79
#define IR_DIM         9  // ШИМ подсветки
#define IR_CDS         2  // АЦП датчика освещённости
#define CAMERA_I2C     0
#define IRIS_PWM       -  // Диафрагма IRIS
#define AUDIO_DAC      -
#define AUDIO_ADC      -
#define CAMERA_UART    0
#define ALARM_IN       65
#define ALARM_OUT      66
#define LED1           77

// Режимы аудио ввода/вывода
#define AUDIO_BLOCKING_MODE   -1
#define AUDIO_NOBLOCKING_MODE 0

#define PWM_PERIOD 400000
