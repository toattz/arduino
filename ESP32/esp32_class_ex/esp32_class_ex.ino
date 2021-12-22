class Display
{
  protected:
    int _x=0;
    int _y=0;

  public:
    void setCursor(int x,int y)
    {
      _x=x;
      _y=y;
    }

    void print(String txt)
    {
      Serial.printf("At (%u,%u) show %s\n",_x,_y,txt.c_str());
    }  
};

class OLED:public Display
{
  public:
    void drawLine()
    {
      Serial.println("OLED drawLine()");
    }

    void createBitmap()
    {
      Serial.println("OLED createBitmap()");
    }
};

class LCD:public Display
{
  public:
    void createChar()
    {
      Serial.println("LCD createChar()");
    }

    void backlight()
    {
      Serial.println("LCD backlight");
    }
};

void setup() 
{
  Serial.begin(115200);
  OLED oled;
  oled.setCursor(1,3);
  oled.print("OLED test print function");
  oled.drawLine();
  oled.createBitmap();

  LCD led;
  led.setCursor(8,8);
  led.print("LED tttttt");
  led.createChar();
  led.backlight();
}

void loop() {
  // put your main code here, to run repeatedly:

}
