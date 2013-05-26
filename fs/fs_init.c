/**
@file       fs_init.c

@brief      Initializing file system
*/

#include "cleon_conf.h"
#include "app_define.h"
#include "sys_define.h"
#include "hal_define.h"
#include "fs_define.h"

// File system variables
FATFS Fatfs;		 
FIL   fileObject;	 
UINT  bw;

/*----------------------------------------------------------------------------*/
/**
@brief    Initializing file system

@return   void 

@param    void
*/
void FS_Init(void)
{ 
    disk_initialize(0);
}
