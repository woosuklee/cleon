/**
@file       sys_mmc.h

@brief      Micro SD card related functions
*/

#ifndef __SYS_MMC_H__
#define __SYS_MMC_H__

#define SYS_MMC_LED_INDICATION_FOR_ERROR    _ENABLE_

#define SYS_MMC_IsMicroSDInserted()   !((MICROSD_STATUS_PORT(IN) & BV(MICROSD_STATUS_PIN)) >> MICROSD_STATUS_PIN)

void SYS_MMC_Init(void);
void SYS_MMC_InitIO(void);
void SYS_MMC_FastMode(void);
void SYS_MMC_ReadFrame(unsigned char* ptrucBuffer, unsigned int uiSize);
void SYS_MMC_SendFrame(unsigned char* ptrucBuffer, unsigned int uiSize);
void SYS_MMC_DeselectCard(void);
void SYS_MMC_SelectCard(void);

#endif
