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

void traffic_normal_work(void)
{
  int direction = 0;

  if (direction == 0){
    gpio_set(gpioa, red_1 | green_2);
    traffic_delay(LONG_DELAY);
    traffic_blink(gpioa, red_1 | green_2);
    gpio_clear(gpioa, red_1 | green_2);
    
    traffic_blink(gpioa, yellow_1 | yellow_2);

    gpio_set(gpioa, red_2 | green_1);
    traffic_delay(LONG_DELAY);
    traffic_blink(gpioa, red_2 | green_1);
    gpio_clear(gpioa, red_2 | green_1);

    direction = 1;
  }else{
    gpio_set(gpioa, red_2 | green_1);
    traffic_delay(LONG_DELAY);
    traffic_blink(gpioa, red_2 | green_1);
    gpio_clear(gpioa, red_2 | green_1);

    traffic_blink(gpioa, yellow_1 | yellow_2);

    gpio_set(gpioa, red_1 | green_2);
    traffic_delay(LONG_DELAY);
    traffic_blink(gpioa, red_1 | green_2);
    gpio_clear(gpioa, red_1 | green_2);
    direction = 0;
  }
}

void traffic_service(void)
{
  gpio_set(gpioa, yellow_1 | yellow_2);
  traffic_delay(LONG_DELAY);
  gpio_clear(gpioa, yellow_1 | yellow_2);
  traffic_delay(LONG_DELAY);
}

// 0x11 - 2 dir red
//   22 - 2 dir yellow
//   33 - 2 dir green
void traffic_manual(unsigned char d1, unsigned char d2){
  switch (d1){
  case 1:
  gpio_set(gpioa, red_1);
    break;
  case 2:
  gpio_set(gpioa, yellow_1);
    break;
  case 3:
  gpio_set(gpioa, green_1);
    break;

  switch (d2){
  case 1:
  gpio_set(gpioa, red_2);
    break;
  case 2:
  gpio_set(gpioa, yellow_2);
    break;
  case 3:
  gpio_set(gpioa, green_2);
    break;
  }
}
}

// void traffic_manual(){
//   gpio_set(gpioa, red_1 | red_2 | yellow_1 | yellow_2 | green_1 | green_2);
// }

// void traffic_manual(int r1,int r2, int y1,int y2,int g1,int g2)
// {
//   if (r1 == 0) {gpio_clear(gpioa, red_1);} else{gpio_set(gpioa, red_1);}
//   if (r2 == 0) {gpio_clear(gpioa, red_2);} else{gpio_set(gpioa, red_2);}
//   if (y1 == 0) {gpio_clear(gpioa, yellow_1);} else{gpio_set(gpioa, yellow_1);}
//   if (y2 == 0) {gpio_clear(gpioa, yellow_2);} else{gpio_set(gpioa, yellow_2);}
//   if (g1 == 0) {gpio_clear(gpioa, green_1);} else{gpio_set(gpioa, green_1);}
//   if (g2 == 0) {gpio_clear(gpioa, green_2);} else{gpio_set(gpioa, green_2);}
// }

void traffic_clear(void){
  gpio_clear(gpioa, red_1 | red_2 | yellow_1 | yellow_2 | green_1 | green_2);
}