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

void demo(void)
{
	int animation_active;
	int i = 0, j = 0;
	int rpm, kmh;
	float voltage;
	double mph;
	strip_setBrightness(0x60);
	wipe_out(-1);
	if (delay_ms_waitmove(3000)) return;
	while (1)
	{
		i = 0;
		do
		{
			animation_active = draw_intro(i, INTRO_SIZE); // return zero means done, otherwise returns frame delay
			strip_show();
			if (animation_active > 0) {
				delay_ms(20);
			}
			i++;
		}
		while (animation_active != false && canbus_ismoving() == false);
		strip_blank();
		strip_show();
		if (delay_ms_waitmove(2000)) return;
		voltage = 11.8;
		draw_voltage_fadein(voltage, 0xFF, 0xFF);
		if (delay_ms_waitmove(3000)) return;
		for (; voltage >= 10.5 && canbus_ismoving() == false; voltage -= 0.2)
		{
			draw_voltage(voltage, 0xFF, 0xFF);
			strip_show();
			delay_ms(50);
		}
		for (i = 0; voltage <= 12.8 && canbus_ismoving() == false; voltage += 0.1, i += 3)
		{
			draw_voltage(voltage, 0xFF, 0xFF);
			strip_show();
			delay_ms(20 + i);
		}
		if (delay_ms_waitmove(800)) return;
		wipe_out(4);
		rpm = 500;
		draw_tachometer_fadein(rpm);
		for (; rpm <= 4500 && canbus_ismoving() == false; rpm += 15)
		{
			draw_tachometer(rpm);
			strip_show();
			delay_ms(2000 / 500);
		}
		for (; rpm >= 2000 && canbus_ismoving() == false; rpm -= 8)
		{
			draw_tachometer(rpm);
			strip_show();
			delay_ms(2000 / 500);
		}
		for (; rpm <= RPM_MAX - 50 && canbus_ismoving() == false; rpm += 12)
		{
			draw_tachometer(rpm);
			strip_show();
			delay_ms(2000 / 500);
		}
		for (; rpm >= 500 && canbus_ismoving() == false; rpm -= 8)
		{
			draw_tachometer(rpm);
			strip_show();
			delay_ms(2000 / 500);
		}
		wipe_out(2);
		kmh = 0;
		mph = 0;
		draw_speed_fadein_random(0xFF);
		for (; mph <= 15 && canbus_ismoving() == false; kmh++) {
			mph = calc_mph(kmh);
			draw_speedometer(mph, 0xFF, 0xFF);
			strip_show();
			delay_ms(250);
		}
		for (; mph <= 40 && canbus_ismoving() == false; kmh++) {
			mph = calc_mph(kmh);
			draw_speedometer(mph, 0xFF, 0xFF);
			strip_show();
			delay_ms(125);
		}
		for (; mph >= 35 && canbus_ismoving() == false; kmh--) {
			mph = calc_mph(kmh);
			draw_speedometer(mph, 0xFF, 0xFF);
			strip_show();
			delay_ms(200);
		}
		for (; mph <= 38 && canbus_ismoving() == false; kmh++) {
			mph = calc_mph(kmh);
			draw_speedometer(mph, 0xFF, 0xFF);
			strip_show();
			delay_ms(200);
		}
		for (; mph >= 36 && canbus_ismoving() == false; kmh--) {
			mph = calc_mph(kmh);
			draw_speedometer(mph, 0xFF, 0xFF);
			strip_show();
			delay_ms(200);
		}
		for (; mph <= 80 && canbus_ismoving() == false; kmh++) {
			mph = calc_mph(kmh);
			draw_speedometer(mph, 0xFF, 0xFF);
			strip_show();
			delay_ms(125);
		}
		for (; mph >= 72 && canbus_ismoving() == false; kmh--) {
			mph = calc_mph(kmh);
			draw_speedometer(mph, 0xFF, 0xFF);
			strip_show();
			delay_ms(200);
		}
		for (; mph <= 100 && canbus_ismoving() == false; kmh++) {
			mph = calc_mph(kmh);
			draw_speedometer(mph, 0xFF, 0xFF);
			strip_show();
			delay_ms(125);
		}
		for (; mph >= 0 && canbus_ismoving() == false; kmh--) {
			mph = calc_mph(kmh);
			draw_speedometer(mph, 0xFF, 0xFF);
			strip_show();
			delay_ms(50);
		}
		if (delay_ms_waitmove(3000)) return;
		wipe_out(1);
		voltage = 12.2;
		draw_voltage_fadein(voltage, 0xFF, 0xFF);
		draw_voltage(voltage, 0xFF, 0xFF);
		strip_show();
		if (delay_ms_waitmove(2000)) return;
		wipe_out(4);
		if (delay_ms_waitmove(5000)) return;

		break;
	}
}

bool delay_ms_waitmove(int32_t x)
{
	while (x > 0)
	{
		if (canbus_ismoving()) {
			return true;
		}
		delay_ms(100);
		x -= 100;
	}
	return false;
}