#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/portpins.h>
#include <avr/pgmspace.h>

//FreeRTOS include files
#include "FreeRTOS.h"
#include "task.h"
#include "croutine.h"

uint8_t temp = 0;

enum LEDState {INIT, UP, DOWN} led_state;
enum BUTTONState {INIT_B, CHANGE, WAIT} button_state;

void LEDS_Init(){
	led_state = INIT;
}

void BUTTON_Init(){
	button_state = INIT_B;
}

void LEDS_Tick() {
	//Actions
	switch(led_state) {
		case INIT:
			PORTD = 1;
			break;
		case UP:
			PORTD = PORTD << 1 ;
			break;
		case DOWN:
			PORTD = PORTD >> 1;
			break;
	}
	//Transitions
	switch(led_state) {
		case INIT:
			led_state = UP;
			break;
		case UP:
			if(PORTD >= 128) {
				led_state = DOWN;
			}
			else
				led_state = UP;
			break;
		case DOWN:
			if(PORTD <= 1) {
				led_state = UP;
			}
			else
				led_state = DOWN;
			break;
	}
}

void BUTTON_Tick() {
	//Actions
	switch (button_state)
	{
		case INIT_B:
			break;
		case CHANGE:
			if (led_state == UP && PORTD != 1)
			{
				led_state = DOWN;
			} 
			else if (led_state == DOWN && PORTD != 128)
			{
				led_state = UP;
			}
			break;
		case WAIT:
			break;
	}
	//Transitions
	switch (button_state)
	{
		case INIT_B:
			button_state = WAIT;
			break;
		case CHANGE:
			button_state = WAIT;
			break;
		case WAIT:
			temp = PINA & 0x01;
			if(temp == 0x00) {
				button_state = CHANGE;
			}
			break;
	}
}

void LedSecTask()
{
	LEDS_Init();
	for(;;)
	{
		LEDS_Tick();
		vTaskDelay(100);
	}
}

void ReadButtonTask()
{
	BUTTON_Init();
	for(;;)
	{
		BUTTON_Tick();
		vTaskDelay(100);
	}
}

void StartSecPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(LedSecTask, (signed portCHAR *)"LedSecTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
}

void ReadButton(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(ReadButtonTask, (signed portCHAR *)"ReadButtonTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
}

int main(void)
{
	DDRA = 0x00; PORTA=0xFF;
	DDRD = 0xFF; //PORTD output
	//Start Tasks
	StartSecPulse(1);
	ReadButton(2);
	//RunSchedular
	vTaskStartScheduler();
	
	return 0;
}