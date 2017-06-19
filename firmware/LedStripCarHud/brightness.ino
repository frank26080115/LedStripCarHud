/*
circuit tested, dark reads 185, bright reads 944, disconnected reads less than 10
*/

uint8_t get_brightness(void)
{
	static double filter = BRIGHTNESS_MIN;
	static int32_t chase = BRIGHTNESS_MIN << BRIGHTNESS_FILTER_SHIFT;

	int32_t r, c;
	double x;
	uint16_t sun, pot_1, pot_2;
	uint16_t pot_min, pot_max;

	sun   = (uint16_t)analog_read(IDX_SUN);
	pot_1 = (uint16_t)analog_read(IDX_POT_3);
	pot_2 = (uint16_t)analog_read(IDX_POT_4);
	x = map_pots(sun,
		pot_1 < pot_2 ? pot_1 : pot_2,
		pot_1 < pot_2 ? pot_2 : pot_1,
		BRIGHTNESS_MIN,
		BRIGHTNESS_MAX);
	r = (int32_t)lround(x);
	r = r > BRIGHTNESS_MAX ? BRIGHTNESS_MAX : (r < BRIGHTNESS_MIN ? BRIGHTNESS_MIN : r);

	//dbg_printf("get_brightness: sun=%d, pot1=%d, pot2=%d, r=%d, ", sun, pot_1, pot_2, r);

	c = chase >> BRIGHTNESS_FILTER_SHIFT;
	if (r > c && c < 255) {
		chase += 1;
	}
	else if (r < c && c > 0) {
		chase -= 1;
	}
	r = chase >> BRIGHTNESS_FILTER_SHIFT;

	//dbg_printf("chase=%d, out=%d\n", chase, r);

	return (uint8_t)r;
}