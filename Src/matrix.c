#include "main.h"
#include "matrix.h"


uint16_t delay_time = 300; //default delay time
uint8_t blinking = 0;

const uint8_t all_matrix_number = 1; //number of used matrices

uint8_t matrix_LED_list[8][1] = {0}; //list of all LED setup
GPIO_TypeDef* Port_list[1]; //list of ports
uint16_t Pin_list[1]; //list of pins
uint8_t matrix_list=0; //ID of matrices


void matrix_init(GPIO_TypeDef* PORT, uint16_t PIN, Matrix_struct* matrix_name)
{

	spi_send(0x09, 0x00, PORT, PIN);   //  no decoding
	spi_send(0x0a, 0x01, PORT, PIN);   //  brightness intensity
	spi_send(0x0b, 0x07, PORT, PIN);   //  scan limit = 8 LEDs
	spi_send(0x0c, 0x01, PORT, PIN);   //  power down =0，normal mode = 1
	spi_send(0x0f, 0x00, PORT, PIN);   //  no test display


	matrix_name->Port = PORT;   //add selected port of matrix CS
	Port_list[matrix_list] = PORT;  //add Port to port list
	matrix_name->Pin = PIN;   //add selected pin of matrix CS
	Pin_list[matrix_list] = PIN;  //add Pin to pin list
	matrix_name->matrix_ID=matrix_list;

	matrix_list++;

}

void matrix_update_all(void) //update all LEDs on all matrices by sending coordinates of matrix_LED_list
{
	for (uint8_t m=0;m<(2*all_matrix_number);m=m+2)
	{
		for(uint8_t n=0;n<8;n++)
			spi_send(n+1, matrix_LED_list[m][n],Port_list[m],Pin_list[m]);
	}
	//HAL_Delay(delay_time);
}


void matrix_toggle_blink()
{
	blinking=blinking^=1; //delay time in ms
}

void matrix_set_blink_time(uint16_t time)
{
	delay_time=time; //delay time in ms
}

void matrix_LED_on_row(Matrix_struct* matrix_name, uint8_t row, uint8_t LED) //turn on LEDs on chosen row
{
	matrix_LED_list[matrix_name->matrix_ID][row-1] = LED;
}

void matrix_LED_on(Matrix_struct* matrix_name, uint8_t x, uint8_t y) //turn on single LED in chosen coordinate
{
	matrix_LED_list[matrix_name->matrix_ID][x-1] |= (1<<(y-1));
}


void matrix_LED_off(Matrix_struct* matrix_name, uint8_t x, uint8_t y) //turn off single LED in chosen coordinate
{
	matrix_LED_list[matrix_name->matrix_ID][x-1] &= ~(1<<(y-1));
}



void matrix_clear(Matrix_struct* matrix_name) //turn off all LEDs in chosen matrix
{
	for (uint8_t z=0; z<8; z++)
	{
		matrix_LED_list[matrix_name->matrix_ID][z] = 0b00000000;
		uint8_t a = matrix_LED_list[matrix_name->matrix_ID][z];

	}
}

void matrix_send_cmd(Matrix_struct* matrix_name, dict* matrix_dict)
{
	for (uint8_t i=1;i<9;i++)
	 {
	  matrix_LED_on_row(matrix_name, i, matrix_dict->value[i-1]);
	 }

	  matrix_update_all();
}

