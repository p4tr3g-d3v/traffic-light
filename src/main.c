#include "utils.h"
#include "traffic.h"

#define I2C_BUFFER_SIZE 3
#define I2C_BUFFER_COUNT 1

static uint8_t in_buffer[I2C_BUFFER_COUNT][I2C_BUFFER_SIZE+1];
static bool in_buffer_state;

#define CMD_TURN_OFF 0x10
#define CMD_TURN_ON 0x11

#define TRAFF_LIGHT_OFF 0x20
#define TRAFF_LIGHT_ON 0x21
#define TRAFF_LIGHT_MANUAL 0x23
#define TRAFF_LIGHT_SERVICE 0x22

int traffic_status = 0;

//TODO: i2c
void i2c1_ev_isr(void)
{
  uint32_t sr1, sr2;
  static uint8_t rb = 0;

  sr1 = I2C_SR1(I2C1);

  if (sr1 & I2C_SR1_ADDR){
    rb = 0;
    sr2 = I2C_SR2(I2C1);
    if (sr2 & I2C_SR2_TRA){
      i2c_nack_current(I2C1);
    }
  }else if (sr1 & I2C_SR1_RxNE){
    uint8_t b = i2c_get_data(I2C1);
  }

  if (rb >= I2C_BUFFER_SIZE) return;

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


}
