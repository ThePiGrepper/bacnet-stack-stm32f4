#include <stdint.h>
#include "hardware.h"
#include "output.h"

static uint16_t TimerPeriod = 0;
static uint16_t ChannelPulse[MAX_ANALOG_OUTPUTS];
static TIM_OCInitTypeDef TIM_OCInitStructure;

void output_init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

  /* TIM4, GPIOB and AFIO clocks enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOE|
//    RCC_APB2Periph_GPIOB |RCC_APB2Periph_AFIO, ENABLE);

  /* GPIOA Configuration: Channel 1, 2 and 3 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* TIM4 Configuration ---------------------------------------------------
   Generate 7 PWM signals with 4 different duty cycles:
   TIM4 = SystemCoreClock, Prescaler = 0, TIM4 counter clock = SystemCoreClock
   SystemCoreClock is set to 72 MHz for Low-density, Medium-density, High-density
   and Connectivity line devices and to 24 MHz for Low-Density Value line and
   Medium-Density Value line devices

   The objective is to generate 7 PWM signal at 17.57 KHz:
     - TIM4_Period = (SystemCoreClock / 17570) - 1
   The channel 1 and channel 1N duty cycle is set to 50%
   The channel 2 and channel 2N duty cycle is set to 37.5%
   The channel 3 and channel 3N duty cycle is set to 25%
   The channel 4 duty cycle is set to 12.5%
   The Timer pulse is calculated as follows:
     - ChannelxPulse = DutyCycle * (TIM1_Period - 1) / 100
  ----------------------------------------------------------------------- */
  /* Compute the value to be set in ARR regiter to generate signal frequency at 17.57 Khz */
  TimerPeriod = (SystemCoreClock / 17570 ) - 1;

  /* Time Base configuration */
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = TimerPeriod;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

//  int i;
//  for (i = 0; i < MAX_ANALOG_OUTPUTS; i++) {
//    ChannelPulse[i] = (uint16_t) (((uint32_t) 50 * (TimerPeriod - 1)) / 100);
//  }
//  /* Compute CCR1 value to generate a duty cycle at 50% for channel 1 and 1N */
//  Channel1Pulse = (uint16_t) (((uint32_t) 5 * (TimerPeriod - 1)) / 10);
//  /* Compute CCR2 value to generate a duty cycle at 37.5%  for channel 2 and 2N */
//  Channel2Pulse = (uint16_t) (((uint32_t) 375 * (TimerPeriod - 1)) / 1000);
//  /* Compute CCR3 value to generate a duty cycle at 25%  for channel 3 and 3N */
//  Channel3Pulse = (uint16_t) (((uint32_t) 25 * (TimerPeriod - 1)) / 100);
//  /* Compute CCR4 value to generate a duty cycle at 12.5%  for channel 4 */
//  Channel4Pulse = (uint16_t) (((uint32_t) 125 * (TimerPeriod- 1)) / 1000);

  /* Channel 1-3 Configuration in PWM mode */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  //TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  //TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  //TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

//  TIM_OCInitStructure.TIM_Pulse = ChannelPulse[0];
//  TIM_OC1Init(TIM4, &TIM_OCInitStructure);
//
//  TIM_OCInitStructure.TIM_Pulse = ChannelPulse[1];
//  TIM_OC2Init(TIM4, &TIM_OCInitStructure);
//
//  TIM_OCInitStructure.TIM_Pulse = ChannelPulse[2];
//  TIM_OC3Init(TIM4, &TIM_OCInitStructure);

//  TIM_OCInitStructure.TIM_Pulse = ChannelPulse[3];
//  TIM_OC4Init(TIM4, &TIM_OCInitStructure);

  /* TIM4 counter enable */
  TIM_Cmd(TIM4, ENABLE);

  /* TIM4 Main Output Enable */
  TIM_CtrlPWMOutputs(TIM4, ENABLE);
}

/* id: uint8_t < MAX_ANALOG_OUTPUTS
 * value: uint8_t between 0 - 100 */
void set_output_pwm(uint8_t id, uint8_t value)
{
  uint16_t tmp;

  tmp = (uint16_t) (((uint32_t) value * (TimerPeriod - 1)) / 100);
  if (tmp != ChannelPulse[id]) {
    ChannelPulse[id] = tmp;
    TIM_OCInitStructure.TIM_Pulse = tmp;
    switch(id)
    {
      case 0:
      TIM_OC1Init(TIM4, &TIM_OCInitStructure);
      case 1:
      TIM_OC2Init(TIM4, &TIM_OCInitStructure);
      case 2:
      TIM_OC3Init(TIM4, &TIM_OCInitStructure);
    }
  }
}
