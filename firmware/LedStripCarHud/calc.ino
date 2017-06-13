double calc_mph(int kmh, signed int rpm, signed int* gear)
{
	/*
	the problem we are trying to solve:
		the LED strip will have 100 LEDs
		the way the speedometer animation works,
		each for every 1 MPH increase in speed, two LEDs light
		this wouldn't look very smooth
		OBD-II does not report in any higher precision
	the purpose of this function is to:
		use engine RPM and known gear ratios to predict
		true speed with higher precision
	this calculation can be skipped by simply providing
	any negative value as the RPM input
	*/

	#ifdef RPM_TO_SPEED_D_TABLE
	static const double dtbl[GEARS_CNT] = RPM_TO_SPEED_D_TABLE;
	#endif
	#ifdef RPM_TO_SPEED_M_TABLE
	static const double mtbl[GEARS_CNT] = RPM_TO_SPEED_M_TABLE;
	#endif
	int i;
	double x, y;
	double mindelta = 1000;
	double bestx;
	double simple;
	int besti = -1;
	simple = (double)kmh;
	simple *= 0.621371;

	if (rpm < 0) {
		if (gear != NULL) {
			*gear = -1;
		}
		return simple;
	}

	/*
	go through each possible gear ratios
	using the known RPM, calculate the expected speed using that gear ratios
	the gear with the speed that is closest to the actual speed is the gear
	we think that we are in
	*/

	for (i = 0; i < GEARS_CNT; i++)
	{
		double delta, deltax, deltay;
		#ifdef RPM_TO_SPEED_D_TABLE
		x = (double)rpm;
		x /= dtbl[i];
		#else
		x = -1000;
		#endif
		#ifdef RPM_TO_SPEED_M_TABLE
		y = (double)rpm;
		y *= mtbl[i];
		#else
		y = -1000;
		#endif
		deltax = abs(simple - x);
		deltay = abs(simple - y);
		if (deltax < mindelta)
		{
			mindelta = deltax;
			bestx = x;
			besti = i;
		}
		if (deltay < mindelta)
		{
			mindelta = deltay;
			bestx = y;
			besti = i;
		}
	}
	if (gear != NULL) {
		*gear = besti;
	}
	y = abs(simple - bestx);

	if (besti == 0 || besti == (GEARS_CNT - 1)) // first gear or reverse
	{
		// return steppy but accurate data because torque converter may be in play
		return simple;
	}

	// calculation is good enough to avoid bad visual display
	if (y <= 0.7)
	{
		return bestx;
	}

	return simple; // calculation is unreliable due to either torque converter or other reason
}

double map_pots(uint16_t x, uint16_t lower, uint16_t higher, uint16_t range_min, uint16_t range_max)
{
	double xd;
	double span_in, span_out;

	span_in = higher - lower;
	span_out = range_max - range_min;
	xd = x - lower;
	xd *= span_out;
	xd /= span_in;
	xd += range_min;

	return xd;
}