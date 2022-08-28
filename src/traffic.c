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

void traffic_normal_work()
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
    gpio_set(gpioa, yellow_1 | yellow_2);
    traffic_delay(LONG_DELAY);
    gpio_clear(gpioa, yellow_1 | yellow_2);
    traffic_delay(LONG_DELAY);
  }
}

void traffic_manual(int r1,int r2, int y1,int y2,int g1,int g2)
{
  if (r1 == 0) {gpio_clear(gpioa, red_1);} else{gpio_set(gpioa, red_1);}
  if (r2 == 0) {gpio_clear(gpioa, red_2);} else{gpio_set(gpioa, red_2);}
  if (y1 == 0) {gpio_clear(gpioa, yellow_1);} else{gpio_set(gpioa, yellow_1);}
  if (y2 == 0) {gpio_clear(gpioa, yellow_2);} else{gpio_set(gpioa, yellow_2);}
  if (g1 == 0) {gpio_clear(gpioa, green_1);} else{gpio_set(gpioa, green_1);}
  if (g2 == 0) {gpio_clear(gpioa, green_2);} else{gpio_set(gpioa, green_2);}
}