/*************************************************************************
	> File Name: m0_struct.c
	> Author:LCY 
	> Mail: 
	> Created Time: Mon 19 Mar 2018 11:22:07 PM CST
 ************************************************************************/

typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;

typedef struct _tem_t
{
    uint8_t ltem;
    uint8_t htem;
}tem_t;
typedef struct _hum_t
{
    uint8_t lhum;
    uint8_t hhum;
}hum_t;

typedef struct _acc_t
{
    char x;
    char y;
    char z;
}acc_t;

typedef struct _adc_t
{
    uint32_t ad0;//data from A/D channel 0
    uint32_t ad3;//data from A/D channel 1
}adc_t;

typedef struct _light_t
{
    uint32_t light;
}light_t;

typedef struct _state_t
{
    uint8_t led_state;
    uint8_t fan_state;
    uint8_t buzz_state;
    uint8_t seven_led_state;
}state_t;


