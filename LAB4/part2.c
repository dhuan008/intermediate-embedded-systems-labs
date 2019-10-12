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

enum LED0TICK {INIT0, ON0, OFF0} led0_state;
enum LED2TICK {INIT2, ON2, OFF2} led2_state;
enum LED4TICK {INIT4, ON4, OFF4} led4_state;

void LED0_INIT(){
	led0_state = INIT0;
}
void LED2_INIT(){
	led2_state = INIT2;
}
void LED4_INIT(){
	led4_state = INIT4;
}

void LED0TICK() {
	//Actions
	switch(led0_state) {
		case INIT0:
		PORTD |= 0;
		break;
		case ON0:
		PORTD |= (1<<PB0);
		break;
		case OFF0:
		PORTD &= ~(1<<PB0);
		break;
	}
	//Transitions
	switch(led0_state) {
		case INIT0:
		led0_state = ON0;
		break;
		case ON0:
		led0_state = OFF0;
		break;
		case OFF0:
		led0_state = ON0;
		break;
	}
}
void LED2TICK() {
	//Actions
	switch(led2_state) {
		case INIT2:
		PORTD |= 0;
		break;
		case ON2:
		PORTD |= (1<<PB2);
		break;
		case OFF2:
		PORTD &= ~(1<<PB2);
		break;
	}
	//Transitions
	switch(led2_state) {
		case INIT2:
		led2_state = ON2;
		break;
		case ON2:
		led2_state = OFF2;
		break;
		case OFF2:
		led2_state = ON2;
		break;
	}
}

void LED4TICK() {
	//Actions
	switch(led4_state) {
		case INIT4:
		PORTD |= 0;
		break;
		case ON4:
		PORTD |= (1<<PB4);
		break;
		case OFF4:
		PORTD &= ~(1<<PB4);
		break;
	}
	//Transitions
	switch(led4_state) {
		case INIT4:
		led4_state = ON4;
		break;
		case ON4:
		led4_state = OFF4;
		break;
		case OFF4:
		led4_state = ON4;
		break;
	}
}

void LedSecTask0()
{
	LED0_INIT();
	for(;;)
	{
		LED0TICK();
		vTaskDelay(500);
	}
}

void LedSecTask2()
{
	LED2_INIT();
	for(;;)
	{
		LED2TICK();
		vTaskDelay(1000);
	}
}

void LedSecTask4()
{
	LED4_INIT();
	for(;;)
	{
		LED4TICK();
		vTaskDelay(2500);
	}
}

void StartSecPulse0(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(LedSecTask0, (signed portCHAR *)"LedSecTask0", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
}

void StartSecPulse2(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(LedSecTask2, (signed portCHAR *)"LedSecTask2", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
}

void StartSecPulse4(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(LedSecTask4, (signed portCHAR *)"LedSecTask4", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
}

int main(void)
{
	DDRA = 0x00; PORTA=0xFF;
	DDRD = 0xFF;
	//Start Tasks
	StartSecPulse0(1);
	StartSecPulse2(1);
	StartSecPulse4(1);
	//RunSchedular
	vTaskStartScheduler();
	
	return 0;
}