class priceTag
{
  public:
    float price=0;

    virtual float tax()
    //float tax()
    {
      return price*1.05;
    }
    
};

class redTag:public priceTag
{
  public:
    float tax()
    {
      return price;
    }
};


void setup()
{
  Serial.begin(115200);
  priceTag *pt;
  pt=new priceTag();
  pt->price=95;
  Serial.println(pt->tax());
  pt=new redTag();
  pt->price=95;
  Serial.println(pt->tax());
}

void loop() {
  // put your main code here, to run repeatedly:

}
