#include<painlessMesh.h>

#define MESH_PREFIX "ESP32_Mesh"
#define MESH_PASSWORD "12345678"
#define MESH_PORT 5555

String nodeName="ESP8266";
const bool IS_ROOT=false;
painlessMesh mesh;
Scheduler userScheduler;

void sendMsg();

Task taskSendMsg(TASK_SECOND*1,TASK_FOREVER,sendMsg);
void sendMsg()
{
  String msg="I amd "+nodeName+", ID:"+mesh.getNodeId();
  mesh.sendBroadcast(msg);
  taskSendMsg.setInterval(random(TASK_SECOND*1,TASK_SECOND*5));
}

void receivedCallback(uint32_t from,String &msg)
{
  Serial.printf("Get message: %s from %u\n",msg.c_str(),from); 
}

void newConnectionCallback(uint32_t nodeId)
{
  Serial.printf("New connection, JSON style:\n%s\n",mesh.subConnectionJson(true).c_str());
}

/*void changeConnectionCallback()
{
  uint8_t nodes=mesh.getNodeList();
  Serial.printf("Connection is changed, node count:%d\n",nodes.size());
}*/

void setup() 
{
  Serial.begin(115200);
  mesh.init(MESH_PREFIX,
            MESH_PASSWORD,
            &userScheduler,
            MESH_PORT);
  mesh.onReceive(receivedCallback);
  mesh.onNewConnection(newConnectionCallback);
  mesh.setRoot(IS_ROOT);
  mesh.setContainsRoot(true);

  userScheduler.addTask(taskSendMsg);
  taskSendMsg.enable();
  Serial.printf("I am %s, ID is %u\n",nodeName,mesh.getNodeId());  
}

void loop() 
{
  mesh.update();
}
