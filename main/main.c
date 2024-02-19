//Shows an animation using 5 leds, the animation can be switched by a DIP switch with sliders 1 and 2, and
//the speed with sliders 3 and 4

#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define TIMER_VALUE(x) (x/(1+speed)) //value for if statement in play_anim affected by speed
#define SWITCH_1 GPIO_NUM_22
#define SWITCH_2 GPIO_NUM_21
#define SWITCH_3 GPIO_NUM_19
#define SWITCH_4 GPIO_NUM_18

void initGpio(void);
uint8_t check_inputs_anim(void);
uint8_t check_inputs_speed(void);
void play_anim(uint8_t anim,uint8_t speed,uint32_t timer);
void turnOnLed(uint8_t led_num);

void initGpio(void)
{
    //inputs and pull downs
    gpio_set_direction(SWITCH_4, GPIO_MODE_INPUT);
    gpio_pulldown_en(SWITCH_4);
    gpio_set_direction(SWITCH_3, GPIO_MODE_INPUT);
    gpio_pulldown_en(SWITCH_3);
    gpio_set_direction(SWITCH_2, GPIO_MODE_INPUT);
    gpio_pulldown_en(SWITCH_2);
    gpio_set_direction(SWITCH_1, GPIO_MODE_INPUT);
    gpio_pulldown_en(SWITCH_1);
}

uint8_t check_inputs_anim(void)
{
    uint8_t anim=0;

    //takes priority to Gpio3
    if(gpio_get_level(SWITCH_1))
        anim=1;
    else if(gpio_get_level(SWITCH_2))
        anim=2;

    return anim;
}

uint8_t check_inputs_speed(void)
{
    uint8_t speed=0;

    //takes priority to Gpio19
    if(gpio_get_level(SWITCH_3))
        speed=1;
    else if(gpio_get_level(SWITCH_4))
        speed=2;

    return speed;
}

void play_anim(uint8_t anim,uint8_t speed,uint32_t timer)
{
    static uint32_t timer_start;
    uint32_t timer_dif;
    static uint8_t old_anim=0xff, local_count;

    //reset timer count
    if(old_anim!=anim)
    {
        timer_start=timer;
        local_count=0;
    }

    timer_dif = timer - timer_start;

    switch(anim)
    {
        case 0:
            if(timer_dif<TIMER_VALUE(100))
                turnOnLed(1);
            else if(timer_dif>=TIMER_VALUE(100) && timer_dif<TIMER_VALUE(200))
                turnOnLed(2);
            else if(timer_dif>=TIMER_VALUE(200) && timer_dif<TIMER_VALUE(300))
                turnOnLed(3);
            else if(timer_dif>=TIMER_VALUE(300) && timer_dif<TIMER_VALUE(400))
                turnOnLed(4);
            else if(timer_dif>=TIMER_VALUE(400) && timer_dif<TIMER_VALUE(500))
                turnOnLed(5);
            else if(timer_dif>=TIMER_VALUE(500))
                timer_start=timer; //reset timer
            break;
        case 1:
            if(timer_dif<TIMER_VALUE(100))
            {
                if(local_count % 2 == 0)
                    turnOnLed(1);
                else 
                    turnOnLed(5);
                local_count++;
            }
            else if(timer_dif>=TIMER_VALUE(100) && timer_dif<TIMER_VALUE(200))
            {
                if(local_count % 2 == 0)
                    turnOnLed(2);
                else 
                    turnOnLed(4);
                local_count++;
            }
            else if(timer_dif>=TIMER_VALUE(200) && timer_dif<TIMER_VALUE(300))
                turnOnLed(3);
            else if(timer_dif>=TIMER_VALUE(300))
                timer_start=timer; //reset timer
            break;
        case 2:
            if(timer_dif<TIMER_VALUE(100))
                turnOnLed(1);
            else if(timer_dif>=TIMER_VALUE(100) && timer_dif<TIMER_VALUE(200))
            {
                if(timer_dif==TIMER_VALUE(100))
                    local_count=0; //starting value  
                switch(local_count)
                {
                    case 0:
                        turnOnLed(1);
                        break;
                    case 1:
                        turnOnLed(2);
                        local_count=0xff; //reset counter
                }
                local_count++; //increment every 10 mills
            }
            else if(timer_dif>=TIMER_VALUE(200) && timer_dif<TIMER_VALUE(300))
            {
                if(timer_dif==TIMER_VALUE(200))
                    local_count=0; //starting value
                switch(local_count)
                {
                    case 0:
                        turnOnLed(1);
                        break;
                    case 1:
                        turnOnLed(2);
                        break;
                    case 3:
                        turnOnLed(3);
                        local_count=0xff; //reset counter
                }
                local_count++; //increment every 10 mills
            }
            else if(timer_dif>=TIMER_VALUE(300) && timer_dif<TIMER_VALUE(400))
            {
                if(timer_dif==TIMER_VALUE(300))
                    local_count=0; //starting value
                switch(local_count)
                {
                    case 0:
                        turnOnLed(1);
                        break;
                    case 1:
                        turnOnLed(2);
                        break;
                    case 3:
                        turnOnLed(3);
                        break;
                    case 4:
                        turnOnLed(4);
                        local_count=0xff; //reset counter
                }
                local_count++; //increment every 10 mills
            }
            else if(timer_dif>=TIMER_VALUE(400) && timer_dif<TIMER_VALUE(500))
            {
                if(timer_dif==TIMER_VALUE(400))
                    local_count=0; //starting value
                switch(local_count)
                {
                    case 0:
                        turnOnLed(1);
                        break;
                    case 1:
                        turnOnLed(2);
                        break;
                    case 3:
                        turnOnLed(3);
                        break;
                    case 4:
                        turnOnLed(4);
                        break;
                    case 5:
                        turnOnLed(5);
                        local_count=0xff; //reset counter
                }
                local_count++; //increment every 10 mills
            }
            else if(timer_dif>=TIMER_VALUE(500))
                timer_start=timer; //reset timer
            break;
        default:
            turnOnLed(0);//turn off all(never gets here)
    }

    old_anim = anim;
}

void turnOnLed(uint8_t led_num)
{
    switch(led_num)
    {
        case 1:
            gpio_set_direction(GPIO_NUM_5, GPIO_MODE_DISABLE);
            gpio_set_direction(GPIO_NUM_4, GPIO_MODE_OUTPUT);
            gpio_set_level(GPIO_NUM_4,1);
            gpio_set_direction(GPIO_NUM_2, GPIO_MODE_OUTPUT);
            gpio_set_level(GPIO_NUM_2,0);
            break;
        case 2:
            gpio_set_direction(GPIO_NUM_5, GPIO_MODE_DISABLE);
            gpio_set_direction(GPIO_NUM_4, GPIO_MODE_OUTPUT);
            gpio_set_level(GPIO_NUM_4,0);
            gpio_set_direction(GPIO_NUM_2, GPIO_MODE_OUTPUT);
            gpio_set_level(GPIO_NUM_2,1);
            break;
        case 3:
            gpio_set_direction(GPIO_NUM_5, GPIO_MODE_OUTPUT);
            gpio_set_level(GPIO_NUM_5,1);
            gpio_set_direction(GPIO_NUM_4, GPIO_MODE_OUTPUT);
            gpio_set_level(GPIO_NUM_4,0);
            gpio_set_direction(GPIO_NUM_2, GPIO_MODE_DISABLE);
            break;
        case 4:
            gpio_set_direction(GPIO_NUM_5, GPIO_MODE_OUTPUT);
            gpio_set_level(GPIO_NUM_5,0);
            gpio_set_direction(GPIO_NUM_4, GPIO_MODE_OUTPUT);
            gpio_set_level(GPIO_NUM_4,1);
            gpio_set_direction(GPIO_NUM_2, GPIO_MODE_DISABLE);
            break;
        case 5:
            gpio_set_direction(GPIO_NUM_5, GPIO_MODE_OUTPUT);
            gpio_set_level(GPIO_NUM_5,0);
            gpio_set_direction(GPIO_NUM_4, GPIO_MODE_DISABLE);
            gpio_set_direction(GPIO_NUM_2, GPIO_MODE_OUTPUT);
            gpio_set_level(GPIO_NUM_2,1);
            break;
        default://off all
            gpio_set_direction(GPIO_NUM_5, GPIO_MODE_DISABLE); 
            gpio_set_direction(GPIO_NUM_4, GPIO_MODE_DISABLE);
            gpio_set_direction(GPIO_NUM_2, GPIO_MODE_DISABLE);
            break;
    }
}

void app_main(void)
{
    uint32_t timer=0;

    initGpio();

    while(1)
    {
        play_anim(check_inputs_anim(),check_inputs_speed(),timer);
        vTaskDelay(10 / portTICK_PERIOD_MS); //10 milisec
        timer++;
    }

}