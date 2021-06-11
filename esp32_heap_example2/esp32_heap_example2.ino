void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.printf("Free Heap:\t%u\n",ESP.getFreeHeap());

  int *pt=(int*)malloc(sizeof(int)*100);

  for(byte i=0;i<100;i++)
  {
    pt[i]=i;
  }

  Serial.printf("Free Heap after create array:\t%u\n",ESP.getFreeHeap());
  free(pt);
  Serial.printf("Free Heap after free array:\t%u\n",ESP.getFreeHeap());
}

void loop() {
  // put your main code here, to run repeatedly:

}
