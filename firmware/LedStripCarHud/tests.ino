void tests(void)
{
	Serial.begin(9600); // USB port will ignore baud parameter
	//test_usb();
	//test_heartbeat();
	//test_analog();
	//test_stripPins();
	//test_strip();
	//test_animations();
	//test_voltmeter();
	//test_tachometer();
	//test_intro();
	//test_speedometer();
	//test_brightness();
	//test_canbus();
	//test_sleep();
	//test_burn();
	test_dials();
}

void test_heartbeat(void)
{
	pinMode(PIN_HEARTBEAT, OUTPUT);
	while (1)
	{
		digitalWrite(PIN_HEARTBEAT, HIGH);
		delay_ms(500);
		digitalWrite(PIN_HEARTBEAT, LOW);
		delay_ms(500);
	}
}

void test_usb(void)
{
	pinMode(PIN_HEARTBEAT, OUTPUT);
	while (1)
	{
		Serial.printf("Hello World\n");
		digitalWrite(PIN_HEARTBEAT, HIGH);
		delay_ms(1000);
		Serial.printf("Hello World\n");
		Serial.send_now();
		digitalWrite(PIN_HEARTBEAT, LOW);
		delay_ms(1000);
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
		Serial.printf("\nv=, %d , ",  analogRead(PIN_VOLTAGE));
		Serial.printf("sun=, %d , ",  analogRead(PIN_SUN));
		Serial.printf("pot1=, %d , ", analogRead(PIN_POT_1));
		Serial.printf("pot2=, %d , ", analogRead(PIN_POT_2));
		Serial.printf("pot3=, %d , ", analogRead(PIN_POT_3));
		Serial.printf("pot4=, %d , ", analogRead(PIN_POT_4));
		Serial.printf("pot5=, %d , ", analogRead(PIN_POT_5));
		Serial.printf("pot6=, %d , ", analogRead(PIN_POT_6));
		delay_ms(200);
	}
}

void test_strip(void)
{
	uint16_t i, j = 0;
	strip_init();
	while (1)
	{
		strip_blank();
		strip_setBrightness(255);
		strip_show();
		delay_ms(50);
		strip_setColourRGB(j + 0, 255, 255, 255);
		strip_setColourRGB(j + 1, 255, 0, 0);
		strip_setColourRGB(j + 2, 0, 255, 0);
		strip_setColourRGB(j + 3, 0, 0, 255);
		strip_setColourRGB(j + 4, 255, 255, 255);
		strip_show();
		delay_ms(100);
		strip_setBrightness(128);
		strip_show();
		delay_ms(100);
		j += 1;
		j %= LED_STRIP_SIZE - 5;
	}
}

void test_stripPins(void)
{
	pinMode(PIN_HEARTBEAT, OUTPUT);
	pinMode(PIN_LEDSTRIP_SCK, OUTPUT);
	pinMode(PIN_LEDSTRIP_DOUT, OUTPUT);
	while (1)
	{
		digitalWrite(PIN_HEARTBEAT, HIGH);
		digitalWrite(PIN_LEDSTRIP_DOUT, HIGH);
		digitalWrite(PIN_LEDSTRIP_SCK,  HIGH);
		Serial.printf("H");
		delay_ms(3000);
		digitalWrite(PIN_HEARTBEAT, LOW);
		digitalWrite(PIN_LEDSTRIP_DOUT, LOW);
		digitalWrite(PIN_LEDSTRIP_SCK,  LOW);
		Serial.printf("L");
		delay_ms(3000);
	}
}

void test_animations(void)
{
	uint8_t i = 0, j = 0;
	strip_init();
	strip_setBrightness(0xFF);
	while (1)
	{
		wipe_out(i);
		delay_ms(500);
		draw_speed_fadein(j);
		delay_ms(2000);
		i++;
		i %= 6;
		j++;
		j %= 7;
	}
}

void test_intro(void)
{
	uint16_t i = 0;
	strip_init();
	strip_blank();
	strip_setBrightness(64);
	strip_show();
	while (1)
	{
		if (draw_intro(i, 3) == 0) {
			i = 0;
		}
		else {
			i++;
		}
		strip_show();
		delay_ms(20);
		
	}
}

void test_voltmeter(void)
{
	uint32_t t;
	strip_init();
	strip_setBrightness(64);
	wipe_out(-1);
	while (1)
	{
		draw_voltage_fadein(get_voltage(), 0xFF, 0xFF);
		t = millis();
		while ((millis() - t) < 1000)
		{
			draw_voltage(get_voltage(), 0xFF, 0xFF);
			strip_show();
		}
		wipe_out(-1);
	}
}

void test_tachometer(void)
{
	uint32_t t;
	strip_init();
	strip_setBrightness(64);
	strip_blank();
	strip_show();
	while (1)
	{
		double rpm = (get_voltage() - 10) * (8000 / 3);
		if (rpm < 0)
		{
			rpm = 0;
		}
		draw_tachometer_fadein(lround(rpm));
		t = millis();
		while ((millis() - t) < 1000)
		{
			draw_tachometer(lround(rpm));
			strip_show();
			get_voltage();
		}
		wipe_out(-1);
	}
}

void test_speedometer(void)
{
	uint32_t t;
	strip_init();
	strip_setBrightness(64);
	strip_blank();
	strip_show();
	while (1)
	{
		double spd = (get_voltage() - 10) * (120 / 3);
		if (spd < 0)
		{
			spd = 0;
		}
		draw_speed_fadein(-1);
		t = millis();
		while ((millis() - t) < 1000)
		{
			draw_speedometer(spd, 0xFF, 0xFF);
			strip_show();
			get_voltage();
		}
		wipe_out(-1);
	}
}

void test_brightness(void)
{
	uint32_t t;
	strip_init();
	while (1)
	{
		t = millis();
		strip_blank();
		strip_setColourRGB(0, DRAWRGB_WHITE(0xFF));
		strip_setColourRGB(10, DRAWRGB_RED(0xFF));
		strip_setColourRGB(20, DRAWRGB_GREEN(0xFF));
		strip_setColourRGB(30, DRAWRGB_BLUE(0xFF));
		strip_setBrightness(get_brightness());
		strip_show();
		tasks(t);
	}
}

void test_canbus(void)
{
	bool failed = false;
	canbus_init();
	while (1)
	{
		uint32_t now;
		int kmh, rpm, pedal;

		now = millis();
		tasks(now);
		if (canbus_readAll(&kmh, &rpm, &pedal) != 0)
		{
			Serial.printf("t=, %d , kmh=, %d , rpm=, %d , pedal=, %d , voltage=, %2.2f , ", now, kmh, rpm, pedal, get_voltage());
			Serial.printf(" %d, %d, %d, %d, %d, %d, %d, ", ecu_other[0], ecu_other[1], ecu_other[2], ecu_other[3], ecu_other[4], ecu_totalReplies, can_totalMessages);
			Serial.printf("\r\n");
			failed = false;
		}
		else
		{
			if (failed == false)
			{
				// only print once, no message flooding
			Serial.printf("read failed\n");
				delay_ms(500);
				failed = true;
		}
	}
		yield();
	}
}

void test_sleep(void)
{
	uint8_t i;
	strip_init();
	strip_blank();
	strip_show();
	canbus_init();
	pinMode(PIN_HEARTBEAT, OUTPUT);
	while (1)
	{
		sleep();
		for (i = 0; i < 6; i++)
		{
			digitalWrite(PIN_HEARTBEAT, HIGH);
			delay_ms(150);
			digitalWrite(PIN_HEARTBEAT, LOW);
			delay_ms(150);
		}
		tasks(millis());
	}
}

void test_burn(void)
{
	uint32_t now, i;
	int pedal, rpm;
	strip_init();
	strip_blank();
	strip_setBrightness(0xFF);
	canbus_init();
	while(true)
	{
		tasks(now = millis());
		if (canbus_readAll(NULL, &rpm, &pedal))
		{
			pedal_track(pedal, rpm);
			pedal = get_pedal();
			strip_blank();
			for (i = 0; i <= pedal / 2; i++) {
				strip_setColourRGB(i, DRAWRGB_WHITE(0xFF));
			}
			strip_show();
		}
	}
}

void test_dials(void)
{
	float i;
	int j;
	uint32_t t;
	strip_init();
	strip_blank();
	strip_setBrightness(0xFF);
	while (true)
	{
		for (i = 10.0; i <= 14.0; i += 1.0)
		{
			wipe_out(-1);
			draw_voltage_fadein(i, 0xFF, 0xFF);
			t = millis();
			while ((millis() - t) < 1000) {
				draw_voltage(i, 0xFF, 0xFF);
				strip_show();
			}
		}
		for (j = 0; j <= RPM_MAX; j += 800)
		{
			wipe_out(-1);
			draw_tachometer_fadein(j);
			t = millis();
			while ((millis() - t) < 1000) {
				draw_tachometer(j);
				strip_show();
			}
		}
		wipe_out(-1);
		draw_speed_fadein(-1);
		for (i = 0.0; i <= 80; i += 0.5)
		{
			draw_speedometer(i, 0xFF, 0xFF);
			strip_show();
			delay_ms(8000 / 160);
		}
		for (i = 80.0; i >= 0; i -= 0.5)
		{
			draw_speedometer(i, 0xFF, 0xFF);
			strip_show();
			delay_ms(8000 / 160);
		}
		for (i = 0.0; i <= 120; i += 1)
		{
			draw_speedometer(i, 0xFF, 0xFF);
			strip_show();
			delay_ms(8000 / 240);
		}
		for (i = 120.0; i >= 0; i -= 1)
		{
			draw_speedometer(i, 0xFF, 0xFF);
			strip_show();
			delay_ms(8000 / 240);
		}
	}
}