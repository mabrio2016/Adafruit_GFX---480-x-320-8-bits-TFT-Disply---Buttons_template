/* Toggle button and simple buttons implementation 
 *  
 */
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
#include <TouchScreen.h>
#define MINPRESSURE 200
#define MAXPRESSURE 1000

// ALL Touch panels and wiring is DIFFERENT
#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin
int TS_LEFT=126,TS_RT=907,TS_TOP=953,TS_BOT=85;
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
int pixel_x, pixel_y;     //Touch_getXY() updates global vars
//

Adafruit_GFX_Button on_btn, off_btn, toggle_btn; //Button Names IDs

boolean P_ON = false; //Toggle Button flag

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

typedef struct botton_param // button parameters template
{
  MCUFRIEND_kbv * gfx;
  int16_t   x;
  int16_t   y;
  uint16_t  w;
  uint16_t  h;
  uint16_t  outline;
  uint16_t  fill;
  uint16_t  textcolor;
  char *    Label;
  uint8_t   textsize;
};
typedef struct circle_param // Circle parameters template
{
  int16_t   x0;
  int16_t   y0;
  int16_t   r;
  uint16_t  color;
};
typedef struct Rectangle_param // Rectangle parameters template
{ 
  int16_t   x;
  int16_t   y;
  int16_t   w;
  int16_t   h;
  uint16_t  color; 
};

typedef struct RoundRect_param // Round Rectangle parameters template
{
  int16_t   x;
  int16_t   y;
  int16_t   w;
  int16_t   h;
  int16_t   r;
  uint16_t  color;
}; 

//Define the screen elements//

botton_param toggle_btn_init{&tft, 80, 100, 100, 40, BLUE, WHITE, BLACK, "ON", 2};
botton_param on_btn_init{&tft, 80, 170, 100, 40, WHITE, MAGENTA, BLACK, "ON", 2};
botton_param off_btn_init{&tft, 190, 170, 100, 40, WHITE, GREEN, BLACK, "OFF", 2};
circle_param LED1_init{80, 40, 20, WHITE};
Rectangle_param Rectangle1_init{20, 140, 230, 60, YELLOW};
RoundRect_param RoundRect1_init{20, 140, 230, 60, 10, YELLOW};

//
void setup(void)
{
    Serial.begin(9600);
    uint16_t ID = tft.readID();
    Serial.print("TFT ID = 0x");
    Serial.println(ID, HEX);
    if (ID == 0xD3D3) ID = 0x9486; // write-only shield
    tft.begin(ID);
    tft.setRotation(1);            //(0)PORTRAIT, (1)LANSCAPE
    tft.fillScreen(BLACK);
    
    //Initiate the Rectangles Elements//
    //InitRectangles(Rectangle1_init, 1, YELLOW);

    //Initiate the Round Rectangles Elements//
    InitRoundRect(RoundRect1_init, 0, YELLOW);
    
    //Initiate the Button Elements//
    InitButtons(&toggle_btn, &toggle_btn_init);
    InitButtons(&on_btn, &on_btn_init);
    InitButtons(&off_btn, &off_btn_init); 

    //Initiate the LEDs Elements//
    InitLED(LED1_init, 0, TFT_GREEN);    


}

void InitButtons(Adafruit_GFX_Button *botton_id, struct botton_param *paramiters){
  botton_id->initButton(&tft, paramiters->x, paramiters->y, paramiters->w, paramiters->h, paramiters->outline, paramiters->fill, paramiters->textcolor, paramiters->Label, paramiters->textsize);
  botton_id->drawButton(false);  
}

void InitLED(struct circle_param paramiters, bool fill, uint16_t color){
  if (fill == false){
    tft.drawCircle(paramiters.x0, paramiters.y0, paramiters.r, paramiters.color); 
    tft.drawCircle(paramiters.x0, paramiters.y0, paramiters.r +1, paramiters.color); 
    tft.drawCircle(paramiters.x0, paramiters.y0, paramiters.r +2, paramiters.color);
  }
  if (fill == true){
    tft.fillCircle(paramiters.x0, paramiters.y0, paramiters.r, color);
  }
}
void InitRectangles(struct Rectangle_param paramiters, bool fill, uint16_t color){
  if (fill == false){
    tft.drawRect(paramiters.x, paramiters.y, paramiters.w, paramiters.h, paramiters.color); 
  }
  if (fill == true){
    tft.fillRect(paramiters.x, paramiters.y, paramiters.w, paramiters.h, paramiters.color);
  }
}
void InitRoundRect(struct RoundRect_param paramiters, bool fill, uint16_t color){
  if (fill == false){
    tft.drawRoundRect(paramiters.x, paramiters.y, paramiters.w, paramiters.h, paramiters.r, paramiters.color); 
  }
  if (fill == true){
    tft.fillRoundRect(paramiters.x, paramiters.y, paramiters.w, paramiters.h, paramiters.r, paramiters.color);
  }
}
  
bool Touch_getXY(void)
{
    TSPoint p = ts.getPoint();
    pinMode(YP, OUTPUT);      //restore shared pins
    pinMode(XM, OUTPUT);
    digitalWrite(YP, HIGH);   //because TFT control pins
    digitalWrite(XM, HIGH);
    bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);
    if (pressed) {
        pixel_x = map(p.y, TS_LEFT=960, TS_RT=94, 0, 480);    // LANSCAPE TouchScreen orientation
        pixel_y = map(p.x, TS_TOP=913, TS_BOT=130, 0, 320);

        //pixel_x = map(p.x, TS_LEFT=130, TS_RT=913, 0, 320); // PORTRAIT TouchScreen orientation
        //pixel_y = map(p.y, TS_TOP=960, TS_BOT=94, 0, 480);
    }
    return pressed;
}

/*  
 * updating multiple buttons from a list
 * 
 * anything more than two buttons gets repetitive
 * 
 * you can place button addresses in separate lists
 * e.g. for separate menu screens
 */
// Array of button addresses to behave like a list
Adafruit_GFX_Button *buttons[] = {&toggle_btn, &on_btn, &off_btn, NULL};

/* update the state of a button and redraw as reqd
 *
 * main program can use isPressed(), justPressed() etc
 */
bool update_button(Adafruit_GFX_Button *b, bool down)
{
    b->press(down && b->contains(pixel_x, pixel_y));
    if (b->justReleased())
        b->drawButton(false);
    if (b->justPressed())
        b->drawButton(true);
    return down;
}

/* most screens have different sets of buttons
 * life is easier if you process whole list in one go
 */
bool update_button_list(Adafruit_GFX_Button **pb)
{
    bool down = Touch_getXY();
    for (int i = 0 ; pb[i] != NULL; i++) {
        update_button(pb[i], down);
    }
    return down;
}

/* compare the simplicity of update_button_list()
 */
void loop(void)
{
     //Touggle Botton//
     update_button_list(buttons);  
     if (toggle_btn.justPressed()) {
        if (P_ON == true) {
          InitLED(LED1_init, 1, BLACK);
          delay(20);
          toggle_btn_init.Label = "ON"; 
          InitButtons(&toggle_btn, &toggle_btn_init);
          //toggle_btn.initButton(&tft,  80, 150, 100, 40, WHITE, CYAN, BLACK, "ON", 2);
          toggle_btn.drawButton(true);
          delay(20);
          P_ON = false; 
          return; // true;      
        }
        if (P_ON == false) {
          InitLED(LED1_init, 1, RED);
          delay(20);
          toggle_btn_init.Label = "OFF";  
          //toggle_btn.initButton(&tft,  80, 150, 100, 40, WHITE, CYAN, BLACK, "OFF", 2);
          InitButtons(&toggle_btn, &toggle_btn_init);
          toggle_btn.drawButton(true);
          delay(20);
          P_ON = true; 
          return; // true;      
        }      
    }
    //Simple Botton//
    if (on_btn.justPressed()) {
        tft.fillCircle(80, 40, 20, TFT_RED);
    }
    
    if (off_btn.justPressed()) {
        tft.fillCircle(80, 40, 20, TFT_BLACK);
    }
}
