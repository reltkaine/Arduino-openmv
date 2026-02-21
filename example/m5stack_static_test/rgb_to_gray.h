/**
   Simple bitmap image library for Arduino
   LGPL version 2.1 Copyright 2021 Yoshino Taro
*/

#include "BmpImage.h"


#include <SD.h>

const char BMPFILE[] = "/apriltags.bmp";
const char format[3][8] = {"GRAY8", "RGB565", "RGB888" };

image_t *temp_img_out = imlib_image_create(240, 240, IMAGE_BPP_RGB565, 0, NULL, 0);



image_t * openBmp_to_rgb565(const char BMP_open[]) {
  File myFile = SD.open(BMP_open);
  if (!myFile) {
    Serial.println(String(BMP_open) + " not found");
 //   return;
  }

  bmp.begin(myFile);
  BmpImage::BMP_IMAGE_PIX_FMT fmt = bmp.getPixFormat();
  if (fmt > 0) {
    Serial.println(BMP_open);
    Serial.println("Orginal  Image format : " + String(format[fmt - 1]));
    Serial.println("Original File size    : " + String(bmp.getBmpSize()));
    Serial.println("Orginal  Image width  : " + String(bmp.getWidth()));
    Serial.println("Orgunal  Image height : " + String(bmp.getHeight()));
  } else {
    Serial.println("Fail: cannot read : " + String(bmp.getHeight()));
    //return;
  }
  myFile.close();




  image_t *temp_img = imlib_image_create(240, 240, IMAGE_BPP_RGB565, 0, NULL, 0);

  if (fmt != BmpImage::BMP_IMAGE_RGB565 && fmt != BmpImage::BMP_IMAGE_NONE) {
    
    //bmp.alignImageLine(true);
    bmp.convertPixFormat(BmpImage::BMP_IMAGE_RGB565);//BMP_IMAGE_GRAY8
  }
  
  temp_img->w = bmp.getWidth();
  temp_img->h = bmp.getHeight();
  temp_img->bpp = IMAGE_BPP_RGB565;//IMAGE_BPP_RGB565;//
  temp_img->data = static_cast<uint8_t*>(fb_alloc(image_size(temp_img)));//
  temp_img->pixels = bmp.getImgBuff();//(uint8_t*)


  fmt = bmp.getPixFormat();
  if (fmt > 0) {
    Serial.println("Convert Image format : " + String(format[fmt - 1]));
    Serial.println("Convert File size    : " + String(bmp.getBmpSize()));
    Serial.println("Convert Image width  : " + String(bmp.getWidth()));
    Serial.println("Convert Image height : " + String(bmp.getHeight()));
  } else {
    Serial.println("Fail: cannot read : " + String(bmp.getHeight()));
  }

  bmp.end();
 
  return temp_img;
}

image_t * openBmp_to_gray(const char BMP_open[]) {
  File myFile = SD.open(BMP_open);
  if (!myFile) {
    Serial.println(String(BMP_open) + " not found");
 //   return;
  }

  bmp.begin(myFile);
  BmpImage::BMP_IMAGE_PIX_FMT fmt = bmp.getPixFormat();
  if (fmt > 0) {
    Serial.println(BMP_open);
    Serial.println("Orginal  Image format : " + String(format[fmt - 1]));
    Serial.println("Original File size    : " + String(bmp.getBmpSize()));
    Serial.println("Orginal  Image width  : " + String(bmp.getWidth()));
    Serial.println("Orgunal  Image height : " + String(bmp.getHeight()));
  } else {
    Serial.println("Fail: cannot read : " + String(bmp.getHeight()));
    //return;
  }
  myFile.close();




  image_t *temp_img = imlib_image_create(240, 240, IMAGE_BPP_RGB565, 0, NULL, 0);

  if (fmt != BmpImage::BMP_IMAGE_GRAY8 && fmt != BmpImage::BMP_IMAGE_NONE) {
    
    //bmp.alignImageLine(true);
    bmp.convertPixFormat(BmpImage::BMP_IMAGE_GRAY8);//BMP_IMAGE_GRAY8
  }
  
  temp_img->w = bmp.getWidth();
  temp_img->h = bmp.getHeight();
  temp_img->bpp = IMAGE_BPP_GRAYSCALE;//IMAGE_BPP_RGB565;//
  temp_img->data = static_cast<uint8_t*>(fb_alloc(image_size(temp_img)));//
  temp_img->pixels = bmp.getImgBuff();//(uint8_t*)


  fmt = bmp.getPixFormat();
  if (fmt > 0) {
    Serial.println("Convert Image format : " + String(format[fmt - 1]));
    Serial.println("Convert File size    : " + String(bmp.getBmpSize()));
    Serial.println("Convert Image width  : " + String(bmp.getWidth()));
    Serial.println("Convert Image height : " + String(bmp.getHeight()));
  } else {
    Serial.println("Fail: cannot read : " + String(bmp.getHeight()));
  }
  

  bmp.end();
  return temp_img;
}
