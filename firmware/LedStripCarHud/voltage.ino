/*
Voltage     ADC (10-bit)
3           215
4           280.5
5           335.8
6           378
7           409
8           433.2
9           452.9
10          468.5
11          482.2
12          494.1
*/

double get_voltage(void)
{
	static double filter = 0;
	double x;
	uint16_t adc, pot_1, pot_2;
	adc   = (uint16_t)analog_read(IDX_VOLTAGE);
	pot_1 = (uint16_t)analog_read(IDX_POT_1);
	pot_2 = (uint16_t)analog_read(IDX_POT_2);
	#ifdef VOLTAGE_USE_POTS
	x = map_pots(adc,
		pot_1 < pot_2 ? pot_1 : pot_2,
		pot_1 < pot_2 ? pot_2 : pot_1,
		VOLTAGE_MIN,
		VOLTAGE_NORMAL);
	#else
	{
		x = map_pots(adc,
		469,
		494,
		10,
		12);
	}
	#endif
	if (filter < 1.0) { // first sample
		filter = x;
	}
	filter = (x * VOLTAGE_FILTER) + (filter * (1 - VOLTAGE_FILTER));

	//dbg_printf("get_voltage: adc=%d, pot1=%d, pot2=%d, v=%2.2lf\n", adc, pot_1, pot_2, filter);

	return filter;
}

