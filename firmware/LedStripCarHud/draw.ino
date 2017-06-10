void draw_tachometer(int rpm, led_t* strip)
{
	int32_t howmuchred;
	int32_t howmanyred, firstredidx;
	int32_t rpmidx;
	#ifdef FADING_HEAD
	int32_t headbright;
	#endif
	int i;

	howmuchred = rpm - RPM_IDLE;
	if (howmuchred < 0) {
		howmuchred = 0;
	}
	howmuchred *= 255;
	howmuchred += (RPM_DISPLAY_THRESH - RPM_IDLE) / 2; // round
	howmuchred /= (RPM_DISPLAY_THRESH - RPM_IDLE);
	if (howmuchred > 255) {
		howmuchred = 255;
	}

	howmanyred =  ((RPM_MAX - RPM_REDLINE) * LED_STRIP_SIZE);
	howmanyred += (RPM_MAX - RPM_IDLE) / 2; // round
	howmanyred /= (RPM_MAX - RPM_IDLE);
	firstredidx = LED_STRIP_SIZE - 1 - howmanyred;

	rpm    -= RPM_IDLE;
	rpmidx  = (rpm * LED_STRIP_SIZE);
	#ifndef FADING_HEAD
	rpmidx += (RPM_MAX - RPM_IDLE) / 2; // round
	#endif
	rpmidx /= (RPM_MAX - RPM_IDLE);
	#ifdef FADING_HEAD
	headbright  = (rpm * LED_STRIP_SIZE);
	headbright %= (RPM_MAX - RPM_IDLE);
	headbright *= 255;
	headbright += (RPM_MAX - RPM_IDLE) / 2;
	headbright /= (RPM_MAX - RPM_IDLE);
	#endif

	for (i = 0; i < LED_STRIP_SIZE; i++)
	{
		if (i <= rpmidx)
		{
			if (i < firstredidx)
			{
				#ifdef FADING_HEAD
				if (i == rpmidx)
				{
					set_color_rgb(&strip[i], 0, headbright, 0);
				}
				#endif
				{
					set_color_rgb(&strip[i], 0, 255, 0);
				}
			}
			else
			{
				set_color_rgb(&strip[i], 255, 0, 255);
			}
		}
		else if (i >= firstredidx)
		{
			set_color_rgb(&strip[i], howmuchred, 0, 0);
		}
		else
		{
			set_color_rgb(&strip[i], 0, 0, 0);
		}
	}
}

void draw_voltage(float voltage, led_t* strip, uint8_t tick_brightness, uint8_t bar_brightness)
{
	int i;
	int mididx;
	int vidx;
	float vidxf;
	char isunder = voltage < VOLTAGE_NORMAL;

	mididx = LED_STRIP_SIZE;
	//mididx += 1;
	mididx /= 2;
	if ((LED_STRIP_SIZE % 2) == 0) {
		mididx += 1;
	}

	vidxf = voltage;
	vidxf -= VOLTAGE_MIN;
	vidxf *= mididx;
	vidxf /= (VOLTAGE_NORMAL - VOLTAGE_MIN);
	vidx = (int)lroundf(vidxf);

	for (i = 0; i < LED_STRIP_SIZE; i++)
	{
		if (i == 0 && vidx == 0)
		{
			set_color_rgb(&strip[i], bar_brightness, 0, bar_brightness);
		}
		else if (i == mididx)
		{
			set_color_rgb(&strip[i], 0, tick_brightness, 0);
		}
		else if (isunder != 0 && (i == (mididx - 1) || i == (mididx + 1)))
		{
			set_color_rgb(&strip[i], 0, tick_brightness / 2, 0);
		}
		else if (i <= vidx)
		{
			set_color_rgb(&strip[i], 0, 0, bar_brightness);
		}
		else
		{
			set_color_rgb(&strip[i], 0, 0, 0);
		}
	}
}

void draw_speedometer(double speed, led_t* strip, uint8_t tick_brightness, uint8_t bar_brightness)
{
	int tickidxes[SPEED_TICKS];
	int i;
	double spd = speed;
	int32_t baridx;
	int32_t backfade;
	char over50 = speed > (double)SPEED_MID;

	tickidxes[0] = 0;
	tickidxes[SPEED_TICKS - 1] = LED_STRIP_SIZE - 1;
	float ticks_step = ticksidxes[SPEED_TICKS - 1];
	ticks_step /= (float)(SPEED_TICKS);
	for (i = 1; i < (SPEED_TICKS - 1); i++)
	{
		tickidxes[i] = (int)lroundf(ticks_step * (float)i); 
	}

	spd *= 2 * LED_STRIP_SIZE;
	spd /= SPEED_MAX;
	baridx = (int32_t)lround(spd);
	if (baridx >= LED_STRIP_SIZE) {
		baridx -= LED_STRIP_SIZE;
	}

	backfade = (int)lround(speed - (double)SPEED_MID);
	if (backfade > 10)
	{
		backfade = 10;
	}
	else if (backfade < 0)
	{
		backfade = 0;
	}
	backfade *= SPEED_BACKFADE;
	if (backfade > 255) {
		backfade = 255;
	}
	backfade = bar_brightness - backfade;

	for (i = 0; i < LED_STRIP_SIZE; i++)
	{
		if (i == 0 && speed < 0.5)
		{
			set_color_rgb(&strip[i], 0, 0, 0);
		}
		else if (i <= baridx)
		{
			if (over50 == 0)
			{
				set_color_rgb(&strip[i], 0, bar_brightness, 0);
			}
			else
			{
				set_color_rgb(&strip[i], bar_brightness, 0, 0);
			}
		}
		else
		{
			if (over50 == 0)
			{
				set_color_rgb(&strip[i], 0, 0, 0);
			}
			else
			{
				set_color_rgb(&strip[i], 0, backfade, 0);
			}
		}
	}

	for (i = 0; i < SPEED_TICKS; i++)
	{
		if (baridx != tickidxes[i])
		{
			set_color_rgb(&strip[tickidxes[i]], 0, 0, tick_brightness);
		}
		else if (over50 == 0)
		{
			set_color_rgb(&strip[tickidxes[i]], tick_brightness, 0, 0);
		}
		else if (over50 != 0)
		{
			set_color_rgb(&strip[tickidxes[i]], tick_brightness, tick_brightness, 0);
		}
	}
}

char draw_intro(int step, int size, led_t* strip)
{
	char didsomething = 0;
	int i, j, s, stage;
	int mididx1, mididx2;
	mididx1 = LED_STRIP_SIZE;
	mididx1 /= 2;
	mididx1 -= 1;
	if ((LED_STRIP_SIZE % 2) == 0)
	{
		mididx2 = mididx1 + 1;
	}
	else
	{
		mididx2 = mididx1;
	}

	stage = step + 1;
	if (stage < size)
	{
		s = stage;
	}
	else
	{
		s = size;
	}

	for (i = 0; i < LED_STRIP_SIZE; i++) {
		set_color_rgb(&strip[i], 0, 0, 0);
	}
	for (i = mididx1 - step, j = 0; i >= 0 && j < s; i--, j++)
	{
		set_color_rgb(&strip[i], 255, 255, 255);
		didsomething = 1;
	}
	for (i = mididx2 + step, j = 0; i < LED_STRIP_SIZE && j < s; i++, j++)
	{
		set_color_rgb(&strip[i], 255, 255, 255);
		didsomething = 1;
	}
	return didsomething;
}

void draw_voltageWarning(led_t* strip, uint32_t time, int mph)
{
	uint8_t b;
	int i, j;

	// setup a blink every half second
	time /= 500;
	time %= 2;
	b = time == 0 ? 255 : 0;

	for (i = 0; i < WARNING_SIZE; i++)
	{
		j = mph < SPEED_MID ? i : (LED_STRIP_SIZE - 1 - i); // pick which end of the strip to blink
		set_color_rgb(&strip[j], b, 0, 0);
	}
}