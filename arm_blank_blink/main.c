#define REG_SZ_IN_BITS 32

#define GPIO_PORT_C_ADDR 0x40011000

#define GPIO_CONF_HIGHPART_REG_OFST 0x04
#define GPIO_PIN_N_OFST_IN_CONF_HIGHPART_REG(X) (X*4-REG_SZ_IN_BITS)

#define GPIO_OUTPUT_REG_OFST 0x0C
#define GPIO_PIN_N_OFST_IN_OUTPUT_REG(X) (1<<X)
#define GPIO_OUTPUT_2MHZ 0b10

#define RCC_ADDR 0x40021000
#define RCC_APB2ENR_REG_OFST 0x018
#define RCC_APB2ENR_IOPC_EN_OFST 0x04

#define vuip (volatile unsigned int *)
#define REG(X) *(vuip(X))
#define forever for(;;)

#define BLUE_PILL_LED_PIN_N 13


void wait ();
void enable_clock_gpioc ();
void conf_led_pin ();
void activate_led ();
void deactivate_led ();



int main ()
{  enable_clock_gpioc();
   conf_led_pin();
   forever
   {  activate_led();
      wait();
      deactivate_led();
      wait();
   }
}


void wait ()
{  volatile int i = 0x20000;
   while (i > 0)
   {  --i;
   }
}


void enable_clock_gpioc ()
{  REG(RCC_ADDR + RCC_APB2ENR_REG_OFST) |= 
      1 << RCC_APB2ENR_IOPC_EN_OFST;
}


void conf_led_pin ()
{  REG(GPIO_PORT_C_ADDR + GPIO_CONF_HIGHPART_REG_OFST) |= 
      (GPIO_OUTPUT_2MHZ << GPIO_PIN_N_OFST_IN_CONF_HIGHPART_REG(BLUE_PILL_LED_PIN_N));
}


void activate_led ()
{  REG(GPIO_PORT_C_ADDR + GPIO_OUTPUT_REG_OFST) |=  GPIO_PIN_N_OFST_IN_OUTPUT_REG(BLUE_PILL_LED_PIN_N);
}


void deactivate_led ()
{  REG(GPIO_PORT_C_ADDR + GPIO_OUTPUT_REG_OFST) &=~ GPIO_PIN_N_OFST_IN_OUTPUT_REG(BLUE_PILL_LED_PIN_N);
}



