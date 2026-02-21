#include "vfs_wrapper.h"
//#include "py/stream.h"
//#include "extmod/vfs.h"
//#include "py/runtime.h"
#include "esp_vfs_fat.h"

 #include "vfs_fat_internal.h"
#include "ff.h"
/************ File OP ************/
/*
 * This file is part of the OpenMV project.
 * Copyright (c) 2013-2016 Kwabena W. Agyeman <kwagyeman@openmv.io>
 * This work is licensed under the MIT license, see the file LICENSE for details.
 *
 * File System Helper Functions
 *
 */





#include <stdio.h>
#include <unistd.h>
#include <string.h>
// #include "py/runtime.h"
// #include "extmod/vfs.h"
// #include "extmod/vfs_fat.h"

#include "common.h"
#include "fb_alloc.h"

#define FF_MIN(x,y) (((x)<(y))?(x):(y))



#include <stdint.h>
#include <stddef.h>

#ifndef ERR_PRINT
#define LOG_PRINT(...)
#endif

#ifndef LOG_PRINT
#define LOG_PRINT(...)
#endif

/*----------------------------------------------------------*/
/* ERROR HELPERS                                            */
/*----------------------------------------------------------*/

int ff_unsupported_format(FIL *fp)
{
    if (fp) f_close(fp);
    LOG_PRINT("ff_unsupported_format\n");   
    
    return 0;
}

int ff_file_corrupted(FIL *fp)
{
    if (fp) f_close(fp);
    LOG_PRINT("ff_file_corrupted!\n");
    return 0;
}

int ff_not_equal(FIL *fp)
{
    if (fp) f_close(fp);
    LOG_PRINT("ff_not_equal!\n");
    return 0;
}

int ff_no_intersection(FIL *fp)
{
    if (fp) f_close(fp);
    LOG_PRINT("ff_no_intersection!\n");
    return 0;
}

bool file_eof(FIL *fp)
{
    if (!fp) return true;
    return f_eof(fp);
}


/*----------------------------------------------------------*/
/* FILE OPEN / CLOSE                                        */
/*----------------------------------------------------------*/

int file_read_open(FIL *fp, const char *path)
{
    FRESULT res = f_open(fp, path, FA_READ|FA_OPEN_EXISTING);
    if (res != FR_OK)
        
    {
        
        LOG_PRINT("%s, not exist!", path);
    }
    return 0;
}

int file_write_open(FIL *fp, const char *path)
{
    f_open(fp, path, FA_WRITE | FA_CREATE_ALWAYS);
    return 0;
}

int file_close(FIL *fp)
{
    f_close(fp);
    return 0;
}

/*----------------------------------------------------------*/
/* FILE POSITION                                            */
/*----------------------------------------------------------*/
int file_seek(FIL *fp, FSIZE_t offset, uint8_t whence)
{
    if (!fp) return -1;
    
    FSIZE_t new_pos;
    
    switch (whence) {
        case SEEK_SET:
            new_pos = offset;
            break;
            
        case SEEK_CUR:
            new_pos = f_tell(fp) + offset;
            break;
            
        case SEEK_END:
            new_pos = f_size(fp) + offset;
            break;
            
        default:
            return -1;
    }
    
    FRESULT res = f_lseek(fp, new_pos);
    return (res == FR_OK) ? 0 : -1;
}

int file_seek_raise(FIL *fp, FSIZE_t offset, uint8_t whence)
{
    return file_seek(fp, offset, whence);
}


int file_truncate(FIL *fp)
{
    return (f_truncate(fp) == FR_OK) ? 0 : -1;
}

int file_sync(FIL *fp)
{
    return (f_sync(fp) == FR_OK) ? 0 : -1;
}

long file_fsize(FIL *fp)
{
    return (long)f_size(fp);
}

/*----------------------------------------------------------*/
/* BUFFER SUPPORT (unchanged logic placeholders)             */
/*----------------------------------------------------------*/

static uint32_t file_buffer_offset = 0;
static uint8_t *file_buffer_pointer = 0;
static uint32_t file_buffer_size = 0;
static uint32_t file_buffer_index = 0;

int file_buffer_init0(void)
{
    file_buffer_offset = 0;
    file_buffer_pointer = 0;
    file_buffer_size = 0;
    file_buffer_index = 0;
    return 0;
}

void file_buffer_on(FIL *fp)
{
    file_buffer_offset = (uint32_t)(f_tell(fp) & 3);
    /* fb_alloc_all() remains user-provided */
 //   return 0;
}

uint32_t file_tell_w_buf(FIL *fp)
{
    return (uint32_t)f_tell(fp);
}

uint32_t file_size_w_buf(FIL *fp)
{
    return (uint32_t)f_size(fp);
}

void file_buffer_off(FIL *fp)
{
    if (!fp) return;
    f_sync(fp);   // flush cache to SD
}

/*----------------------------------------------------------*/
/* READ FUNCTIONS                                           */
/*----------------------------------------------------------*/

int read_byte(FIL *fp, uint8_t *value)
{
    UINT br;
    f_read(fp, value, 1, &br);
    if (br != 1)
    {
        LOG_PRINT("file error!\n");
    }
    return br;
}

int read_byte_expect(FIL *fp, uint8_t value)
{
    UINT br;
    uint8_t str_s;
    f_read(fp, &str_s, 1, &br);
    if (br != 1 || str_s != value)
    {
        LOG_PRINT("file error!\n");
        return -1;
    }
    return br;
}

int read_byte_ignore(FIL *fp)
{
    UINT br;
    uint8_t dummy;
    f_read(fp, &dummy, 1, &br);
    if (br != 1)
    {
        LOG_PRINT("file error!\n");
    }
    return br;
}

int read_word(FIL *fp, uint16_t *value)
{
    UINT br;
    f_read(fp, value, 2, &br);
    if (br != 2)
    {
        LOG_PRINT("file error!\n");
    }
    return (br == 2);
}

int read_word_expect(FIL *fp, uint16_t value)
{
    UINT br;
    uint16_t str_s;
    f_read(fp, &str_s, 2, &br);
    if (br != 2 || str_s != value)
    {
        LOG_PRINT("file error!\n");
        return -1;
    }
    return 1;
}

int read_word_ignore(FIL *fp)
{
    UINT br;
    uint16_t dummy;
    f_read(fp, &dummy, 2, &br);
    if (br != 2)
    {
        LOG_PRINT("file error!\n");
        return -1;
    }
    return 1;
}

int read_long(FIL *fp, uint32_t *value)
{
    UINT br;
    f_read(fp, value, 4, &br);
    if (br != 4)
    {
        LOG_PRINT("file error!\n");
        return -1;
    }
    return 1;
}

int read_long_expect(FIL *fp, uint32_t value)
{
    UINT br;
    uint32_t str_s;
    f_read(fp, &str_s, 4, &br);
    if (br != 4 || str_s != value)
    {
        LOG_PRINT("file error!\n");
        return -1;
    }
    return 1;
}

int read_long_ignore(FIL *fp)
{
    UINT br;
    uint32_t dummy;
    f_read(fp, &dummy, 4, &br);
    return (br == 4);
}

int read_data(FIL *fp, void *data, size_t size)
{
    UINT br;
    f_read(fp, data, (UINT)size, &br);
    return (int)br;
}

/*----------------------------------------------------------*/
/* WRITE FUNCTIONS                                          */
/*----------------------------------------------------------*/

int write_byte(FIL *fp, uint8_t value)
{
    UINT bw;
    f_write(fp, &value, 1, &bw);
    return bw;
}

int write_word(FIL *fp, uint16_t value)
{
    UINT bw;
    f_write(fp, &value, 2, &bw);
    return bw;
}

int write_long(FIL *fp, uint32_t value)
{
    UINT bw;
    f_write(fp, &value, 4, &bw);
    return bw;
}

int write_data(FIL *fp, const void *data, size_t size)
{
    UINT bw;
    f_write(fp, data, (UINT)size, &bw);
    return bw;
}



