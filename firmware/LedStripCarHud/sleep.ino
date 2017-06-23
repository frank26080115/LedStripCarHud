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