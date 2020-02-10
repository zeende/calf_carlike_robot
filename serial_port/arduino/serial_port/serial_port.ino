String message_c;
const char* message;
int vx=0;
int vy=0;
int vth=0;
int send_vx;
int send_vth;
void setup()
{
  Serial.begin(115200);
  while(Serial.read()>= 0){}                 //清空串口0缓存

}
void loop()
{
if (Serial.available())
  { 
   // Serial.println("get");
    while(Serial.available()>0)//当有信号的时候
  {
    message_c +=char(Serial.read());
    delay(1);
  }
//String message_c = Serial.readString(); //串口1接收字符串
//  Serial.print(message_c);                           //串口接收的字符串
//  Serial.println(message_c.length());           //串口接收的字符串长度
  //Serial.print("have got");
  message = message_c.c_str();                //由于sscanf只能识别const char*类型字符串，将String类型字符串转成const char*类型
  //Serial.println(message);
  sscanf(message,"%d,%d",&vx,&vth);   //串口1接收字符串格式为435,25.25
//Serial.println("have cut");

while(Serial.read()>=0){};      //清空串口1缓存，保证字符串的长度稳定
}
// send_vx=(vx-1500);
// send_vth = vth-90;
send_vx=-vx;
send_vth=vth;
  Serial.print(send_vx);                                
  Serial.print(",");
   Serial.print(vy);                                
  Serial.print(",");
  Serial.println(send_vth);
  delay(100);
  message_c = "";
}
