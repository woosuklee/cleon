/**
@file       fs_init.h

@brief      Initializing file system
*/

#ifndef __FS_INIT_H__
#define __FS_INIT_H__

#define FS_Mount                f_mount
#define FS_Open                 f_open
#define FS_Close                f_close
#define FS_Read                 f_read
#define FS_Write                f_write
#define FS_Lseek                f_lseek
#define FS_Truncate             f_truncate
#define FS_Sync                 f_sync
#define FS_OpenDir              f_opendir
#define FS_ReadDir              f_readdir
#define FS_GetFree              f_getfree
#define FS_Stat                 f_stat
#define FS_MakeDir              f_mkdir
#define FS_UnLink               f_unlink
#define FS_Chmod                f_chmod
#define FS_UTime                f_utime
#define FS_Rename               f_rename
#define FS_ChangeDir            f_chdir
#define FS_ChangeDrive          f_chdrive
#define FS_GetCWD               f_getcwd
#define FS_Forward              f_forward
#define FS_MakeFileSystem       f_mkfs
#define FS_DivideDrive          f_fdisk
#define FS_ReadString           f_gets
#define FS_WriteCharacter       f_putc
#define FS_WriteString          f_puts
#define FS_WriteFormattedString f_printf
#define FS_GetCurrentPointer    f_tell
#define FS_TestEOF              f_eof
#define FS_GetSizeOfFile        f_size
#define FS_Error                f_error

void FS_Init(void);

#endif
