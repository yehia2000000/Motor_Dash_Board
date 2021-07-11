
#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "MAPP_interface.h"

#include "DIO_interface.h"
#include "ADC_interface.h"
#include "TIMER_interface.h"

#include "KPD_interface.h"
#include "CLCD_interface.h"
#include "MOTOR_interface.h"
#include "STEPPER_interface.h"
#include "SERVO_interface.h"

#include "SYSTEM_inerface.h"

#include <util/delay.h>

u8 SYSTEM_u8PinState ;

u16 SYSTEM_u16Result ;

u16 SYSTEM_u16OvfVal;
u16 SYSTEM_u16CompVal;
void SYSTEM_voidPressTheButton (void)
{
	SYSTEM_u8PinState = KPD_u8PressKey();
	while (SYSTEM_u8PinState == 0xff)
	{
		SYSTEM_u8PinState = KPD_u8PressKey();

	}

}

u8 SYSTEM_voidLogin (void)
{
	u32 Local_u32IdNumber =0 ;
	u32 Local_u32Pass =0 ;
	u32 Local_u32Check =0 ;
	u8 Local_u8Counter =0 ;

	CLCD_voidSendCommand(1);
	CLCD_voidFullLcd("Enter the ID :",0,0);
	CLCD_voidGotoXY(0x40,1);
	SYSTEM_voidPressTheButton();

	while ((SYSTEM_u8PinState != 'C')&&(SYSTEM_u8PinState != 'E')&&(SYSTEM_u8PinState != 'D'))
	{
		CLCD_voidSendData('0'+SYSTEM_u8PinState);
		Local_u32IdNumber = (10*Local_u32IdNumber) + SYSTEM_u8PinState ;
		SYSTEM_voidPressTheButton();

	}
	if (SYSTEM_u8PinState == 'C')
	{
		CLCD_voidSendCommand(1);
		CLCD_voidFullLcd("BYE BYE ",0,0);

	}
	else if (SYSTEM_u8PinState =='E')
	{
		while (Local_u8Counter<3)
		{
			CLCD_voidSendCommand(1);
			CLCD_voidFullLcd("Enter the Pass :",0,0);
			CLCD_voidGotoXY(0x40,1);

			SYSTEM_voidPressTheButton();
			while ((SYSTEM_u8PinState != 'C')&&(SYSTEM_u8PinState != 'E')&&(SYSTEM_u8PinState != 'D'))
			{
				CLCD_voidSendData('0'+SYSTEM_u8PinState);
				_delay_ms(200);
				CLCD_voidSendCommand(16);
				CLCD_voidSendData('*');
				Local_u32Pass =(Local_u32Pass * 10)+SYSTEM_u8PinState ;
				SYSTEM_voidPressTheButton();

			}

			if (SYSTEM_u8PinState == 'C')
			{
				CLCD_voidSendCommand(1);
				CLCD_voidFullLcd("BYE BYE ",0,0);

			}
			else if (SYSTEM_u8PinState == 'E')
			{
				Local_u32Check =0 ;
				while (Local_u32Pass !=0)
				{
					Local_u32Check =(Local_u32Check*10)+(Local_u32Pass%10);
					Local_u32Pass /=10 ;
				}
				if (Local_u32Check == Local_u32IdNumber)
				{
					Local_u8Counter++;
					break;
				}

			}
			Local_u8Counter++;

		}
	}
	if (Local_u8Counter ==3)
	{
		CLCD_voidSendCommand(1);
		CLCD_voidFullLcd("BYE BYE",0,0);
		_delay_ms(3000);

	}
	return Local_u8Counter ;
}

void SYSTEM_voidDCMotor (void)
{
	CLCD_voidSendCommand(1);
	CLCD_voidFullLcd("Rot DIR :",0,0);
	CLCD_voidFullLcd("1-CW  2-CCW",0x40,1);

	SYSTEM_voidPressTheButton();
	while ((SYSTEM_u8PinState !=1 )&&(SYSTEM_u8PinState != 1))
	{
		SYSTEM_voidPressTheButton();
	}
	if (SYSTEM_u8PinState ==1)
	{
		CLCD_voidSendCommand(1);
		CLCD_voidFullLcd("Speed Control",0,0);
		_delay_ms(3000);
		CLCD_voidSendCommand(1);
		CLCD_voidFullLcd("1-Full 2-POT 3-Lm",0,0);
		SYSTEM_voidPressTheButton();

		while ((SYSTEM_u8PinState !=1 )&&(SYSTEM_u8PinState != 2)&&(SYSTEM_u8PinState != 3))
		{
			SYSTEM_voidPressTheButton();
		}
		if (SYSTEM_u8PinState ==1) /*Full Speed */
		{
			while (SYSTEM_u8PinState != 'C')
			{
				MOTOR_void_C_W(255);
				SYSTEM_voidPressTheButton();

			}
		}
		else if (SYSTEM_u8PinState == 2) /*POT*/
		{
			while (1)
			{
				ADC_u8StartSingleConversionAsyn(0,&SYSTEM_u16Result,&ISR_CCPOT);


			}

		}
		else if (SYSTEM_u8PinState == 3) /*LM35*/
		{
			while (1)
			{
				ADC_u8StartSingleConversionAsyn(1,&SYSTEM_u16Result,&ISR_CCLM);

			}

		}


	}
	else if (SYSTEM_u8PinState ==2)
	{
		CLCD_voidSendCommand(1);
		CLCD_voidFullLcd("Speed Control",0,0);
		_delay_ms(3000);
		CLCD_voidSendCommand(1);
		CLCD_voidFullLcd("1-Full 2-POT 3-Lm",0,0);
		SYSTEM_voidPressTheButton();

		while ((SYSTEM_u8PinState !=1 )&&(SYSTEM_u8PinState != 2)&&(SYSTEM_u8PinState != 3))
		{
			SYSTEM_voidPressTheButton();
		}
		if (SYSTEM_u8PinState ==1) /*Full Speed */
		{
			while (SYSTEM_u8PinState != 'C')
			{
				MOTOR_void_C_C_W(255);
				SYSTEM_voidPressTheButton();

			}
		}
		else if (SYSTEM_u8PinState == 2) /*POT*/
		{
			while (SYSTEM_u8PinState != 'C')
			{
				ADC_u8StartSingleConversionAsyn(0,&SYSTEM_u16Result,&ISR_CCWPOT);
				SYSTEM_voidPressTheButton();

			}

		}
		else if (SYSTEM_u8PinState == 3) /*LM35*/
		{
			while (SYSTEM_u8PinState != 'C')
			{
				ADC_u8StartSingleConversionAsyn(1,&SYSTEM_u16Result,&ISR_CCWLM);
				SYSTEM_voidPressTheButton();

			}

		}


	}

}

void SYSTEM_voidStepper (void)
{
	u16 Local_u16AngleValue =0;

	CLCD_voidSendCommand(1);
	CLCD_voidFullLcd("Rot DIR :",0,0);
	CLCD_voidFullLcd("1-CW  2-CCW",0x40,1);

	SYSTEM_voidPressTheButton();

	while ((SYSTEM_u8PinState !=1 )&&(SYSTEM_u8PinState != 2))
	{
		SYSTEM_voidPressTheButton();
	}

	if (SYSTEM_u8PinState == 1)
	{
		CLCD_voidSendCommand(1);
		CLCD_voidFullLcd("Enter angle :",0,0);
		CLCD_voidGotoXY(0x40,1);

		SYSTEM_voidPressTheButton();

		while (SYSTEM_u8PinState != 'E')
		{

			CLCD_voidSendData('0'+SYSTEM_u8PinState);
			Local_u16AngleValue =(Local_u16AngleValue*10)+(u16)SYSTEM_u8PinState;
			SYSTEM_voidPressTheButton();
		}
		STEEPER_voidCCWAngle(Local_u16AngleValue);

	}
	else if (SYSTEM_u8PinState == 2)
	{
		CLCD_voidSendCommand(1);
		CLCD_voidFullLcd("Enter angle :",0,0);
		CLCD_voidGotoXY(0x40,1);

		SYSTEM_voidPressTheButton();

		while (SYSTEM_u8PinState != 'E')
		{
			CLCD_voidSendData('0'+SYSTEM_u8PinState);
			Local_u16AngleValue =(Local_u16AngleValue*10)+(u16)SYSTEM_u8PinState;
			SYSTEM_voidPressTheButton();
		}
		STEEPER_voidCWAngle(Local_u16AngleValue);

	}
}
void SYSTEM_voidServo (void)
{
	u16 Local_u16AngleValue =0 ;
	CLCD_voidSendCommand(1);
	CLCD_voidFullLcd("Enter angle :",0,0);
	CLCD_voidGotoXY(0x40,1);
	SYSTEM_voidPressTheButton();

	while (SYSTEM_u8PinState != 'E')
	{
		CLCD_voidSendData('0'+SYSTEM_u8PinState);
		Local_u16AngleValue =(Local_u16AngleValue*10)+SYSTEM_u8PinState;
		SYSTEM_voidPressTheButton();
	}
	SYSTEM_u16CompVal=(u16)SERVO_u8Direction((s32)Local_u16AngleValue,&SYSTEM_u16OvfVal,180,0);


	Timer1_SetOnTop(SYSTEM_u16OvfVal);
	Timer_u8SetCompValue(SYSTEM_u16CompVal,TIMER1,CHANNELA);
	Timer_Enable(TIMER1);


}
void ISR_CCPOT (void)
{

	MOTOR_void_C_W(SYSTEM_u16Result);


}

void ISR_CCLM (void)
{
	CLCD_voidSendCommand(1);
	CLCD_voidSendNumber(SYSTEM_u16Result);
	u16 Local_u16Value  ;
	Local_u16Value =(u16)MAPP_S32Rang(14,40,0,255,(s32)SYSTEM_u16Result);
	MOTOR_void_C_W(Local_u16Value);

}
void ISR_CCWPOT (void)
{
	MOTOR_void_C_C_W(SYSTEM_u16Result);

}

void ISR_CCWLM (void)
{
	CLCD_voidSendCommand(1);
	CLCD_voidSendNumber(SYSTEM_u16Result);
	u16 Local_u16Value  ;
	Local_u16Value =(u16)MAPP_S32Rang(12,40,0,255,(s32)SYSTEM_u16Result);
	MOTOR_void_C_C_W(Local_u16Value);

}
