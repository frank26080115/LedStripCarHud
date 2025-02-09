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

#include <FastLED.h>
#include <FlexCAN.h>

#include "hw.h"
#include "config.h"
#include "defs.h"

volatile unsigned int ecu_totalReplies = 0, ecu_totalRequests = 0, can_totalMessages = 0;

void setup()
{
	//tests();
	pinMode(PIN_HEARTBEAT, OUTPUT);
	pinMode(PIN_VOLTAGE, INPUT);
	pinMode(PIN_SUN, INPUT);
	pinMode(PIN_POT_1, INPUT);
	pinMode(PIN_POT_2, INPUT);
	pinMode(PIN_POT_3, INPUT);
	pinMode(PIN_POT_4, INPUT);
	pinMode(PIN_POT_5, INPUT);
	pinMode(PIN_POT_4, INPUT);
	Serial.begin(9600); // USB port will ignore baud parameter
	strip_init();
	strip_blank();
	strip_show();
	canbus_init();
	analog_task();
	strip_setBrightness(get_brightness());
	//wait_serial();
}

void loop()
{
	static int32_t canbus_time = 0;
	static int32_t step_timer = 0;
	static int32_t demo_timer = 0;
	static char prev_car_on = false, prev_ecu_on = true;
	int16_t brightness;
	char car_is_on;
	static char ecu_is_on = false;
	int32_t now;
	char canbus_good;
	static signed int kmh, rpm, pedal;
	static double mph;
	double voltage;
	static uint8_t prev_dial = SHOWDIAL_NONE;
	uint8_t dial = prev_dial;
	static bool dbg_latched = false;
	int32_t heartrate = 500;
	static int dynamic_timeout = CANBUS_ECU_TIMEOUT;

	now = millis();

	tasks(now);
	yield();

	// gather all data

	//dbg_printf("%d, %d\r\n", ecu_totalReplies, ecu_totalRequests);

	canbus_good = canbus_readAll(&kmh, &rpm, &pedal);
	mph = calc_mph(kmh);
	brightness = get_brightness();
	voltage = get_voltage();

	strip_setBrightness(brightness);

	if (canbus_good != false)
	{
		ecu_is_on = true;
		canbus_time = now;
		pedal_track(pedal, rpm);
	}
	pedal = get_pedal();
	heartrate = 500;
	if (pedal > 100) {
		heartrate = 100;
	}
	else if (pedal > 50) {
		heartrate = 250;
	}

	if (((now / heartrate) % 2) == 0) {
		if (dbg_latched == false) {
			dbg_printf("%ul, %d, %d, %3.1f, %d, %d, %2.1f, \r\n", now, rpm, kmh, mph, pedal, brightness, voltage);
		}
		dbg_latched = true;
		digitalWrite(PIN_HEARTBEAT, HIGH);
	}
	else
	{
		dbg_latched = false;
		digitalWrite(PIN_HEARTBEAT, LOW);
	}

	if (rpm >= RPM_ENGINE_ON && canbus_good != false)
	{
		car_is_on = true;
	}
	else if (rpm < RPM_ENGINE_ON && kmh == 0 && canbus_good != false)
	{
		car_is_on = false;
	}
	else if (canbus_good == false)
	{
		if ((now > canbus_time) && (now - canbus_time) > dynamic_timeout)
		{
			car_is_on = false;
			ecu_is_on = false;
		}
	}

	if (prev_ecu_on == false && ecu_is_on != false
		&& rpm == 0 && kmh == 0
	)
	{
		int animation_active;
		int animation_steps = 0;

		randomSeed(millis());

		dbg_printf("intro animation ... ");

		strip_setBrightness(brightness);
		do
		{
			animation_active = draw_intro(animation_steps, INTRO_SIZE); // return zero means done, otherwise returns frame delay
			strip_show();
			if (animation_active > 0) {
				delay_ms(20);
			}
			animation_steps++;
		}
		while (animation_active != false);
		dial = SHOWDIAL_NONE;
		strip_blank();
		strip_show();
		dbg_printf("done\r\n");
		/*
		the way this intro animation is called is different from the fade-in function calls...
		why?
		I'm just playing around with different ways of implemention of animation
		*/
	}
	else if (ecu_is_on == false && prev_ecu_on != false && canbus_good == false)
	{
		dbg_printf("ECU is not on, sleeping\r\n");
		wipe_out(-1);
		digitalWrite(PIN_HEARTBEAT, LOW);
		if (voltage > 6)
		{
			// this is to prevent a USB re-enumeration annoyance, not really a bug
			sleep();
			// restore states to seem like new
			prev_car_on = false;
			prev_ecu_on = false;
			prev_dial = SHOWDIAL_NONE;
			while (millis() > 5) {
				systick_millis_count = 0; // almost a reboot
				// I should be disabling interrupts to do this
				// but chances of failure is low
				// and finding the right function call is hard for the Teensy 3
			}
			canbus_time = millis();
			dynamic_timeout = CANBUS_ECU_TIMEOUT * 20;
		}
		else {
			delay_ms(WAKEUP_INTERVAL); // simulate sleep for the sake of LED blinks
			// no change in any states here
		}
		return;
	}

	prev_car_on = car_is_on;
	prev_ecu_on = ecu_is_on;

	if (canbus_good != false) // only make decisions based on valid data
	{
		if (dial == SHOWDIAL_MAYBE_SPEED)
		{
			if ((now - step_timer) > 2000) { // we have been revving but not moving for so long
				dial = SHOWDIAL_RPM;
				dbg_printf("VROOOOM VROOOOM\r\n");
			}
		}

		if (demo_timer != 0 && (now - demo_timer) > 5000) { // floor it for a few seconds to activate demo
			demo();
			demo_timer = 0;
			prev_dial = SHOWDIAL_NONE;
			return; // the demo function is a long blocking function, all variables are now out of date, loop again
		}

		if (kmh > 0) // is moving
		{
			dial = SHOWDIAL_SPEED;
		}
		else if (kmh <= 0 && rpm >= RPM_DISPLAY_THRESH && pedal >= PEDAL_REQUIRED)
		{
			/*
			the tachometer display is more of a demo
			but we want to avoid showing the tachometer when we are simply
			trying to accelerate from 0
			so we have a unknown state that triggers a timer
			after the timer expires, if we are still not moving, then show the tachometer
			*/
			if (prev_dial != SHOWDIAL_MAYBE_SPEED && prev_dial != SHOWDIAL_RPM) {
				dbg_printf("possible revving\r\n");
				step_timer = now;
				dial = SHOWDIAL_MAYBE_SPEED;
			}
		}
		else if (kmh <= 0 && rpm < RPM_ENGINE_ON) {
			dial = SHOWDIAL_VOLTAGE;
		}
		else if (dial == SHOWDIAL_NONE) {
			dbg_printf("odd... nothing shown\r\n");
			dial = SHOWDIAL_SPEED;
		}

		if (rpm <= 0 && kmh <= 0 && pedal > 100)
		{
			// activate demo by flooring it while the engine is off
			if (demo_timer == 0)
			{
				demo_timer = now;
			}
		}
		else
		{
			demo_timer = 0;
		}

		if (kmh > 0 || rpm > 0) {
			dynamic_timeout = CANBUS_ECU_TIMEOUT;
		}
	}

	// this is a last ditch bug fix where the dial is wiped but leaves a blank strip
	if (dial == SHOWDIAL_NONE && prev_dial != dial)
	{
		dial = prev_dial;
	}

	/*
	here we determine and show various transition animations
	note that due to the UX and logic used, transitions should only happen when the car is stopped
	this means from a UX point of view, we are not as restricted on how to do them
	*/

	// do fade-out animations
	if (prev_dial != SHOWDIAL_NONE && dial != prev_dial)
	{
		if (((prev_dial == SHOWDIAL_SPEED && dial == SHOWDIAL_MAYBE_SPEED) || (prev_dial == SHOWDIAL_MAYBE_SPEED && dial == SHOWDIAL_SPEED)) == false) {
			dbg_printf("fade-out\r\n");
			wipe_out(-1);
			prev_dial = SHOWDIAL_NONE; // forces animations
		}
	}

	// do fade-in animations
	if (prev_dial != SHOWDIAL_SPEED && prev_dial != SHOWDIAL_MAYBE_SPEED && (dial == SHOWDIAL_SPEED || dial == SHOWDIAL_MAYBE_SPEED))
	{
		dbg_printf("speed fade-in\r\n");
		draw_speed_fadein(-1);
		canbus_time += 3000; // a really fast start seem to make the ECU timeout even jf it's on
	}
	else if (prev_dial != dial && dial == SHOWDIAL_VOLTAGE)
	{
		dbg_printf("voltage fade-in\r\n");
		draw_voltage_fadein(voltage, TICK_BRIGHTNESS / 4, BAR_BRIGHTNESS / 4);
	}
	else if (prev_dial != dial && dial == SHOWDIAL_RPM)
	{
		dbg_printf("RPM fade-in\r\n");
		draw_tachometer_fadein(rpm);
	}

	prev_dial = dial;

	// show the correct dial
	if (dial == SHOWDIAL_VOLTAGE)
	{
		draw_voltage(voltage, TICK_BRIGHTNESS / 4, BAR_BRIGHTNESS / 4); // voltmeter can be a bit dimmer to prevent glitchy LEDs
	}
	else if (dial == SHOWDIAL_RPM)
	{
		draw_tachometer(rpm);
	}
	else if (dial == SHOWDIAL_SPEED || dial == SHOWDIAL_MAYBE_SPEED)
	{
		draw_speedometer(mph, TICK_BRIGHTNESS, BAR_BRIGHTNESS);
	}

	if (voltage < 6) { // it says 6 here but in reality
		// the voltage divider is highly non-linear, this is pretty much "switch is off"
		// what it actually means is that USB is connected (since it's the only other power supply)
		strip_setBrightness(0); // the LED strip sucks a lot of power
		// USB shouldn't be used to power the strip
		// the circuit I built technically prevents this due to input power diodes
		dbg_printf("power warning!\r\n");
	}

	strip_show();
}