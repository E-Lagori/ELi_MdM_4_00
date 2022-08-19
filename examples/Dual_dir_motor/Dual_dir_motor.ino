#include <ELi_MdM_4_00.h>
#include <ELi_McM_4_00.h>

struct MD3_6_4_00_Pinconfig p1 = {.IN1 = CtrlBUS_AE,.IN2 = CtrlBUS_AF,.VREF = 0}; // Pins for Motor 1
struct MD3_6_4_00_Pinconfig p2 = {.IN1 = CtrlBUS_AC,.IN2 = CtrlBUS_AD,.VREF = 0}; // Pins for Motor 2


MD3_6_4_00_DD test3(p1, 100); // Motor 1 initialization
MD3_6_4_00_DD test4(p2, 100); // Motor 2 initialization

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
    test3.startmotor();
    test4.startmotor();
}

void loop() {
  // put your main code here, to run repeatedly:
  test3.setspeed(80,0); // 80% duty cycle for 
  delay(1000);
  test3.setspeed(50,1);
  delay(1000);
  test4.setspeed(80,0);
  delay(1000);
  test4.setspeed(50,1);
  delay(1000);  
}
