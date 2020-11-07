/* UART Driver for Nucleo-F103RB
 * THIS DRIVER COMES WITH ABSOLUTELY NO WARRANTY!
 *
 * uart_driver.c
 *
 *  Created on: Nov 7, 2020
 *      Author: Uğur Özgür
 *      ozgur19@itu.edu.tr ugur-ozgur.gen.tr
 */

#include "stm32f1xx_hal.h"
#include "stdlib.h"
#include "string.h"

#include "uart_driver.h"

//Create a Ring Buffer for TX Operations
char write_buffer[BUFFER_LENGTH_TX];
int wb_head = 0;
int wb_tail = 0;

//Create a Ring Buffer for RX Operations
char read_buffer[BUFFER_LENGTH_RX];
int rb_head = 0;
int rb_tail = 0;

//int read_buffer_waiting_bytes(){
//	if(rb_head<rb_tail)
//		return (rb_head+256 - rb_tail);
//	else
//		return rb_head - rb_tail;
//}

void uart_driver_init(void) {
	RCC->APB2ENR |= (0b1 << 2); //GPIOA Clock Enable
	RCC->APB1ENR |= (0b1 << 17); //USART2 Clock Enable
	GPIOA->CRL &= ~(0x74 << 8); //USART2 GPIOA Alternate Function
	GPIOA->CRL |= (0x8B << 8); //USART2 GPIOA Alternate Function
	GPIOA->ODR |= (0b1 << 3); //USART2 RX Pull-up
	USART2->BRR = (USARTDIV); //USART2 BaudRate
	USART2->CR1 |= 0x202C; //USART2 Enable, TX RX Enable, RXNE Interrupt Enable
	NVIC->ISER[1] |= (0b1 << 6); //USART2 Interrupt Enable
}

void uart_driver_write(char data_value[]) {
	int data_size = strlen(data_value); //Get Data Length
	for (int i = 0; i < data_size; i++) {
		write_buffer[wb_head++] = data_value[i]; //Write Data to Buffer
		if (wb_head == BUFFER_LENGTH_TX) {wb_head = 0;} //OVERFLOW
	}
	USART2->CR1 |= (0b1 << 7); //USART2 TXE Interrupt Enable
}

char uart_driver_read_byte_buffer(void) {
	if (rb_head == rb_tail) //Check the Buffer for Waiting Bytes
		return 0;
	else {
		char send_value = 0;
		send_value = read_buffer[rb_tail++]; //Get Value From Buffer
		if (rb_tail == BUFFER_LENGTH_RX) {rb_tail = 0;} //OVERFLOW
		return send_value;
	}
}

void USART2_IRQHandler(void) {
	if (((USART2->CR1 & (0b1 << 7)) == 0b1 << 7)&& ((USART2->SR & (0b1 << 7)) == 0b1 << 7)) { //Check the Interrupt Reason
		if (wb_tail != wb_head) { //Check the Buffer for Waiting Bytes
			USART2->DR = write_buffer[wb_tail++]; //USART2 Send Byte from Buffer
			if (wb_tail == BUFFER_LENGTH_TX) {
				wb_tail = 0;
			} //OVERFLOW
		} else {
			USART2->CR1 &= ~(0b1 << 7); //USART2 TXE Interrupt Disable
		}
	}
	if (((USART2->CR1 & (0b1 << 5)) == 0b1 << 5)&& ((USART2->SR & (0b1 << 5)) == 0b1 << 5)) { //Check the Interrupt Reason
		read_buffer[rb_head++] = USART2->DR; //Write Data to Buffer
		if (rb_head == BUFFER_LENGTH_RX) {rb_head = 0;} //OVERFLOW
	}
}
