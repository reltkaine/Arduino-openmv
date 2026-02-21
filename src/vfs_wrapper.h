

#include "stdint.h"
#include "stdbool.h"
//#include "py/mpconfig.h"
//#include "py/obj.h"
#include "esp_vfs_fat.h"

 #include "vfs_fat_internal.h"
#include "ff.h"
//typedef FIL* FIL;
int file_write_open_raise(FIL* fp, const char *path);
int file_read_open_raise(FIL* fp, const char *path);
int file_write_open(FIL* fp, const char *path);
int file_read_open(FIL* fp, const char *path);
int file_close(FIL *fp);
int file_seek(FIL *fp, FSIZE_t offset, uint8_t whence);
bool file_eof(FIL *fp);
int file_seek_raise(FIL *fp, FSIZE_t offset, uint8_t whence);
UINT file_save_data(const char* path, uint8_t* data, UINT length, int* error_code);
UINT file_size(FIL *fp);
void file_buffer_on(FIL *fp);
void file_buffer_off(FIL *fp);

void fs_unsupported_format(FIL *fp);
void fs_file_corrupted(FIL *fp);
void fs_not_equal(FIL *fp);
void fs_no_intersection(FIL *fp);



int file_corrupted_raise(FIL *fp);
const char *ffs_strerror(int res);


int read_byte(FIL *fp, uint8_t* value);
int read_byte_raise(FIL *fp, uint8_t* value);
int read_byte_expect(FIL *fp, uint8_t value);
int read_byte_ignore(FIL *fp);
int write_byte(FIL *fp, uint8_t value);
int write_byte_raise(FIL *fp, uint8_t value);

int read_word(FIL *fp, uint16_t* value);
int read_word_raise(FIL *fp, uint16_t* value);
int read_word_expect(FIL *fp, uint16_t value);
int read_word_ignore(FIL *fp);
int write_word(FIL *fp, uint16_t value);
int write_word_raise(FIL *fp, uint16_t value);

int read_long(FIL *fp, uint32_t* value);
int read_long_raise(FIL *fp, uint32_t* value);
int read_long_expect(FIL *fp, uint32_t value);
int read_long_ignore(FIL *fp);
int write_long(FIL *fp, uint32_t value);
int write_long_raise(FIL *fp, uint32_t value);

int read_data(FIL *fp, void *data, UINT size);
int read_data_raise(FIL *fp, void *data, UINT size);
int write_data(FIL *fp, const void *data, UINT size);
int write_data_raise(FIL *fp, const void *data, UINT size);
int file_write(FIL *fp, void *data, UINT size, UINT* size_out);
int file_read(FIL *fp, void *data, UINT size, UINT* size_out);
int read_data_raise(FIL *fp, void *data, UINT size);


