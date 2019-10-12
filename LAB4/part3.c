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
enum LEDState {INIT, UP, DOWN} led_state;

void LEDS_Init(){
	led_state = INIT;
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
			break;
		case DOWN:
			if(PORTD <= 1) {
				led_state = UP;
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

void StartSecPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(LedSecTask, (signed portCHAR *)"LedSecTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
}	
 
int main(void) 
{ 
   DDRA = 0x00; PORTA=0xFF;
   DDRD = 0xFF;
   //Start Tasks  
   StartSecPulse(1);
    //RunSchedular 
   vTaskStartScheduler(); 
 
   return 0; 
}