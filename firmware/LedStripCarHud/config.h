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

#ifndef _CONFIG_H_
#define _CONFIG_H_

#define LED_STRIP_SIZE 49

#define CANBUS_DEFAULT_BAUD 500000

#define FADING_HEAD

#define RPM_MAX                8000
#define RPM_REDLINE            6500
#define RPM_IDLE               500
#define RPM_ENGINE_ON          50
#define RPM_DISPLAY_THRESH     800
#define PEDAL_REQUIRED         5
#define TACHOMETER_NEEDLE_SIZE 5

#define SPEED_TICK_SPACING   6
#define SPEED_BACKFADE       8
#define SPEED_NEEDLE_SIZE    2
#define SPEED_MAX            120

#define VOLTAGE_MIN    10.0
#define VOLTAGE_NORMAL 12.0
#define VOLTAGE_MAX    14.0
#define VOLTAGE_WARN   11.0
#define VOLTAGE_FILTER 0.02

#define BRIGHTNESS_FILTER_CONST 0.02
#define BRIGHTNESS_FILTER_SHIFT 7
#define BRIGHTNESS_MIN 16
#define BRIGHTNESS_MAX 255

#define ANALOG_FILTER_CONST 0.02

#define TICK_BRIGHTNESS 0xFF
#define BAR_BRIGHTNESS 0xFF
#define VOLTMETER_BRIGHTNESS_MAX 255
#define VOLTMETER_FADE_DELAY 5
//#define VOLTAGE_USE_POTS
#define VOLTAGE_OFF_THRESH 6

#define INTRO_SIZE 5
#define WARNING_SIZE 3
#define WIPEOUT_TIME 1000
#define FADEIN_TIME 1000

#define VOLTMETER_TIME (60 * 5)
#define CANBUS_TX_INTERVAL 5
#define CANBUS_RX_TIMEOUT 300
#define CANBUS_TX_TIMEOUT 0
#define CANBUS_TX_BOXES 9
#define CANBUS_FAILURE_TOLERANCE 9
#define CANBUS_ECU_TIMEOUT 1000

#define WAKEUP_INTERVAL 5000

#endif
