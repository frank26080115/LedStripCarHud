/*
    This file is part of LedStripCarHud

    LedStripCarHud is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    LedStripCarHud is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with LedStripCarHud.  If not, see <http://www.gnu.org/licenses/>.
*/

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
				// currently this is only used for statistics
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