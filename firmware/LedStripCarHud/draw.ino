void draw_tachometer(int rpm)
{
	int32_t howmuchred;
	int32_t howmanyred, firstredidx;
	int32_t rpmidx;
	#ifdef FADING_HEAD
	int32_t headbright, headbright2;
	#endif
	int i;
	int tail = TACHOMETER_NEEDLE_SIZE;

	/*
	"red" refers to the engine redline indicator on the tachometer
	*/

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
	if (rpm >= RPM_MAX) { // HA, like that'll ever happen
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
				else if (i == (rpmidx - tail))
				{
					strip_setColourRGB(i, DRAWRGB_GREEN(BAR_BRIGHTNESS - headbright));
				}
				else
				#endif
				{
					if (i >= (rpmidx - tail))
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
				else if (i == (rpmidx - tail))
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

void draw_speedometer(double speed, uint8_t tick_brightness, uint8_t bar_brightness)
{
	int8_t tickspace;
	int i;
	double spd = speed;
	int32_t baridx;
	#ifdef FADING_HEAD
	float headbright;
	int32_t headbrighti;
	#endif
	int trailsize;
	int tickspeed = 10;
	int spdmax1, spdmax2;
	int addbar;

	spdmax1 = LED_STRIP_SIZE / SPEED_TICK_SPACING;
	spdmax1 *= tickspeed; // result should be 80
	spdmax2 = SPEED_MAX; // limit beyond the last tick

	tickspace = SPEED_TICK_SPACING;
	trailsize = SPEED_NEEDLE_SIZE;
	trailsize += (int)lround(spd - ((double)spdmax1)); // sorry this is a bit lazy
	if (trailsize < SPEED_NEEDLE_SIZE) {
		trailsize = SPEED_NEEDLE_SIZE;
	}

	spd *= LED_STRIP_SIZE;
	spd /= spdmax1;
	#ifndef FADING_HEAD
	baridx = (int32_t)lround(spd);
	#else
	baridx = (int32_t)floor(spd);
	#endif

	// for better UX, center of "needle" is the speed
	addbar = SPEED_NEEDLE_SIZE;
	addbar /= 2;
	baridx += addbar;

	// if baridx is negative, we don't care

	#ifdef FADING_HEAD
	if (speed < ((float)spdmax1))
	{
		headbright  = (speed * LED_STRIP_SIZE);
		headbright  = fmodf(headbright, ((float)spdmax1));
		headbright *= bar_brightness;
		headbright /= ((float)spdmax1);
		headbrighti = (int)lroundf(headbright);
	}
	else if (speed >= ((float)spdmax1))
	{
		float rmd = speed - floor(speed);
		headbright = rmd * bar_brightness;
		headbrighti = bar_brightness - (int)lroundf(headbright);
		// since the bar grows from right to left in this case,
		// the "head" (on the right) is hidden by the final tick
		// what we actually care about is the dimming of the "tail" (on the left)
	}
	#endif

	if (baridx >= (LED_STRIP_SIZE - 1)) {
		baridx = (LED_STRIP_SIZE - 1);
	}

	for (i = 0; i < LED_STRIP_SIZE; i++)
	{
		if (i == 0 && speed < 0.5)
		{
			strip_setColourRGB(i, DRAWRGB_BLACK()); // this will be overwritten by the first tick
		}
		else if (i <= baridx && i >= (baridx - trailsize)) // is part of the needle
		{
			#ifdef FADING_HEAD
			if (i == baridx)
			{
				strip_setColourRGB(i, DRAWRGB_ORANGE(headbrighti, 4));
			}
			else if (i == (baridx - trailsize))
			{
				strip_setColourRGB(i, DRAWRGB_ORANGE(bar_brightness - headbrighti, 4));
			}
			else
			#endif
			{
				strip_setColourRGB(i, DRAWRGB_ORANGE(bar_brightness, 4));
			}
		}
		else
		{
			strip_setColourRGB(i, DRAWRGB_BLACK()); // default to nothing
		}

		if ((i % tickspace) == 0) // is a tick
		{
			if (i <= baridx)
			{
				if (i == 0) {
					strip_setColourRGB(i, DRAWRGB_BLUE(tick_brightness));
				}
				else if ((i / tickspace) <= 4) {
					strip_setColourRGB(i, DRAWRGB_RED(tick_brightness));
				}
				else {
					strip_setColourRGB(i, DRAWRGB_PURPLE(tick_brightness));
				}
			}
			else {
				strip_setColourRGB(i, DRAWRGB_BLUE(tick_brightness));
			}
		}
	}

	if (strip_getColourAny(0) <= 0) {
		strip_setColourRGB(0, DRAWRGB_BLUE(tick_brightness));
	}
}

void draw_voltage_fadein(float voltage, uint8_t tick_brightness, uint8_t bar_brightness)
{
	int16_t i, j;
	int mididx;
	float fakev;
	int vidx;
	float vidxf;

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
			strip_setColourRGB(i, DRAWRGB_GREEN(tick_brightness));
			strip_setBrightness(get_brightness());
			strip_show();
			delay_ms(800 / 2 / LED_STRIP_SIZE);
		}
	}
	else
	{
		for (i = 0; i <= tick_brightness; i++)
		{
			strip_setColourRGB(mididx, DRAWRGB_GREEN(i));
			strip_setBrightness(get_brightness());
			strip_show();
			delay_ms(800 / tick_brightness);
		}
	}

	for (fakev = VOLTAGE_MIN; fakev <= voltage; fakev += 0.01)
	{
		draw_voltage(fakev, tick_brightness, bar_brightness);
		strip_setColourRGB(mididx, DRAWRGB_GREEN(tick_brightness));
		strip_setColourRGB(0, DRAWRGB_BLUE(bar_brightness));
		strip_setBrightness(get_brightness());
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
			strip_setBrightness(get_brightness());
			strip_show();
			delay_ms(500 / howmuchred);
		}
	}
	else if (i == 1)
	{
		for (i = LED_STRIP_SIZE - 1; i >= firstredidx; i--)
		{
			strip_setColourRGB(i, DRAWRGB_RED(howmuchred));
			strip_setBrightness(get_brightness());
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
			strip_setBrightness(get_brightness());
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
		strip_setBrightness(get_brightness());
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

void draw_speed_fadein(int8_t r)
{
	uint32_t x, b;
	int startbright = strip_getBrightness();
	int endbright;

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

	endbright = get_brightness();

	// do a final smooth fade to the right brightness
	// the animations with moving dots will have brightness adjusted with each movement, which is great
	// but the animations that involve increasing brightness, those I want an uninterrupted increase
	// so we adjust for brightness after the animation is done
	for (x = startbright; x != endbright; ) {
		strip_setBrightness(x);
		strip_show();
		delay_ms(10);
		endbright = get_brightness();
		if (x > endbright) {
			x--;
		}
		else if (x < endbright) {
			x++;
		}
	}
}

void draw_speed_fadein_train(uint8_t b)
{
	int16_t i, j;
	for (i = 0; i < LED_STRIP_SIZE; i++)
	{
		strip_blank();
		for (j = i; j >= 0; j -= SPEED_TICK_SPACING)
		{
			strip_setColourRGB(j, DRAWRGB_BLUE(b));
		}
		strip_setBrightness(get_brightness());
		strip_show();
		delay_ms(FADEIN_TIME / LED_STRIP_SIZE);
	}
}

void draw_speed_fadein_2train(uint8_t b)
{
	int16_t i, j;
	for (i = 0; i <= (4 * SPEED_TICK_SPACING); i++)
	{
		strip_blank();
		for (j = i; j >= 0; j -= SPEED_TICK_SPACING)
		{
			strip_setColourRGB(j, DRAWRGB_BLUE(b));
		}
		for (j = LED_STRIP_SIZE - 1 - i; j < LED_STRIP_SIZE; j+= SPEED_TICK_SPACING)
		{
			strip_setColourRGB(j, DRAWRGB_BLUE(b));
		}
		strip_setBrightness(get_brightness());
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
		for (j = 0; j < LED_STRIP_SIZE; j += SPEED_TICK_SPACING)
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
	for (i = 0; i < LED_STRIP_SIZE; i += SPEED_TICK_SPACING)
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
	for (i = 0; i <= LED_STRIP_SIZE / 2; i += SPEED_TICK_SPACING)
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
	for (i = 0; i <= LED_STRIP_SIZE / 2; i += SPEED_TICK_SPACING)
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
				rnd = (random() % 9) * SPEED_TICK_SPACING;
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
		if ((rnd % SPEED_TICK_SPACING) != 0)
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
		for (i = 0; i < LED_STRIP_SIZE && alllit != false; i += SPEED_TICK_SPACING)
		{
			if (strip_getColourB(i) <= 0) {
				alllit = false;
			}
		}
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