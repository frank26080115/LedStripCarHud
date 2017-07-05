/*
    This file is part of LedStripCarHud

    LedStripCarHud is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    LedStripCarHud is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with LedStripCarHud.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _HW_H_
#define _HW_H_

#define PIN_VOLTAGE A7
#define IDX_VOLTAGE  7
#define PIN_SUN     A6
#define IDX_SUN      6
#define PIN_POT_1   A0
#define IDX_POT_1    0
#define PIN_POT_2   A1
#define IDX_POT_2    1
#define PIN_POT_3   A2
#define IDX_POT_3    2
#define PIN_POT_4   A3
#define IDX_POT_4    3
#define PIN_POT_5   A4
#define IDX_POT_5    4
#define PIN_POT_6   A5
#define IDX_POT_6    5

#define PIN_HEARTBEAT 6

#define PIN_CAN_TX 3
#define PIN_CAN_RX 4

#define PIN_LEDSTRIP_SCK  13
#define PIN_LEDSTRIP_DOUT 11

#define CANn Can0

#define LED_STRIP_SIZE_VIRTUAL 144

#define LEDSTRIP_POWER_LIMIT (4 * 0xFF * 3)

#define CPU_RESET() (*((uint32_t *)0xE000ED0C) = 0x5FA0004) // doesn't seem to work right

#endif