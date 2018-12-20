#include  <TM1637Display.h>
// Module connection  pins
#define CLK 2
#define DIO 3
#define MQPIN A7

TM1637Display  display(CLK, DIO);
int n=0;
const uint8_t  MSG_BLOW[]={124,48,92,0};    //blow  message
const uint8_t  MSG_INIT[]={6,55,48,0};      //ini  message
const uint8_t  MSG_BA[]={127,247,0,0};      //BA:  message
uint8_t data[] = {  0xff, 0xff, 0xff, 0xff };  //blank  message for animations

void setup()
{
  display.setBrightness(0x0f);
  display.setSegments(MSG_INIT);
  stabilise();
}

void loop(){
  display.setSegments(MSG_BLOW);      //ready, show blow message
  int a,b;
  a=analogRead(MQPIN);
  b=a;

  while(abs(a-b)<10){      //wait for a large change
    b=a;
    a=analogRead(MQPIN);
    delay(300);
  }

  for(int i=1;i<64;i=i<<1){      //wait a bit
    data[0]=i-1;
    display.setSegments(data,1,3);   //filling circle pattern
    delay(500);
  }

  n=analogRead(MQPIN)-b;    //get reading above stable background

  if(n<0){n=0;}    //avoid invalid values near zero

  n=n/10;          //scale

  display.setSegments(MSG_BA,2,0);    //BA:
  display.showNumberDec(n,1,2,2);    //display analog value
  delay(10000);        //show reading, and let sensor  stabilise
  stabilise();
}

void stabilise(){
  data[0]=1;
  int a;
  int b=0;
  a=analogRead(MQPIN);
  display.setSegments(MSG_INIT);
  delay(500);
  //   wait for analog input to settle
  while(abs(a-b)>0){
    b=a;
    a=analogRead(MQPIN);
    data[0]=data[0]<<1;          //cycle pattern so it looks like its  doing something
    if(data[0]>32){data[0]=1;}
    display.setSegments(data,1,3);
    delay(500);
  }
}
