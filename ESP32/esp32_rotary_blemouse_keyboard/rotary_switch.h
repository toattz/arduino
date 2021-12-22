#include<switch.h>

class RotarySwitch{
  private:
    byte clkPin;
    byte dtPin;
    bool prev=LOW;
    bool now=LOW;
    Switch *pSW;

  public:
    typedef enum:byte{
      /*RELEASED,
      PRESSED,
      LONG_PRESSED,
      PRESSING,
      RELEASED_FROM_PRESS,
      RELEASED_FROM_LONGPRESS,*/
      TURN_LEFT,
      TURN_RIGHT
    }Status;
    Status status=RELEASED;

    RotarySwitch(byte _clk,byte _dt,byte _sw){
      clkPin=_clk;
      dtPin=_dt;
      pSW=new Switch(_sw,LOW,true);
      pinMode(clkPin,INPUT);
      pinMode(dtPin,INPUT);
      prev=digitalRead(clkPin);
    }

    Status check()
    {
      Status swStatus=(Status)pSW->check();
      now=digitalRead(clkPin);

      if(now!=prev)
      {
        if(digitalRead(dtPin)!=now)
        {
          status=TURN_RIGHT;
        }
        else
        {
          status=TURN_LEFT;
        }
      }
      else
      { 
        status=RELEASED;
      }
      prev=now;

      if(swStatus!=RELEASED)
      {
        status=swStatus;
      }
      return status;
    }
};
