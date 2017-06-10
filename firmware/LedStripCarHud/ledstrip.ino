uint8_t led_adjustChan(uint8_t c, uint8_t b)
{
	int32_t r;
	if (c <= 0) {
		return 0;
	}
	else if (c >= 255) {
		return b;
	}
	r = c;
	r *= b;
	r += 127;
	r /= 255;
	if (r <= 0) {
		return 1;
	}
	if (r >= b) {
		return b;
	}
	return r;
}

void led_adjustLed(led_t* led_in, led_t* led_out, uint8_t b)
{
	led_out->red   = led_adjustChan(led_in->red,   b);
	led_out->green = led_adjustChan(led_in->green, b);
	led_out->blue  = led_adjustChan(led_in->blue,  b);
}

