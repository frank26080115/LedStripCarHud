#include <Snooze.h>

SnoozeTimer slptimer;
SnoozeBlock slpconfig(slptimer);

void sleep(void)
{
	int rpm;
	while (1)
	{
		digitalWrite(PIN_HEARTBEAT, LOW);
		slptimer.setTimer(WAKEUP_INTERVAL);
		Snooze.hibernate(slpconfig);

		digitalWrite(PIN_HEARTBEAT, HIGH);
		if (canbus_readAll(NULL, &rpm, NULL)) {
			if (rpm > RPM_ENGINE_ON) {
				break;
			}
		}
	}
}