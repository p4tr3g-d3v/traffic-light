#include "utils.h"
#include "traffic.h"

unsigned char status_byte_from_i2c = 0x80;

int main(void)
{
  rcc_clock_setup_pll(&rcc_hse_25mhz_3v3[RCC_CLOCK_3V3_84MHZ]);
  clock_setup();
  gpio_setup();
  i2c_setup();
  usart_setup();

  while (1)
  {
    if (get_buffer_ready(0)) {
			status_byte_from_i2c = get_buffer(0)[0];
			clear_buffer_ready(0);
      traffic_clear();
		}
    int tl_work_bit = 8;
    int tl_work = (status_byte_from_i2c & (1 << tl_work_bit - 1)) > 0;

    int tl_manual_bit = 7;
    int tl_manual = (status_byte_from_i2c & (1 << tl_manual_bit - 1)) > 0;

    if (tl_work == 1)
    {
      traffic_normal_work();
    }
    else if (tl_work == 0 && tl_manual == 0)
    {
      traffic_service();
    }
    else if (tl_manual == 1)
    {
      int red1 = (status_byte_from_i2c & (1 << 6 - 1)) > 0;
      int red2 = (status_byte_from_i2c & (1 << 5 - 1)) > 0;
      int yellow1 = (status_byte_from_i2c & (1 << 4 - 1)) > 0;
      int yellow2 = (status_byte_from_i2c & (1 << 3 - 1)) > 0;
      int green1 = (status_byte_from_i2c & (1 << 2 - 1)) > 0;
      int green2 = (status_byte_from_i2c & (1 << 1 - 1)) > 0;
      
      traffic_manual(red1, red2, yellow1, yellow2, green1, green2);
    }
  }
}
