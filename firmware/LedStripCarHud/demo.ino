void demo(void)
{
	int animation_active;
	int i = 0, j = 0;
	int rpm, kmh;
	float voltage;
	double mph;
	strip_setBrightness(0x80);
	wipe_out(-1);
	delay_ms(3000);
	while (1)
	{
		i = 0;
		do
		{
			animation_active = draw_intro(i, INTRO_SIZE); // return zero means done, otherwise returns frame delay
			strip_show();
			if (animation_active > 0) {
				delay_ms(20);
			}
			i++;
		}
		while (animation_active != false);
		strip_blank();
		strip_show();
		delay_ms(2000);
		voltage = 11.8;
		draw_voltage_fadein(voltage, 0xFF, 0xFF);
		delay_ms(3000);
		for (; voltage >= 10.5; voltage -= 0.2)
		{
			draw_voltage(voltage, 0xFF, 0xFF);
			strip_show();
			delay_ms(50);
		}
		for (i = 0; voltage <= 12.8; voltage += 0.1, i += 3)
		{
			draw_voltage(voltage, 0xFF, 0xFF);
			strip_show();
			delay_ms(20 + i);
		}
		delay_ms(800);
		wipe_out(4);
		rpm = 500;
		draw_tachometer_fadein(rpm);
		for (; rpm <= 4500; rpm += 15)
		{
			draw_tachometer(rpm);
			strip_show();
			delay_ms(2000 / 500);
		}
		for (; rpm >= 2000; rpm -= 8)
		{
			draw_tachometer(rpm);
			strip_show();
			delay_ms(2000 / 500);
		}
		for (; rpm <= RPM_MAX - 50; rpm += 12)
		{
			draw_tachometer(rpm);
			strip_show();
			delay_ms(2000 / 500);
		}
		for (; rpm >= 500; rpm -= 8)
		{
			draw_tachometer(rpm);
			strip_show();
			delay_ms(2000 / 500);
		}
		wipe_out(2);
		kmh = 0;
		mph = 0;
		draw_speed_fadein_random(0xFF);
		for (; mph <= 15; kmh++) {
			mph = calc_mph(kmh);
			draw_speedometer(mph, 0xFF, 0xFF);
			strip_show();
			delay_ms(250);
		}
		for (; mph <= 40; kmh++) {
			mph = calc_mph(kmh);
			draw_speedometer(mph, 0xFF, 0xFF);
			strip_show();
			delay_ms(125);
		}
		for (; mph >= 35; kmh--) {
			mph = calc_mph(kmh);
			draw_speedometer(mph, 0xFF, 0xFF);
			strip_show();
			delay_ms(200);
		}
		for (; mph <= 38; kmh++) {
			mph = calc_mph(kmh);
			draw_speedometer(mph, 0xFF, 0xFF);
			strip_show();
			delay_ms(200);
		}
		for (; mph >= 36; kmh--) {
			mph = calc_mph(kmh);
			draw_speedometer(mph, 0xFF, 0xFF);
			strip_show();
			delay_ms(200);
		}
		for (; mph <= 80; kmh++) {
			mph = calc_mph(kmh);
			draw_speedometer(mph, 0xFF, 0xFF);
			strip_show();
			delay_ms(125);
		}
		for (; mph >= 72; kmh--) {
			mph = calc_mph(kmh);
			draw_speedometer(mph, 0xFF, 0xFF);
			strip_show();
			delay_ms(200);
		}
		for (; mph <= 100; kmh++) {
			mph = calc_mph(kmh);
			draw_speedometer(mph, 0xFF, 0xFF);
			strip_show();
			delay_ms(125);
		}
		for (; mph >= 0; kmh--) {
			mph = calc_mph(kmh);
			draw_speedometer(mph, 0xFF, 0xFF);
			strip_show();
			delay_ms(120);
		}
		delay_ms(3000);
		wipe_out(1);
		voltage = 12.2;
		draw_voltage_fadein(voltage, 0xFF, 0xFF);
		draw_voltage(voltage, 0xFF, 0xFF);
		strip_show();
		delay_ms(2000);
		wipe_out(4);
		delay_ms(5000);

		break;
	}
}