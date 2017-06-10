double get_voltage(void)
{
	static double filter = 0;
	double x;
	uint16_t adc, pot_1, pot_2
	adc   = (uint16_t)analogRead(PIN_VOLTAGE);
	pot_1 = (uint16_t)analogRead(PIN_POT_1);
	pot_2 = (uint16_t)analogRead(PIN_POT_2);
	x = map_pots(adc,
		pot_1 < pot_2 ? pot_1 : pot_2,
		pot_1 < pot_2 ? pot_2 : pot_1,
		VOLTAGE_MIN,
		VOLTAGE_NORMAL);
	if (filter < 1.0) { // first sample
		filter = x;
	}
	filter = (x * VOLTAGE_FILTER) + (filter * (1 - VOLTAGE_FILTER));

	dbf_printf("get_voltage: adc=%d, pot1=%d, pot2=%d, v=%2.2lf\n", adc, pot_1, pot_2, filter);

	return filter;
}

