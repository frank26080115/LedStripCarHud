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

/*
circuit tested, dark reads 185, bright reads 944, disconnected reads less than 10
*/

uint8_t get_brightness(void)
{
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