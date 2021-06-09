void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.printf("Heap available:\t%u\n",ESP.getFreeHeap());

  int *pt=new int[100];
  for(byte i=0;i<100;i++)
  {
    pt[i]=i;
  }
  Serial.printf("After create array:\t%u\n",ESP.getFreeHeap());
  delete pt;
  Serial.printf("Delete array:\t%u\n",ESP.getFreeHeap());
}

void loop() {
  // put your main code here, to run repeatedly:

}
