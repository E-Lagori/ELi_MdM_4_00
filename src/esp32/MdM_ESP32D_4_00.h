/* Header for MdM_ESP32D_4_00.h
 *
 * Copyright (C) 2018  E-LAGORi https://github.com/E-Lagori/ELi_MdM_4_00/tree/main/src/esp32
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
 * This header file describes the public API for SerialDebug.
 *
 */



#ifndef MdM_ESP32D_4_00_H
#define MdM_ESP32D_4_00_H

#include <ELi_McM_4_00.h>
#include "driver/mcpwm.h"
#include "soc/mcpwm_reg.h"
#include "soc/mcpwm_struct.h"

struct MD3_6_4_00_Pinconfig{
  uint8_t IN1,IN2, VREF;
};
class MD3_6_4_00_SD{
  private:
    MD3_6_4_00_Pinconfig pin;
    mcpwm_unit_t pwm_num;
    mcpwm_io_signals_t io;
    mcpwm_timer_t tim;
    mcpwm_operator_t opr;
    uint32_t freq;
    float maxspeed;
  public:
    MD3_6_4_00_SD(MD3_6_4_00_Pinconfig, float,mcpwm_unit_t, mcpwm_io_signals_t, uint32_t);
    void startmotor();
    void setspeed(float);
    void stopmotor();
};

class MD3_6_4_00_DD{
  private:
    MD3_6_4_00_Pinconfig pin;
    mcpwm_unit_t pwm_num;
    mcpwm_timer_t tim;
    uint32_t freq;
    float maxspeed;
    bool dir;
  public:
    MD3_6_4_00_DD(MD3_6_4_00_Pinconfig, float, mcpwm_unit_t, mcpwm_timer_t,uint32_t);
    void startmotor();
    void setspeed(float,bool);
    void stopmotor();
};
#endif
