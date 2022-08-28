#include "utils.h"
#include "traffic.h"

#define I2C_BUFFER_SIZE 3
#define I2C_BUFFER_COUNT 1

static uint8_t in_buffer[I2C_BUFFER_COUNT][I2C_BUFFER_SIZE + 1];
static bool in_buffer_state;

unsigned char status_byte_from_i2c = 0x80;

// TODO: i2c
void i2c1_ev_isr(void)
{
  uint32_t sr1, sr2;
  static uint8_t rb = 0;

  sr1 = I2C_SR1(I2C1);

  if (sr1 & I2C_SR1_ADDR)
  {
    rb = 0;
    sr2 = I2C_SR2(I2C1);
    if (sr2 & I2C_SR2_TRA)
    {
      i2c_nack_current(I2C1);
    }
  }
  else if (sr1 & I2C_SR1_RxNE)
  {
    uint8_t b = i2c_get_data(I2C1);
  }

  if (rb >= I2C_BUFFER_SIZE)
    return;

  // if (!rb && b > 0 && b <= I2C_BUFFER_SIZE)

  // int sr1 = I2C_SR1(I2C1);

  // if (sr1 & I2C_SR1_ADDR)
  // {
  //   int sr2 = I2C_SR2(I2C1);
  //   (void)sr2;
  // }

  // if (sr1 & I2C_SR1_AF)
  // {
  //   I2C_SR1(I2C1) &= ~(I2C_SR1_AF);
  // }

  // if (sr1 & I2C_SR1_STOPF)
  // {
  //   i2c_peripheral_enable(I2C1);
  // }
}

int main(void)
{
  rcc_clock_setup_pll(&rcc_hse_25mhz_3v3[RCC_CLOCK_3V3_84MHZ]);
  clock_setup();
  gpio_setup();
  i2c_setup();
  usart_setup();

  while (1)
  {
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
