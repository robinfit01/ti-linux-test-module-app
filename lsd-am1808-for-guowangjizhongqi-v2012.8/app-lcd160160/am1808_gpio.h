#ifndef _AM1808_GPIO_H_
#define _AM1808_GPIO_H_

#define M_GPIO_OUTPUT  	0
#define M_GPIO_INPUT	1
#define M_GPIO_OUTPUT_HIGH  	1
#define M_GPIO_OUTPUT_LOW		0

//#define DEVICE_NAME "/dev/am1808-all-gpio" //设备名(/dev/led) 
#define MOTOR_MAGIC 'g'
#define SET_GPIO_DIRECT                 (_IOW(MOTOR_MAGIC, 0,int))     // cmd是这个值  arg的值 1=输入  0=输出 
#define SET_GPIO_VALUE_BASE 		(_IOW(MOTOR_MAGIC, 0,int)+200)   // cmd是这个值  arg的值 1=高电平  0=高电平 
#define GET_GPIO_VALUE_BASE 		(_IOW(MOTOR_MAGIC, 0,int)+400)   // cmd是这个值  arg的值表示返回值的指针

#define KEY_UP  	(4*16+1)
#define KEY_DOWN  	(4*16+0)
#define KEY_LEFT  	(4*16+2)
#define KEY_RIGHT  	(4*16+6)
#define KEY_OK  	(4*16+7)
#define KEY_CANCEL  	(4*16+3)

#define LCD_RST       (0*16+12)
#define LCD_BL        (0*16+14)

extern int fn_am1808_gpio_init(void);
extern int fn_get_am1808_gpio_value(int gpio_num);
extern int fn_set_am1808_gpio_value(int gpio_num,int value);
extern int fn_set_am1808_gpio_direct(int gpio_num,int direct);
extern int fn_am1808_gpio_exit(void);

#endif
