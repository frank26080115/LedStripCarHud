#ifndef _DEFS_H_
#define _DEFS_H_

#include <stdint.h>

enum
{
	ECU_PID_REQUEST  = 0x07DF,
	ECU_PID_REPLY    = 0x07E8,
	// the PIDs below can be found on Wikipedia
	ECUPID_ENGINE_RPM  = 0x0C, // formula is given as ((buf[3]*256) + buf[4])/4
	ECUPID_SPEED       = 0x0D, // in KM/H units always
	ECUPID_THROTTLE    = 0x11, // may or may not be connected to the pedal directly, most new cars are drive-by-wire
	ECUPID_ACCEL_PEDAL_POS_D   = 0x49, // one of three possible pedal potentiometers, uncalibrated raw value
	ECUPID_ACCEL_PEDAL_POS_E   = 0x4A, // one of three possible pedal potentiometers, uncalibrated raw value
	ECUPID_ACCEL_PEDAL_POS_F   = 0x4B, // one of three possible pedal potentiometers, uncalibrated raw value
	ECUPID_REL_ACCEL_PEDAL_POS = 0x5A, // this one takes into account offset voltages
	ECUPID_PEDAL = ECUPID_REL_ACCEL_PEDAL_POS,
};

typedef struct
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
}
led_t;

#define DRAWRGB_WHITE(x)  (x), (x), (x)
#define DRAWRGB_RED(x)    (x), 0, 0
#define DRAWRGB_GREEN(x)  0, (x), 0
#define DRAWRGB_BLUE(x)   0, 0, (x)
#define DRAWRGB_YELLOW(x) (x), (x), 0
#define DRAWRGB_ORANGE(x) (x), ((x) + 1) / 2, 0
#define DRAWRGB_PURPLE(x) (x), 0, (x)
#define DRAWRGB_BLACK()   0, 0, 0

#define dbg_printf Serial.printf

#endif