// File Name: key.c
// Author: Tanaka Hao
// Description: functions about keys.
// Date: 22.12.28

#include "key.h"
#include "types.h"

sbit BUT1 = P3^1;
sbit BUT2 = P3^0;

bit scanButton1()
{
	static uchar press_time=0;
	if (BUT1==0)
	{
		if (++press_time<=0) --press_time;
		if (press_time==2)
		{
			return 1;
		}
	}
	else
	{
		press_time=0;
	}
	return 0;
}

bit scanButton2()
{
	static uchar press_time=0;
	if (BUT2==0)
	{
		if (++press_time<=0) --press_time;
		if (press_time==2)
		{
			return 1;
		}
	}
	else
	{
		press_time=0;
	}
	return 0;
}