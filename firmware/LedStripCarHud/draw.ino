void draw_tachometer(int rpm)
{
	int32_t howmuchred;
	int32_t howmanyred, firstredidx;
	int32_t rpmidx;
	#ifdef FADING_HEAD
	int32_t headbright, headbright2;
	#endif
	int i;

	howmuchred = rpm - RPM_IDLE;
	if (howmuchred < 0) {
		howmuchred = 0;
	}
	howmuchred *= TICK_BRIGHTNESS;
	howmuchred += (RPM_DISPLAY_THRESH - RPM_IDLE) / 2; // round
	howmuchred /= (RPM_DISPLAY_THRESH - RPM_IDLE);
	if (howmuchred > TICK_BRIGHTNESS) {
		howmuchred = TICK_BRIGHTNESS;
	}

	howmanyred =  ((RPM_MAX - RPM_REDLINE) * LED_STRIP_SIZE);
	howmanyred += (RPM_MAX - RPM_IDLE) / 2; // round
	howmanyred /= (RPM_MAX - RPM_IDLE);
	firstredidx = LED_STRIP_SIZE - 1 - howmanyred;

	rpmidx  = (rpm * LED_STRIP_SIZE);
	#ifndef FADING_HEAD
	rpmidx += (RPM_MAX) / 2; // round
	#endif
	rpmidx /= (RPM_MAX);
	#ifdef FADING_HEAD
	headbright = RPM_MAX;
	headbright += LED_STRIP_SIZE / 2;
	headbright /= LED_STRIP_SIZE;
	headbright2 = rpm % headbright;
	headbright2 *= BAR_BRIGHTNESS;
	headbright2 += headbright / 2;
	headbright2 /= headbright;
	headbright = headbright2;
	if (headbright > BAR_BRIGHTNESS) {
		headbright = BAR_BRIGHTNESS;
	}
	#endif

	for (i = 0; i < LED_STRIP_SIZE; i++)
	{
		strip_setColourRGB(i, DRAWRGB_BLACK());
		if (i <= rpmidx)
		{
			if (i < firstredidx)
			{
				#ifdef FADING_HEAD
				if (i == rpmidx)
				{
					strip_setColourRGB(i, DRAWRGB_GREEN(headbright));
				}
				else if (i == (rpmidx - 5))
				{
					strip_setColourRGB(i, DRAWRGB_GREEN(BAR_BRIGHTNESS - headbright));
				}
				else
				#endif
				{
					if (i >= (rpmidx - 5))
					{
						strip_setColourRGB(i, DRAWRGB_GREEN(BAR_BRIGHTNESS));
					}
				}
			}
			else
			{
				#ifdef FADING_HEAD
				if (i == rpmidx)
				{
					strip_setColourRGB(i, DRAWRGB_PURPLE(headbright));
				}
				else if (i == (rpmidx - 5))
				{
					strip_setColourRGB(i, DRAWRGB_PURPLE(BAR_BRIGHTNESS - headbright));
				}
				else
				#endif
				{
					strip_setColourRGB(i, DRAWRGB_PURPLE(BAR_BRIGHTNESS));
				}
			}
		}
		else if (i >= firstredidx)
		{
			strip_setColourRGB(i, DRAWRGB_RED(howmuchred));
		}
		}
	if (rpmidx >= firstredidx) {
		strip_setColourRGB(0, DRAWRGB_RED(howmuchred));
		strip_setColourRGB(1, DRAWRGB_RED(howmuchred));
	}
	else {
		strip_setColourRGB(0, DRAWRGB_BLUE(howmuchred));
		strip_setColourRGB(1, DRAWRGB_BLUE(howmuchred));
	}
}

void draw_voltage(float voltage, uint8_t tick_brightness, uint8_t bar_brightness)
{
	int i;
	int mididx;
	int vidx;
	float vidxf;
	bool isunder = voltage < VOLTAGE_NORMAL;
	#ifdef FADING_HEAD
	float headbright;
	int32_t headbrighti;
	#endif

	if (isunder == false) {
		tick_brightness /= 3;
		bar_brightness /= 3;
	}

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
	vidx = (int)floor(vidxf);
	#else
	vidx = (int)lroundf(vidxf);
	#endif

	#ifdef FADING_HEAD
	headbright  = ((voltage - VOLTAGE_MIN) * mididx);
	headbright  = fmodf(headbright, (int)VOLTAGE_MAX - (int)VOLTAGE_MIN);
	headbright *= bar_brightness;
	headbright /= ((int)VOLTAGE_MAX - (int)VOLTAGE_MIN);
	headbrighti = (int)lroundf(headbright);
	#endif

	for (i = 0; i < LED_STRIP_SIZE; i++)
	{
		if (i == 0 && vidx == 0)
		{
			strip_setColourRGB(i, DRAWRGB_PURPLE(bar_brightness));
		}
		else if (i == mididx)
		{
			strip_setColourRGB(i, DRAWRGB_GREEN(tick_brightness));
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

void draw_speedometer(double speed, bool highspeed, uint8_t tick_brightness, uint8_t bar_brightness)
{
	int8_t tickspace;
	int i;
	double spd = speed;
	int32_t baridx;
	#ifdef FADING_HEAD
	float headbright;
	int32_t headbrighti;
	#endif

	tickspace = (highspeed == false) ? SPEED_TICKSPACING_SLOW : SPEED_TICKSPACING_FAST;

	spd *= LED_STRIP_SIZE;
	spd /= (highspeed == false) ? 80 : 120;
	#ifndef FADING_HEAD
	baridx = (int32_t)lround(spd);
	#else
	baridx = (int32_t)floor(spd);
	#endif

	#ifdef FADING_HEAD
	headbright  = (speed * LED_STRIP_SIZE);
	headbright  = fmodf(headbright, (highspeed == false) ? 80 : 120);
	headbright *= bar_brightness;
	headbright /= (highspeed == false) ? 80 : 120;
	headbrighti = (int)lroundf(headbright);
	#endif

	for (i = 0; i < LED_STRIP_SIZE; i++)
	{
		strip_setColourRGB(i, DRAWRGB_BLACK());
		if (i == 0 && speed < 0.5)
		{
			strip_setColourRGB(i, DRAWRGB_BLACK()); // this will be overwritten by the first tick
		}
		else if (i <= baridx && i >= (baridx - 3))
		{
			#ifdef FADING_HEAD
			if (i == baridx)
			{
				strip_setColourRGB(i, DRAWRGB_ORANGE(headbrighti));
			}
			else if (i == (baridx - 3))
				{
				strip_setColourRGB(i, DRAWRGB_ORANGE(bar_brightness - headbrighti));
				}
				else
			#endif
				{
				strip_setColourRGB(i, DRAWRGB_ORANGE(bar_brightness));
				}
			}
			else
			{
			strip_setColourRGB(i, DRAWRGB_BLACK());
		}
		if ((i % tickspace) == 0)
				{
			uint8_t tb = tick_brightness;
			if (highspeed != false && (i % (tickspace * 2)) != 0) {
				tb /= BRIGHTNESS_MIN * 2;
				}
			if (baridx != i)
				{
				if (i < baridx)
				{
					if (highspeed == false)
					{
						if (i == 0) {
							strip_setColourRGB(i, DRAWRGB_BLUE(tb));
				}
						else if ((i / tickspace) <= 4) {
							strip_setColourRGB(i, DRAWRGB_RED(tb));
			}
						else {
							strip_setColourRGB(i, DRAWRGB_PURPLE(tb));
		}
					}
					else if (highspeed != false)
					{
						if (i == 0) {
							strip_setColourRGB(i, DRAWRGB_BLUE(tb));
						}
						else if ((i / tickspace) <= 6) {
							strip_setColourRGB(i, DRAWRGB_RED(tb));
						}
						else {
							strip_setColourRGB(i, DRAWRGB_PURPLE(tb));
						}
					}
				}
				else {
					strip_setColourRGB(i, DRAWRGB_BLUE(tb));
				}
			}
		}
	}
}

void draw_voltage_fadein(float voltage, uint8_t tick_brightness, uint8_t bar_brightness)
{
	int16_t i, j;
	int mididx;
	float fakev;
	int vidx;
	float vidxf;
	bool isunder = voltage < VOLTAGE_NORMAL;
	uint8_t tick_brightness2 = tick_brightness;
	uint8_t bar_brightness2 = bar_brightness;

	// dim for power saving
	if (isunder == false) {
		tick_brightness2 /= 3;
		bar_brightness2 /= 3;
	}

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

	strip_blank();
	strip_show();
	if (random() % 2 == 0)
		{
		for (i = 0; i <= mididx; i++)
			{
			strip_blank();
			strip_setColourRGB(i, DRAWRGB_GREEN(tick_brightness2));
			strip_show();
			delay_ms(800 / 2 / LED_STRIP_SIZE);
		}
			}
			else
			{
		for (i = 0; i <= tick_brightness2; i++)
		{
			strip_setColourRGB(mididx, DRAWRGB_GREEN(i));
			strip_show();
			delay_ms(800 / tick_brightness2);
		}
			}

	for (fakev = VOLTAGE_MIN; fakev <= voltage; fakev += 0.01)
	{
		uint8_t tb = tick_brightness, bb = bar_brightness;
		if (fakev < VOLTAGE_NORMAL) {
			tb = tick_brightness2;
			tb = bar_brightness2;
		}
		draw_voltage(fakev, tb, bb);
		strip_setColourRGB(mididx, DRAWRGB_GREEN(tick_brightness2));
		strip_setColourRGB(0, DRAWRGB_BLUE(bar_brightness2));
		strip_show();
		delay_ms(5);
	}
}

void draw_tachometer_fadein(int rpm)
{
	int32_t howmuchred;
	int32_t howmanyred, firstredidx;
	int32_t rpmidx;
	int i, j;

	howmuchred = rpm - RPM_IDLE;
	if (howmuchred < 0) {
		howmuchred = 0;
		}
	howmuchred *= 0xFF;
	howmuchred += (RPM_DISPLAY_THRESH - RPM_IDLE) / 2; // round
	howmuchred /= (RPM_DISPLAY_THRESH - RPM_IDLE);
	if (howmuchred > 0xFF) {
		howmuchred = 0xFF;
	}

	howmanyred =  ((RPM_MAX - RPM_REDLINE) * LED_STRIP_SIZE);
	howmanyred += (RPM_MAX - RPM_IDLE) / 2; // round
	howmanyred /= (RPM_MAX - RPM_IDLE);
	firstredidx = LED_STRIP_SIZE - 1 - howmanyred;

	strip_blank();

	i = random() % 3;
	if (i == 0)
	{
		for (i = 0; i <= howmuchred; i++)
	{
			for (j = firstredidx; j < LED_STRIP_SIZE; j++)
		{
				strip_setColourRGB(j, DRAWRGB_RED(i));
		}
			strip_show();
			delay_ms(500 / howmuchred);
		}
	}
	else if (i == 1)
		{
		for (i = LED_STRIP_SIZE - 1; i >= firstredidx; i--)
		{
			strip_setColourRGB(i, DRAWRGB_RED(howmuchred));
			strip_show();
			delay_ms(500 / howmanyred);
		}
		}
	else if (i == 2)
	{
		for (i = 0; i < LED_STRIP_SIZE; i++)
		{
			strip_blank();
			for (j = i - howmanyred; j <= i; j++)
			{
				if (j >= 0)
				{
					strip_setColourRGB(j, DRAWRGB_RED(howmuchred));
				}
			}
			strip_show();
			delay_ms(500 / LED_STRIP_SIZE);
		}
	}

	for (i = 0; i < rpm; i+= 10)
	{
		draw_tachometer(i);
		rpmidx  = (i * LED_STRIP_SIZE);
		rpmidx += (RPM_MAX) / 2; // round
		rpmidx /= (RPM_MAX);
		for (j = LED_STRIP_SIZE - 1; j >= firstredidx && j >= rpmidx; j--) {
			strip_setColourRGB(j, DRAWRGB_RED(howmuchred));
		}
		strip_show();
		delay_ms(500 / (rpm / 10));
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
		strip_setColourRGB(i, DRAWRGB_WHITE(0xFF));
		didsomething = 1;
	}
	for (i = mididx2 + step, j = 0; i < LED_STRIP_SIZE && j < s; i++, j++)
	{
		strip_setColourRGB(i, DRAWRGB_WHITE(0xFF));
		didsomething = 1;
	}
	return didsomething;
}

void draw_voltageWarning(uint32_t time, int mph)
{
	uint8_t b;
	int i, j;

	// setup a blink every half second
	time /= 500;
	time %= 2;
	b = time == 0 ? 255 : 0;

	for (i = 0; i < WARNING_SIZE; i++)
	{
		j = (LED_STRIP_SIZE - 1);
		strip_setColourRGB(j, DRAWRGB_RED(b));
	}
}

void draw_speed_fadein(int8_t r)
{
	uint32_t x, b;
	if (r < 0) {
		x = random() % 7;
	}
	else {
		x = r;
	}

	b = TICK_BRIGHTNESS;
	b /= 3;
	b *= 2;

	switch (x)
	{
		case 0:
			draw_speed_fadein_train(b);
			break;
		case 1:
			draw_speed_fadein_2train(b);
			break;
		case 2:
			draw_speed_fadein_bright(b);
			break;
		case 3:
			draw_speed_fadein_brighteach(b);
			break;
		case 4:
			draw_speed_fadein_brighteach2(b);
			break;
		case 5:
			draw_speed_fadein_brighteach3(b);
			break;
		case 6:
			draw_speed_fadein_random(b);
			break;
	}
}

void draw_speed_fadein_train(uint8_t b)
{
	int16_t i, j;
	for (i = 0; i < LED_STRIP_SIZE; i++)
	{
		strip_blank();
		for (j = i; j >= 0; j -= SPEED_TICKSPACING_SLOW)
		{
			strip_setColourRGB(j, DRAWRGB_BLUE(b));
		}
		strip_show();
		delay_ms(FADEIN_TIME / LED_STRIP_SIZE);
	}
}

void draw_speed_fadein_2train(uint8_t b)
{
	int16_t i, j;
	for (i = 0; i <= (4 * SPEED_TICKSPACING_SLOW); i++)
	{
		strip_blank();
		for (j = i; j >= 0; j -= SPEED_TICKSPACING_SLOW)
		{
			strip_setColourRGB(j, DRAWRGB_BLUE(b));
		}
		for (j = LED_STRIP_SIZE - 1 - i; j < LED_STRIP_SIZE; j+= SPEED_TICKSPACING_SLOW)
		{
			strip_setColourRGB(j, DRAWRGB_BLUE(b));
		}
		strip_show();
		delay_ms(FADEIN_TIME / (LED_STRIP_SIZE / 2));
	}
}

void draw_speed_fadein_bright(uint8_t b)
{
	int16_t i, j;
	strip_blank();
	strip_show();
	for (i = 0; i < b; i++)
	{
		for (j = 0; j < LED_STRIP_SIZE; j += SPEED_TICKSPACING_SLOW)
		{
			strip_setColourRGB(j, DRAWRGB_BLUE(i));
		}
		strip_show();
		delay_ms(FADEIN_TIME / b);
	}
}

void draw_speed_fadein_brighteach(uint8_t b)
{
	int16_t i, j;
	strip_blank();
	strip_show();
	for (i = 0; i < LED_STRIP_SIZE; i += SPEED_TICKSPACING_SLOW)
	{
		for (j = 0; j < b; j++)
		{
			strip_setColourRGB(i, DRAWRGB_BLUE(j));
			strip_show();
			delay_ms((FADEIN_TIME / b) / 8);
		}
	}
}

void draw_speed_fadein_brighteach2(uint8_t b)
{
	int16_t i, j;
	strip_blank();
	strip_show();
	for (i = 0; i <= LED_STRIP_SIZE / 2; i += SPEED_TICKSPACING_SLOW)
	{
		for (j = 0; j < b; j++)
		{
			strip_setColourRGB((LED_STRIP_SIZE / 2) + i, DRAWRGB_BLUE(j));
			strip_setColourRGB((LED_STRIP_SIZE / 2) - i, DRAWRGB_BLUE(j));
			strip_show();
			delay_ms((FADEIN_TIME / b) / 4);
		}
	}
}

void draw_speed_fadein_brighteach3(uint8_t b)
{
	int16_t i, j;
	strip_blank();
	strip_show();
	for (i = 0; i <= LED_STRIP_SIZE / 2; i += SPEED_TICKSPACING_SLOW)
	{
		for (j = 0; j < b; j++)
		{
			strip_setColourRGB(i, DRAWRGB_BLUE(j));
			strip_setColourRGB(LED_STRIP_SIZE - 1 - i, DRAWRGB_BLUE(j));
			strip_show();
			delay_ms((FADEIN_TIME / b) / 4);
		}
	}
}

void draw_speed_fadein_random(uint8_t b)
{
	bool alllit = false;
	bool found;
	uint32_t starttime = millis();
	while (alllit == false)
	{
		int16_t i;
		uint32_t rnd, spd, minspd, spdspread, chance;

		if (ecu_kmh == 0 && (millis() - starttime) < (FADEIN_TIME + 500)) {
			minspd = 1;
			spdspread = 3;
			chance = 3;
		}
		else {
			minspd = 5;
			spdspread = 10;
			chance = 2;
		}

		spd = (random() % spdspread) + minspd;

		do
		{
			rnd = random() % chance;
			if (rnd == 0)
			{
				rnd = (random() % 9) * SPEED_TICKSPACING_SLOW;
			}
			else
			{
				rnd = random() % LED_STRIP_SIZE;
			}
			if (strip_getColourAny(rnd) > 0) {
				found = false;
			}
			else {
				found = true;
			}
		}
		while (found == false);

		strip_setColourRGB(rnd, DRAWRGB_BLUE(b));
		strip_show();
		if ((rnd % SPEED_TICKSPACING_SLOW) != 0)
		{
			for (i = b; ; i -= spd)
			{
				int16_t j;
				j = i < 0 ? 0 : i;
				strip_setColourRGB(rnd, DRAWRGB_BLUE(j));
				strip_show();
				if (j <= 0) {
					break;
				}
			}
		}

		alllit = true;
		for (i = 0; i < LED_STRIP_SIZE && alllit != false; i += SPEED_TICKSPACING_SLOW)
		{
			if (strip_getColourB(i) <= 0) {
				alllit = false;
			}
		}
	}
}

void draw_speed_warning(void)
{
	uint8_t i;
	for (i = 0; i < 3; i++)
	{
		strip_show();
		delay_ms(150);
		strip_fill(DRAWRGB_RED(0xFF));
		strip_show();
		delay_ms(150);
		strip_blank();
	}
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
		// check if there is any LEDs still lit
		for (i = 0; i < LED_STRIP_SIZE && has == false; i++) {
			if (strip_getColourAny(i) > 0) {
				has = true;
			}
		}
		if (has != false)
		{
			while (found == false)
			{
				uint32_t rnd = random() % LED_STRIP_SIZE;
				if (strip_getColourAny(rnd) > 0) { // don't waste time on blanks
					found = true;
					did = true;
					strip_setColourRGB(rnd, DRAWRGB_BLACK());
					strip_show();

					// animate faster as time passes
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