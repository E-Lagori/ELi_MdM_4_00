
/*****************************************
 * Library   : MdM_ESP32D_4_00.cpp - Library for dual motor driver carrier for E-LAGORi.
 * Programmer: Anish Bekal
 * Comments  : This library is to use with Dual 3.6A motor driver from E-Lagori
 * Versions  :
 * ------ 	---------- 		-------------------------
 * 0.1.0  	2018-08-17		First beta
 * 0.2.0    2018-08-26    First release
 *****************************************/

/*
 * Source for MdM_ESP32D_4_00
 *
 * Copyright (C) 2018  Anish Bekal https://www.e-lagori.com/product/motor-driver-board/
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * This file contains the code for ESP32MotorControl library.
 *
 */

/*
 * TODO list:
 * - Stepper motor drive using MCPWM 
 */

/*
 * TODO known issues:
 */
#include <ELi_MdM_4_00.h>
#include <ELi_McM_4_00.h>

#include "driver/mcpwm.h"
#include "soc/mcpwm_reg.h"
#include "soc/mcpwm_struct.h"
#include <Arduino.h>


MD3_6_4_00_SD::MD3_6_4_00_SD(MD3_6_4_00_Pinconfig p, float maxspeed, float maxrate, mcpwm_unit_t unt, mcpwm_io_signals_t io, uint32_t f){
  mcpwm_config_t pwm_conf;
  this->pin = p;
  this->freq = f;
  this->wait = 1/float(this->freq) *1000.0; //in ms
  this->pwm_num = unt;
  this->maxspeed = maxspeed;
  this->p_speed = 0;
  this->maxrate = maxrate;
  this->io = io;
  
  switch(this->io){
    case MCPWM0A: this->tim = MCPWM_TIMER_0;this->opr = MCPWM_OPR_A;pwm_conf.cmpr_a = 0;break;
    case MCPWM0B: this->tim = MCPWM_TIMER_0;this->opr = MCPWM_OPR_B;pwm_conf.cmpr_b = 0;break;
    case MCPWM1A: this->tim = MCPWM_TIMER_1;this->opr = MCPWM_OPR_A;pwm_conf.cmpr_a = 0;break;
    case MCPWM1B: this->tim = MCPWM_TIMER_1;this->opr = MCPWM_OPR_B;pwm_conf.cmpr_b = 0;break;
    case MCPWM2A: this->tim = MCPWM_TIMER_2;this->opr = MCPWM_OPR_A;pwm_conf.cmpr_a = 0;break;
    case MCPWM2B: this->tim = MCPWM_TIMER_2;this->opr = MCPWM_OPR_B;pwm_conf.cmpr_b = 0;break;
  };
  pwm_conf.frequency = this->freq;
  pwm_conf.counter_mode = MCPWM_UP_COUNTER;
  pwm_conf.duty_mode = MCPWM_DUTY_MODE_0;
  mcpwm_gpio_init(this->pwm_num, this->io, this->pin.IN1);
  mcpwm_init(this->pwm_num, this->tim, &pwm_conf);
}

void MD3_6_4_00_SD::startmotor(){
  mcpwm_start(this->pwm_num,this->tim);
  this->setspeed(this->p_speed,0);
}

void MD3_6_4_00_SD::setspeed(float speed){
  static float temp = ((this->maxspeed - abs(speed))/this->maxspeed);
  mcpwm_set_duty(this->pwm_num, this->tim, this->opr, (temp < 0?0:temp) * 100);
}

void MD3_6_4_00_SD::stopmotor(){
  this->setspeed_lin(0);
   mcpwm_set_signal_low(this->pwm_num, this->tim, this->opr);
   mcpwm_stop(this->pwm_num,this->tim);
}

void MD3_6_4_00_SD::setspeed_lin(float sp){
  float temp_speed = sp - this->p_speed;
  float iter = (abs(temp_speed)/this->maxspeed)*(this->maxrate/this->wait*1000.0);
  int int_iter = iter;
  float del_pwm = temp_speed/int_iter;
  float extr_wait = fmod(iter, 1)*this->wait;
  float pwm = this->p_speed;
  for(int i=0; i<int_iter; i++){
      pwm += del_pwm;
      this->setspeed(pwm,(pwm > 0)?0:1);
      delay(this->wait);
  }
  delay(extr_wait);
  this->setspeed(sp,(sp > 0)?0:1);
  this->p_speed=sp;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------
MD3_6_4_00_DD::MD3_6_4_00_DD(MD3_6_4_00_Pinconfig p, float maxspeed, float maxrate, mcpwm_unit_t unt, mcpwm_timer_t t, uint32_t f){
  mcpwm_io_signals_t tempA, tempB;
  this->pin = p;
  this->freq = f;
  this->wait = 1/float(this->freq) *1000.0; //in ms
  this->pwm_num = unt;
  this->maxspeed = maxspeed;
  this->tim = t;
  this->p_speed = 0;
  this->maxrate = maxrate;

  switch (this->tim){
    case MCPWM_TIMER_0: tempA = MCPWM0A; tempB = MCPWM0B; break;
    case MCPWM_TIMER_1: tempA = MCPWM1A; tempB = MCPWM1B; break;
    case MCPWM_TIMER_2: tempA = MCPWM2A; tempB = MCPWM2B; break;
  }
  mcpwm_gpio_init(this->pwm_num, tempA, this->pin.IN1);
  mcpwm_gpio_init(this->pwm_num, tempB, this->pin.IN2);
  mcpwm_config_t pwm_conf;
  pwm_conf.frequency = this->freq;
  pwm_conf.cmpr_a = 0;
  pwm_conf.cmpr_b = 0;
  pwm_conf.counter_mode = MCPWM_UP_COUNTER;
  pwm_conf.duty_mode = MCPWM_DUTY_MODE_0;
  mcpwm_init(this->pwm_num, this->tim, &pwm_conf);
}

void MD3_6_4_00_DD::startmotor(){
   mcpwm_start(this->pwm_num,this->tim);
   this->setspeed(this->p_speed,0);
}

void MD3_6_4_00_DD::setspeed(float speed, bool dir){
  float temp = speed/this->maxspeed;
  if(dir){
    mcpwm_set_signal_low(this->pwm_num, this->tim, MCPWM_OPR_B);
    mcpwm_set_duty(this->pwm_num, this->tim, MCPWM_OPR_A, (temp>1? 1:temp)  * 100);
    mcpwm_set_duty_type(this->pwm_num, this->tim, MCPWM_OPR_A, MCPWM_DUTY_MODE_0); 
  }
  else{
    mcpwm_set_signal_low(this->pwm_num, this->tim, MCPWM_OPR_A);
    mcpwm_set_duty(this->pwm_num, this->tim, MCPWM_OPR_B, (temp>1? 1:temp) * 100);
    mcpwm_set_duty_type(this->pwm_num, this->tim, MCPWM_OPR_B, MCPWM_DUTY_MODE_0); 
  }
}

void MD3_6_4_00_DD::stopmotor(){
   this->setspeed_lin(0);
   mcpwm_set_signal_low(this->pwm_num, this->tim, MCPWM_OPR_A);
   mcpwm_set_signal_low(this->pwm_num, this->tim, MCPWM_OPR_B);
   mcpwm_stop(this->pwm_num,this->tim);

}

void MD3_6_4_00_DD::setspeed_lin(float sp){
  float temp_speed = sp - this->p_speed;
  float iter = (abs(temp_speed)/this->maxspeed)*(this->maxrate/this->wait*1000.0);
  int int_iter = iter;
  float del_pwm = temp_speed/int_iter;
  float extr_wait = fmod(iter, 1)*this->wait;
  float pwm = this->p_speed;
  for(int i=0; i<int_iter; i++){
      pwm += del_pwm;
      this->setspeed(abs(pwm),(pwm > 0)?0:1);
      delay(this->wait);
  }
  delay(extr_wait);
  this->setspeed(abs(sp),(sp > 0)?0:1);
  this->p_speed=sp;
  Serial.println(this->p_speed);
}
