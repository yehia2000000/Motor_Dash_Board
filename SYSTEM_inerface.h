/****************************************************************/
/****************************************************************/
/***********************   YEHIA SHAHIN   ***********************/
/***********************   Layer :APP    ***********************/
/***********************   SWC (DRIVER):PING *********************/
/***********************   DATA : 24-8-2020 *********************/
/***********************   VERSION : 1.00   *********************/
/****************************************************************/
/****************************************************************/
#ifndef SYSTEM_INTERFACE_H_
#define SYSTEM_INTERFACE_H_

void SYSTEM_voidPressTheButton (void);

u8 SYSTEM_voidLogin (void);

void SYSTEM_voidDCMotor (void);
void SYSTEM_voidStepper (void);
void SYSTEM_voidServo (void);

void ISR_CCPOT (void);
void ISR_CCLM (void);
void ISR_CCWPOT (void);
void ISR_CCWLM (void);

#endif
