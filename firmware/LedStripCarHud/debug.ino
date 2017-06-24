void wait_serial(void)
{
	while (Serial.available() <= 0) {
		tasks(millis());
	}
	Serial.read();
}

void dbg_hex(uint8_t* data, uint8_t len)
{
	uint8_t i;
	for (i = 0; i < len; i++) {
		dbg_printf("%02X", data[i]);
	}
}

void dbg_canmsg(CAN_message_t* x)
{
	dbg_printf("CANMSG[%06X]:", x->id);
	dbg_hex(x->buf, x->len);
}

void dbg_strip(led_t* s, uint16_t len, uint8_t b)
{
	uint16_t i;
	Serial.printf("=LEDs=[");
	for (i = 0; i < len; i++)
	{
		uint8_t dbgc;
		led_t* ptr = &s[i];
		if ((ptr->r == 0 && ptr->g == 0 && ptr->b == 0) || b == 0) {
			dbgc = '-';
		}
		else if (ptr->r == ptr->g && ptr->r == ptr->b) {
			dbgc = 'W';
		}
		else if (ptr->r > 0 && ptr->g == 0 && ptr->b == 0) {
			dbgc = 'R';
		}
		else if (ptr->g > 0 && ptr->r == 0 && ptr->b == 0) {
			dbgc = 'G';
		}
		else if (ptr->b > 0 && ptr->g == 0 && ptr->r == 0) {
			dbgc = 'B';
		}
		else if (ptr->r > 0 && ptr->g == 0 && ptr->b > 0) {
			dbgc = 'P';
		}
		else if (ptr->r > 0 && ptr->g > 0 && ptr->b == 0) {
			dbgc = 'Y';
		}
		else if (ptr->r == 0 && ptr->g > 0 && ptr->b > 0) {
			dbgc = 'T';
		}
		else {
			dbgc = '?';
		}
		Serial.printf("%c", dbgc);
	}
	Serial.printf("](%d)\r\n", b);
}