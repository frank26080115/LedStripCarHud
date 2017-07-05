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

#include <Snooze.h>

SnoozeTimer slptimer;
SnoozeBlock slpconfig(slptimer);

void sleep(void)
{
	// wipe_out(-1);
	while (1)
	{
		// prevents glitchy stuck strip
		strip_blank();
		strip_show();

		digitalWrite(PIN_HEARTBEAT, LOW);

		slptimer.setTimer(WAKEUP_INTERVAL);
		CANn.end(); // just in case this peripheral doesn't wake up properly
		Snooze.hibernate(slpconfig);

		// wake up and briefly check if the ECU is on
		canbus_init();
		digitalWrite(PIN_HEARTBEAT, HIGH);
		if (canbus_simpleCheck()) {
				break;
		}
	}
}