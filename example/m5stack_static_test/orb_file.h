
#include "Arduino.h"
#include "esp_vfs_fat.h"

#include "vfs_fat_internal.h"
#include "vfs_wrapper.h"
#include "ff.h"
void read_orf()
{

  FATFS FatFs;
  array_t *kpts;

  FIL fp;
  int x;

  if (f_mount(&FatFs, "", 1) == FR_OK) {
    Serial.print(" OK");
  }

  const char *path1 = ".565shape.bmp";
  const char *path2 = "./565shape.bmp";
  x = f_open(&fp, path2, FA_READ);

  if (!x) {


    x = f_open(&fp, path1, FA_READ);
    Serial.print("Result 1 ");
    Serial.println(x);
  }

  orb_load_descriptor(&fp, kpts);

}






array_t * py_image_find_keypoints(image_t *img) {
  image_t *arg_img = img;

  rectangle_t roi;
  roi.x = 0;
  roi.y = 0;
  roi.w = arg_img->w;
  roi.h = arg_img->h;

  int threshold = 20;
  bool normalized = false;
  float scale_factor = 1.5f;
  int max_keypoints = 100;
  corner_detector_t corner_detector = CORNER_AGAST;

#ifndef IMLIB_ENABLE_FAST
  // Force AGAST when FAST is disabled.
  corner_detector = CORNER_AGAST;
#endif

  // Find keypoints
  fb_alloc_mark();
  array_t *kpts = orb_find_keypoints(arg_img, normalized, threshold, scale_factor, max_keypoints, corner_detector, &roi);
  // kpts_out = kpts;
  fb_alloc_free_till_mark();

  if (Serial.println(array_length(kpts))) {
    Serial.println("template NOT EMPTY");

    //   return kp_obj;
  } else {
    Serial.println("template  EMPTY");

  }
  Serial.println();
  Serial.println();
  //  return mp_const_none;
  fb_free_all();
  return kpts;
}







void py_image_match_descriptor(array_t* kpts_out1, array_t* kpts_out2) {
  array_t *kpts1 = kpts_out1;
  if (Serial.println(array_length(kpts1))) {
    Serial.println("template 1 NOT EMPTY");

    //   return kp_obj;
  } else {
    Serial.println("template 1 EMPTY");
  }
  array_t *kpts2 = kpts_out2;
  if (Serial.println(array_length(kpts2))) {
    Serial.println("template 2 NOT EMPTY");

    //   return kp_obj;
  } else {
    Serial.println("template 2 EMPTY");
  }
  int threshold = 85;
  int filter_outliers = false;


  int theta = 0;          // Estimated angle of rotation
  int count = 0;          // Number of matches
  point_t c = {0};        // Centroid
  rectangle_t r = {0};    // Bounding rectangle

  if (array_length(kpts1) && array_length(kpts1)) {
    Serial.println("orb inside");
    fb_alloc_mark();
    int *match = static_cast<int*>(fb_alloc(array_length(kpts1) * sizeof(int) * 2));

    // Match the two keypoint sets
    count = orb_match_keypoints(kpts1, kpts2, match, threshold, &r, &c, &theta);
    /*
        for (int i = 0; i < count * 2; i += 2) {

          Serial.println("matches");
          Serial.println(i);
          Serial.println(match[i + 0]);
          Serial.println(match[i + 1]);
        }
    */
    // Free match list
    fb_alloc_free_till_mark();

    if (filter_outliers == true) {
      count = orb_filter_keypoints(kpts2, &r, &c);
    }
  }
  Serial.println("matches");
  Serial.print("center x ");
  Serial.println(c.x);
  Serial.print("center y ");
  Serial.println(c.y);
  Serial.print("rectangle x ");
  Serial.println(r.x);
  Serial.print("rectangle y ");
  Serial.println(r.y);
  Serial.print("rectangle w ");
  Serial.println(r.w);
  Serial.print("rectangle h ");
  Serial.println(r.h);
  Serial.print("count ");
  Serial.println(count);
  Serial.print("theta ");
  Serial.println(theta);
  /*
      py_kptmatch_obj_t *o = m_new_obj(py_kptmatch_obj_t);
      o->base.type = &py_kptmatch_type;
      o->cx = mp_obj_new_int(c.x);
      o->cy = mp_obj_new_int(c.y);
      o->x = mp_obj_new_int(r.x);
      o->y = mp_obj_new_int(r.y);
      o->w = mp_obj_new_int(r.w);
      o->h = mp_obj_new_int(r.h);
      o->count = mp_obj_new_int(count);
      o->theta = mp_obj_new_int(theta);
      o->match = match_list;
      match_obj = o;
    Serial.println();
    Serial.println();
  */
  fb_free_all();
}
