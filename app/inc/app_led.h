#ifndef APP_LED_H_
#define APP_LED_H_

#define LED_CPU PortA_PIN0
// simulate the Ignition key on the car
#define IG_INPUT PortA_PIN1
#define LED_IG_STATE   PortA_PIN2


#define LED1 PortA_PIN3
#define LED2 PortA_PIN4
#define LED3 PortA_PIN5

#define LED_ON   STD_LOW
#define LED_OFF  STD_HIGH

extern void app_led_20ms_runnable(void);
extern void app_led_1000ms_runnable(void);

#endif /* APP_LED_H_ */
