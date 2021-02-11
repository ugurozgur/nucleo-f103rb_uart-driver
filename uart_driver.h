/* UART Driver for Nucleo-F103RB
 * THIS DRIVER COMES WITH ABSOLUTELY NO WARRANTY!
 *
 * uart_driver.h
 *
 *  Created on: Nov 7, 2020
 *      Author: Uğur Özgür
 *      ozgur19@itu.edu.tr ugur-ozgur.gen.tr
 */

#ifndef MODULES_UART_DRIVER_H_
#define MODULES_UART_DRIVER_H_

#define BUFFER_LENGTH_TX 256 //Adjust TX buffer length, consider RAM usage
#define BUFFER_LENGTH_RX 256 //Adjust RX buffer length, consider RAM usage
#define USARTDIV 0xD0<<4 //SEE STM32F103 REFERENCE MANUAL page 798

void uart_driver_init(void); //Call this function in start
void uart_driver_write(char data_value[]); //Use this function to send data
char uart_driver_read_byte_buffer(void); //Returns the oldest byte from the RX buffer

#endif /* MODULES_UART_DRIVER_H_ */
