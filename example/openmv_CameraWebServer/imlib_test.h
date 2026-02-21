#include"Arduino-openmv.h"
extern "C" {
#include "imlib.h"
#include "ff.h"

}
image_t *temp_img = imlib_image_create(240, 240, IMAGE_BPP_RGB565, 0, NULL, 0);

void find_rect(image_t *img) {
  image_t *arg_img = NULL;
  arg_img = img;

  rectangle_t roi;
  roi.x = 0;
  roi.y = 0;
  roi.w = arg_img->w - 1;
  roi.h = arg_img->h - 1;


  uint32_t threshold = 1000;

  list_t out;
  fb_alloc_mark();
  imlib_find_rects(&out, arg_img, &roi, threshold);
  fb_alloc_free_till_mark();

  for (size_t i = 0; list_size(&out); i++) {
    find_rects_list_lnk_data_t lnk_data;
    list_pop_front(&out, &lnk_data);
    imlib_draw_circle(temp_img, lnk_data.corners[0].x, lnk_data.corners[0].y, 10, COLOR_R8_G8_B8_TO_RGB565(0x00, 0xff, 0x00), 1, false);

    imlib_draw_circle(temp_img, lnk_data.corners[1].x, lnk_data.corners[1].y, 10, COLOR_R8_G8_B8_TO_RGB565(0x00, 0xff, 0x00), 1, false);
    imlib_draw_circle(temp_img, lnk_data.corners[2].x, lnk_data.corners[2].y, 10, COLOR_R8_G8_B8_TO_RGB565(0x00, 0xff, 0x00), 1, false);
    imlib_draw_circle(temp_img, lnk_data.corners[3].x, lnk_data.corners[3].y, 10, COLOR_R8_G8_B8_TO_RGB565(0x00, 0xff, 0x00), 1, false);

    //Serial.println("RECTANGLE");

    //tft.drawCircle(lnk_data.corners[0].x, lnk_data.corners[0].y, 10, TFT_BLUE);
    //Serial.printf("corner#%u : x-%u y-%u \n", i, lnk_data.corners[0].x, lnk_data.corners[0].y);

    //Serial.printf("corner#%u : x-%u y-%u \n", i, lnk_data.corners[1].x, lnk_data.corners[1].y);

    //tft.drawCircle(lnk_data.corners[1].x, lnk_data.corners[1].y, 10, TFT_BLUE);
    //Serial.printf("corner#%u : x-%u y-%u \n", i, lnk_data.corners[2].x, lnk_data.corners[2].y);

    //tft.drawCircle(lnk_data.corners[2].x, lnk_data.corners[2].y, 10, TFT_BLUE);
    //Serial.printf("corner#%u : x-%u y-%u \n", i, lnk_data.corners[3].x, lnk_data.corners[3].y);

    //tft.drawCircle(lnk_data.corners[3].x, lnk_data.corners[3].y, 10, TFT_BLUE);
    /*
            py_rect_obj_t *o = m_new_obj(py_rect_obj_t);
            o->base.type = &py_rect_type;
            o->corners = mp_obj_new_tuple(4, (mp_obj_t [])
                {mp_obj_new_tuple(2, (mp_obj_t []) {mp_obj_new_int(lnk_data.corners[0].x), mp_obj_new_int(lnk_data.corners[0].y)}),
                 mp_obj_new_tuple(2, (mp_obj_t []) {mp_obj_new_int(lnk_data.corners[1].x), mp_obj_new_int(lnk_data.corners[1].y)}),
                 mp_obj_new_tuple(2, (mp_obj_t []) {mp_obj_new_int(lnk_data.corners[2].x), mp_obj_new_int(lnk_data.corners[2].y)}),
                 mp_obj_new_tuple(2, (mp_obj_t []) {mp_obj_new_int(lnk_data.corners[3].x), mp_obj_new_int(lnk_data.corners[3].y)})});
            o->x = mp_obj_new_int(lnk_data.rect.x);
            o->y = mp_obj_new_int(lnk_data.rect.y);
            o->w = mp_obj_new_int(lnk_data.rect.w);
            o->h = mp_obj_new_int(lnk_data.rect.h);
            o->magnitude = mp_obj_new_int(lnk_data.magnitude);

            objects_list->items[i] = o;
    */
  }


  fb_free_all();//<<------need to free memory after used or it will crashhhhh
}



void find_lines(image_t *img_ts)
{
  //Serial.printf("show LINE\n?");
  image_t *arg_img = NULL;
  arg_img = img_ts;

  rectangle_t roi;
  roi.x = 0;
  roi.y = 0;//////////////////
  roi.w = arg_img->w - 1;
  roi.h = arg_img->h - 1;


  unsigned int x_stride = 2;
  unsigned int y_stride = 1;
  uint32_t threshold = 1000;
  unsigned int theta_margin = 25;
  unsigned int rho_margin = 25;

  list_t out_;
  fb_alloc_mark();
  imlib_find_lines(&out_, arg_img, &roi, x_stride, y_stride, threshold, theta_margin, rho_margin);
  fb_alloc_free_till_mark();


  for (size_t i = 0; list_size(&out_); i++) {
    find_lines_list_lnk_data_t lnk_data;
    list_pop_front(&out_, &lnk_data);

    imlib_draw_line(temp_img, lnk_data.line.x1, lnk_data.line.y1, lnk_data.line.x2, lnk_data.line.y2, COLOR_R8_G8_B8_TO_RGB565(0x00, 0xff, 0x00), 3);

    // printf("fine line ones!\n");

    //Serial.printf("line#%u : x-%u y-%u w-%u h-%u\n", i, lnk_data.line.x1, lnk_data.line.y1, lnk_data.line.x2, lnk_data.line.y2);
    //tft.drawLine( lnk_data.line.x1, lnk_data.line.y1, lnk_data.line.x2, lnk_data.line.y2, TFT_RED);
    /*
        if (img_ts->pixfmt == PIXFORMAT_RGB888) {
          imlib_draw_line(img_ts, lnk_data.line.x1, lnk_data.line.y1, lnk_data.line.x2, lnk_data.line.y2, COLOR_R8_G8_B8_TO_RGB888(0x00, 0xff, 0x00), 1);
          lcd.drawLine(lnk_data.line.x1, lnk_data.line.y1, lnk_data.line.x2 ,lnk_data.line.y2,
                            COLOR_11R8_G8_B8_TO_RGB888(0x00, 0xff, 0x00));
        } else {
          imlib_draw_line(img_ts, lnk_data.line.x1, lnk_data.line.y1, lnk_data.line.x2, lnk_data.line.y2, COLOR_R8_G8_B8_TO_RGB565(0x00, 0xff, 0x00), 1);
          lcd.drawLine(lnk_data.line.x1, lnk_data.line.y1, lnk_data.line.x2, lnk_data.line.y2,
                            COLOR_R8_G8_B8_TO_RGB565(0x00, 0xff, 0x00));

        }
    */
    // py_line_obj_t *o = m_new_obj(py_line_obj_t);
    // o->base.type = &py_line_type;
    // o->x1 = mp_obj_new_int(lnk_data.line.x1);
    // o->y1 = mp_obj_new_int(lnk_data.line.y1);
    // o->x2 = mp_obj_new_int(lnk_data.line.x2);
    // o->y2 = mp_obj_new_int(lnk_data.line.y2);
    // int x_diff = lnk_data.line.x2 - lnk_data.line.x1;
    // int y_diff = lnk_data.line.y2 - lnk_data.line.y1;
    // o->length = mp_obj_new_int(fast_roundf(fast_sqrtf((x_diff * x_diff) + (y_diff * y_diff))));
    // o->magnitude = mp_obj_new_int(lnk_data.magnitude);
    // o->theta = mp_obj_new_int(lnk_data.theta);
    // o->rho = mp_obj_new_int(lnk_data.rho);

    // objects_list->items[i] = o;
  }
  fb_free_all();//<<------need to free memory after used or it will crashhhhh
}

void find_cricle(image_t *img_ts)
{
  //Serial.printf("CIRCLE \n");
  if (!img_ts)
  {
    return;
  }
  image_t *arg_img = img_ts;

  rectangle_t roi;
  roi.x = 0;
  roi.y = 0;
  roi.w = arg_img->w - 1;
  roi.h = arg_img->h - 1;

  unsigned int x_stride = 2;

  unsigned int y_stride = 1;

  uint32_t threshold = 3000;
  unsigned int x_margin = 10;
  unsigned int y_margin = 10;
  unsigned int r_margin = 10;
  unsigned int r_min = 15;
  unsigned int r_max = 25;
  unsigned int r_step = 2;

  list_t out;
  fb_alloc_mark();
  imlib_find_circles(&out, arg_img, &roi, x_stride, y_stride, threshold, x_margin, y_margin, r_margin,
                     r_min, r_max, r_step);
  fb_alloc_free_till_mark();

  for (size_t i = 0; list_size(&out); i++) {
    find_circles_list_lnk_data_t lnk_data;
    list_pop_front(&out, &lnk_data);

    //Serial.printf("CIRCLE#%u : x-%u y-%u r-%ld \n", i, lnk_data.p.x, lnk_data.p.y, lnk_data.r);
    //tft.fillCircle(lnk_data.p.x, lnk_data.p.y, lnk_data.r / 2, TFT_BLUE);
    imlib_draw_circle(temp_img, lnk_data.p.x, lnk_data.p.y, lnk_data.r, COLOR_R8_G8_B8_TO_RGB565(0x00, 0xff, 0x00), 1, false);

    /*
        if (img_ts->pixfmt == PIXFORMAT_RGB888) {
          imlib_draw_circle(img_ts, lnk_data.p.x, lnk_data.p.y, lnk_data.r,            COLOR_R8_G8_B8_TO_RGB888(0x00, 0xff, 0x00), 1, false);
          lcd.drawCircle(lnk_data.p.x, lnk_data.p.y, lnk_data.r, COLOR_R8_G8_B8_TO_RGB888(0x00, 0xff, 0x00));
        } else {
          imlib_draw_circle(img_ts, lnk_data.p.x, lnk_data.p.y, lnk_data.r,           COLOR_R8_G8_B8_TO_RGB565(0x00, 0xff, 0x00), 1, false);
          lcd.drawCircle(lnk_data.p.x, lnk_data.p.y, lnk_data.r, COLOR_R8_G8_B8_TO_RGB565(0x00, 0xff, 0x00));

        }
    */
  }
  //Serial.println("End Circle");
  fb_free_all();//<<------need to free memory after used or it will crashhhhh


}





void find_blob(image_t *img)

{
  image_t *arg_img = NULL;
  arg_img = img;


  // //Serial.println("Blob start");
  //  //Serial.printf("arg_img:%ux%u pixfmt=%d data=%p pixels=%p\n", arg_img->w, arg_img->h, arg_img->bpp, arg_img->data, arg_img->pixels);
  //  //Serial.print("image_size(arg_img) ");
  //  //Serial.println(image_size(arg_img));

  list_t thresholds;
  thresholds.size = 100;
  list_init(&thresholds, sizeof(color_thresholds_list_lnk_data_t));

  //WORN_PRINT("list info, size:%d",thresholds.size);

  color_thresholds_list_lnk_data_t tmp_ct;
  int info_bark[] = {30, 100, 15, 127, 15, 127};//<<RED color // { 41, 100, -128, 127, -128, 127};
  tmp_ct.LMin = info_bark[0];
  tmp_ct.AMin = info_bark[2];
  tmp_ct.BMin = info_bark[4];
  tmp_ct.LMax = info_bark[1];
  tmp_ct.AMax = info_bark[3];
  tmp_ct.BMax = info_bark[5];
  list_push_back(&thresholds, &tmp_ct);

  bool invert = false;  //false;

  rectangle_t roi;
  roi.x = 0;
  roi.y = 0;
  roi.w = arg_img->w;
  roi.h = arg_img->h;


  unsigned int x_stride = 2;
  unsigned int y_stride = 1;

  unsigned int area_threshold = 10;  //##10
  unsigned int pixels_threshold = 10;
  bool merge = true;
  int margin = 0;
  unsigned int x_hist_bins_max = 0;
  unsigned int y_hist_bins_max = 0;

  list_t out;
  fb_alloc_mark();
  int x = 9;

  imlib_find_blobs(&out, arg_img, &roi, x_stride, y_stride, &thresholds, invert,
                   area_threshold, pixels_threshold, merge, margin,

                   NULL, NULL, NULL, NULL);
  fb_alloc_free_till_mark();
  list_free(&thresholds);

  for (size_t i = 0; list_size(&out); i++) {
    find_blobs_list_lnk_data_t lnk_data;
    list_pop_front(&out, &lnk_data);

    //  //Serial.printf("BLOB#%u : x-%u y-%u w-%u h-%u\n", i, lnk_data.rect.x, lnk_data.rect.y, lnk_data.rect.w, lnk_data.rect.h);
    //   //tft.drawRect(lnk_data.rect.x, lnk_data.rect.y, lnk_data.rect.w, lnk_data.rect.h, TFT_GREEN);
    imlib_draw_rectangle(temp_img, lnk_data.rect.x, lnk_data.rect.y, lnk_data.rect.w, lnk_data.rect.h, COLOR_R8_G8_B8_TO_RGB565(0xff, 0xff, 0x00), 1, 0);
  }
  //Serial.println();
  //Serial.println();
  //Serial.println();

  //Serial.println("Blob End");
  //  fb_free_all();//<<------need to free memory after used or it will crashhhhh

  fb_free_all();

}
