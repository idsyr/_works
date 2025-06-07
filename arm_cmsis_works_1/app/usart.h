#include "stm32f103xb.h"
#include "core_cm3.h"

void init_usart1() {
  RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

  GPIOA->CRH |= ( 0b00 << GPIO_CRH_MODE10_Pos );
  GPIOA->CRH |= ( 0b01 << GPIO_CRH_CNF10_Pos );
  GPIOA->CRH |= ( 0b01 << GPIO_CRH_MODE9_Pos );
  GPIOA->CRH |= ( 0b10 << GPIO_CRH_CNF9_Pos );

  USART1->BRR = 7500;

  USART1->CR1 |= USART_CR1_TE;
  USART1->CR1 |= USART_CR1_RE;
  USART1->CR1 |= USART_CR1_RXNEIE;
  USART1->CR1 |= ( 0b0 << USART_CR1_PCE_Pos );
  USART1->CR1 |= ( 0b0 << USART_CR1_M_Pos );
  USART1->CR1 |= ( 0b0 << USART_CR1_SBK_Pos );
  USART1->CR1 |= USART_CR1_UE;

  NVIC_SetPriority( USART1_IRQn, 1 );
  NVIC_EnableIRQ( USART1_IRQn );
}

void putc_usart1( char c ) {
  while ( !( USART1->SR & USART_SR_TXE ) )
    ;
  USART1->DR = c;
  while ( !( USART1->SR & USART_SR_TC ) )
    ;
}

char getc_usart1() {
  while ( !( USART1->SR & USART_SR_RXNE ) )
    ;
  return USART1->DR;
}

void send_usart1( char const * msg, uint16_t sz ) {
  for ( uint16_t tec = 0; tec < sz; ++tec ) {
    putc_usart1( msg[tec] );
  }
}
