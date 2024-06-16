#include<stdio.h>
#include<stdlib.h>
#include<util//delay.h>

//Function will parse through an array and return the required position in said array
int array_position(int arr[5], int input) {											
	int i, pos;
	//Loop will parse through the array
	for (i = 0; i < 5; i++) {	
		//Will parse through the array until its value is greater or equal to the input value													
		if (arr[i] >= input) {	
			//Stores the array position in the pos variable 													
			pos = i;	
			//Returns the value of pos															
			return pos;																
		}
	}
	//Return -1 if the input value is outside the array's range
	return -1;																		
}

//Function will iterate through a square matrix(bidimensional table) and return the required element
int table_search(int table[5][5], int speed_pos, int slope_pos) {					
	int element, i, j;
	//First for loop will parse through the column of the table
	for (i = 0; i < 5; i++)			
	//Second for loop will parse through the rows of the table												
		for (j = 0; j < 5; j++)		
			//If the i and j values match the speed and slope positions, the function will return the searched element												
			if (i == speed_pos && j == slope_pos) {			
				//The found element of the table will be stored in the element variable						
				element = table[i][j];												
				return element;
	}
	//Returns -1 if i and j values are outside the table's range
	return -1;																		
}

int bilinearInterpolation(int speed, int slope, int speed_arr[5], int slope_arr[5]) {
	int speed_pos, slope_pos;
	double R1, R2, RPM;
	int bidimensional_table[5][5] = { {33, 33, 33, 33, 33}, 
									  {64, 51, 42, 55, 64}, 
									  {127,108,83,110,127}, 
									  {191,157,139,162,191}, 
									  {255,255,255,255,255} };
	//Speed position will be returned based on the input speed value compared to the array reference values
	speed_pos = array_position(speed_arr, speed);	
	//Slope position will be returned based on the input slope value compared to the array reference values								
	slope_pos = array_position(slope_arr, slope);									
	R1 = table_search(bidimensional_table, speed_pos - 1, slope_pos - 1) + ((speed - speed_arr[speed_pos - 1])*
		(table_search(bidimensional_table, speed_pos - 1, slope_pos) - 
		 table_search(bidimensional_table, speed_pos - 1, slope_pos - 1)) / (speed_arr[speed_pos] - speed_arr[speed_pos - 1]));
		 
	R2 = table_search(bidimensional_table, speed_pos, slope_pos - 1) + ((speed - speed_arr[speed_pos - 1])*
		(table_search(bidimensional_table, speed_pos, slope_pos) - 
	     table_search(bidimensional_table, speed_pos, slope_pos - 1)) / (speed_arr[speed_pos] - speed_arr[speed_pos - 1]));
		 
	RPM = R1 + ((slope - slope_arr[slope_pos - 1])*(R2 - R1) / (slope_arr[slope_pos] - slope_arr[slope_pos - 1]));
	return RPM;
}

//Function which will map the input value from an input interval and return 
//the value in a desired output interval
long map(long input, long in_min, long in_max, long out_min, long out_max) {		
	return (input - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void Lcd_PinWrite(char ch)
{
	if(ch & 1)
	PORTD |= (1 << PORTD4);
	else
	PORTD &= ~(1 << PORTD4);
	
	if(ch & 2)
	PORTD |= (1 << PORTD5);
	else
	PORTD &= ~(1 << PORTD5);
	
	if(ch & 4)
	PORTD |= (1 << PORTD6);
	else
	PORTD &= ~(1 << PORTD6);
	
	if(ch & 8)
	PORTD |= (1 << PORTD7);
	else
	PORTD &= ~(1 << PORTD7);
}

void Lcd_CmdWrite(char ch)
{
	PORTC &= ~(1 << PORTC0); 
	Lcd_PinWrite(ch);
	PORTC |= (1 << PORTC1);
	_delay_ms(1);
	PORTC &= ~(1 << PORTC1);
	_delay_ms(1);
}

void Lcd_ClearDisplay()
{
	Lcd_CmdWrite(0);
	Lcd_CmdWrite(1);
}

void Lcd_SetCursorPos(char row, char col)
{
	char a, b;
	if(row == 1)
	{
		a = 0x80 >> 4;
		b = (0x80 + col) & 0x0F;
		Lcd_CmdWrite(a);
		Lcd_CmdWrite(b);
	}
	else if(row == 2)
	{
		a = 0xC0 >> 4;
		b = (0xC0 + col) & 0x0F;
		Lcd_CmdWrite(a);
		Lcd_CmdWrite(b);
	}
}

void Lcd_Init()
{
	Lcd_PinWrite(0x00);
	_delay_ms(20);

	Lcd_CmdWrite(0x03);
	_delay_ms(5);
	Lcd_CmdWrite(0x03);
	_delay_ms(11);
	Lcd_CmdWrite(0x03);

	Lcd_CmdWrite(0x02);
	Lcd_CmdWrite(0x02);
	Lcd_CmdWrite(0x08);
	Lcd_CmdWrite(0x00);
	Lcd_CmdWrite(0x0C);
	Lcd_CmdWrite(0x00);
	Lcd_CmdWrite(0x06);
}

void Lcd_WriteChar(char ch)
{
	char temp, y;
	temp = ch & 0x0F;
	y = ch & 0xF0;
	PORTC |= (1 << PORTC0); 
	Lcd_PinWrite(y >> 4);             
	PORTC |= (1 << PORTC1);
	_delay_ms(1);
	PORTC &= ~(1 << PORTC1);
	_delay_ms(1);
	Lcd_PinWrite(temp);
	PORTC |= (1 << PORTC1);
	_delay_ms(1);
	PORTC &= ~(1 << PORTC1);
	_delay_ms(1);
}

void Lcd_WriteString(char *str)
{
	int i;
	for(i = 0; str[i] != '\0'; i++)
		Lcd_WriteChar(str[i]);
}

void Lcd4_Shift_Right()
{
	Lcd_CmdWrite(0x01);
	Lcd_CmdWrite(0x0C);
}

void Lcd4_Shift_Left()
{
	Lcd_CmdWrite(0x01);
	Lcd_CmdWrite(0x08);
}
