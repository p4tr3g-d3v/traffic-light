#include "utils.h"
#include "traffic.h"

unsigned char traffic_status = 0x00;
unsigned char traffic_directions = 0x00;



static inline uint8_t read_gpio_value() {
	return ~(gpio_get(GPIOB, GPIO12|GPIO13|GPIO14|GPIO15)>>12)&0x0F;
}

int main(void)
{
  rcc_clock_setup_pll(&rcc_hse_25mhz_3v3[RCC_CLOCK_3V3_84MHZ]);
  clock_setup();
  gpio_setup(); //* Traffic pins PA(0-6)
  i2c_setup(I2C_ADDRESS + read_gpio_value() + 1); //! Set address in utils.h
  usart_setup();

  while (1)
  {
    if (get_buffer_ready(0)) {
			traffic_status = get_buffer(0)[0];
      traffic_directions = get_buffer(1)[0];
			clear_buffer_ready(0);
      traffic_clear();
      blink_led();
		}

    if(traffic_status == 0){
      traffic_normal_work();
    }else if (traffic_status == 1)
    {
      traffic_service();
    }else if (traffic_status == 2){
      unsigned char two_dir = traffic_directions & 0x0F;
      unsigned char one_dir = traffic_directions >> 4;
      if (two_dir >=1 && two_dir <= 3 && one_dir >=1 && one_dir <=3){
        traffic_manual(one_dir, two_dir);
      }
    }
  }
}


void blink_led(){
  for(int i = 0; i<4; i ++){
    gpio_toggle(GPIOC, GPIO13);
    for (unsigned long i = 0; i < 1000000; i++)
    { /* Wait a bit. */
      __asm__("nop");
    }
  }
}