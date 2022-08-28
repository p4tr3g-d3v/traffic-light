#include "utils.h"

void traffic_blink(uint32_t gpio_port, uint16_t gpios)
{
  int j = 0;
  while(j < 3)
  {
    gpio_set(gpio_port, gpios);
    for (unsigned long i = 0; i < 10000000; i++)
    { /* Wait a bit. */
      __asm__("nop");
    }
    gpio_clear(gpio_port, gpios);
    for (unsigned long i = 0; i < 10000000; i++)
    { /* Wait a bit. */
      __asm__("nop");
    }
    j++;
  }
  gpio_clear(gpio_port, gpios);
}

void traffic_delay(int d)
{
  if (d == 0) {
    for (unsigned long i = 0; i < 10000000; i++)
    { /* Wait a bit. */
      __asm__("nop");
    }
  }else if (d == 1)
  {
    for (unsigned long i = 0; i < 100000000; i++)
    { /* Wait a bit. */
      __asm__("nop");
    }
  }
  
}

// Setup clock`s
void clock_setup()
{
  // Enable clock per channel
  rcc_periph_clock_enable(RCC_GPIOA);
  rcc_periph_clock_enable(RCC_GPIOB);
  rcc_periph_clock_enable(RCC_GPIOC);

  rcc_periph_clock_enable(RCC_I2C1);
  rcc_periph_clock_enable(RCC_USART1);
}
// GPIO Setup
void gpio_setup()
{
  // i2c setup
  gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO6 | GPIO7);
  gpio_set_af(GPIOB, GPIO_AF4, GPIO6 | GPIO7);
  gpio_set_output_options(GPIOB, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO6 | GPIO7);
  // i2c setup end

  // Builtin led setup
  gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO13);
  gpio_clear(GPIOC, GPIO13);
  // Builtin led setup end

  // Traffic light setup
  gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO0 | GPIO1 | GPIO2 | GPIO3 | GPIO4 | GPIO5 | GPIO6);
  gpio_clear(GPIOA, GPIO0 | GPIO1 | GPIO2 | GPIO3 | GPIO4 | GPIO5 | GPIO6);
  // Traffic light setup end

  // UART setup
  gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO9);
  gpio_set_af(GPIOA, GPIO_AF7, GPIO9);
  // UART setup end
}

#define I2C_ADDRESS 0x15 //! change

void i2c_setup()
{
  i2c_peripheral_disable(I2C1);                                  // Вырубаем перефирию
  i2c_reset(I2C1);                                               // Сбрасываем
  i2c_set_standard_mode(I2C1);                                   // Устанавливаем стандартный режим(скорости) 100КГц
  nvic_enable_irq(NVIC_I2C1_EV_IRQ);                             // Включаем контроллер прерываний
  i2c_set_own_7bit_slave_address(I2C1, I2C_ADDRESS);             // Устанавливаем адрес
  i2c_enable_interrupt(I2C1, I2C_CR2_ITEVTEN | I2C_CR2_ITBUFEN); // Включаем прерывания
  i2c_peripheral_enable(I2C1);                                   // Включаем переферию
  i2c_enable_ack(I2C1);                                          // Отвечаем при опросе
}
void usart_setup()
{
  usart_set_baudrate(USART1, 115200);
  usart_set_databits(USART1, 8);
  usart_set_stopbits(USART1, USART_STOPBITS_1);
  usart_set_mode(USART1, USART_MODE_TX); //! RX|TX
  usart_set_parity(USART1, USART_PARITY_NONE);
  usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);
  usart_enable(USART1);
}

int _write(int fd, char *ptr, int len)
{
  int i = 0;

  /*
   * Write "len" of char from "ptr" to file id "fd"
   * Return number of char written.
   *
   * Only work for STDOUT, STDIN, and STDERR
   */
  if (fd > 2)
  {
    return -1;
  }
  while (*ptr && (i < len))
  {
    usart_send_blocking(USART1, *ptr);
    if (*ptr == '\n')
    {
      usart_send_blocking(USART1, '\r');
    }
    i++;
    ptr++;
  }
  return i;
}
