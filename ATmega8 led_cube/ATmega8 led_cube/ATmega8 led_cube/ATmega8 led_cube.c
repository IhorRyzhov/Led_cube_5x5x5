/*
 * ATmega8_led_cube.c
 *
 * Created: 24.02.2017 23:40:14
 *  Author: Игорь
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define SH_CP PC0
#define DS PC1
#define ST_CP PC2
#define MR PC3

#define ON 1
#define OFF 2

uint16_t mas [] =
{
	0b0001000100010001,
	0b1000100010001000,
	0b0100010001000100,
	0b0010001000100010,
	0b0001000100010001
};

volatile unsigned char f; // счетчик слоев для динамической индикации

volatile unsigned char cub [5] [5]; // массив светодиодов

unsigned char izolt [13] [5] = // массив букв
{
	{
		0b10001,
		0b10010,
		0b11100,
		0b10010,
		0b10001
	},
	
	{
		0b11110,
		0b10001,
		0b11110,
		0b10001,
		0b11110
	},	
	
	{
		0b01110,
		0b10001,
		0b00111,
		0b10001,
		0b01110
	},
	
	{
		0b01111,
		0b10000,
		0b10000,
		0b10000,
		0b01111
	},
	
	{
		0b11111,
		0b00100,
		0b00100,
		0b00100,
		0b00100
	},
	
	{
		0b11111,
		0b10001,
		0b10001,
		0b10001,
		0b10001
	},
	
	{
		0b11110,
		0b10001,
		0b11110,
		0b10000,
		0b10000
	},
	
	{
		0b01110,
		0b10001,
		0b10001,
		0b10001,
		0b01110
	},
	
	{
		0b10001,
		0b10011,
		0b10101,
		0b11001,
		0b10001		
	},
	
	{
		0b11110,
		0b00001,
		0b00110,
		0b00001,
		0b11110
	},
	
	{
		0b01111,
		0b01001,
		0b01001,
		0b01001,
		0b10001
	},
	
	{
		0b01111,
		0b10001,
		0b01111,
		0b01001,
		0b10001
	},
	
	{
		0b10010,
		0b10010,
		0b10010,
		0b11110,
		0b00001
	} 
	
	}; 

unsigned char zyfry [4] [5] = // массив цифр
{	
	{
		0b01110,//6
		0b01000,
		0b01110,
		0b01010,
		0b01110
	},
	
	{
		0b01110,//3
		0b00010,
		0b01110,
		0b00010,
		0b01110
	},
	
	{
		0b00010,//1
		0b00110,
		0b00010,
		0b00010,
		0b00010
	},
	
	{
		0b01110,//8
		0b01010,
		0b01110,
		0b01010,
		0b01110,
	}	
};

void layer (volatile unsigned char mas[] [5], volatile unsigned char level) // отрисовка слоя
{				
    for(unsigned char i = 0; i < 5; i++)
	{
		unsigned char x = 0b00000001;
				
	    for(unsigned char j = 0; j < 5; j++)
	    {		
		    if(mas[i] [level] & x)
		     {
			     PORTC|=(1<<DS);
		     }
		     PORTC|=(1<<SH_CP);					
		     _delay_us(1);			
		     PORTC&=~(1<<SH_CP);
		     PORTC&=~(1<<DS);
			 _delay_us(1); //
		     x = x << 1;
	    } 
	
	    if(i < 4)
	    {
	         for(unsigned char k = 0; k < 3; k ++)
	         {
		         PORTC|=(1<<SH_CP);					
		         _delay_us(1);			
		         PORTC&=~(1<<SH_CP);
				 _delay_us(1); //
	         }
	    }	
	}	
	
	PORTC|=(1<<ST_CP);
	_delay_us(1);
	PORTC&=~(1<<ST_CP);
}	
	
void cube_3_x_3 (unsigned char x, unsigned char y, unsigned char z, unsigned char state) // отрисовка кубика; x, y, z (0, 1, 2) : координаты, state : ON - зажечь кубик, OFF - погасить
{
	for(unsigned char a = x; a < (x + 3); a++)
	{		
		for(unsigned char b = y; b < (y + 3); b++)
		{
			for( unsigned char c = z; c < (z + 3); c++)
			{
				if(state == 1)
				cub [c] [b] |= (1 << (4 - a));
				if(state == 2)
				cub [c] [b] &=~ (1 << (4 - a));
			}			
		}		
	}
}	

void cube_draw (void) // перемещение кубика 
{
	for( char a = 0; a < 3; a++)
	  {
		  cube_3_x_3(a,0,0,ON);
		  _delay_ms(150);
		  cube_3_x_3(a,0,0,OFF);		  
	  }
	  
	  for( char a = 1; a < 3; a++)
	  {
		  cube_3_x_3(2,a,0,ON);
		  _delay_ms(150);
		  cube_3_x_3(2,a,0,OFF);		  
	  }
	 
	  for( char a = 1; a < 3; a++)
	  {
		  cube_3_x_3(2,2,a,ON);
		  _delay_ms(150);
		  cube_3_x_3(2,2,a,OFF);		  
	  }
	  
	  for( char a = 2; a > 0; a--)
	  {
		  cube_3_x_3(a-1,2,2,ON);
		  _delay_ms(150);
		  cube_3_x_3(a-1,2,2,OFF);		  
	  }
	  
	  for( char a = 2; a > 0; a--)
	  {
		  cube_3_x_3(0,a-1,2,ON);
		  _delay_ms(150);
		  cube_3_x_3(0,a-1,2,OFF);		  
	  }
	  
	  for( char a = 2; a > 1; a--)
	  {
		  cube_3_x_3(0,0,a-1,ON);
		  _delay_ms(150);
		  cube_3_x_3(0,0,a-1,OFF);		  
	  }
	  
	  for( char a = 0; a < 3; a++)
	  {
		  cube_3_x_3(0,a,0,ON);
		  _delay_ms(150);
		  cube_3_x_3(0,a,0,OFF);		  
	  }
	  
	  for( char a = 1; a < 3; a++)
	  {
		  cube_3_x_3(0,2,a,ON);
		  _delay_ms(150);
		  cube_3_x_3(0,2,a,OFF);		  
	  }
	  
	  for( char a = 1; a < 3; a++)
	  {
		  cube_3_x_3(a,2,2,ON);
		  _delay_ms(150);
		  cube_3_x_3(a,2,2,OFF);		  
	  }
	  
	  for( char a = 2; a > 0; a--)
	  {
		  cube_3_x_3(2,a-1,2,ON);
		  _delay_ms(150);
		  cube_3_x_3(2,a-1,2,OFF);		  
	  }
	  
	  for( char a = 2; a > 0; a--)
	  {
		  cube_3_x_3(2,0,a-1,ON);
		  _delay_ms(150);
		  cube_3_x_3(2,0,a-1,OFF);		  
	  }
	  
	  for( char a = 2; a > 0; a--)
	  {
		  cube_3_x_3(a-1,0,0,ON);
		  _delay_ms(150);
		  cube_3_x_3(a-1,0,0,OFF);		  
	  }
	  
	  _delay_ms(400);	
}	

void romb (unsigned char opt) // отрисовка ромба; opt (0, 1, 2, 3) : вариант вращения ромба
{	
	switch(opt)
	{
	
	case 0:
		
		cub [2]	[0] = 0b00100;
		cub [2]	[1] = 0b01110;
		cub [2]	[2] = 0b11111;
		cub [2]	[3] = 0b01110;
		cub [2]	[4] = 0b00100;
	
		_delay_ms(100);
	
		cub [2]	[0] = 0b00000;
		cub [2]	[1] = 0b00000;
		cub [2]	[3] = 0b00000;
		cub [2]	[4] = 0b00000;
	
		cub [0]	[0] = 0b00100;
		cub [1]	[1] = 0b01110;
		cub [3]	[3] = 0b01110;
		cub [4]	[4] = 0b00100;
	
		_delay_ms(100);
	
		cub [0]	[0] = 0b00000;
		cub [1]	[1] = 0b00000;
		cub [3]	[3] = 0b00000;
		cub [4]	[4] = 0b00000;
	
		cub [0]	[2] = 0b00100;
		cub [1]	[2] = 0b01110;
		cub [3]	[2] = 0b01110;
		cub [4]	[2] = 0b00100;
	
		_delay_ms(100);
	
		cub [0]	[2] = 0b00000;
		cub [1]	[2] = 0b00000;
		cub [3]	[2] = 0b00000;
		cub [4]	[2] = 0b00000;
	
		cub [0]	[4] = 0b00100;
		cub [1]	[3] = 0b01110;
		cub [3]	[1] = 0b01110;
		cub [4]	[0] = 0b00100;
	
		_delay_ms(100);
	
		cub [0]	[4] = 0b00000;
		cub [1]	[3] = 0b00000;
		cub [2] [2] = 0b00000;
		cub [3]	[1] = 0b00000;
		cub [4]	[0] = 0b00000;
	
	break;	
	
	case 1:
	
		cub [2]	[0] = 0b00100;
		cub [2]	[1] = 0b01110;
		cub [2]	[2] = 0b11111;
		cub [2]	[3] = 0b01110;
		cub [2]	[4] = 0b00100;
	
		_delay_ms(100);
	
		cub [2]	[0] = 0b00000;
		cub [2]	[1] = 0b00000;
		cub [2]	[3] = 0b00000;
		cub [2]	[4] = 0b00000;

		cub [0]	[4] = 0b00100;
		cub [1]	[3] = 0b01110;
		cub [3]	[1] = 0b01110;
		cub [4]	[0] = 0b00100;
	
		_delay_ms(100);
	
		cub [0]	[4] = 0b00000;
		cub [1]	[3] = 0b00000;
		cub [3]	[1] = 0b00000;
		cub [4]	[0] = 0b00000;

		cub [0]	[2] = 0b00100;
		cub [1]	[2] = 0b01110;
		cub [3]	[2] = 0b01110;
		cub [4]	[2] = 0b00100;
	
		_delay_ms(100);
	
		cub [0]	[2] = 0b00000;
		cub [1]	[2] = 0b00000;
		cub [3]	[2] = 0b00000;
		cub [4]	[2] = 0b00000;

		cub [0]	[0] = 0b00100;
		cub [1]	[1] = 0b01110;
		cub [3]	[3] = 0b01110;
		cub [4]	[4] = 0b00100;
	
		_delay_ms(100);
	
		cub [0]	[0] = 0b00000;
		cub [1]	[1] = 0b00000;
		cub [2] [2] = 0b00000;
		cub [3]	[3] = 0b00000;
		cub [4]	[4] = 0b00000;

	break;
	
	case 2:
	
		cub [2]	[0] = 0b00100;
		cub [2]	[1] = 0b01110;
		cub [2]	[2] = 0b11111;
		cub [2]	[3] = 0b01110;
		cub [2]	[4] = 0b00100;
	
		_delay_ms(100);
	
		cub [2]	[0] = 0b00100;
		cub [2]	[1] = 0b00100;
		cub [2] [2] = 0b00100;
		cub [2]	[3] = 0b00100;
		cub [2]	[4] = 0b00100;
	
		cub [0]	[2] = 0b10000;	
		cub [1]	[1] = 0b01000;
		cub [1]	[2] = 0b01000;
		cub [1]	[3] = 0b01000;		
		cub [3]	[1] = 0b00010;
		cub [3]	[2] = 0b00010;
		cub [3]	[3] = 0b00010;
		cub [4] [2] = 0b00001;
	
		_delay_ms(100);
	
		cub [0]	[2] = 0b00100;	
		cub [1]	[1] = 0b00100;
		cub [1]	[2] = 0b00100;
		cub [1]	[3] = 0b00100;	
		cub [3]	[1] = 0b00100;
		cub [3]	[2] = 0b00100;
		cub [3]	[3] = 0b00100;
		cub [4] [2] = 0b00100;
	
		_delay_ms(100);
	
		cub [0]	[2] = 0b00001;	
		cub [1]	[1] = 0b00010;
		cub [1]	[2] = 0b00010;
		cub [1]	[3] = 0b00010;	
		cub [3]	[1] = 0b01000;
		cub [3]	[2] = 0b01000;
		cub [3]	[3] = 0b01000;
		cub [4] [2] = 0b10000;
	
		_delay_ms(100);
	
		cub [0]	[2] = 0b00000;	
	
		cub [2] [0] = 0b00000;
		cub [2] [1] = 0b00000;
		cub [2] [2] = 0b00000;
		cub [2] [3] = 0b00000;
		cub [2] [4] = 0b00000;
	
		cub [1]	[1] = 0b00000;
		cub [1]	[2] = 0b00000;
		cub [1]	[3] = 0b00000;	
		cub [3]	[1] = 0b00000;
		cub [3]	[2] = 0b00000;
		cub [3]	[3] = 0b00000;
		cub [4] [2] = 0b00000;
	
	break;
	
	case 3:
	
		cub [2]	[0] = 0b00100;
		cub [2]	[1] = 0b01110;
		cub [2]	[2] = 0b11111;
		cub [2]	[3] = 0b01110;
		cub [2]	[4] = 0b00100;
	
		_delay_ms(100);
	
		cub [2]	[0] = 0b00100;
		cub [2]	[1] = 0b00100;
		cub [2] [2] = 0b00100;
		cub [2]	[3] = 0b00100;
		cub [2]	[4] = 0b00100;
	
		cub [0]	[2] = 0b00001;	
		cub [1]	[1] = 0b00010;
		cub [1]	[2] = 0b00010;
		cub [1]	[3] = 0b00010;	
		cub [3]	[1] = 0b01000;
		cub [3]	[2] = 0b01000;
		cub [3]	[3] = 0b01000;
		cub [4] [2] = 0b10000;
	
		_delay_ms(100);
	
		cub [0]	[2] = 0b00100;	
		cub [1]	[1] = 0b00100;
		cub [1]	[2] = 0b00100;
		cub [1]	[3] = 0b00100;	
		cub [3]	[1] = 0b00100;
		cub [3]	[2] = 0b00100;
		cub [3]	[3] = 0b00100;
		cub [4] [2] = 0b00100;
	
		_delay_ms(100);
	
		cub [0]	[2] = 0b10000;	
		cub [1]	[1] = 0b01000;
		cub [1]	[2] = 0b01000;
		cub [1]	[3] = 0b01000;	
		cub [3]	[1] = 0b00010;
		cub [3]	[2] = 0b00010;
		cub [3]	[3] = 0b00010;
		cub [4] [2] = 0b00001;
	
		_delay_ms(100);
	
		cub [0]	[2] = 0b00000;		
		cub [2] [0] = 0b00000;
		cub [2] [1] = 0b00000;
		cub [2] [2] = 0b00000;
		cub [2] [3] = 0b00000;
		cub [2] [4] = 0b00000;	
		cub [1]	[1] = 0b00000;
		cub [1]	[2] = 0b00000;
		cub [1]	[3] = 0b00000;		
		cub [3]	[1] = 0b00000;
		cub [3]	[2] = 0b00000;
		cub [3]	[3] = 0b00000;
		cub [4] [2] = 0b00000;
	
	break;
	
	}	
}	

void letter (unsigned char bukv[]) // отрисовка и перемещение одной буквы, bukv[5] : массив сегментов - буква
{
	for(unsigned char h = 0; h < 5; h++)
	{		
		for(unsigned char i = 0; i < 5; i++)	
		{
			cub [h] [i] = bukv [i];
		}
		
		_delay_ms(200);	
		
		for(unsigned char i = 0; i < 5; i++)	
		{
			cub [h] [i] = 0;
		}	
	}	
}	

void string (unsigned char arr[] [5]) // отрисовка и перемещение нескольких букв, arr : массив букв
{	
	letter(arr[8]);
	letter(arr[9]);
	letter(arr[7]);
	letter(arr[10]);
	letter(arr[11]);
	letter(arr[12]);
	letter(arr[8]);
	letter(arr[11]);
	
	_delay_ms(150);	
}
	
void digits (unsigned char arr[] [5]) // отрисовка нескольких цифр, array : массив цифр
{
	letter(arr[0]);
	_delay_ms(100);
	letter(arr[1]);
	_delay_ms(100);
	letter(arr[2]);
	_delay_ms(100);
	letter(arr[3]);
	_delay_ms(400);
}

void plane (unsigned char i) // отрисовка и перемещение плоскостей, i : количество повторов
{
	while(i--)
	{
		cub [0] [1] = 0b11111;
		cub [1] [2] = 0b11111;
		cub [2] [3] = 0b11111;
		cub [3] [4] = 0b11111;
		cub [4] [3] = 0b11111;
	
		_delay_ms(100);
	
		cub [0] [1] = 0b00000;
		cub [1] [2] = 0b00000;
		cub [2] [3] = 0b00000;
		cub [3] [4] = 0b00000;
		cub [4] [3] = 0b00000;
	
		cub [0] [2] = 0b11111;
		cub [1] [3] = 0b11111;
		cub [2] [4] = 0b11111;
		cub [3] [3] = 0b11111;
		cub [4] [2] = 0b11111;
	
		_delay_ms(100);
	
		cub [0] [2] = 0b00000;
		cub [1] [3] = 0b00000;
		cub [2] [4] = 0b00000;
		cub [3] [3] = 0b00000;
		cub [4] [2] = 0b00000;
	
		cub [0] [3] = 0b11111;
		cub [1] [4] = 0b11111;
		cub [2] [3] = 0b11111;
		cub [3] [2] = 0b11111;
		cub [4] [1] = 0b11111;
	
		_delay_ms(100);
	
		cub [0] [3] = 0b00000;
		cub [1] [4] = 0b00000;
		cub [2] [3] = 0b00000;
		cub [3] [2] = 0b00000;
		cub [4] [1] = 0b00000;
	
		cub [0] [4] = 0b11111;
		cub [1] [3] = 0b11111;
		cub [2] [2] = 0b11111;
		cub [3] [1] = 0b11111;
		cub [4] [0] = 0b11111;
	
		_delay_ms(100);
	
		cub [0] [4] = 0b00000;
		cub [1] [3] = 0b00000;
		cub [2] [2] = 0b00000;
		cub [3] [1] = 0b00000;
		cub [4] [0] = 0b00000;
	
		cub [0] [3] = 0b11111;
		cub [1] [2] = 0b11111;
		cub [2] [1] = 0b11111;
		cub [3] [0] = 0b11111;
		cub [4] [1] = 0b11111;
	
		_delay_ms(100);
	
		cub [0] [3] = 0b00000;
		cub [1] [2] = 0b00000;
		cub [2] [1] = 0b00000;
		cub [3] [0] = 0b00000;
		cub [4] [1] = 0b00000;
	
		cub [0] [2] = 0b11111;
		cub [1] [1] = 0b11111;
		cub [2] [0] = 0b11111;
		cub [3] [1] = 0b11111;
		cub [4] [2] = 0b11111;
	
		_delay_ms(100);
	
		cub [0] [2] = 0b00000;
		cub [1] [1] = 0b00000;
		cub [2] [0] = 0b00000;
		cub [3] [1] = 0b00000;
		cub [4] [2] = 0b00000;
	
		cub [0] [1] = 0b11111;
		cub [1] [0] = 0b11111;
		cub [2] [1] = 0b11111;
		cub [3] [2] = 0b11111;
		cub [4] [3] = 0b11111;
	
		_delay_ms(100);
	
		cub [0] [1] = 0b00000;
		cub [1] [0] = 0b00000;
		cub [2] [1] = 0b00000;
		cub [3] [2] = 0b00000;
		cub [4] [3] = 0b00000;
	
		cub [0] [0] = 0b11111;
		cub [1] [1] = 0b11111;
		cub [2] [2] = 0b11111;
		cub [3] [3] = 0b11111;
		cub [4] [4] = 0b11111;
	
		_delay_ms(100);
	
		cub [0] [0] = 0b00000;
		cub [1] [1] = 0b00000;
		cub [2] [2] = 0b00000;
		cub [3] [3] = 0b00000;
		cub [4] [4] = 0b00000;
		
	}
}

void zmeyka (unsigned char i) // i : количество повторов
{
	for(unsigned char y = 0 ; y < i; y++)
	{
		for(unsigned char j = 0; j < 5; j++)
		{		
			for(unsigned char i = 0; i < 5; i++)
			{		
				cub [0] [j] |= (1 << i);
				_delay_ms(50);
			}
	
			for(unsigned char i = 1; i < 5; i++)
			{		
				cub [i] [j] |= (1 << 4);
				_delay_ms(50);
			}
	
			for(unsigned char i = 4; i > 0; i--)
			{		
				cub [4] [j] |= (1 << (i - 1));
				_delay_ms(50);
			}
	
			for(unsigned char i = 3; i > 0; i--)
			{		
				cub [i] [j] |= (1 << 0);
				_delay_ms(50);
			}	
		}	
	
		for(unsigned char j = 0; j < 5; j++)
		{		
			for(unsigned char i = 0; i < 5; i++)
			{		
				cub [0] [j] &=~(1 << i);
				_delay_ms(50);
			}
	
			for(unsigned char i = 1; i < 5; i++)
			{		
				cub [i] [j] &=~(1 << 4);
				_delay_ms(50);
			}
	
			for(unsigned char i = 4; i > 0; i--)
			{		
				cub [4] [j] &=~(1 << (i - 1));
				_delay_ms(50);
			}
	
			for(unsigned char i = 3; i > 0; i--)
			{		
				cub [i] [j] &=~(1 << 0);
				_delay_ms(50);
			}	
		}
	}
}

void stena (void)
{
	// 1
	
	for(uint8_t i = 0; i < 5; i++)
	{
		cub [0] [i] = 0b11111;
	}	
	_delay_ms(100);	
	for(uint8_t i = 0; i < 5; i++)
	{
		cub [0] [i] = 0b00000;
	}
	
	cub [0] [0] = 0b11111;
	cub [1] [2] = 0b11111;
	cub [2] [4] = 0b11111;
	_delay_ms(100);
	cub [0] [0] = 0b00000;
	cub [1] [2] = 0b00000;
	cub [2] [4] = 0b00000;
	
	for(uint8_t i = 0; i < 5; i++)
	{
		cub [i] [i] = 0b11111;
	}
	_delay_ms(100);
	for(uint8_t i = 0; i < 5; i++)
	{
		cub [i] [i] = 0b00000;
	}
	
	cub [0] [0] = 0b11111;
	cub [2] [1] = 0b11111;
	cub [4] [2] = 0b11111;
	_delay_ms(100);
	cub [0] [0] = 0b00000;
	cub [2] [1] = 0b00000;
	cub [4] [2] = 0b00000;
	
	for(uint8_t i = 0; i < 5; i++)
	{
		cub [i] [0] = 0b11111;
	}
	_delay_ms(100);
	for(uint8_t i = 0; i < 5; i++)
	{
		cub [i] [0] = 0b00000;
	}
	
	// 2
	
	for(uint8_t i = 0; i < 5; i++)
	{
		cub [i] [0] = 0b00001;
		cub [i] [1] = 0b00100;
		cub [i] [2] = 0b10000;		
	}
	_delay_ms(100);
	for(uint8_t i = 0; i < 5; i++)
	{
		cub [i] [0] = 0b00000;
		cub [i] [1] = 0b00000;
		cub [i] [2] = 0b00000;
	}
	
	for(uint8_t i = 0; i < 5; i++)
	{
		cub [i] [0] = 0b00001;
		cub [i] [1] = 0b00010;
		cub [i] [2] = 0b00100;
		cub [i] [3] = 0b01000;
		cub [i] [4] = 0b10000;
	}
	_delay_ms(100);
	for(uint8_t i = 0; i < 5; i++)
	{
		cub [i] [0] = 0b00000;
		cub [i] [1] = 0b00000;
		cub [i] [2] = 0b00000;
		cub [i] [3] = 0b00000;
		cub [i] [4] = 0b00000;
	}
	
	for(uint8_t i = 0; i < 5; i++)
	{
		cub [i] [0] = 0b00001;
		cub [i] [2] = 0b00010;
		cub [i] [4] = 0b00100;
	}
	_delay_ms(100);
	for(uint8_t i = 0; i < 5; i++)
	{
		cub [i] [0] = 0b00000;
		cub [i] [2] = 0b00000;
		cub [i] [4] = 0b00000;
	}
	
	for(uint8_t i = 0; i < 5; i++)
	{
		cub [i] [0] = 0b00001;
		cub [i] [1] = 0b00001;
		cub [i] [2] = 0b00001;
		cub [i] [3] = 0b00001;
		cub [i] [4] = 0b00001;		
	}
	_delay_ms(100);
	for(uint8_t i = 0; i < 5; i++)
	{
		cub [i] [0] = 0b00000;
		cub [i] [1] = 0b00000;
		cub [i] [2] = 0b00000;
		cub [i] [3] = 0b00000;
		cub [i] [4] = 0b00000;
	}
	
	// 3
	
	for(uint8_t i = 0; i < 5; i++)
	{
		cub [0] [i] = 0b00100;
		cub [2] [i] = 0b00010;
		cub [4] [i] = 0b00001;
	}
	_delay_ms(100);
	for(uint8_t i = 0; i < 5; i++)
	{
		cub [0] [i] = 0b00000;
		cub [2] [i] = 0b00000;
		cub [4] [i] = 0b00000;		
	}
	
	for(uint8_t i = 0; i < 5; i++)
	{
		cub [0] [i] = 0b10000;
		cub [1] [i] = 0b01000;
		cub [2] [i] = 0b00100;
		cub [3] [i] = 0b00010;
		cub [4] [i] = 0b00001;
	}
	_delay_ms(100);
	for(uint8_t i = 0; i < 5; i++)
	{
		cub [0] [i] = 0b00000;
		cub [1] [i] = 0b00000;
		cub [2] [i] = 0b00000;
		cub [3] [i] = 0b00000;
		cub [4] [i] = 0b00000;
	}
	
	for(uint8_t i = 0; i < 5; i++)
	{
		cub [2] [i] = 0b10000;
		cub [3] [i] = 0b00100;
		cub [4] [i] = 0b00001;
	}
	_delay_ms(100);
	for(uint8_t i = 0; i < 5; i++)
	{
		cub [2] [i] = 0b00000;
		cub [3] [i] = 0b00000;
		cub [4] [i] = 0b00000;
	}
	
	for(uint8_t i = 0; i < 5; i++)
	{
		cub [4] [i] = 0b11111;
	}
	_delay_ms(100);
	for(uint8_t i = 0; i < 5; i++)
	{
		cub [4] [i] = 0b00000;
	}
	
	// 4
	
	cub [4] [4] = 0b11111;
	cub [3] [2] = 0b11111;
	cub [2] [0] = 0b11111;
	_delay_ms(100);
	cub [4] [4] = 0b00000;
	cub [3] [2] = 0b00000;
	cub [2] [0] = 0b00000;
	
	for(uint8_t i = 0; i < 5; i++)
	{
		cub [i] [i] = 0b11111;
	}
	_delay_ms(100);
	for(uint8_t i = 0; i < 5; i++)
	{
		cub [i] [i] = 0b00000;
	}
	
	cub [0] [2] = 0b11111;
	cub [2] [3] = 0b11111;
	cub [4] [4] = 0b11111;
	_delay_ms(100);
	cub [0] [2] = 0b00000;
	cub [2] [3] = 0b00000;
	cub [4] [4] = 0b00000;
	
	for(uint8_t i = 0; i < 5; i++)
	{
		cub [i] [4] = 0b11111;
	}
	_delay_ms(100);
	for(uint8_t i = 0; i < 5; i++)
	{
		cub [i] [4] = 0b00000;
	}
	
	// 5
	
	for(uint8_t i = 0; i < 5; i++)
	{
		cub [i] [2] = 0b00001;
		cub [i] [3] = 0b00100;
		cub [i] [4] = 0b10000;
	}
	_delay_ms(100);
	for(uint8_t i = 0; i < 5; i++)
	{
		cub [i] [2] = 0b00000;
		cub [i] [3] = 0b00000;
		cub [i] [4] = 0b00000;
	}
	
	for(uint8_t i = 0; i < 5; i++)
	{
		cub [i] [0] = 0b00001;
		cub [i] [1] = 0b00010;
		cub [i] [2] = 0b00100;
		cub [i] [3] = 0b01000;
		cub [i] [4] = 0b10000;
	}
	_delay_ms(100);
	for(uint8_t i = 0; i < 5; i++)
	{
		cub [i] [0] = 0b00000;
		cub [i] [1] = 0b00000;
		cub [i] [2] = 0b00000;
		cub [i] [3] = 0b00000;
		cub [i] [4] = 0b00000;
	}
	
	for(uint8_t i = 0; i < 5; i++)
	{
		cub [i] [0] = 0b00100;
		cub [i] [2] = 0b01000;
		cub [i] [4] = 0b10000;
	}
	_delay_ms(100);
	for(uint8_t i = 0; i < 5; i++)
	{
		cub [i] [0] = 0b00000;
		cub [i] [2] = 0b00000;
		cub [i] [4] = 0b00000;
	}
	
	for(uint8_t i = 0; i < 5; i++)
	{
		cub [i] [0] = 0b10000;
		cub [i] [1] = 0b10000;
		cub [i] [2] = 0b10000;
		cub [i] [3] = 0b10000;
		cub [i] [4] = 0b10000;
	}
	_delay_ms(100);
	for(uint8_t i = 0; i < 5; i++)
	{
		cub [i] [0] = 0b00000;
		cub [i] [1] = 0b00000;
		cub [i] [2] = 0b00000;
		cub [i] [3] = 0b00000;
		cub [i] [4] = 0b00000;
	}
	
	// 6
	
	for(uint8_t i = 0; i < 5; i++)
	{
		cub [0] [i] = 0b10000;
		cub [2] [i] = 0b01000;
		cub [4] [i] = 0b00100;
	}
	_delay_ms(100);
	for(uint8_t i = 0; i < 5; i++)
	{
		cub [0] [i] = 0b00000;
		cub [2] [i] = 0b00000;
		cub [4] [i] = 0b00000;
	}
	
	for(uint8_t i = 0; i < 5; i++)
	{
		cub [0] [i] = 0b10000;
		cub [1] [i] = 0b01000;
		cub [2] [i] = 0b00100;
		cub [3] [i] = 0b00010;
		cub [4] [i] = 0b00001;
	}
	_delay_ms(100);
	for(uint8_t i = 0; i < 5; i++)
	{
		cub [0] [i] = 0b00000;
		cub [1] [i] = 0b00000;
		cub [2] [i] = 0b00000;
		cub [3] [i] = 0b00000;
		cub [4] [i] = 0b00000;
	}
	
	for(uint8_t i = 0; i < 5; i++)
	{
		cub [0] [i] = 0b10000;
		cub [1] [i] = 0b00100;
		cub [2] [i] = 0b00001;
	}
	_delay_ms(100);
	for(uint8_t i = 0; i < 5; i++)
	{
		cub [0] [i] = 0b00000;
		cub [1] [i] = 0b00000;
		cub [2] [i] = 0b00000;
	}
	
	for(uint8_t i = 0; i < 5; i++)
	{
		cub [0] [i] = 0b11111;
	}
	_delay_ms(200);
	for(uint8_t i = 0; i < 5; i++)
	{
		cub [0] [i] = 0b00000;
	}
	
	_delay_ms(200);
}

void test (void) // включение всех светодиодов
{
	for(unsigned char i = 0; i < 5; i++)
	{
		for(unsigned char j = 0; j < 5; j++)
		{
			cub[i] [j] = 0b11111111;
		}
	}
	
	_delay_ms(500);
	
	for(unsigned char i = 0; i < 5; i++)
	{
		for(unsigned char j = 0; j < 5; j++)
		{
			cub[i] [j] = 0b00000000;
		}
	}
}

void rotate (void)
{
	for(unsigned char n = 0 ; n < 32; n++)
	{	
		for(unsigned char i = 0; i < 5; i++) // 1 storona
		{
			cub[4][i] = mas[i] >> 11;
		}	
		
		for(unsigned char i = 0; i < 5; i++) // 2 storona
		{
			if(mas[i] & (1 << 10)) cub[3][i] |= (1 << 0);
			if(mas[i] & (1 << 9)) cub[2][i] |= (1 << 0);
			if(mas[i] & (1 << 8)) cub[1][i] |= (1 << 0);
		}
	
		for(unsigned char i = 0; i < 5; i++) // 3 storona
		{		
			for(unsigned char j = 3; j < 8; j++)
			{
				if(mas[i]&(1 << j)) cub[0][i] |= (1 << (7 - j));
			}		
		}
	
		for(unsigned char i = 0; i < 5; i++) // 4 storona
		{
			if(mas[i] & (1 << 2)) cub[1][i] |= (1 << 4);
			if(mas[i] & (1 << 1)) cub[2][i] |= (1 << 4);
			if(mas[i] & (1 << 0)) cub[3][i] |= (1 << 4);
		}
	
		for(uint16_t k = 0; k < 5; k++)
		{	
			if(mas[k]&0b1000000000000000)	
			{				
				mas[k] = mas[k] << 1;
				mas[k] |= (1 << 0);
			}
			else
			{
				mas[k] = mas[k] << 1;
			}
		}
	
		_delay_ms(150);
	
		for(unsigned char i = 0; i < 5; i++)
		{
			cub[0] [i] = 0b00000000;
			cub[4] [i] = 0b00000000;
		}
		
		for(unsigned char i = 0; i < 5; i++)
		{
			for(unsigned char j = 0; j < 5; j++)
			{
				cub[i][j] &=~((1 << 0)|(1 << 4));
			}
		}
	
	}
}

ISR(TIMER0_OVF_vect)
{
	TCNT0 = 159;
	
	PORTD&=~((1 << PD0)|(1 << PD1)|(1 << PD2)|(1 << PD3)|(1 << PD4));
	
	switch (f)	
	{
		case 0: layer(cub, 0); PORTD |= (1 << PD4); break;
	    case 1: layer(cub, 1); PORTD |= (1 << PD3); break;
		case 2: layer(cub, 2); PORTD |= (1 << PD2); break;
		case 3: layer(cub, 3); PORTD |= (1 << PD1); break;
		case 4: layer(cub, 4); PORTD |= (1 << PD0); break;
	}

	if(++f == 5) f = 0;
}	
			
int main(void)
{
	DDRC |= (1 << SH_CP)|(1 << DS)|(1 << ST_CP)|(1 << MR);
	PORTC&=~((1 << SH_CP)|(1 << DS)|(1 << ST_CP));
	PORTC |= (1 << MR);
	
	DDRD |= (1 << PD0)|(1 << PD1)|(1 << PD2)|(1 << PD3)|(1 <<PD4);
	
	PORTC&=~(1 << MR);
	PORTC |= (1 << ST_CP);
	_delay_ms(1);
	PORTC&=~((1 << ST_CP));
	PORTC |= (1 << MR);
	
	asm("sei");
		
	TIMSK |= (1 << TOIE0); // TIMSK0 |= (1 << TOIE0);
	TCNT0 = 157;
	TCCR0 |= (1 << CS02); // TCCR0B |= (1 << CS02);
	
	test();
				
	while(1)
    {  				
		if((PINC&(1 << PC5)))
		{
			_delay_ms(50);
			if((PINC&(1 << PC5)))
			{
				rotate();
				cube_draw();
				
				for(char i = 0; i < 3; i++)
				romb(0);
				for(char i = 0; i < 3; i++)
				romb(2);
				for(char i = 0; i < 3; i++)
				romb(1);
				for(char i = 0; i < 3; i++)
				romb(3);
				
				_delay_ms(400);
				plane(4);
				zmeyka(1);
				stena();
				string(izolt);
				digits(zyfry);				
				
			}
		}	
    }
}