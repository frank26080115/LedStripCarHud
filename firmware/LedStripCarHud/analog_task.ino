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