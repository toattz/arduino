#include<time.h>
#include<sys/time.h>
void setup()
{
 Serial.begin(115200);
 time_t today=time(NULL);
 struct tm xmas={0,0,0,25,11,125};
 today=mktime(&xmas);
 struct timeval tv={.tv_sec=today};
 struct timezone tz={.tz_minuteswest=8*60};
 settimeofday(&tv,&tz);
 setenv("TZ","CTS-8",1);
 

}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
  time_t now=time(NULL);
  Serial.printf("ctime output:%s\n",ctime(&now));
  struct tm *localtm=localtime(&now);  
  Serial.println(localtm,"self formart %Y/%m/%d %H:%M:%S");

}
