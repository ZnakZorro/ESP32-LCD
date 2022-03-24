/*

 
  int l=2;
  char *j = "test";
  long k = 123456789;
  char s = 'g';
  float f = 2.3;

  printf("test %d %l %c %s %f", l, k, s, j, f);

char buffer[40];
sprintf(buffer, "The %d burritos are %s degrees F", numBurritos, tempStr);

typedef struct
{ int a,b;
} Struc;


#include "../../../lv_examples.h"
#if LV_USE_LABEL



LV_STATE_DEFAULT
LV_STATE_PRESSED
LV_STATE_FOCUSED
LV_STATE_FOCUSED | LV_STATE_PRESSED, lv_color_hex(0xf88));


static lv_style_t style1;
lv_style_set_bg_color(&style1, LV_STATE_DEFAULT, LV_COLOR_WHITE);
lv_style_set_bg_color(&style1, LV_STATE_PRESSED, LV_COLOR_GRAY);
lv_style_set_bg_color(&style1, LV_STATE_FOCUSED, LV_COLOR_RED);
lv_style_set_bg_color(&style1, LV_STATE_FOCUSED | LV_STATE_PRESSED, lv_color_hex(0xf88));

void lv_ex_label_1(void)
{
    lv_obj_t * label1 = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_long_mode(label1, LV_LABEL_LONG_BREAK);     //Break the long lines
    lv_label_set_recolor(label1, true);                      //Enable re-coloring by commands in the text
    lv_label_set_align(label1, LV_LABEL_ALIGN_CENTER);       //Center aligned lines
    lv_label_set_text(label1, "#0000ff Re-color# #ff00ff words# #ff0000 of a# label "
                              "and  wrap long text automatically.");
    lv_obj_set_width(label1, 150);
    lv_obj_align(label1, NULL, LV_ALIGN_CENTER, 0, -30);

    lv_obj_t * label2 = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_long_mode(label2, LV_LABEL_LONG_SROLL_CIRC);     //Circular scroll
    lv_obj_set_width(label2, 150);
    lv_label_set_text(label2, "It is a circularly scrolling text. ");
    lv_obj_align(label2, NULL, LV_ALIGN_CENTER, 0, 30);
}

#endif


Input device events
LV_EVENT_PRESSED An object has been pressed
LV_EVENT_PRESSING An object is being pressed (called continuously while pressing)
LV_EVENT_PRESS_LOST An object is still being pressed but slid cursor/finger off of the object
LV_EVENT_SHORT_CLICKED An object was pressed for a short period of time, then released. Not called if scrolled.
LV_EVENT_LONG_PRESSED An object has been pressed for at least the long_press_time specified in the input device driver. Not called if scrolled.
LV_EVENT_LONG_PRESSED_REPEAT Called after long_press_time in every long_press_repeat_time ms. Not called if scrolled.
7   LV_EVENT_CLICKED    Called on release if an object did not scroll (regardless of long press)
LV_EVENT_RELEASED Called in every case when an object has been released
LV_EVENT_SCROLL_BEGIN Scrolling begins. The event parameter is NULL or an lv_anim_t * with a scroll animation descriptor that can be modified if required.
LV_EVENT_SCROLL_END Scrolling ends.
LV_EVENT_SCROLL An object was scrolled
LV_EVENT_GESTURE A gesture is detected. Get the gesture with lv_indev_get_gesture_dir(lv_indev_get_act());
LV_EVENT_KEY A key is sent to an object. Get the key with lv_indev_get_key(lv_indev_get_act());
LV_EVENT_FOCUSED An object is focused
LV_EVENT_DEFOCUSED An object is unfocused
LV_EVENT_LEAVE An object is unfocused but still selected
LV_EVENT_HIT_TEST Perform advanced hit-testing. Use lv_hit_test_info_t * a = lv_event_get_hit_test_info(e) and check if a->point can click the object or not. If not set a->res = false

 */
/*
 * 
 * 
 * 
 * 
 * 



enumerator LV_PALETTE_RED
enumerator LV_PALETTE_PINK
enumerator LV_PALETTE_PURPLE
enumerator LV_PALETTE_DEEP_PURPLE
enumerator LV_PALETTE_INDIGO
enumerator LV_PALETTE_BLUE
enumerator LV_PALETTE_LIGHT_BLUE
enumerator LV_PALETTE_CYAN
enumerator LV_PALETTE_TEAL
enumerator LV_PALETTE_GREEN
enumerator LV_PALETTE_LIGHT_GREEN
enumerator LV_PALETTE_LIME
enumerator LV_PALETTE_YELLOW
enumerator LV_PALETTE_AMBER
enumerator LV_PALETTE_ORANGE
enumerator LV_PALETTE_DEEP_ORANGE
enumerator LV_PALETTE_BROWN
enumerator LV_PALETTE_BLUE_GREY
enumerator LV_PALETTE_GREY
enumerator _LV_PALETTE_LAST
enumerator LV_PALETTE_NONE



Szkic używa 1 143 766 bajtów (36%) pamięci programu. Maksimum to 3145728 bajtów.
Zmienne globalne używają 85 692 bajtów (26%) pamięci dynamicznej, pozostawiając 241988 bajtów dla zmiennych lokalnych. Maksimum to 327680 bajtów.








 */
