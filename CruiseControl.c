#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include "licenta.h"

//CPU frequency = 16 MHz
#define F_CPU 16000000UL																

int dutyCycle = 0;
int speed_Data, slope_Data;

//start ADC conversion
void start_conversion(){
	//ADSC bit starts the conversion																
	ADCSRA |= (1 << ADSC);																
}

void ADC_speed_setup(){
	//REFS0 set for use of AVcc with external capacitor, MUX1 and MUX0 are used 
	//to select the analog input connected to the ADC(ADC3)
	ADMUX = (1 << REFS0) | (1 << MUX0) | (1 << MUX1);	
	//ADEN enables the ADC, ADIF becomes one when an ADC conversion completes 
	//and data registers are updated, ADPS register is used	
	//to select the prescaler value							
	ADCSRA = (1 << ADEN) | (1 << ADIF) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);	 
	//DIDR0 disables the digital input buffer on the corresponding ADC																					
	DIDR0 = (1 << ADC3D);																
	
	start_conversion();
}

void ADC_slope_setup(){
	//REFS0 set for use of AVcc with external capacitor. MUX2 selects ADC4 as the analog input
	ADMUX = (1 << REFS0) | (1 << MUX2);
	//ADEN enables the ADC, ADIF becomes one when an ADC conversion completes 
	//and data registers are updated, ADPS register is used 
	//to select the prescaler value													
	ADCSRA = (1 << ADEN) | (1 << ADIF) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);	
	//DIDR0 disables the digital input buffer on the corresponding ADC
	DIDR0 = (1 << ADC4D);																
	
	start_conversion();
}

void DC_setup(){
	//Sets pins PB0, PB3, PB4 as outputs, the pins corresponding to the L289N 
	//driver being IN1(PB0), IN2(PB4), ENA(PB3)
	DDRB = (1 << PORTB0) | (1 << PORTB3) | (1 << PORTB4);	
	
	//Toggles all pins off							
	PORTB = 0x00;																		
}

void MotorON(){
	DC_setup();
	
	//Sets IN1 to 1 and IN2 to 0 to establish a forward rotation of the motor
	PORTB = (1 << PORTB0) | (0 << PORTB4);												
}

int main(void)
{
	//speed reference array to be used in the bilinear interpolation function
	int speed_array[] = { 0, 204, 409, 614, 1023 };		
	//slope reference array to be used in the bilinear interpolation function									
	int slope_array[] = { 0, 204, 409, 614, 1023 };		
	//arrays where the converted decimal to char values will be stored								
	char analog[15], message[20];
	
	//Sets every pin of PORTD as outputs														
	DDRD = 0xFF;
	//Sets PC0, PC1 and PC2 as outputs																		
	DDRC = (1 << PORTC0) | (1 << PORTC1) | (1 << PORTC2);								
	
	//PWM setup
	//in Fast PWM mode, setting COM2A1 will clear OC2A on compare match and set at bottom; 
	//WGM20-1 are set to select the Fast PWM Mode
	TCCR2A = (1 << COM2A1) | (1 << WGM20) | (1 << WGM21);		
		
	//Enables overflow interrupt					
	TIMSK2 = (1 << TOIE2);															
	//enables global interrupts
	sei();				
	//CS20 and CS22 are set to select the clock source																
	TCCR2B = (1 << CS20) | (1 << CS22);	
													
	//Initializes the LCD by performing a reset
	Lcd_Init();		
	//Clears the LCD display																	
	Lcd_ClearDisplay();		
	//Initializes the DC motor and sets its wheel rotation direction															
	MotorON();																			
	while(1)
	{
		//Initializes the ADC channel corresponding to its analog input
		ADC_speed_setup();	
		//Waits until the ADC conversion is complete(when ADIF bit is set)															
		while(ADCSRA & (1 << ADIF)){}	
		//The analog value is stored in the speed_Data variable												
		speed_Data = ADC;	
		
		//Using the linear interpolation method, the acquired analog value is mapped on a [0-256] interval															
		int AnalogToMotor = map(speed_Data, 2, 1022, 0, 256);		
		//Duty cycle receives the mapped value of the analog input					
		dutyCycle = AnalogToMotor;	
															
		//itoa function converts the decimal(10) value into characters 
		//that will be stored in the analog character array
		itoa(AnalogToMotor, analog, 10);
		
		//Sets the LCD cursor on row 1 column 0												
		Lcd_SetCursorPos(1, 0);	
		
		//The string to be displayed on row 1 column 0 of the LCD														
		Lcd_WriteString("Speed: ");		
		//Displays the character array with the converted and stored decimal value												
		Lcd_WriteString(analog);	
		//Will delete the character on column 8 row 1, if the speed value is less than 10, 
		//to avoid the display of unwanted residual characters													
		if(AnalogToMotor < 10){	
			//Sets the LCD cursor on row 1 column 8														
			Lcd_SetCursorPos(1, 8);		
			//Writes a blank character to delete the unwanted character												
			Lcd_WriteChar(' ');															
		}
		//Will delete the character on column 9 row 1, if the speed values is less than 100, 
		//to avoid the display of unwanted residual characters
		if(AnalogToMotor < 100){	
			//Sets the LCD cursor on row 1 column 9													
			Lcd_SetCursorPos(1, 9);	
			//Writes a blank character to delete the unwanted character													
			Lcd_WriteChar(' ');															
		}
		//Will delete the character on column 10 row 1, if the speed value is less than 1000, 
		//to avoid the display of unwanted residual characters
		if(AnalogToMotor < 1000){	
			//Sets the LCD cursor on row 1 column 10													
			Lcd_SetCursorPos(1, 10);	
			//Writes a blank character to delete the unwanted character												
			Lcd_WriteChar(' ');															
		}
		
		//Initializes the ADC channel corresponding to its analog input
		ADC_slope_setup();		
		//Waits until the ADC conversion is complete(when ADIF bit is high)														
		while(ADCSRA & (1 << ADIF)){}		
		//The read analog value is stored in the slope_Data variable											
		slope_Data = ADC;		
																
		//Sets the LCD cursor on row 2 column 9
		Lcd_SetCursorPos(2, 9);		
		//The string(A for road Angle) to be displayed on row 2 column 9 of the LCD													
		Lcd_WriteString("A: ");		
		//Converts the decimal value into characters that will be stored in the message character array													
		itoa(slope_Data, message, 10);	
		//Displays the character array containing the converted decimal value												
		Lcd_WriteString(message);	
		//Will delete the character on row 2 column 13, if the slope value is less than 10, 
		//to avoid the display of unwanted residual characters													
		if(slope_Data < 10){			
			//Sets the LCD cursor on row 2 column 13												
			Lcd_SetCursorPos(2, 13);	
			//Writes a blank character to eliminate the unwanted character												
			Lcd_WriteChar(' ');														
		}
		//Will delete the character on row 2 column 14, if the slope value is less than 100, 
		//to avoid the display of unwanted residual characters
		if(slope_Data < 100){		
			//Sets the LCD cursor on row 2 column 14														
			Lcd_SetCursorPos(2, 14);	
			//Writes a blank character to eliminate the unwanted character											
			Lcd_WriteChar(' ');														
		}
		//Will delete the character on row 2 column 15, if the slope value is less than 1000, 
		//to avoid the display of unwanted residual characters
		if(slope_Data < 1000){		
			//Sets the LCD cursor on row 2 column 15													
			Lcd_SetCursorPos(2, 15);
			//Write a black character to eliminate the unwanted character													
			Lcd_WriteChar(' ');														
		}
		
		//Sets the LCD cursor on row 2 column 0
		Lcd_SetCursorPos(2, 0);		
		//The string to be displayed on row 2 column 0														
		Lcd_WriteString("RPM: ");		
		
		//The bilinear interpolation function will take the speed and slope arrays, the analog inputs and,		
		//after performing a series of calculations, will return an approximated value of the motor RPM											
		double RPM = bilinearInterpolation(speed_Data, slope_Data, speed_array, slope_array);	
		
		//Converts the obtained value into characters that are stored in the message character array																					
		itoa(RPM, message, 10);		
		//Displays the character array containing the converted decimal value 														
		Lcd_WriteString(message);	
		//Will delete the character on row 2 column 7, if the RPM value is less than 100, 
		//to avoid the display of unwanted residual characters														
		if(RPM < 100){		
			//Sets the LCD cursor on row 2 column 7 																
			Lcd_SetCursorPos(2, 7);				
			//Writes a blank character to eliminate the unwanted character											
			Lcd_WriteChar(' ');															
		}
	}
} 

//When the timer overflows an Interrupt Service Routine will be triggered 
ISR(TIMER2_OVF_vect){	
	//Sets the TOP value with the calculated duty cycle																	
	OCR2A = dutyCycle;																		
}