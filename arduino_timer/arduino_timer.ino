#include<Arduino_FreeRTOS.h>

typedef enum{
  IDLE,
  SETUP,
  RUNNING
}SystemStatus;

typedef enum{
  RELEASED,
  PRESSED,
  LONG_PRESSED,
  PRESSING,
  RELEASED_FROM_PRESS,
  RELEASED_FROM_LONG_PRESS
}SWStatus;

void taskLCD(void *pdParam)
{
  
}

void taskTimer(void *pdParam)
{
  
}

void setup() 
{
  xTaskCreate(taskLCD,"task LCD",100,NULL,1,NULL);
  xTaskCreate(taskTimer,"task Timer",100,NULL,1,NULL);
  vTaskStartScheduler();
}

void loop() {
  // put your main code here, to run repeatedly:

}
