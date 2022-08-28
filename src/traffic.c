#include "traffic.h"

#define SHORT_DELAY 0
#define LONG_DELAY 1

// 0,3 - red
// 1,4 - yellow
// 2,5 - green
// -----------------------------------
//|  1  |  2  |  3  |  4  |  5  |  6  |
//| + - | * - | - - | - + | - * | - - |
//| - - | - - | * * | - - | - - | * * |
//| - + | - * | - - | + - | * - | - - |
// -----------------------------------
// (+=on) (-=off) (*=blink)

uint16_t green_1 = GPIO2, 
         yellow_1 = GPIO1, 
         red_1 = GPIO0, 
         green_2 = GPIO5, 
         yellow_2 = GPIO4, 
         red_2 = GPIO3;

uint32_t gpioa = GPIOA;

void traffic_normal_work(void)
{
  int direction = 0;

  if (direction == 0){
    gpio_set(gpioa, red_1 | green_2);
    traffic_delay(SHORT_DELAY);
    traffic_blink(gpioa, red_1 | green_2);
    traffic_blink(gpioa, yellow_1 | yellow_2);
    gpio_set(gpioa, red_2 | green_1);
    traffic_delay(SHORT_DELAY);
    direction = 1;
  }else{
    gpio_set(gpioa, red_2 | green_1);
    traffic_delay(SHORT_DELAY);
    traffic_blink(gpioa, red_2 | green_1);
    traffic_blink(gpioa, yellow_1 | yellow_2);
    gpio_set(gpioa, red_1 | green_2);
    traffic_delay(SHORT_DELAY);
    direction = 0;
  }
}

void traffic_service()
{
  while(true){
    gpio_set(GPIOA, GPIO1 | GPIO4);
    traffic_delay(LONG_DELAY);
    gpio_clear(GPIOA, GPIO1 | GPIO4);
    traffic_delay(LONG_DELAY);
  }
}