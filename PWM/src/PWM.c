/*
 * Copyright 2022 NXP
 * NXP confidential.
 * This software is owned or controlled by NXP and may only be used strictly
 * in accordance with the applicable license terms.  By expressly accepting
 * such terms or by downloading, installing, activating and/or otherwise using
 * the software, you are agreeing that you have read, and that you agree to
 * comply with and are bound by, such license terms.  If you do not agree to
 * be bound by the applicable license terms, then you may not retain, install,
 * activate or otherwise use the software.
 */

#ifdef __USE_CMSIS
#include "LPC13xx.h"
#endif

#include <cr_section_macros.h>

#include <stdio.h>
#define PWM_PERIOD 500  // PWM periyodu
#define PWM_DUTY_CYCLE 250  // PWM görev orani (darbe genisligi)
void SystemCLKInit(void);
void PWM_Init(void);
void Timer16_1_init (void);

 int main(void)
{	 SystemCoreClockUpdate();
     //SystemCLKInit();
     PWM_Init();
     Timer16_1_init();
     while (1){

}
return 0;
}
void SystemCLKInit(void)
{
  /* Harici osilatörü kullanmak için güç tüketimini azaltan dahili osilatörü devre disi birakalim */
  LPC_SYSCON->PDRUNCFG &= ~(1 << 5);
  /* Harici osilatörün frekansini ayarlayalim (12 MHz) */
  LPC_SYSCON->SYSOSCCTRL = 0x00; // HF (High-frequency) modunu devre disi birak
  for (volatile uint32_t i = 0; i < 200; i++) __NOP();

  /* Harici osilatörü kullanarak sistem saatini yapilandiralim */
  LPC_SYSCON->SYSPLLCLKSEL = 0x01;     // SYSOSCCLK'i PLL'ye kaynagi olarak seç
  LPC_SYSCON->SYSPLLCLKUEN = 0x01;     // SYSPLLCLK kaynagini güncelle
  LPC_SYSCON->SYSPLLCLKUEN = 0x00;     // Güncellemeyi durdur
  LPC_SYSCON->SYSPLLCLKUEN = 0x01;     // Tekrar güncelle
  while (!(LPC_SYSCON->SYSPLLCLKUEN & 0x01)); // Bekle, güncelleme tamamlanana kadar

  /* PLL bölücü oranlarini ayarlayarak 72 MHz sistem saatini elde edelim */
  //LPC_SYSCON->SYSPLLCTRL = 0x25;  // M bölücü = 6, P bölücü = 2
  LPC_SYSCON->SYSPLLCTRL = 0x23;  // M bölücü = 4, P bölücü = 2
  LPC_SYSCON->PDRUNCFG     &= ~(1 << 7);          /* Power-up SYSPLL          */
  while (!(LPC_SYSCON->SYSPLLSTAT & 0x01));	      /* Wait Until PLL Locked    */


  LPC_SYSCON->MAINCLKSEL = 0x03;       // SYSPLLCLK'i ana saat kaynagi olarak seç
  LPC_SYSCON->MAINCLKUEN = 0x01;       // MAINCLK kaynagini güncelle
  LPC_SYSCON->MAINCLKUEN = 0x00;       // Güncellemeyi durdur
  LPC_SYSCON->MAINCLKUEN = 0x01;       // Tekrar güncelle
  while (!(LPC_SYSCON->MAINCLKUEN & 0x01));   // Bekle, güncelleme tamamlanana kadar

  LPC_SYSCON->SYSAHBCLKDIV = 0x01;    // AHBCLKDIV'i 1 olarak ayarla
  LPC_SYSCON->SYSTICKCLKDIV  = LPC_SYSCON->SYSAHBCLKDIV;  /* use same divider for Systick and main clock */
  return;
}

void PWM_Init(void)
{
    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 7);  // CT16B0 için saat sinyalini etkinlestir
    LPC_TMR16B0->TCR = 0x002;
    LPC_TMR16B0->MCR = 0x400;	//timer will be reset by mr3
    LPC_TMR16B0->MR0 = PWM_DUTY_CYCLE;  // PWM görev oranini ayarla
    LPC_TMR16B0->MR3 = PWM_PERIOD;	//pwm period
    LPC_TMR16B0->PR = 48;	//prescaler(71 dec)
    LPC_TMR16B0->CTCR = 0x000;	//not required
    LPC_TMR16B0->PWMC = 0x009;	//mr0 and mr3 engaged
    LPC_IOCON->PIO0_8 = 0x002;	//pin pio1-9 for mat0
    LPC_TMR16B0->EMR = 0x000;		//not required
    LPC_TMR16B0->IR = 0x000;		//no interrrupts
    LPC_TMR16B0->TCR = 0x001;  // Timer'i baslat
    return;
}
void Timer16_1_init (void){
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<8);	//enable timer16b1
	LPC_TMR16B1->TCR = 0x002;	//reset timer
	LPC_TMR16B1->MCR = 0x400;	//timer will be reset by mr3
	LPC_TMR16B1->MR0 = 125;	//pwm duty cycle < pwm period
	LPC_TMR16B1->MR3 = PWM_PERIOD;	//pwm period
	LPC_TMR16B1->PR = 48;	//prescaler
	LPC_TMR16B1->CTCR = 0x000;	//not required
	LPC_TMR16B1->PWMC = 0x009;	//mr0 and mr3 engaged
	LPC_IOCON->PIO1_9 = 0x001;	//pin pio1-9 for mat0
	LPC_TMR16B1->EMR = 0x000;		//not required
	LPC_TMR16B1->IR = 0x000;		//no interrrupts
	LPC_TMR16B1->TCR = 0x001;		//start timer
	}
