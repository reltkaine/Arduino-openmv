#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
//sudo chmod 666 /dev/ttyUSB0


#include "Arduino.h"
#include "esp_vfs_fat.h"

#include "vfs_fat_internal.h"


#include <SD.h>
extern "C" {
#include "imlib.h"
#include "ff.h"

}
#include "orb_file.h"

#include "pins_arduino.h"
#include <M5Unified.h>
#include <M5GFX.h>

#include "test_imlib.h"
#include "rgb_to_gray.h"


void setup() {
  M5.begin();
  M5.Lcd.fillScreen(GREEN); // put your setup code here, to run once:

  M5.update();
  fb_alloc_init0();
  const char* path1 = "/565shape.bmp";



  Serial.begin(115200);

  while (!Serial) {
    ;
  }

  Serial.printf("hello\n");


  const char format[3][8] = { "GRAY8", "RGB565", "RGB888" };
  if (!SD.begin(4, SPI, 25000000)) {
    Serial.println("FFat Mount Failed");
  }
  Serial.println("FFat read");





  File myFile = SD.open(path1);
  if (!myFile) {
    Serial.println(String(path1) + " not found");
  }


  //  setup_gray();

  //read_orf();







  Serial.println("FreeHEAP SD");
  Serial.println(ESP.getFreeHeap());
  Serial.println(ESP.getMaxAllocHeap());

  bmp.begin(myFile);
  BmpImage::BMP_IMAGE_PIX_FMT fmt = bmp.getPixFormat();
  if (fmt > 0) {
    Serial.println(path1);
    Serial.println("Orginal  Image format : " + String(format[fmt - 1]));
    Serial.println("Original File size    : " + String(bmp.getBmpSize()));
    Serial.println("Orginal  Image width  : " + String(bmp.getWidth()));
    Serial.println("Orgunal  Image height : " + String(bmp.getHeight()));
  } else {
    Serial.println("Fail: cannot read : " + String(bmp.getHeight()));
  }
  myFile.close();

  /*
      BMP_IMAGE_NONE   = 0
      BMP_IMAGE_GRAY8  = 1
      BMP_IMAGE_RGB565 = 2
      BMP_IMAGE_RGB888 = 3
    Serial.println(path1);   */
  // Serial.println("create img start ");
  // Serial.println("FreeHEAP Image start");
  // Serial.println(ESP.getFreeHeap());
  // Serial.println(ESP.getMaxAllocHeap());

  image_t *temp_img = imlib_image_create(240, 240, IMAGE_BPP_RGB565, 0, NULL, 0);

  if (fmt != BmpImage::BMP_IMAGE_RGB565 && fmt != BmpImage::BMP_IMAGE_NONE) {
    bmp.convertPixFormat(BmpImage::BMP_IMAGE_RGB565);
    //bmp.alignImageLine(true);
  }

  temp_img->w = bmp.getWidth();
  temp_img->h = bmp.getHeight();
  temp_img->bpp = IMAGE_BPP_RGB565;
  temp_img->data = static_cast<uint8_t*>(fb_alloc(image_size(temp_img)));//
  temp_img->pixels = bmp.getImgBuff();//(uint8_t*)

  //imlib_draw_rectangle(temp_img, 0, 0, 50, 50, COLOR_R8_G8_B8_TO_RGB565(0xff, 0x00, 0xff), 1, 1);

  M5.Lcd.drawBitmap(0, 0,  bmp.getWidth(), bmp.getHeight(), temp_img->pixels);

  fmt = bmp.getPixFormat();
  if (fmt > 0) {
    Serial.println(path1);
    Serial.println("Convert Image format : " + String(format[fmt - 1]));
    Serial.println("Convert File size    : " + String(bmp.getBmpSize()));
    Serial.println("Convert Image width  : " + String(bmp.getWidth()));
    Serial.println("Convert Image height : " + String(bmp.getHeight()));
  } else {
    Serial.println("Fail: cannot read : " + String(bmp.getHeight()));
  }

  find_lines(temp_img);
  find_cricle(temp_img);
  find_rect(temp_img);
  find_blob(temp_img);
  bmp.end();




  ///  april_tag(openBmp_to_rgb565("/apriltags.bmp"));  doesnt work!!!

  data_matrix(openBmp_to_rgb565("/datamatrix.bmp"));



  qr_code(openBmp_to_rgb565("/qrcode.bmp"));


  array_t *kpts_out, *kpts_out1; //kpts_out;
  kpts_out = py_image_find_keypoints(openBmp_to_rgb565("/graffiti-gray.bmp"));
  kpts_out1 = py_image_find_keypoints(openBmp_to_rgb565("/graffiti.bmp"));

  py_image_match_descriptor(kpts_out, kpts_out1) ;



  fb_free_all();


  M5.update();
  Serial.printf("end\n");





}

void loop() {
  // put your main code here, to run repeatedly:

}




/*
 * ets Jun  8 2016 00:22:57
 * 
 * Serial Outpu Sample......................................

rst:0x1 (POWERON_RESET),boot:0x17 (SPI_FAST_FLASH_BOOT)
configsip: 0, SPIWP:0xee
clk_drv:0x00,q_drv:0x00,d_drv:0x00,cs0_drv:0x00,hd_drv:0x00,wp_drv:0x00
mode:DIO, clock div:1
load:0x3fff0030,len:4888
load:0x40078000,len:16456
load:0x40080400,len:4
load:0x40080404,len:3476
entry 0x400805b4
auto detect board:1
hello
FFat read
FreeHEAP SD
306300
110580
/565shape.bmp
Orginal  Image format : RGB888
Original File size    : 57654
Orginal  Image width  : 160
Orgunal  Image height : 120
/565shape.bmp
Convert Image format : RGB565
Convert File size    : 38466
Convert Image width  : 160
Convert Image height : 120
show LINE
?line#0 : x-22 y-0 w-22 h-118
line#1 : x-0 y-38 w-158 h-35
line#2 : x-57 y-0 w-57 h-118
line#3 : x-0 y-75 w-158 h-72
line#4 : x-84 y-117 w-158 h-44
CIRCLE 
CIRCLE#0 : x-119 y-55 r-19 
End Circle
RECTANGLE
corner#0 : x-144 y-56 
corner#0 : x-118 y-31 
corner#0 : x-94 y-55 
corner#0 : x-119 y-81 
RECTANGLE
corner#1 : x-57 y-75 
corner#1 : x-57 y-38 
corner#1 : x-22 y-39 
corner#1 : x-23 y-74 
Blob start
BLOB#0 : x-99 y-36 w-39 h-39
BLOB#1 : x-23 y-39 w-34 h-35



Blob End
/datamatrix.bmp
Orginal  Image format : RGB888
Original File size    : 57722
Orginal  Image width  : 160
Orgunal  Image height : 120
Convert Image format : RGB565
Convert File size    : 38466
Convert Image width  : 160
Convert Image height : 120
Data Matrix code
DATA matrix ID
https://openmv.io/xV⸮⸮⸮?h
/qrcode.bmp
Orginal  Image format : RGB888
Original File size    : 230522
Orginal  Image width  : 320
Orgunal  Image height : 240
Convert Image format : RGB565
Convert File size    : 153666
Convert Image width  : 320
Convert Image height : 240
320
QR code
https://openmv.ioxV⸮⸮+⸮?h
/graffiti-gray.bmp
Orginal  Image format : GRAY8
Original File size    : 77878
Orginal  Image width  : 320
Orgunal  Image height : 240
Convert Image format : RGB565
Convert File size    : 153666
Convert Image width  : 320
Convert Image height : 240
100
template NOT EMPTY


/graffiti.bmp
Orginal  Image format : RGB888
Original File size    : 230522
Orginal  Image width  : 320
Orgunal  Image height : 240
Convert Image format : RGB565
Convert File size    : 153666
Convert Image width  : 320
Convert Image height : 240
100
template NOT EMPTY


100
template 1 NOT EMPTY
100
template 2 NOT EMPTY
orb inside
matches
center x 163
center y 115
rectangle x 48
rectangle y 41
rectangle w 239
rectangle h 159
count 8
theta 0
end

 * 
 * 
 */
 */
