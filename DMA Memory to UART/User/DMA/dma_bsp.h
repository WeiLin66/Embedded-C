#ifndef __DNA_BSP_H_
#define __DNA_BSP_H_

#include "stm32f4xx_dma.h"
#include "stm32f4xx_rcc.h"
#include "bsp_led.h"
#include "stdint.h"
#include "string.h"

#define DMA_STREAM     DMA2_Stream7 // change to other stream
#define DMA_CHANNEL    DMA_Channel_4 // change to other channel
#define TEMP_SIZE      256
#define TIMEOUT				 0x00000400
#define DMA_CLEAR_FLAG DMA_FLAG_TCIF0

extern void dma_cfg(void);

#endif /*__DNA_BSP_H_*/
