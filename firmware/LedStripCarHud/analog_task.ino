#define ANALOG_CHAN_CNT 8

double analog_filtered[ANALOG_CHAN_CNT];
const uint8_t analog_chan[ANALOG_CHAN_CNT] = {
	PIN_POT_1,
	PIN_POT_2,
	PIN_POT_3,
	PIN_POT_4,
	PIN_POT_5,
	PIN_POT_6,
	PIN_SUN,
	PIN_VOLTAGE,
};

void analog_task(void)
{
	static bool hasdone = false;
	uint8_t i;
	for (i = 0; i < ANALOG_CHAN_CNT; i++) {
		uint16_t raw = analogRead(analog_chan[i]);
		yield(); // note: analogRead also calls yield
		if (hasdone != false)
		{
			double dbl = raw;
			double flt = analog_filtered[i];
			flt = flt * (1 - ANALOG_FILTER_CONST);
			flt += dbl * ANALOG_FILTER_CONST;
			analog_filtered[i] = flt;
		}
		else
		{
			analog_filtered[i] = raw;
		}
	}
	hasdone = true;
}

uint16_t analog_read(uint8_t chan)
{
	return (uint16_t)lround(analog_filtered[chan]);
}