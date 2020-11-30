#include "N76E003.h"
#include "Common.h"
#include "Delay.h"
#include "SFR_Macro.h"
#include "Function_define.h"

void PWM_set_period(unsigned int period_value)
{
	PWMPL = period_value & 0x00FF;
	PWMPH = (period_value & 0xFF00)>>8;
}

void PWM_set_on(unsigned int on_value)
{
	PWM2L = (on_value & 0x00FF);
  PWM2H = ((on_value & 0xFF00) >> 8);
  set_LOAD;
}

int i;
void main(void)
{
    P05_Quasi_Mode;
	  P30_Input_Mode;
    PWM2_P05_OUTPUT_ENABLE;
		PWM_IMDEPENDENT_MODE;
		PWM_CLOCK_DIV_8;
	  PWM_set_period(2000);
/**********************************************************************
	PWM frequency = Fpwm/((PWMPH,PWMPL) + 1) <Fpwm = Fsys/PWM_CLOCK_DIV> 
								= (16MHz/8)/(0x7CF + 1)
								= 1KHz (1ms)
***********************************************************************/	
	  PWM2H = 0x00;
		PWM2L = 0x00;
    set_LOAD;
    set_PWMRUN;
		while(1){
			for(i = 0; i <= 2000; i += 10)
        {
            PWM_set_on(i);
            Timer0_Delay1ms(100);
		}
		}
	}