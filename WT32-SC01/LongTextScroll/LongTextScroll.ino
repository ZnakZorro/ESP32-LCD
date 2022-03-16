//#define LGFX_AUTODETECT
#define LGFX_WT32_SC01
#include <LovyanGFX.hpp>

LGFX lcd;
LGFX_Sprite canvas;

static constexpr char text[] = "Był sobie pewien bardzo zmęczony drwal, który wciąż tracił czas i energię, tnąc drzewo tępą siekierą, ponieważ, jak mówił, nie miał czasu jej naostrzyć.";
static constexpr size_t textlen = sizeof(text) / sizeof(text[0]);
size_t textpos = 0;

void setup(void) 
{
  lcd.init();

  
  if (lcd.width() < lcd.height()) lcd.setRotation(lcd.getRotation() ^ 1);

  canvas.setColorDepth(8);
  //canvas.setFont(&fonts::lgfxJapanMinchoP_40);  //28 32 36 40 //https://m5stack.lang-ship.com/howto/m5gfx/font/
  canvas.setFont(&fonts::Font4);
  //canvas.setTextFont(8);
  
  canvas.setTextWrap(false);        // 右端到達時のカーソル折り返しを禁止
  canvas.createSprite(lcd.width() + 36, 36); // 画面幅+１文字分の横幅を用意
}

void loop(void)
{
  int32_t cursor_x = canvas.getCursorX() - 1;   // 現在のカーソル位置を取得し、1ドット左に移動
  if (cursor_x <= 0) // カーソル位置が左端に到達している場合は一周したと判断
  {
    textpos = 0;            // 文字列の読取り位置をリセット
    cursor_x = lcd.width(); // 新たな文字が画面右端に描画されるようにカーソル位置を変更
  }

  canvas.setCursor(cursor_x, 0); 
  canvas.scroll(-1, 0);          
  while (textpos < textlen && cursor_x <= lcd.width()) 
  {
    canvas.print(text[textpos++]);   
    cursor_x = canvas.getCursorX();  
  }

  canvas.pushSprite(&lcd, 0, 0);
}
