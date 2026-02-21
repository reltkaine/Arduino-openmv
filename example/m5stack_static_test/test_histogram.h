extern "C" {
#include "imlib.h"
#include "ff.h"
  //#include "ff_wrapper.h"
  //extern c end///////////////////////////////
}
#include<assert.h>
////// not yet tested just somewhat translation port


void bar_code(image_t *img) {
  Serial.println("BARCODE");
  image_t *arg_img = img;

  rectangle_t roi;
   roi.x = 0;
  roi.y = 0;
  roi.w = arg_img->w;
  Serial.println(roi.w);
  roi.h = arg_img->h;

  list_t out;
  fb_alloc_mark();
  imlib_find_barcodes(&out, arg_img, &roi);
  fb_alloc_free_till_mark();

  for (size_t i = 0; list_size(&out); i++) {
    find_barcodes_list_lnk_data_t lnk_data;
    list_pop_front(&out, &lnk_data);
     Serial.println("bar code");
      Serial.println(lnk_data.payload);
    /*
            py_barcode_obj_t *o = m_new_obj(py_barcode_obj_t);
            o->base.type = &py_barcode_type;
            o->corners = mp_obj_new_tuple(4, (mp_obj_t [])
                {mp_obj_new_tuple(2, (mp_obj_t []) {mp_obj_new_int(lnk_data.corners[0].x), mp_obj_new_int(lnk_data.corners[0].y)}),
                 mp_obj_new_tuple(2, (mp_obj_t []) {mp_obj_new_int(lnk_data.corners[1].x), mp_obj_new_int(lnk_data.corners[1].y)}),
                 mp_obj_new_tuple(2, (mp_obj_t []) {mp_obj_new_int(lnk_data.corners[2].x), mp_obj_new_int(lnk_data.corners[2].y)}),
                 mp_obj_new_tuple(2, (mp_obj_t []) {mp_obj_new_int(lnk_data.corners[3].x), mp_obj_new_int(lnk_data.corners[3].y)})});
            o->x = mp_obj_new_int(lnk_data.rect.x);
            o->y = mp_obj_new_int(lnk_data.rect.y);
            o->w = mp_obj_new_int(lnk_data.rect.w);
            o->h = mp_obj_new_int(lnk_data.rect.h);
            o->payload = mp_obj_new_str(lnk_data.payload, lnk_data.payload_len);
            o->type = mp_obj_new_int(lnk_data.type);
            o->rotation = mp_obj_new_float(IM_DEG2RAD(lnk_data.rotation));
            o->quality = mp_obj_new_int(lnk_data.quality);

            objects_list->items[i] = o;
    */
    xfree(lnk_data.payload);
  }fb_free_all();
}


void data_matrix(image_t *img) {
  image_t *arg_img = img;
 Serial.println("Data Matrix code");
  rectangle_t roi;
     roi.x = 0;
  roi.y = 0;
  roi.w = arg_img->w;
 
  roi.h = arg_img->h;


  int effort = 200;

  list_t out;
  fb_alloc_mark();
  imlib_find_datamatrices(&out, arg_img, &roi, effort);
  fb_alloc_free_till_mark();

  for (size_t i = 0; list_size(&out); i++) {
    find_datamatrices_list_lnk_data_t lnk_data;
    list_pop_front(&out, &lnk_data);

        Serial.println("DATA matrix ID");
    Serial.println(lnk_data.payload);
    /*
            py_datamatrix_obj_t *o = m_new_obj(py_datamatrix_obj_t);
            o->base.type = &py_datamatrix_type;
            o->corners = mp_obj_new_tuple(4, (mp_obj_t [])
                {mp_obj_new_tuple(2, (mp_obj_t []) {mp_obj_new_int(lnk_data.corners[0].x), mp_obj_new_int(lnk_data.corners[0].y)}),
                 mp_obj_new_tuple(2, (mp_obj_t []) {mp_obj_new_int(lnk_data.corners[1].x), mp_obj_new_int(lnk_data.corners[1].y)}),
                 mp_obj_new_tuple(2, (mp_obj_t []) {mp_obj_new_int(lnk_data.corners[2].x), mp_obj_new_int(lnk_data.corners[2].y)}),
                 mp_obj_new_tuple(2, (mp_obj_t []) {mp_obj_new_int(lnk_data.corners[3].x), mp_obj_new_int(lnk_data.corners[3].y)})});
            o->x = mp_obj_new_int(lnk_data.rect.x);
            o->y = mp_obj_new_int(lnk_data.rect.y);
            o->w = mp_obj_new_int(lnk_data.rect.w);
            o->h = mp_obj_new_int(lnk_data.rect.h);
            o->payload = mp_obj_new_str(lnk_data.payload, lnk_data.payload_len);
            o->rotation = mp_obj_new_float(IM_DEG2RAD(lnk_data.rotation));
            o->rows = mp_obj_new_int(lnk_data.rows);
            o->columns = mp_obj_new_int(lnk_data.columns);
            o->capacity = mp_obj_new_int(lnk_data.capacity);
            o->padding = mp_obj_new_int(lnk_data.padding);

            objects_list->items[i] = o;
    */
    xfree(lnk_data.payload);
  }
fb_free_all();
}


void april_tag(image_t *img) {
  Serial.println("AprilTag Start");
  image_t *arg_img = img;

  rectangle_t roi;
  roi.x = 0;
  roi.y = 0;
  roi.w = arg_img->w;
  Serial.println(roi.w);
  roi.h = arg_img->h;
  //#ifndef IMLIB_ENABLE_HIGH_RES_APRILTAGS
  //    PY_ASSERT_TRUE_MSG((roi.w * roi.h) < 65536, "The maximum supported resolution for find_apriltags() is < 64K pixels.");
  //#endif
  //    if ((roi.w < 4) || (roi.h < 4)) {
  //        return mp_obj_new_list(0, NULL);
  //    }

  apriltag_families_t families = TAG36H11;
  // 2.8mm Focal Length w/ OV7725 sensor for reference.
      float fx = (2.8 / 3.984) * roi.w;
  // 2.8mm Focal Length w/ OV7725 sensor for reference.
    float fy =  (2.8 / 2.952) * roi.h;
  // Use the image versus the roi here since the image should be projected from the camera center.
    float cx =  roi.w * 0.5;
  // Use the image versus the roi here since the image should be projected from the camera center.
   float cy =  roi.h * 0.5;

  list_t out;
  fb_alloc_mark();
  imlib_find_apriltags(&out, arg_img, &roi, families, fx, fy, cx, cy);
  fb_alloc_free_till_mark();

  for (size_t i = 0; list_size(&out); i++) {
    find_apriltags_list_lnk_data_t lnk_data;
    list_pop_front(&out, &lnk_data);

    Serial.println("AprilTag ID");
    Serial.println(lnk_data.id);
    Serial.println(lnk_data.corners[0].y);
    Serial.println(lnk_data.family);
    Serial.println(lnk_data.rect.x);
    

    /*
            py_apriltag_obj_t *o = m_new_obj(py_apriltag_obj_t);
            o->base.type = &py_apriltag_type;
            o->corners = mp_obj_new_tuple(4, (mp_obj_t [])
                {mp_obj_new_tuple(2, (mp_obj_t []) {mp_obj_new_int(lnk_data.corners[0].x), mp_obj_new_int(lnk_data.corners[0].y)}),
                 mp_obj_new_tuple(2, (mp_obj_t []) {mp_obj_new_int(lnk_data.corners[1].x), mp_obj_new_int(lnk_data.corners[1].y)}),
                 mp_obj_new_tuple(2, (mp_obj_t []) {mp_obj_new_int(lnk_data.corners[2].x), mp_obj_new_int(lnk_data.corners[2].y)}),
                 mp_obj_new_tuple(2, (mp_obj_t []) {mp_obj_new_int(lnk_data.corners[3].x), mp_obj_new_int(lnk_data.corners[3].y)})});
            o->x = mp_obj_new_int(lnk_data.rect.x);
            o->y = mp_obj_new_int(lnk_data.rect.y);
            o->w = mp_obj_new_int(lnk_data.rect.w);
            o->h = mp_obj_new_int(lnk_data.rect.h);
            o->id = mp_obj_new_int(lnk_data.id);
            o->family = mp_obj_new_int(lnk_data.family);
            o->cx = mp_obj_new_int(lnk_data.centroid.x);
            o->cy = mp_obj_new_int(lnk_data.centroid.y);
            o->rotation = mp_obj_new_float(lnk_data.z_rotation);
            o->decision_margin = mp_obj_new_float(lnk_data.decision_margin);
            o->hamming = mp_obj_new_int(lnk_data.hamming);
            o->goodness = mp_obj_new_float(lnk_data.goodness);
            o->x_translation = mp_obj_new_float(lnk_data.x_translation);
            o->y_translation = mp_obj_new_float(lnk_data.y_translation);
            o->z_translation = mp_obj_new_float(lnk_data.z_translation);
            o->x_rotation = mp_obj_new_float(lnk_data.x_rotation);
            o->y_rotation = mp_obj_new_float(lnk_data.y_rotation);
            o->z_rotation = mp_obj_new_float(lnk_data.z_rotation);

            objects_list->items[i] = o;*/
  }

fb_free_all();
}

void qr_code(image_t *img) {
  image_t *arg_img = img;

  rectangle_t roi;
    roi.x = 0;
  roi.y = 0;
  roi.w = arg_img->w;
  Serial.println(roi.w);
  roi.h = arg_img->h;

  list_t out;
  fb_alloc_mark();
  imlib_find_qrcodes(&out, arg_img, &roi);
  fb_alloc_free_till_mark();

  for (size_t i = 0; list_size(&out); i++) {
    find_qrcodes_list_lnk_data_t lnk_data;
    list_pop_front(&out, &lnk_data);
    Serial.println("QR code");
    Serial.println(lnk_data.payload);
    /*
            py_qrcode_obj_t *o = m_new_obj(py_qrcode_obj_t);
            o->base.type = &py_qrcode_type;
            o->corners = mp_obj_new_tuple(4, (mp_obj_t [])
                {mp_obj_new_tuple(2, (mp_obj_t []) {mp_obj_new_int(lnk_data.corners[0].x), mp_obj_new_int(lnk_data.corners[0].y)}),
                 mp_obj_new_tuple(2, (mp_obj_t []) {mp_obj_new_int(lnk_data.corners[1].x), mp_obj_new_int(lnk_data.corners[1].y)}),
                 mp_obj_new_tuple(2, (mp_obj_t []) {mp_obj_new_int(lnk_data.corners[2].x), mp_obj_new_int(lnk_data.corners[2].y)}),
                 mp_obj_new_tuple(2, (mp_obj_t []) {mp_obj_new_int(lnk_data.corners[3].x), mp_obj_new_int(lnk_data.corners[3].y)})});
            o->x = mp_obj_new_int(lnk_data.rect.x);
            o->y = mp_obj_new_int(lnk_data.rect.y);
            o->w = mp_obj_new_int(lnk_data.rect.w);
            o->h = mp_obj_new_int(lnk_data.rect.h);
            o->payload = mp_obj_new_str(lnk_data.payload, lnk_data.payload_len);
            o->version = mp_obj_new_int(lnk_data.version);
            o->ecc_level = mp_obj_new_int(lnk_data.ecc_level);
            o->mask = mp_obj_new_int(lnk_data.mask);
            o->data_type = mp_obj_new_int(lnk_data.data_type);
            o->eci = mp_obj_new_int(lnk_data.eci);

            objects_list->items[i] = o;
    */
    xfree(lnk_data.payload);
  }
fb_free_all();
}


void image_get_histogram(image_t *img)
{
  //  BLOB#0 : x-99 y-36 w-39 h-39
  //BLOB#1 : x-23 y-39 w-34 h-35
  image_t *arg_img = img;

  list_t thresholds;
  list_init(&thresholds, sizeof(color_thresholds_list_lnk_data_t));
  //py_helper_keyword_thresholds(n_args, args, 1, kw_args, &thresholds);  2
  bool invert =  false;
  image_t *other =  NULL;


  //py_helper_keyword_rectangle_roi(arg_img, n_args, args, 3, kw_args, &roi); 4
  rectangle_t roi;
  roi.x = 99;
  roi.y = 36;
  roi.w = 99 + 39 - 1;
  roi.h = 36 + 39 - 1;

  histogram_t h;
  switch (arg_img->bpp) {
    case IMAGE_BPP_BINARY: {
        int bins = 2;
        h.LBinCount = 2;
        h.ABinCount = 0;
        h.BBinCount = 0;
        fb_alloc_mark();
        h.LBins = static_cast<float*>(fb_alloc(h.LBinCount * sizeof(float)));
        h.ABins = NULL;
        h.BBins = NULL;
        imlib_get_histogram(&h, arg_img, &roi, NULL, false);
        list_free(&thresholds);
        Serial.println("binary");
        break;
      }
    case IMAGE_BPP_GRAYSCALE: {
        int bins = 2;
        h.LBinCount = 2;
        h.ABinCount = 0;
        h.BBinCount = 0;
        fb_alloc_mark();
        h.LBins = static_cast<float*>(fb_alloc(h.LBinCount * sizeof(float)));
        h.ABins = NULL;
        h.BBins = NULL;
        imlib_get_histogram(&h, arg_img, &roi, NULL, false);//imlib_get_histogram(&hist, arg_img, &roi, &thresholds, invert, other);
        list_free(&thresholds);
        Serial.println("gray");
        break;
      }
    case IMAGE_BPP_RGB565: {
        h.LBinCount = COLOR_L_MAX - COLOR_L_MIN + 1;
        h.ABinCount = COLOR_A_MAX - COLOR_A_MIN + 1;
        h.BBinCount = COLOR_B_MAX - COLOR_B_MIN + 1;
        fb_alloc_mark();
        h.LBins = static_cast<float*>(fb_alloc(h.LBinCount * sizeof(uint32_t)));
        h.ABins = static_cast<float*>(fb_alloc(h.ABinCount * sizeof(uint32_t)));
        h.BBins = static_cast<float*>(fb_alloc(h.BBinCount * sizeof(uint32_t)));
        //imlib_get_histogram(&h, arg_img, &roi, &thresholds, invert, other);
        //  imlib_get_histogram(histogram_t *out, image_t *ptr, rectangle_t *roi, list_t *thresholds, bool invert);
        imlib_get_histogram(&h, arg_img, &roi, NULL, false);
        list_free(&thresholds);
        Serial.println("rgb565");
        break;









      }
    default: {
        Serial.println("EMPTY");
      }
  }
  /*
      py_histogram_obj_t *o = m_new_obj(py_histogram_obj_t);
      o->base.type = &py_histogram_type;
      o->bpp = arg_img->bpp;

      o->LBins = mp_obj_new_list(h.LBinCount, NULL);
      o->ABins = mp_obj_new_list(h.ABinCount, NULL);
      o->BBins = mp_obj_new_list(h.BBinCount, NULL);
  */
  for (int i = 0; i < h.LBinCount; i++) {
    Serial.println(h.LBins[i]);

  }

  for (int i = 0; i < h.ABinCount; i++) {
    Serial.println(h.ABins[i]);

  }

  for (int i = 0; i < h.BBinCount; i++) {
    Serial.println(h.BBins[i]);

  }

  fb_alloc_free_till_mark();


}
