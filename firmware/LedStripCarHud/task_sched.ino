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