//#define DEBUG_CANBUS // hexdump captured messages

volatile unsigned int ecu_kmh = 0;
volatile unsigned int ecu_kmh_zeros = 0;
volatile unsigned int ecu_rpm = 0;
volatile unsigned int ecu_rpm_zeros = 0;
volatile unsigned int ecu_pedal = 0;
volatile unsigned int ecu_hasNew = 0;

volatile unsigned int ecu_other[8];

bool canbus_started = false;

bool canbus_readAll(int* kmh, int* rpm, int* pedal)
{
	if (ecu_hasNew < 3) {
		return false;
			}
	if (kmh != NULL) {
		*kmh = ecu_kmh;
				}
	if (rpm != NULL) {
		*rpm = ecu_rpm;
			}
	if (pedal != NULL) {
		*pedal = ecu_pedal;
					}
	ecu_hasNew = 0;
	return true;
}

class EcuListener : public CANListener 
{
	public:
		bool gotFrame(CAN_message_t &frame, int mailbox); //overrides the parent version so we can actually do something
		bool frameHandler (CAN_message_t &frame, int mailbox, uint8_t controller);
};

bool EcuListener::gotFrame(CAN_message_t &frame, int mailbox)
{
	bool success = false;
	uint8_t pid;
	uint16_t temp;

	can_totalMessages++;

	if (frame.id != ECU_PID_REPLY) {
		return false;
				}
	ecu_totalReplies++;

	pid = frame.buf[2];
	switch (pid)
	{
		case ECUPID_ENGINE_RPM:
			temp = ((frame.buf[3]*256) + frame.buf[4])/4;
			if (temp == 0) {
				ecu_rpm_zeros++;
			}
			else {
				ecu_rpm_zeros = 0;
			}
			if (ecu_rpm_zeros == 0) {
				ecu_rpm = temp;
			}
			else if (temp == 0 && ecu_rpm_zeros > 5) {
				ecu_rpm = 0;
				ecu_rpm_zeros = 5;
			}
			success = true;
			break;
		case ECUPID_SPEED:
			temp = frame.buf[3];
			if (temp == 0) {
				ecu_kmh_zeros++;
			}
			else {
				ecu_kmh_zeros = 0;
			}
			if (ecu_kmh_zeros == 0 || ecu_kmh < 5) {
				ecu_kmh = temp;
			}
			else if (temp == 0 && ecu_kmh_zeros > 2) {
				ecu_kmh = 0;
				ecu_kmh_zeros = 2;
			}
			success = true;
			break;
		case ECUPID_PEDAL:
			ecu_pedal = frame.buf[3];
			success = true;
			break;
			}

	// this block of code is for data collection only
	/*
	switch (pid)
	{
		case ECUPID_THROTTLE:
			ecu_other[0] = frame.buf[3];
			break;
		case ECUPID_ACCEL_PEDAL_POS_D:
			ecu_other[1] = frame.buf[3];
			break;
		case ECUPID_ACCEL_PEDAL_POS_E:
			ecu_other[2] = frame.buf[3];
			break;
		case ECUPID_ACCEL_PEDAL_POS_F:
			ecu_other[3] = frame.buf[3];
			break;
		case ECUPID_REL_ACCEL_PEDAL_POS:
			ecu_other[4] = frame.buf[3];
			break;
		}
	//*/

	if (success != false) {
		ecu_hasNew += 1;
	}
	return success;
}

bool EcuListener::frameHandler (CAN_message_t &frame, int mailbox, uint8_t controller) 
{
	bool r = gotFrame(frame, mailbox);
	return true;
}

EcuListener canListener;

void canbus_ecuRequestFill(CAN_message_t* x, uint8_t pid)
{
	x->id = ECU_PID_REQUEST;
	x->rtr = 0;
	x->len = 8;
	//x->timeout = CANBUS_TX_TIMEOUT;
	x->buf[0] = 0x02;
	x->buf[1] = 0x01;
	x->buf[2] = pid;
	x->buf[3] = 0x00;
	x->buf[4] = 0x00;
	x->buf[5] = 0x00;
	x->buf[6] = 0x00;
	x->buf[7] = 0x00;
}

void canbus_init(void)
{
	uint8_t i;
	CAN_filter_t flt;

	/*
	I am still unsure if the filtering is working
	*/

	flt.id = ECU_PID_REPLY; flt.rtr = 0; flt.ext = 0;
	//canListener.begin();
	CANn.begin(CANBUS_DEFAULT_BAUD, flt);
	//CANn.setNumTXBoxes(CANBUS_TX_BOXES);
	for (i = 0; i < NUM_MAILBOXES; i++) {
		CANn.setMask(0x7FF, i);
		flt.id = ECU_PID_REPLY; flt.rtr = 0; flt.ext = 0;
		CANn.setFilter(flt, i);
	}
	CANn.attachObj(&canListener);
	canListener.attachGeneralHandler();
	canbus_started = true;
}

void canbus_writeRequests(void)
{
	CAN_message_t msg;
	static uint8_t tick = 0;
	static uint8_t failures = 0;
	uint8_t pid = 0;

	/*
	the initialization code should have opened up more than 3 outbound mailboxes
	*/

	do
			{
		tick++;
		if (tick == 1) {
			pid = ECUPID_ENGINE_RPM;
			}
		else if (tick == 2) {
			pid = ECUPID_SPEED;
		}
		else if (tick == 3) {
			pid = ECUPID_PEDAL;
		}
		/*
		else if (tick == 4) {
			pid = ECUPID_ACCEL_PEDAL_POS_D;
		}
		else if (tick == 5) {
			pid = ECUPID_ACCEL_PEDAL_POS_E;
		}
		else if (tick == 6) {
			pid = ECUPID_ACCEL_PEDAL_POS_F;
		}
		else if (tick == 7) {
			pid = ECUPID_REL_ACCEL_PEDAL_POS;
		}
		else if (tick == 8) {
			pid = ECUPID_THROTTLE;
		}
		//*/
		else {
			tick = 0;
		}
	}
	while (tick == 0 || pid == 0);

	canbus_ecuRequestFill(&msg, pid);
	if (canbus_started != false)
		{
		if (CANn.write(msg) == 0) {
			failures++;
		}
		else {
			ecu_totalRequests++;
		}
	}
	yield();

	// give the CAN peripheral a kick in the ass if it fails too often
	if (failures >= CANBUS_FAILURE_TOLERANCE) {
		Serial.printf("too many fails\r\n");
		canbus_reset();
		failures = 0;
	}
}

bool canbus_simpleCheck(void)
{
	CAN_message_t msg;
	uint32_t t, now;
	ecu_hasNew = 0;
	canbus_ecuRequestFill(&msg, ECUPID_ENGINE_RPM);
	if (CANn.write(msg) == 0) {
		Serial.printf("failed single RPM request\r\n");
		return false;
	}
	t = millis();
	now = t;
	while ((now - t) < 500 && ecu_hasNew <= 0) {
		tasks(now);
		now = millis();
	}
	return (ecu_hasNew > 0);
}

void canbus_reset(void)
{
	CANn.end();
	canbus_init();
}