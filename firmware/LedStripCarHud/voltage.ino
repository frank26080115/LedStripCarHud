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
		// I manually calibrated this by hand, pots are ignored
		x = map_pots(adc,
		469,
		525,
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

