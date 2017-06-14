//#define DEBUG_CANBUS // hexdump captured messages

/*
warning: due to the timeouts used, this function actually takes a significant amount of blocking timeout
timeout is required for best functionality
the rest of the code, especially the infinite impulse response filtering, need to take this into account
*/
bool canbus_readAll(int* kmh, int* rpm, int* pedal)
{
	bool success = 1;
	uint8_t tmp[2];
	int res;

	if (kmh != NULL)
	{
		success &= canbus_readOne(ECUPID_SPEED, tmp);
		res = tmp[0];
		*kmh = res;
	}

	// RPM is the only data value that is most likely non-zero and also changing constantly
	// we always read it as a sanity check, even if the value is not requested
	success &= canbus_readOne(ECUPID_ENGINE_RPM, tmp);
	res = ((tmp[0]*256) + tmp[1])/4;
	if (rpm != NULL) {
		*rpm = res;
	}

	if (pedal != NULL)
	{
		success &= canbus_readOne(ECUPID_PEDAL, tmp);
		res = tmp[0];
		*pedal = res; // ranged 0 to 255, 255 means 100%
	}

	return success;
}

bool canbus_readOne(uint8_t pid, uint8_t* result)
{
	CAN_message_t txmsg, rxmsg;
	char txsuccess = 0, rxsuccess = 0;
	uint32_t tmr;
	canbus_ecuRequestFill(&txmsg, pid);
	tmr = millis();
	while ((millis() - tmr) < CANBUS_TIMEOUT && rxsuccess == 0)
	{
		if (txsuccess == 0) { // only transmit one
			if (CANn.write(txmsg) != 0) {
				txsuccess = 1; // only transmit one
			}
			continue; // nothing transmitted, do not bother reading
		}
		rxmsg.timeout = CANBUS_TIMEOUT;
		if (CANn.read(rxmsg))
		{
			#ifdef DEBUG_CANBUS
			dbg_printf("rxmsg: ");
			{
				uint8_t i;
				uint8_t* msg_ptr = (uint8_t*)&rxmsg;
				for (i = 0; i < sizeof(CAN_message_t); i++) {
					dbg_printf("%02X ", msg_ptr[i]);
				}
			}
			dbg_printf("\n");
			#endif
			if (rxmsg.id == ECU_PID_REPLY)
			{
				if (rxmsg.buf[2] == pid)
				{
					if (result != NULL)
					{
						result[0] = rxmsg.buf[3];
						result[1] = rxmsg.buf[4];
					}
					rxsuccess = 1;
				}
			}
		}
	}
	return (bool)rxsuccess;
}

void canbus_ecuRequestFill(CAN_message_t* x, uint8_t pid)
{
	x->id = ECU_PID_REQUEST;
	x->rtr = 0;
	x->len = 8;
	x->timeout = CANBUS_TIMEOUT; // zero doesn't work as well
	x->buf[0] = 0x02;
	x->buf[1] = 0x01;
	x->buf[2] = pid;
	x->buf[3] = 0x00;
	x->buf[4] = 0x00;
	x->buf[5] = 0x00;
	x->buf[6] = 0x00;
	x->buf[7] = 0x00;
}

bool canbus_scan(void)
{
	uint32_t baud;
	uint32_t tmr;
	uint8_t attempts;
	CAN_message_t txmsg, rxmsg;
	char success = 0;
	int result;
	Serial.printf("Begin CAN bus scanning...\n");
	for (baud = 125000; baud <= 500000; baud += 125000)
	{
		CANn.begin(baud);
		for (attempts = 0; attempts < 3 && success == 0; attempts++)
		{
			uint8_t tmp[2];
			if ((success = canbus_readOne(ECUPID_ENGINE_RPM, tmp)) != 0)
			{
				result = ((tmp[0]*256) + tmp[1])/4;
			}
		}
		if (success != 0)
		{
			Serial.printf("CAN bus at %u baud got ECU reply, RPM = %d\n", baud, result);
			return 1; // we quit this function leaving the CAN system still functional
		}
		CANn.end();
		Serial.printf("CAN bus at %u baud FAILED to get reply\n", baud);
		delay(500);
	}
	return 0;
}

void canbus_init(void)
{
	uint8_t i;
	CAN_filter_t flt;
	flt.id = ECU_PID_REPLY;
	flt.rtr = 0;
	flt.ext = 0;
	CANn.begin(CANBUS_DEFAULT_BAUD);
	for (i = 0; i < 8; i++) {
		CANn.setFilter(flt, i);
		CANn.setMask(ECU_PID_REPLY, i);
	}
}

void canbus_reset(void)
{
	CANn.end();
	canbus_init();
}