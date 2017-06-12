void tests(void)
{
	Serial.begin(9600); // USB port will ignore baud parameter
	test_usb();
	test_heartbeat();
	test_analog();
	test_strip();
	test_canbus();
}

void test_heartbeat(void)
{
	pinMode(PIN_HEARTBEAT, OUTPUT);
	while (1)
	{
		digitalWrite(PIN_HEARTBEAT, HIGH);
		delay(500);
		digitalWrite(PIN_HEARTBEAT, LOW);
		delay(500);
	}
}

void test_usb(void)
{
	while (1)
	{
		Serial.printf("Hello World\n");
		delay(1000);
		Serial.printf("Hello World\n");
		Serial.send_now();
		delay(1000);
	}
}

void test_analog(void)
{
	pinMode(PIN_VOLTAGE, INPUT);
	pinMode(PIN_SUN, INPUT);
	pinMode(PIN_POT_1, INPUT);
	pinMode(PIN_POT_2, INPUT);
	pinMode(PIN_POT_3, INPUT);
	pinMode(PIN_POT_4, INPUT);
	pinMode(PIN_POT_5, INPUT);
	pinMode(PIN_POT_4, INPUT);

	while (1)
	{
		Serial.printf("\n\tv=, %d , ", analogRead(PIN_VOLTAGE));
		Serial.printf("\tsun=, %d , ", analogRead(PIN_SUN));
		Serial.printf("\tpot1=, %d , ", analogRead(PIN_POT_1));
		Serial.printf("\tpot2=, %d , ", analogRead(PIN_POT_2));
		Serial.printf("\tpot3=, %d , ", analogRead(PIN_POT_3));
		Serial.printf("\tpot4=, %d , ", analogRead(PIN_POT_4));
		Serial.printf("\tpot5=, %d , ", analogRead(PIN_POT_5));
		Serial.printf("\tpot6=, %d , ", analogRead(PIN_POT_6));
		delay(500);
	}
}

void test_strip(void)
{
	uint16_t i, j = 0;
	strip_init();
	while (1)
	{
		for (i = 0; i < LED_STRIP_SIZE; i++)
		{
			strip_setColourRGB(i, 0, 0, 0);
		}
		strip_setBrightness(255);
		strip_show();
		delay(500);
		strip_setColourRGB(j + 0, 255, 255, 255);
		strip_setColourRGB(j + 1, 255, 0, 0);
		strip_setColourRGB(j + 2, 0, 255, 0);
		strip_setColourRGB(j + 3, 0, 0, 255);
		strip_setColourRGB(j + 4, 255, 255, 255);
		strip_show();
		delay(500);
		strip_setBrightness(128);
		strip_show();
		delay(500);
		j += 1;
		j %= LED_STRIP_SIZE - 5;
	}
}

void test_canbus(void)
{
	while(canbus_scan() == 0)
	{
	}
	while (1)
	{
		int kmh, rpm, pedal;
		if (canbus_readAll(&kmh, &rpm, &pedal) != 0)
		{
			Serial.printf("kmh=, %d , rpm=, %d , pedal=, %d , \n", kmh, rpm, pedal);
		}
		delay(500);
	}
	
}