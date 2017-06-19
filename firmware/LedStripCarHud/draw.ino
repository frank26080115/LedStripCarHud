void draw_tachometer(int rpm)
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
					strip_setColourRGB(i, DRAWRGB_GREEN(headbright));
				}
				#endif
				{
					strip_setColourRGB(i, DRAWRGB_GREEN(255));
				}
			}
			else
			{
				strip_setColourRGB(i, DRAWRGB_PURPLE(255));
			}
		}
		else if (i >= firstredidx)
		{
			strip_setColourRGB(i, DRAWRGB_RED(howmuchred));
		}
		else
		{
			strip_setColourRGB(i, DRAWRGB_BLACK());
		}
	}
}

void draw_voltage(float voltage, uint8_t tick_brightness, uint8_t bar_brightness)
{
	int i;
	int mididx;
	int vidx;
	float vidxf;
	char isunder = voltage < VOLTAGE_NORMAL;
	#ifdef FADING_HEAD
	float headbright;
	int32_t headbrighti;
	#endif

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
	#ifdef FADING_HEAD
	vidx = (int)lfloorf(vidxf);
	#else
	vidx = (int)lroundf(vidxf);
	#endif

	#ifdef FADING_HEAD
	headbright  = ((voltage - VOLTAGE_MIN) * mididx);
	headbright %= (VOLTAGE_MAX - VOLTAGE_MIN);
	headbright *= bar_brightness;
	headbright /= (VOLTAGE_MAX - VOLTAGE_MIN);
	headbrighti = (int)lroundf(headbright);
	#endif

	for (i = 0; i < LED_STRIP_SIZE; i++)
	{
		if (i == 0 && vidx == 0)
		{
			strip_setColourRGB(i, bar_brightness, 0, bar_brightness);
		}
		else if (i == mididx)
		{
			strip_setColourRGB(i, 0, tick_brightness, 0);
		}
		else if (isunder != 0 && (i == (mididx - 1) || i == (mididx + 1)))
		{
			strip_setColourRGB(i, 0, tick_brightness / 2, 0);
		}
		else if (i <= vidx)
		{
			#ifdef FADING_HEAD
			if (i == vidx) {
				strip_setColourRGB(i, DRAWRGB_BLUE(headbrighti));
			}
			else
			#endif
			strip_setColourRGB(i, DRAWRGB_BLUE(bar_brightness));
		}
		else
		{
			strip_setColourRGB(i, DRAWRGB_BLACK());
		}
	}
}

void draw_speedometer(double speed, uint8_t tick_brightness, uint8_t bar_brightness)
{
	int tickidxes[SPEED_TICKS];
	int i;
	double spd = speed;
	int32_t baridx;
	int32_t backfade;
	char over50 = speed > (double)SPEED_MID;
	#ifdef FADING_HEAD
	float headbright;
	int32_t headbrighti;
	#endif

	tickidxes[0] = 0;
	tickidxes[SPEED_TICKS - 1] = LED_STRIP_SIZE - 1;
	float ticks_step = tickidxes[SPEED_TICKS - 1];
	ticks_step /= (float)(SPEED_TICKS);
	for (i = 1; i < (SPEED_TICKS - 1); i++)
	{
		tickidxes[i] = (int)lroundf(ticks_step * (float)i); 
	}

	spd *= LED_STRIP_SIZE;
	spd /= SPEED_MID;
	#ifndef FADING_HEAD
	baridx = (int32_t)lround(spd);
	#else
	baridx = (int32_t)lfloorf(spd);
	#endif

	#ifdef FADING_HEAD
	headbright  = (speed * LED_STRIP_SIZE);
	headbright %= (SPEED_MID);
	headbright *= bar_brightness;
	headbright /= (SPEED_MID);
	headbrighti = (int)lroundf(headbright);
	#endif

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
			strip_setColourRGB(i, 0, 0, 0);
		}
		else if (i <= baridx)
		{
			#ifdef FADING_HEAD
			if (i == baridx)
			{
				if (over50 == 0)
				{
					strip_setColourRGB(i, DRAWRGB_YELLOW(headbrighti));
				}
				else
				{
					strip_setColourRGB(i, DRAWRGB_RED(headbrighti));
				}
			}
			else
			#endif
			{
				if (over50 == 0)
				{
					strip_setColourRGB(i, DRAWRGB_YELLOW(bar_brightness));
				}
				else
				{
					strip_setColourRGB(i, DRAWRGB_RED(bar_brightness));
				}
			}
		}
		else
		{
			if (over50 == 0)
			{
				strip_setColourRGB(i, DRAWRGB_BLACK());
			}
			else
			{
				strip_setColourRGB(i, DRAWRGB_YELLOW(backfade));
			}
		}
	}

	for (i = 0; i < SPEED_TICKS; i++)
	{
		uint8_t tickidx = tickidxes[i];
		if (baridx != tickidx)
		{
			strip_setColourRGB(tickidx, DRAWRGB_BLUE(tick_brightness));
		}
		else
		{
			#ifdef FADING_HEAD
			strip_setColourRGB(tickidx, headbrighti, 0, tick_brightness);
			#else
			strip_setColourRGB(tickidx, 255, 0, tick_brightness);
			#endif
		}
	}
}

char draw_intro(int step, int size)
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
		strip_setColourRGB(i, DRAWRGB_BLACK());
	}
	for (i = mididx1 - step, j = 0; i >= 0 && j < s; i--, j++)
	{
		strip_setColourRGB(i, DRAWRGB_WHITE(255));
		didsomething = 1;
	}
	for (i = mididx2 + step, j = 0; i < LED_STRIP_SIZE && j < s; i++, j++)
	{
		strip_setColourRGB(i, DRAWRGB_WHITE(255));
		didsomething = 1;
	}
	return didsomething;
}

void wipe_out(int8_t r)
{
	uint32_t ani;
	int16_t i, j, b;
	if (r < 0) {
		ani = random() % 6; // chose animation randomly
	}
	else {
		ani = r; // selected animation
	}

	b = strip_getBrightness(); // cache for restoring later

	switch (ani)
	{
		case 0: // left to right wipe
			for (i = 0; i < LED_STRIP_SIZE; i++)
			{
				for (j = 0; j < i; j++)
				{
					strip_setColourRGB(j, DRAWRGB_BLACK());
				}
				strip_show();
				delay_ms(WIPEOUT_TIME / LED_STRIP_SIZE);
			}
			break;
		case 1: // right to left wipe
			for (i = LED_STRIP_SIZE - 1; i >= 0; i--)
			{
				for (j = LED_STRIP_SIZE - 1; j >= i; j--)
				{
					strip_setColourRGB(j, DRAWRGB_BLACK());
				}
				strip_show();
				delay_ms(WIPEOUT_TIME / LED_STRIP_SIZE);
			}
		case 2: // center to outwards wipe
			for (i = LED_STRIP_SIZE / 2, j = LED_STRIP_SIZE / 2; i >= 0 || j < LED_STRIP_SIZE; i--, j++)
			{
				if (i >= 0)
				{
					strip_setColourRGB(i, DRAWRGB_BLACK());
				}
				if (j < LED_STRIP_SIZE)
				{
					strip_setColourRGB(j, DRAWRGB_BLACK());
				}
				strip_show();
				delay_ms(WIPEOUT_TIME / LED_STRIP_SIZE / 2);
			}
			break;
		case 3: // outside to center wipe
			for (i = 0, j = LED_STRIP_SIZE - 1; i < LED_STRIP_SIZE || j >= 0; i++, j--)
			{
				if (j >= 0)
				{
					strip_setColourRGB(j, DRAWRGB_BLACK());
				}
				if (i < LED_STRIP_SIZE)
				{
					strip_setColourRGB(i, DRAWRGB_BLACK());
				}
				strip_show();
				delay_ms(WIPEOUT_TIME / LED_STRIP_SIZE / 2);
			}
			break;
		case 4:
			snow_out();
			break;
		default: // brightness dim
			for (i = b; i >= 0; i--)
			{
				strip_setBrightness(i);
				strip_show();
				delay_ms(1000 / b);
			}
			break;
	}

	strip_blank();
	strip_show();

	strip_setBrightness(b); // restore brightness setting because likely another animation follows
}

void snow_out(void)
{
	uint32_t start = millis();
	int32_t tdiff;
	bool did = true;
	while (did != false)
	{
		bool found = false;
		bool has = false;
		uint8_t i;
		for (i = 0; i < LED_STRIP_SIZE && has == false; i++) {
			if (strip_getColourR(i) > 0 || strip_getColourG(i) > 0 || strip_getColourB(i) > 0) {
				has = true;
			}
		}
		if (has != false)
		{
			while (found == false)
			{
				uint32_t rnd = random() % LED_STRIP_SIZE;
				if (strip_getColourR(rnd) > 0 || strip_getColourG(rnd) > 0 || strip_getColourB(rnd) > 0) {
					found = true;
					did = true;
					strip_setColourRGB(rnd, DRAWRGB_BLACK());
					strip_show();
					tdiff = 1200 - (millis() - start);
					if (tdiff < 0) {
						tdiff = 0;
					}
					tdiff /= LED_STRIP_SIZE;
					delay_ms(10 + tdiff);
				}
			}
		}
		else
		{
			did = false;
		}
	}
}