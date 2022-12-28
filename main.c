// File Name: main.c
// Author: Tanaka Hao
// Description: To show random pattern of 8 LEDs. 2 modes supported: manual & Auto
// Date: 22.12.28

#include <REGX52.H>
#include <stdlib.h>
#include "types.h"
#include "key.h"

#define MAX 255
#define MIN 1

typedef enum{
    KEYM,
    AUTO
}ENUM_STATE;     
ENUM_STATE system_state = KEYM;    
bit key_auto_flag = 0;   //0 -- key mode; 1 -- Auto mode

void configTimer0();
void systemStateCtrl();
void systemStateWork();
void KeyModeWork();
void AutoModeWork();
uint random(uint seed);

static bit pressed = 0;
static uchar flag1ms, flag10ms, flag100ms;
static uchar LED=0xFF;
static uint seed=0;

void main()
{
	configTimer0();
	
	while (1)
	{
		if (seed < 65535)
		{
			seed+=1;
		}
		else
		{
			seed=0;
		}
		systemStateWork();
		if (++flag100ms>=100)
		{
			flag100ms=0;
			P2=LED;
		}
	}
}

void systemStateCtrl()
{
	switch (system_state)
	{
		case KEYM:
			if (key_auto_flag) system_state=AUTO;
			break;
		case AUTO:
			if (!key_auto_flag) system_state=KEYM;
			break;
	}
}

void systemStateWork()
{
	if (scanButton2())
	{
		key_auto_flag = ~key_auto_flag;
		systemStateCtrl();
	}
	switch( system_state ){
		case KEYM:
			KeyModeWork();   
			break;
		case AUTO:
			AutoModeWork();    
			break;
  }
}

void KeyModeWork()
{
		if (pressed)
		{
			LED=random(seed);
			pressed=0;
		}
		if (flag1ms) flag1ms=0;
		if (++flag10ms>=10)
		{
			flag10ms=0;
			pressed=scanButton1();
		}
}

void AutoModeWork()
{
	static uchar count = 0;
		if (flag1ms) flag1ms=0;
		if (++flag10ms>=10)
		{
			flag10ms=0;
			if (++count==150)
			{
				LED=random(seed);
				count=0;
			}
		}
}

uint random(uint seed)
{
	 uint value;
	 srand(seed);
	 value = rand() % (MAX + 1 - MIN)+ MIN;               //get a random num (MIN-MAX)
	 return value;
}

void configTimer0() //delay 1ms
{
    TMOD=0x01; //0000 0001
    TH0 = (65536 - 1000) / 256; 
    TL0 = (65536 - 1000) % 256;
    TR0 = 1;
    ET0 = 1;
    EA = 1;
}

void timer0() interrupt 1
{
	flag1ms=1;
	TH0 = (65536 - 1000) / 256; 
  TL0 = (65536 - 1000) % 256;
}
