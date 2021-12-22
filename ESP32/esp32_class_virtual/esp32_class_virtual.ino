class Shape
{
  public:
    virtual float area()=0;
    virtual float perimeter()=0;
};

class Rect:public Shape
{
  private:
    float _w,_h;
  public:
    Rect(float w,float h)
    {
      _w=w;
      _h=h;
    }

    Rect(float w)
    {
      _w=w;
      _h=w;
    }
    
    float area()
    {
      return _w*_h;
    }
    float perimeter()
    {
      return (_w+_h)*2;
    }
};

class Circle:public Shape
{
  private:
    float _r;

  public:
    Circle(float r)
    {
      _r=r;
    }
    float area()
    { 
      return _r*_r*PI;
    }
    float perimeter()
    {
      return _r*2*PI;
    }
};


void setup() 
{
  Serial.begin(115200);
  Shape *pt;
  Rect rect(10,50);
  Circle circle(20);
  pt=&rect;
  Serial.printf("Ract area:%.1f",pt->area());
  Serial.printf(" primeter:%.1f\n",pt->perimeter());
  pt=&circle;
  Serial.printf("Circle area:%.1f",pt->area());
  Serial.printf(" perimeter:%.1f\n",pt->perimeter());
}

void loop() {
  // put your main code here, to run repeatedly:

}
