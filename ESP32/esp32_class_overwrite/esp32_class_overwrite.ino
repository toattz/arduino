class priceTag
{
  public:
    float price;

    float tax()
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
  priceTag *tag=new priceTag();
  tag->price=95;
  Serial.printf("%f\n",tag->tax());
  redTag *redtag=new redTag();
  redtag->price=95;
  Serial.printf("%f\n",redtag->tax());

}

void loop() {
  // put your main code here, to run repeatedly:

}
