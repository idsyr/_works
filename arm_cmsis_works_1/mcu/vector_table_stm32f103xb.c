extern int ( *const vector_table_concrete[] )( void );

static void default_handle() {
  for ( ;; )
    ;
}

__attribute__( ( weak, alias( "default_handle" ) ) ) void handle_wwdg(); // WWDG
__attribute__( ( weak, alias( "default_handle" ) ) ) void handle_pvd();  // PVD
__attribute__( ( weak, alias( "default_handle" ) ) ) void handle_tamper(); // Tamper
__attribute__( ( weak, alias( "default_handle" ) ) ) void handle_rtc();    // RTC
__attribute__( ( weak, alias( "default_handle" ) ) ) void handle_flash();  // Flash
__attribute__( ( weak, alias( "default_handle" ) ) ) void handle_rcc();    // RCC
__attribute__( ( weak, alias( "default_handle" ) ) ) void handle_exti0();  // EXTI0
__attribute__( ( weak, alias( "default_handle" ) ) ) void handle_exti1();  // EXTI1
__attribute__( ( weak, alias( "default_handle" ) ) ) void handle_exti2();  // EXTI2
__attribute__( ( weak, alias( "default_handle" ) ) ) void handle_exti3();  // EXTI3
__attribute__( ( weak, alias( "default_handle" ) ) ) void handle_exti4();  // EXTI4
__attribute__( ( weak, alias( "default_handle" ) ) ) void handle_dma1_channel1(); // DMA1_Channel1
__attribute__( ( weak, alias( "default_handle" ) ) ) void handle_dma1_channel2(); // DMA1_Channel2
__attribute__( ( weak, alias( "default_handle" ) ) ) void handle_dma1_channel3(); // DMA1_Channel3
__attribute__( ( weak, alias( "default_handle" ) ) ) void handle_dma1_channel4(); // DMA1_Channel4
__attribute__( ( weak, alias( "default_handle" ) ) ) void handle_dma1_channel5(); // DMA1_Channel5
__attribute__( ( weak, alias( "default_handle" ) ) ) void handle_dma1_channel6(); // DMA1_Channel6
__attribute__( ( weak, alias( "default_handle" ) ) ) void handle_dma1_channel7(); // DMA1_Channel7
__attribute__( ( weak, alias( "default_handle" ) ) ) void handle_adc1_2();        // ADC1_2
__attribute__( ( weak, alias( "default_handle" ) ) ) void handle_usb_hp_can_tx(); // USB_HP_CAN_TX
__attribute__( ( weak, alias( "default_handle" ) ) ) void handle_usb_lp_can_rx0(); // USB_LP_CAN_RX0
__attribute__( ( weak, alias( "default_handle" ) ) ) void handle_can_rx1();        // CAN_RX1
__attribute__( ( weak, alias( "default_handle" ) ) ) void handle_can_sce();        // CAN_SCE
__attribute__( ( weak, alias( "default_handle" ) ) ) void handle_exti9_5();        // EXTI9_5
__attribute__( ( weak, alias( "default_handle" ) ) ) void handle_tim1_brk();       // TIM1_BRK
__attribute__( ( weak, alias( "default_handle" ) ) ) void handle_tim1_up();        // TIM1_UP
__attribute__( ( weak, alias( "default_handle" ) ) ) void handle_tim1_trg_com();   // TIM1_TRG_COM
__attribute__( ( weak, alias( "default_handle" ) ) ) void handle_tim1_cc();        // TIM1_CC
__attribute__( ( weak, alias( "default_handle" ) ) ) void handle_tim2();           // TIM2
__attribute__( ( weak, alias( "default_handle" ) ) ) void handle_tim3();           // TIM3
__attribute__( ( weak, alias( "default_handle" ) ) ) void handle_tim4();           // TIM4
__attribute__( ( weak, alias( "default_handle" ) ) ) void handle_i2c1_ev();        // I2C1_EV
__attribute__( ( weak, alias( "default_handle" ) ) ) void handle_i2c1_er();        // I2C1_ER
__attribute__( ( weak, alias( "default_handle" ) ) ) void handle_i2c2_ev();        // I2C2_EV
__attribute__( ( weak, alias( "default_handle" ) ) ) void handle_i2c2_er();        // I2C2_ER
__attribute__( ( weak, alias( "default_handle" ) ) ) void handle_spi1();           // SPI1
__attribute__( ( weak, alias( "default_handle" ) ) ) void handle_spi2();           // SPI2
__attribute__( ( weak, alias( "default_handle" ) ) ) void handle_usart1(); // USART1
__attribute__( ( weak, alias( "default_handle" ) ) ) void handle_usart2(); // USART2
__attribute__( ( weak, alias( "default_handle" ) ) ) void handle_usart3(); // USART3
__attribute__( ( weak, alias( "default_handle" ) ) ) void handle_exti15_10(); // EXTI15_10
__attribute__( ( weak, alias( "default_handle" ) ) ) void handle_rtcalarm();  // RTCAlarm
__attribute__( ( weak, alias( "default_handle" ) ) ) void handle_usbwakeup(); // USBWakeup
__attribute__( ( weak, alias( "default_handle" ) ) ) void handle_tim8_brk();  // TIM8_BRK
__attribute__( ( weak, alias( "default_handle" ) ) ) void handle_tim8_up();   // TIM8_UP
__attribute__( ( weak, alias( "default_handle" ) ) ) void handle_tim8_trg_com(); // TIM8_TRG_COM
__attribute__( ( weak, alias( "default_handle" ) ) ) void handle_tim8_cc();      // TIM8_CC
__attribute__( ( weak, alias( "default_handle" ) ) ) void handle_adc3();         // ADC3
__attribute__( ( weak, alias( "default_handle" ) ) ) void handle_fsmc();         // FSMC
__attribute__( ( weak, alias( "default_handle" ) ) ) void handle_sdio();         // SDIO
__attribute__( ( weak, alias( "default_handle" ) ) ) void handle_tim5();         // TIM5
__attribute__( ( weak, alias( "default_handle" ) ) ) void handle_spi3();         // SPI3
__attribute__( ( weak, alias( "default_handle" ) ) ) void handle_uart4();        // UART4
__attribute__( ( weak, alias( "default_handle" ) ) ) void handle_uart5();        // UART5
__attribute__( ( weak, alias( "default_handle" ) ) ) void handle_tim6();         // TIM6
__attribute__( ( weak, alias( "default_handle" ) ) ) void handle_tim7();         // TIM7
__attribute__( ( weak, alias( "default_handle" ) ) ) void handle_dma2_channel1(); // DMA2_Channel1
__attribute__( ( weak, alias( "default_handle" ) ) ) void handle_dma2_channel2(); // DMA2_Channel2
__attribute__( ( weak, alias( "default_handle" ) ) ) void handle_dma2_channel3(); // DMA2_Channel3
__attribute__( ( weak, alias( "default_handle" ) ) ) void handle_dma2_channel4_5(); // DMA2_Channel4_5

__attribute__( ( section( ".vector_table_concrete" ) ) ) void ( *const vector_table_f103c8t6[] )( void ) = {
    handle_wwdg,           // WWDG
    handle_pvd,            // PVD
    handle_tamper,         // Tamper
    handle_rtc,            // RTC
    handle_flash,          // Flash
    handle_rcc,            // RCC
    handle_exti0,          // EXTI0
    handle_exti1,          // EXTI1
    handle_exti2,          // EXTI2
    handle_exti3,          // EXTI3
    handle_exti4,          // EXTI4
    handle_dma1_channel1,  // DMA1_Channel1
    handle_dma1_channel2,  // DMA1_Channel2
    handle_dma1_channel3,  // DMA1_Channel3
    handle_dma1_channel4,  // DMA1_Channel4
    handle_dma1_channel5,  // DMA1_Channel5
    handle_dma1_channel6,  // DMA1_Channel6
    handle_dma1_channel7,  // DMA1_Channel7
    handle_adc1_2,         // ADC1_2
    handle_usb_hp_can_tx,  // USB_HP_CAN_TX
    handle_usb_lp_can_rx0, // USB_LP_CAN_RX0
    handle_can_rx1,        // CAN_RX1
    handle_can_sce,        // CAN_SCE
    handle_exti9_5,        // EXTI9_5
    handle_tim1_brk,       // TIM1_BRK
    handle_tim1_up,        // TIM1_UP
    handle_tim1_trg_com,   // TIM1_TRG_COM
    handle_tim1_cc,        // TIM1_CC
    handle_tim2,           // TIM2
    handle_tim3,           // TIM3
    handle_tim4,           // TIM4
    handle_i2c1_ev,        // I2C1_EV
    handle_i2c1_er,        // I2C1_ER
    handle_i2c2_ev,        // I2C2_EV
    handle_i2c2_er,        // I2C2_ER
    handle_spi1,           // SPI1
    handle_spi2,           // SPI2
    handle_usart1,         // USART1
    handle_usart2,         // USART2
    handle_usart3,         // USART3
    handle_exti15_10,      // EXTI15_10
    handle_rtcalarm,       // RTCAlarm
    handle_usbwakeup,      // USBWakeup
    handle_tim8_brk,       // TIM8_BRK
    handle_tim8_up,        // TIM8_UP
    handle_tim8_trg_com,   // TIM8_TRG_COM
    handle_tim8_cc,        // TIM8_CC
    handle_adc3,           // ADC3
    handle_fsmc,           // FSMC
    handle_sdio,           // SDIO
    handle_tim5,           // TIM5
    handle_spi3,           // SPI3
    handle_uart4,          // UART4
    handle_uart5,          // UART5
    handle_tim6,           // TIM6
    handle_tim7,           // TIM7
    handle_dma2_channel1,  // DMA2_Channel1
    handle_dma2_channel2,  // DMA2_Channel2
    handle_dma2_channel3,  // DMA2_Channel3
    handle_dma2_channel4_5 // DMA2_Channel4_5
};
