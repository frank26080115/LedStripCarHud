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
	char success = 0;
	uint32_t tmr;
	canbus_ecuRequestFill(&txmsg, pid);
	CANn.write(txmsg);
	tmr = millis();
	while ((millis() - tmr) < CANBUS_TIMEOUT && success == 0)
	{
		while (CANn.available())
		{
			CANn.read(rxmsg);
			if (rxmsg.id == ECU_PID_REPLY)
			{
				if (rxmsg.buf[2] == pid)
				{
					if (result != NULL)
					{
						result[0] = rxmsg.buf[3];
						result[1] = rxmsg.buf[4];
					}
					success = 1;
				}
			}
		}
	}
	return (bool)success;
}

void canbus_ecuRequestFill(CAN_message_t* x, uint8_t pid)
{
	x->id = ECU_PID_REQUEST;
	x->rtr = 0;
	x->len = 8;
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
	for (baud = 250000; baud <= 500000; baud += 250000)
	{
		CANn.begin(250000);
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
			return 1;
		}
		CANn.end();
		Serial.printf("CAN bus at %u baud FAILED to get reply\n", baud);
		delay(500);
	}
	return 0;
}

void canbus_init(void)
{
	CANn.begin(250000); // change the baud here
}