/*
 * MatrixUsart.h
 *
 *  Created on: May 13, 2021
 *      Author: Admin
 *
 * 	!!!!!!!!!!PLEASE READ ME!!!!!!!!!!!!!!!!!!!!!!
 *      Configure USART with STM32CubeIDE, enable global interrupts
 * 	IN MAIN FUNCTION ADD UART_INTERRUPT_INIT();
 * 	THATS IT :))
 */

#ifndef INC_MATRIXUART_H_
#define INC_MATRIXUART_H_

#include "main.h"
#include <string.h>

/****************************************************\
* Define
\****************************************************/
#define BUFFER_SIZE 30
#define DICTIONARY_SIZE 6

/****************************************************\
* Typedef
\****************************************************/
typedef enum StateMachine{
	OFF,
	ON
} State;

typedef struct dictionary{
	char *key;
	uint8_t value[8];
}dict;

/****************************************************\
* Private function prototypes
\****************************************************/
void usart_send(const char chr);
void usart_send_string(const char *s);
int isBufferEmpty();
void clearBuffer();
void dictionary_Init();

void handleBackspace();
void handleOnState();
void offStateActions();
void onStateActions();

void UART_INTERRUPT_INIT();
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

#endif /* INC_MATRIXUART_H_ */
