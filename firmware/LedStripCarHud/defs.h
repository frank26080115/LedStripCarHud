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
	ECUPID_ACCEL_PEDAL_POS_D   = 0x49, // one of three possible pedal potentiometers, uncalibrated raw value // values 37 to 201 observed from my Hyundai
	ECUPID_ACCEL_PEDAL_POS_E   = 0x4A, // one of three possible pedal potentiometers, uncalibrated raw value // lower values reflected
	ECUPID_ACCEL_PEDAL_POS_F   = 0x4B, // one of three possible pedal potentiometers, uncalibrated raw value // no data obtained from my Hyundai
	ECUPID_REL_ACCEL_PEDAL_POS = 0x5A, // this one takes into account offset voltages // no data obtained from my Hyundai
	ECUPID_PEDAL = ECUPID_ACCEL_PEDAL_POS_D, // POS_D works, tested on my Hyundai
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
#define DRAWRGB_ORANGE(x, d) (x), ((x) + ((d) / 2)) / (d), 0
#define DRAWRGB_PURPLE(x) (x), 0, (x)
#define DRAWRGB_BLACK()   0, 0, 0

enum
{
	SHOWDIAL_NONE,
	SHOWDIAL_VOLTAGE,
	SHOWDIAL_RPM,
	SHOWDIAL_SPEED,
	SHOWDIAL_MAYBE_SPEED,
};

//#define dbg_printf Serial.printf // enable
#define dbg_printf(...) // disable

#endif