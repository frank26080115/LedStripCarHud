#if (defined(_ADAFRUIT_DOT_STAR_H_) && defined(FASTLED_VERSION)) || (!defined(_ADAFRUIT_DOT_STAR_H_) && !defined(FASTLED_VERSION))
#error Please select library for DotStar
#elif defined(_ADAFRUIT_DOT_STAR_H_)
Adafruit_DotStar* strip;
#elif defined(FASTLED_VERSION)
CFastLED* strip;
CRGB strip_array[LED_STRIP_SIZE];
#endif

uint8_t global_brightness;
led_t led_strip[LED_STRIP_SIZE];

void strip_init(void)
{
	#if defined(_ADAFRUIT_DOT_STAR_H_)
	strip = new Adafruit_DotStar(LED_STRIP_SIZE);
	#elif defined(FASTLED_VERSION)
	strip = &FastLED;
	FastLED.addLeds<APA102, PIN_LEDSTRIP_DOUT, PIN_LEDSTRIP_SCK, RGB>(strip_array, LED_STRIP_SIZE);
	#endif
}

void strip_show(void)
{
	uint16_t i;
	#ifdef DEBUG_STRIP
	dbg_printf("=LEDs=[", dbgc);
	#endif
	for (i = 0; i < LED_STRIP_SIZE; i++)
	{
		led_t* ptr = &led_strip[i];
		#if defined(_ADAFRUIT_DOT_STAR_H_)
		strip->setPixelColor(i, led_adjustChan(ptr->r, global_brightness), led_adjustChan(ptr->g, global_brightness), led_adjustChan(ptr->b, global_brightness));
		#elif defined(FASTLED_VERSION)
		CRGB* dot = &strip_array[i];
		dot->r = led_adjustChan(ptr->r, global_brightness);
		dot->g = led_adjustChan(ptr->g, global_brightness);
		dot->b = led_adjustChan(ptr->b, global_brightness);
		#endif
		#ifdef DEBUG_STRIP
		{
			uint8_t dbgc;
			if ((ptr->r == 0 && ptr->g == 0 && ptr->b == 0) || global_brightness == 0) {
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
			dbg_printf("%c", dbgc);
		}
		#endif
	}
	#ifdef DEBUG_STRIP
	dbg_printf("] (%d)\n", global_brightness);
	#endif
	strip->show();
}

void strip_setBrightness(uint8_t x)
{
	global_brightness = x;
}

void strip_setColourRGB(uint16_t n, uint8_t r, uint8_t g, uint8_t b)
{
	led_t* ptr = &led_strip[n];
	ptr->r = r;
	ptr->g = g;
	ptr->b = b;
}

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