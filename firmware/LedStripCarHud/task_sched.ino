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

void delay_ms(uint32_t x)
{
	uint32_t ts = millis();
	uint32_t now = ts;
	while ((now - ts) < x)
	{
		now = millis(); // millis will block interrupts for a brief moment, we do not want to call it too often
		tasks(now);
		yield();
	}
}

void tasks(uint32_t now)
{
	static uint32_t last_canbus_time = 0;

	if ((now - last_canbus_time) >= CANBUS_TX_INTERVAL) {
		last_canbus_time = now;
		canbus_writeRequests();
	}

	analog_task(); // this is a blocking task that takes a while
	// it will always yield to interrupts, that's what we want
	get_voltage();
	get_brightness();
	// calling these will cause filters to execute

	yield();
}