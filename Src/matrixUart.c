/*
 * MatrixUsart.c
 *
 *  Created on: May 13, 2021
 *      Author: Admin
 */

#include "matrixUart.h"
#include "matrix.h"

/****************************************************\
* Constants
\****************************************************/
const char START_MESSAGE[] = "\nPLEASE PRESS ENTER";
const char COMMAND_PROMPT[] = "\n>";
const char END_MESSAGE[] = "\n<GOING OFFLINE\n";
const char COMMAND_NOT_FOUND_MESSAGE[] = "\n>COMMAND NOT FOUND\n  >";

const uint8_t ENTER_CHAR = 0x0d;
const uint8_t ESC_CHAR = 0x1b;
const uint8_t BACKSPACE_CHAR = 0x08;

dict dictList[DICTIONARY_SIZE] =
{
        { .key = "arrow NE", .value = {0b00000000, 0b00000010, 0b00000100, 0b01001000, 0b01010000, 0b01100000, 0b01111000, 0b00000000} },
        { .key = "arrow NW", .value = {0b00000000, 0b01111000, 0b01100000, 0b01010000, 0b01001000, 0b00000100, 0b00000010, 0b00000000} },
        { .key = "arrow SE", .value = {0b00000000, 0b01000000, 0b00100000, 0b00010010, 0b00001010, 0b00000110, 0b00011110, 0b00000000} },
        { .key = "arrow SW", .value = {0b00000000, 0b00011110, 0b00000110, 0b00001010, 0b00010010, 0b00100000, 0b01000000, 0b00000000} },
        { .key = "sign 90", .value = {0b00000000, 0b01001110, 0b01001010, 0b01111110, 0b00000000, 0b01111110, 0b01000010, 0b01111110} },
        { .key = "question", .value = {0b00000000, 0b00000000, 0b00000010, 0b10110001, 0b00010001, 0b00001110, 0b00000000, 0b00000000} }

};

/****************************************************\
* Variables
\****************************************************/
uint8_t received;
char buffer[BUFFER_SIZE];
volatile int bufferCounter = 0;
extern UART_HandleTypeDef huart2;
extern uint8_t flagSPI;
extern dict *current_dictionary;
State stateMachine = OFF;
extern uint8_t index;
/*****************************
 * ***********************\
* Private function
\****************************************************/
// send one byte through USART
void usart_send(const char chr) {
   while(!(USART2->ISR & USART_ISR_TC));
   USART2->TDR = chr;

}

// send a string through USART
void usart_send_string(const char *s) {
    int i=0;
    while (s[i]) {
        usart_send(s[i++]);
    }
}

// fun returns 1 if buff is empty
int isBufferEmpty()
{
	int flag = 1;
	for(int i = 0; i < BUFFER_SIZE; i++)
	{
		if(buffer[i] != 0)
		{
			flag = 0;
			break;
		}
	}
	return flag;
}

void clearBuffer()
{
	memset(buffer, 0, sizeof buffer);
	bufferCounter = 0;
}

void handleBackspace()
{
	if(bufferCounter > 0) bufferCounter--;
}

void handleOnState()
{
	uint8_t flag = 0;
	for (uint8_t i =0;i<DICTIONARY_SIZE;i++)
	{
		if(strcmp(buffer, dictList[i].key) == 0)
		{
			//index = i;
			//flagSPI=1;
			usart_send_string("\nIT WORKS");
			flag = 1;
			break;
		}
	}
	if (flag == 0)
	{
		if(isBufferEmpty() == 1) usart_send_string(COMMAND_PROMPT);
		else usart_send_string(COMMAND_NOT_FOUND_MESSAGE);
	}
	clearBuffer();
}

void offStateActions()
{
	if(received == ENTER_CHAR)
	{
		stateMachine = ON;
		usart_send_string(COMMAND_PROMPT);
	}
}

void onStateActions()
{
	if(received == ESC_CHAR)
	{
		clearBuffer();
		stateMachine = OFF;
		usart_send_string(END_MESSAGE);
		usart_send_string(START_MESSAGE);
	}
	else if(received == BACKSPACE_CHAR)
		handleBackspace();
	else
		if(received == ENTER_CHAR) handleOnState();
		else buffer[bufferCounter++] = received;
}

void UART_INTERRUPT_INIT()
{
	HAL_UART_Receive_IT(&huart2, &received, 1);
	usart_send_string(START_MESSAGE);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART2)
	{
		switch(stateMachine)
		{
			default:
			case OFF:
				offStateActions();
				break;

			case ON:
				onStateActions();
				break;

		}
		usart_send(received);

		HAL_UART_Receive_IT(&huart2, &received, 1); // Ponowne włączenie nasłuchiwania
		HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);

		if(bufferCounter >= BUFFER_SIZE) clearBuffer();
	}
}
