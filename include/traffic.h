#ifndef TRAFFIC_H
#define TRAFFIC_H

#include "utils.h"

static uint16_t green_1 = GPIO2, 
         yellow_1 = GPIO1, 
         red_1 = GPIO0, 
         green_2 = GPIO5, 
         yellow_2 = GPIO4, 
         red_2 = GPIO3;

static uint32_t gpioa = GPIOA;


void traffic_normal_work(void);
void traffic_service(void);
void traffic_manual(int r1,int r2, int y1,int y2,int g1,int g2);
void traffic_clear(void);

#endif