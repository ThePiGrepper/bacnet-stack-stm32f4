#include <stdint.h>
#include "hardware.h"
#include "input.h"

void input_init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_StructInit(&GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

uint8_t get_input_value(uint8_t index)
{
  uint8_t ret = 0;

  switch (index) {
    case 0:
      ret = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15);
      break;
    case 1:
      ret = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3);
      break;
    case 2:
      ret = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4);
      break;
    case 3:
      ret = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8);
      break;
    case 4:
      ret = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9);
      break;
    default:
      break;
  }
  return ret;
}
