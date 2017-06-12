bool canbus_read(int* kmh, int* rpm, int* pedal)
{
	CAN_message_t txmsg;
	
}

void canbus_ecuRequestFill(CAN_message_t* x, uint8_t pid)
{
	x->id = 0x07DF; // PID_REQUEST
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

int32_t canbus_parseRPM(CAN_message_t* msg)
{
	return ((msg->buf[3]*256) + msg->buf[4])/4; // 
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
		Can0.begin(250000);
		for (attempts = 0; attempts < 3 && success == 0; attempts++)
		{
			success = 0;
			canbus_ecuRequestFill(&txmsg, 0x0C);
			Can0.write(txmsg);
			tmr = millis();
			while ((millis() - tmr) < 1000)
			{
				while (Can0.available())
				{
					Can0.read(rxmsg);
					if (rxmsg.id == 0x07E8)
					{
						if (rxmsg.buf[2] == 0x0C)
						{
							result = canbus_parseRPM(&rxmsg);
							success = 1;
						}
					}
				}
				if (success != 0)
				{
					break;
				}
			}
		}
		if (success != 0)
		{
			Serial.printf("CAN bus at %u baud got ECU reply, RPM = %d\n", baud, result);
			return 1;
		}
		Can0.end();
		Serial.printf("CAN bus at %u baud FAILED to get reply\n", baud);
		delay(500);
	}
	return 0;
}