
#include <esp_vfs_fat.h>
#include <vfs_fat_internal.h>
#include <ff.h>
extern "C" {
#include "imlib.h"

}
#include <esp_system.h>
#include "esp_camera.h"

// ===========================
// Select camera model in board_config.h
// ===========================

#include "board_config.h"
#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();       // Invoke custom
#define TFT_DC    12
#define TFT_RST   15
//#define TFT_CS    10 // only for displays with CS pin
#define TFT_MOSI    13 // for hardware SPI data pin (all of available pins)
#define TFT_SCLK  14   // for hardware SPI sclk pin (all of available pins)

//You can use different type of hardware initialization
//using hardware SPI (11, 13 on UNO; 51, 52 on MEGA; ICSP-4, ICSP-3 on DUE and etc)
//Arduino_ST7789 tft = Arduino_ST7789(TFT_DC, TFT_RST); //for display without CS pin
//Arduino_ST7789 tft = Arduino_ST7789(TFT_DC, TFT_RST, TFT_CS); //for display with CS pin
//or you can use software SPI on all available pins (slow)
//Arduino_ST7789 tft = Arduino_ST7789(TFT_DC, TFT_RST, TFT_MOSI, TFT_SCLK); //for display without CS pin
//Arduino_ST7789 tft = Arduino_ST7789(TFT_DC, TFT_RST, TFT_MOSI, TFT_SCLK, TFT_CS); //for display with CS pin

#include "imlib_test.h"



void setupLedFlash() {
#if defined(LED_GPIO_NUM)
  ledcAttach(LED_GPIO_NUM, 5000, 8);
#else
  log_i("LED flash is disabled -> LED_GPIO_NUM undefined");
#endif
}

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ;
  }
  tft.init();   // initialize a ST7789 chip, 240x240 pixels
  Serial.println("Initialized tft");

  uint16_t time = millis();
  tft.fillScreen(TFT_BLACK);
  time = millis() - time;
  tft.fillScreen(TFT_BLACK);

  Serial.printf("Camera Setup");
  Serial.setDebugOutput(true);
  Serial.println();
  fb_alloc_init0();

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.frame_size = FRAMESIZE_UXGA;
  //config.pixel_format = PIXFORMAT_JPEG;  // for streaming
  config.pixel_format = PIXFORMAT_RGB565; // for face detection/recognition
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 12;
  config.fb_count = 1;

  // if PSRAM IC present, init with UXGA resolution and higher JPEG quality
  //                      for larger pre-allocated frame buffer.
  if (config.pixel_format == PIXFORMAT_JPEG) {
    if (psramFound()) {
      config.jpeg_quality = 10;
      config.fb_count = 2;
      config.grab_mode = CAMERA_GRAB_LATEST;
    } else {
      // Limit the frame size when PSRAM is not available
      config.frame_size = FRAMESIZE_SVGA;
      config.fb_location = CAMERA_FB_IN_DRAM;
    }
  } else {
    // Best option for face detection/recognition
    config.frame_size = FRAMESIZE_240X240;
#if CONFIG_IDF_TARGET_ESP32S3
    config.fb_count = 2;
#endif
  }

#if defined(CAMERA_MODEL_ESP_EYE)
  pinMode(13, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
#endif

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  sensor_t *s = esp_camera_sensor_get();
  // initial sensors are flipped vertically and colors are a bit saturated
  if (s->id.PID == OV3660_PID) {
    s->set_vflip(s, 1);        // flip it back
    s->set_brightness(s, 1);   // up the brightness just a bit
    s->set_saturation(s, -2);  // lower the saturation
  }
  // drop down frame size for higher initial frame rate
  if (config.pixel_format == PIXFORMAT_JPEG) {
    s->set_framesize(s, FRAMESIZE_QVGA);
  }

#if defined(CAMERA_MODEL_M5STACK_WIDE) || defined(CAMERA_MODEL_M5STACK_ESP32CAM)
  s->set_vflip(s, 1);
  s->set_hmirror(s, 1);
#endif

#if defined(CAMERA_MODEL_ESP32S3_EYE)
  s->set_vflip(s, 1);
#endif

  // Setup LED FLash if LED pin is defined in camera_pins.h
#if defined(LED_GPIO_NUM)
  setupLedFlash();
#endif

  Serial.printf("Camera Setup End");
}


image_t *temp_img = imlib_image_create(240, 240, IMAGE_BPP_RGB565, 0, NULL, 0);


void loop() {

  camera_fb_t * fb = esp_camera_fb_get();
  Serial.printf("Camera fb \n");
  if (!fb || !fb->buf) {
    delay(1);  // let camera breathe
    esp_camera_fb_return(fb);
    Serial.printf("Camera capture failed\n");
    temp_img = NULL;
    return;
  } else {

    Serial.printf("fb to image\n");


    temp_img->w = fb->width;
    temp_img->h = fb->height;
    temp_img->bpp = IMAGE_BPP_RGB565;
    temp_img->data = static_cast<uint8_t*>(fb_alloc(image_size(temp_img)));//bmp.getImgBuff();
    temp_img->pixels = fb->buf;//(uint8_t*)

    esp_camera_fb_return(fb);
    //tft.drawXBitmap(0, 0,fb->buf,temp_img->w ,temp_img->h,TFT_BLACK);
    //tft.pushImage(0, 0,temp_img->w ,temp_img->h,fb->buf,true,NULL);

    tft.pushImage(0, 0, temp_img->w , temp_img->h, (uint16_t*) temp_img->pixels); //(uint16_t *)fb->buf



//    find_lines(temp_img);//work  uncomment to test
    //find_rect(temp_img); ///<== barely work uncomment to test

//    find_cricle(temp_img);//somewhat work uncomment to test
    find_blob(temp_img);// work uncomment to test red threshold
    
    fb_free_all();//<<------need to free memory after used or it will keep on crashing when memory is filled
    //delay(10);  // let camera breathe
    Serial.printf("circle get\n");





    Serial.printf("end loop\n");


    //temp_img = NULL;
    //  fb = NULL;
  }


}
