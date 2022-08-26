#include <ELi_MdM_4_00.h>
#include <ELi_McM_4_00.h>

struct MD3_6_4_00_Pinconfig p1 = {.IN1 = CtrlBUS_AE,.IN2 = CtrlBUS_AF,.VREF = 0};
struct MD3_6_4_00_Pinconfig p2 = {.IN1 = CtrlBUS_AC,.IN2 = CtrlBUS_AD,.VREF = 0};

MD3_6_4_00_DD test3(p2, 100,MCPWM_UNIT_0,MCPWM_TIMER_0);
MD3_6_4_00_DD test4(p1, 100,MCPWM_UNIT_1,MCPWM_TIMER_0);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  test3.startmotor();
  test4.startmotor();
}

void loop() {
  // put your main code here, to run repeatedly:
  test3.setspeed(80,0);
  delay(100);
  test3.setspeed(0,0);
  
  test3.setspeed(50,1);
  delay(100);
  test3.setspeed(0,1);
  
  test4.setspeed(80,0);
  delay(100);
  test4.setspeed(0,0);
  
  test4.setspeed(50,1);
  delay(100);
  test4.setspeed(0,1);

}
