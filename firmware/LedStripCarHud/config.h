#ifndef _CONFIG_H_
#define _CONFIG_H_

#define LED_STRIP_SIZE 70

#define RPM_MAX 10000
#define RPM_REDLINE 8000
#define RPM_IDLE 500
#define RPM_ENGINE_ON 50
#define RPM_DISPLAY_THRESH 1500
#define PEDAL_REQUIRED 2

#define SPEED_MID               100
#define SPEED_MAX               200
#define SPEED_DANGER            220
#define SPEED_DANGER_HYSTERESIS 90
#define SPEED_DISPLAY_THRESH    2
#define SPEED_TICKS             11
#defube SPEED_BACKFADE          8

#define VOLTAGE_MIN    10.0
#define VOLTAGE_NORMAL 12.0
#define VOLTAGE_WARN   11.0
#define VOLTAGE_SCALE
#define VOLTAGE_FILTER 0.02

#define BRIGHTNESS_FILTER 0.02
#define BRIGHTNESS_MIN 32
#define BRIGHTNESS_MAX 255

#define TICK_BRIGHTNESS 200
#define BAR_BRIGHTNESS 200
#define VOLTMETER_BRIGHTNESS_MAX 255
#define VOLTMETER_FADE_DELAY 5

#define INTRO_SIZE 5
#define WARNING_SIZE 3

#define VOLTMETER_TIME (60 * 5)
#define CANBUS_TIMEOUT 2000

// data from Hyundai Santa Fe SE 2011
// http://www.hyundaiproductinformation.com/vehicles/11_vehicles/11_santaFe/santaFe_featureslist_mech.asp
// gear ratios are 4.651 2.831 1.842 1.386 1.000 0.772 3.393
// wheel is 713 rev/mile
#define GEARS_CNT 7
#define RPM_TO_SPEED_M_TABLE {0.018068, 0.029683, 0.045621, 0.06063, 0.084034, 0.108852, 0.024767}
#define RPM_TO_SPEED_D_TABLE {55.3469, 33.6889, 21.9198, 16.4934, 11.9, 9.1868, 40.3767}

#endif