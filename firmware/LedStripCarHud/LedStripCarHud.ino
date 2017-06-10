
void setup()
{
}

void loop()
{
	static int32_t frm_time = 0, voltmeter_time = 0;
	static int32_t canbus_time = 0;
	static char prev_car_on = FALSE, prev_ecu_on = FALSE;
	int16_t brightness;
	char car_is_on;
	static char ecu_is_on = FALSE;
	int32_t now;
	char tick = FALSE, draw = FALSE;
	char canbus_good;
	int16_t voltmeter_brightness = 0;
	static signed int kmh, rpm, pedal, gear;
	static double mph;
	double voltage;

	// gather all data

	canbus_good = read_canbus(&kmh, &rpm, &pedal);
	mph = calc_mph(kmh, rpm, &gear);
	brightness = get_brightness();
	voltage = get_voltage();

	now = millis();
	if ((now - frm_time) >= 10) {
		tick = TRUE;
		frm_time = now;
	}

	if (((now / 500) % 2) == 0) {
		debug_data(canbus_good, kmh, mph, pedal, gear, brightness, voltage);
	}

	if (canbus_good != FALSE)
	{
		ecu_is_on = TRUE;
	}

	if (rpm > RPM_ENGINE_ON && canbus_good != FALSE)
	{
		car_is_on = TRUE;
		canbus_time = now;
	}
	else if (canbus_good == FALSE)
	{
		if ((now - canbus_time) > CANBUS_TIMEOUT)
		{
			car_is_on = FALSE;
			ecu_is_on = FALSE;
		}
	}

	if (
		(prev_car_on == FALSE && car_is_on != FALSE) ||
		(prev_ecu_on == FALSE && ecu_is_on != FALSE)
	)
	{
		bool animation_active;
		int animation_steps = 0;
		strip_setBrightness(brightness);
		do
		{
			animation_active = draw_intro(animation_steps, INTRO_SIZE, &leds);
			strip_update(&leds);
			delay(1000 / 50);
		}
		while (animation_active != FALSE);
	}
	if (
		(car_is_on == FALSE && prev_car_on != FALSE) ||
		(ecu_is_on == FALSE && prev_ecu_on != FALSE)
	{
		voltmeter_time = now;
		if (car_is_on == FALSE && prev_car_on != FALSE) {
			voltmeter_brightness = 0;
		}
		if (ecu_is_on == FALSE && prev_ecu_on != FALSE)
		{
			wipe_out();
			sleep();
		}
	}

	prev_car_on = car_is_on;
	prev_ecu_on = ecu_is_on;

	if (voltmeter_brightness != 0 && VOLTMETER_TIME != 0 && (now - voltmeter_time) >= VOLTMETER_TIME)
	{
		wipe_out();
	}

	if (mph < SPEED_DISPLAY_THRESH && pedal < PEDAL_REQUIRED && rpm < RPM_DISPLAY_THRESH &&
			(
				voltmeter_time == 0 ||
				(now - voltmeter_time) < VOLTMETER_TIME
				|| VOLTMETER_TIME == 0
			) &&
			(
				car_is_on == FALSE ||
				ecu_is_on == FALSE
			)
		)
	{
		static uint32_t ticks = 0;
		int16_t tmp_bright;
		draw_voltage(voltage, &leds, TICK_BRIGHTNESS, BAR_BRIGHTNESS);
		draw = TRUE;
		if(voltmeter_brightness == 0) {
			voltmeter_time = now;
		}
		tmp_bright = voltmeter_brightness >= 0 ? voltmeter_brightness : -voltmeter_brightness;
		if (tmp_bright > brightness) {
			tmp_bright = brightness;
		}
		strip_setBrightness(tmp_bright);
		voltmeter_brightness++;
		if (voltmeter_brightness > VOLTMETER_BRIGHTNESS_MAX) {
			voltmeter_brightness = VOLTMETER_BRIGHTNESS_MAX;
		}
		if (tick != FALSE) {
			ticks++;
			if (ticks >= VOLTMETER_FADE_DELAY) {
				voltmeter_brightness++;
				ticks = 0;
			}
		}
	}
	else if (mph < SPEED_DISPLAY_THRESH && rpm >= RPM_DISPLAY_THRESH && pedal >= PEDAL_REQUIRED)
	{
		draw_tachometer(rpm, &leds);
		draw = TRUE;
		voltmeter_brightness = 0;
		strip_setBrightness(brightness);
	}
	else
	{
		draw_speedometer(mph, &leds, TICK_BRIGHTNESS, BAR_BRIGHTNESS);
		draw = TRUE;
		voltmeter_brightness = 0;

		if (voltage < VOLTAGE_WARN)
		{
			draw_voltageWarning(&leds, now, mph);
		}

		strip_setBrightness(brightness);
	}

	if (draw != FALSE)
	{
		strip_update();
	}
}