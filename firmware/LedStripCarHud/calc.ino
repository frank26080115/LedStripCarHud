double calc_mph(int kmh)
{
	double simple;
	simple = (double)kmh;
	simple *= 0.621371;
		return simple;
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
