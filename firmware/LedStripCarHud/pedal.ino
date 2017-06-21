uint8_t pedal_min = 0;
uint8_t pedal_max = 175; // obtained for my Hyundai through experimenting
bool pedal_has = false;

/*
perhaps in the future, the gas pedal can be used as a user input
double tap, triple tap, etc
*/

void pedal_track(uint8_t pedal, uint16_t rpm)
{
	static uint8_t prev = 0;
	static uint8_t consec = 0;
	uint8_t x = pedal;

	if (x == 0) {
		return; // do nothing for 0, it's an invalid reading, at least on my car
	}

	if (x == prev)
	{
		if (consec < 100)
		{
			consec++;
		}
		else
		{
			if (pedal_min == 0 || x < pedal_min)
			{
				pedal_min = x;
				pedal_has = true;
			}
			if (pedal_max == 0 || x > pedal_max)
			{
				pedal_max = x;
			}
		}
	}
	else
	{
		consec = 0;
	}
	prev = x;
}

uint8_t get_pedal(void)
{
	int x = ecu_pedal - pedal_min;
	if (pedal_has == false) { // only provide results after we have enough samples to give a calibrated result
		return 0;
	}

	if (x < 0) {
		x = 0;
	}
	return x;
}