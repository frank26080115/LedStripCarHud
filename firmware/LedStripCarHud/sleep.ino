#include <Snooze.h>

SnoozeTimer slptimer;
SnoozeBlock slpconfig(slptimer);

void sleep(void)
{
	// wipe_out(-1);
	strip_blank();
	strip_show();
	while (1)
	{
		digitalWrite(PIN_HEARTBEAT, LOW);
		slptimer.setTimer(WAKEUP_INTERVAL);
		CANn.end();
		Snooze.hibernate(slpconfig);

		canbus_init();
		digitalWrite(PIN_HEARTBEAT, HIGH);
		if (canbus_simpleCheck()) {
				break;
			}
		}
}