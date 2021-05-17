#include "main.h"
#include "matrixUart.h"

#ifndef __matrix_H__
#define __matrix_H__

#define ll_matrix_number 4


typedef struct
{

GPIO_TypeDef* Port;
uint16_t Pin;
uint8_t matrix_ID;

}Matrix_struct;

void matrix_init(GPIO_TypeDef* PORT, uint16_t PIN, Matrix_struct* matrix_name);
void matrix_update_all(void);
void matrix_set_blink_time(uint16_t time);
void matrix_LED_on_row(Matrix_struct* matrix_name, uint8_t row, uint8_t LED);
void matrix_LED_on(Matrix_struct* matrix_name, uint8_t x, uint8_t y);
void matrix_LED_off(Matrix_struct* matrix_name, uint8_t x, uint8_t y);
void matrix_clear(Matrix_struct* matrix_name);
void matrix_send_cmd(Matrix_struct* matrix_name, dict* matrix_dict);
#endif
