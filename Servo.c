//#define F_CPU 20000000
//#define USART_BAUDRATE 9600											//Desired Baud Rate
//#define BAUD_PRESCALER (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)
//
//#define ASYNCHRONOUS (0<<UMSEL00)									//USART Mode Selection
//
//#define DISABLED    (0<<UPM00)
//#define EVEN_PARITY (2<<UPM00)
//#define ODD_PARITY  (3<<UPM00)
//#define PARITY_MODE  DISABLED										//USART Parity Bit Selection
//
//#define ONE_BIT (0<<USBS0)
//#define TWO_BIT (1<<USBS0)
//#define STOP_BIT ONE_BIT											//USART Stop Bit Selection
//
//#define FIVE_BIT  (0<<UCSZ00)
//#define SIX_BIT   (1<<UCSZ00)
//#define SEVEN_BIT (2<<UCSZ00)
//#define EIGHT_BIT (3<<UCSZ00)
//#define DATA_BIT   EIGHT_BIT										//USART Data Bit Selection
//
//#include <avr/io.h>
//#include <avr/interrupt.h>
//#include <util/delay.h>
//
//double dutyCycle = 0;
//int analog_Data;
//
//void USART_Init()
//{
	////Set Baud Rate
	//UBRR0H = BAUD_PRESCALER >> 8;
	//UBRR0L = BAUD_PRESCALER;
	//
	////Set Frame Format
	//UCSR0C = ASYNCHRONOUS | PARITY_MODE | STOP_BIT | DATA_BIT;
	//
	////Enable Receiver and Transmitter
	//UCSR0B = (1<<RXEN0) | (1<<TXEN0);
//}
//
//void USART_TransmitPolling(uint8_t DataByte)
//{
	//while (( UCSR0A & (1 << UDRE0)) == 0) {};							
	////Do nothing until UDR is ready
	//UDR0 = DataByte;													
	////Load data into the USART data register UDR0
//}
//
//void start_conversion(){
	////Starts the ADC conversion
	//ADCSRA |= (1 << ADSC);												
	////Setting the ADSC bit will start the conversion
//}
//
//void ADC_setup(){
	//ADMUX = (1 << REFS0) | (1 << MUX0) | (1 << MUX2);									
	////REFS0 entails the use of AVcc with external capacitor at AREF pin, 
	////while MUX0 and MUX2 select ADC5 as the desired analog input channel
	//
	//ADCSRA = (1 << ADEN) | (1 << ADIE) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);	
	////Setting the ADEN bit enables the ADC, ADIE enables the conversion complete interrupt, 
	////and ADPS0-2 set the prescaler value to 128
	//
	//DIDR0 = (1 << ADC5D);																
	////Disables the digital input buffer on the corresponding ADC
	//
	//start_conversion();																	
	////Start of the conversion
//}
//
//void Servo_start(){																		
	////Starts the movement of the Servo motor
	//while(dutyCycle < 255){																
		////While the duty cycle is less than 255, 
		////its value will be increased until it no longer fulfills the condition
		//dutyCycle += 2;
		//_delay_ms(20);
		//
	//}
	//
	//while(dutyCycle > 0){																
		////While the duty cycle is greater than 0, 
		////its value will be decreased until it no longer fulfills the condition
		//dutyCycle -= 2;
		//_delay_ms(20);
	//}
//}
//
//int main(void)
//{
	////Sets pin PORTB1 as an output pin
	//DDRB = (1 << PORTB1);									
	//
	////Setting bit COM1A1 will clear OC1A on compare match, and bit WGM10 with WGM12 to select Fast PWM 8-bit mode
	//TCCR1A = (1 << COM1A1) | (1 << WGM10);					
	//
	////Setting bit TOIE1 will enable global interrupts
	//TIMSK1 =  (1 << TOIE1);									
	//
	////Initializes the USART
	//USART_Init();											
	//
	////Initializes the ADC channel
	//ADC_setup();											
	//
	////Enables global interrupts
	//sei();													
	//
	////Setting bits WGM10 and WGM12 to select Fast PWM 8 bit mode, 
	////and CS10 and CS12 selects the clock source as clk/1024
	//TCCR1B = (1 << WGM12) | (1 << CS10) | (1 << CS12);		
	//
	//while(1){	
		//if(analog_Data >  200){		
			////The microcontroller will send the data from the sensor
			////on the Serial Port Control Panel
			//USART_TransmitPolling(analog_Data);									
			////If the analog data is greater than 200, 
			////the operation of the Servo motor will begin
			//Servo_start();
		//}
		//else{
			//PORTB = 0x00;
		//}
	//}
//}
//
//ISR(TIMER1_OVF_vect){																	
	////The Interrupt Service Routine will be triggered when the timer overflows
	//OCR1A = dutyCycle;																	
	////Sets the TOP value with the necessary duty cycle
//}
//
//ISR(ADC_vect){																			
	////The Interrupt Service Routine will be triggered when the ADC conversion is complete
	//analog_Data = ADC;																	
	////The input analog value will be stored in the analog_Data variable
	//start_conversion();
//}