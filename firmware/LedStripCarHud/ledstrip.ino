/*
    This file is part of LedStripCarHud

    LedStripCarHud is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    LedStripCarHud is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with LedStripCarHud.  If not, see <http://www.gnu.org/licenses/>.
*/

CFastLED* strip;
CRGB strip_array[LED_STRIP_SIZE_VIRTUAL];

uint8_t global_brightness;
led_t led_strip[LED_STRIP_SIZE_VIRTUAL];

uint32_t strip_power = 0;

#define DEBUG_STRIP

void strip_init(void)
{
	pinMode(PIN_LEDSTRIP_DOUT, OUTPUT);
	pinMode(PIN_LEDSTRIP_SCK, OUTPUT);
	strip = &FastLED;
	FastLED.addLeds<APA102
		, PIN_LEDSTRIP_DOUT, PIN_LEDSTRIP_SCK
		, RGB
		, DATA_RATE_MHZ(6)
	>(strip_array, LED_STRIP_SIZE_VIRTUAL);

	//strip_blank();
	//strip_show();
}

void strip_show(void)
{
	uint16_t i;
	uint8_t b = global_brightness;
	uint32_t pwr;
	// copy over colours, adjusting for RGB order
	// I know that I can adjust the order with the FastLED initializer
	while (1)
	{
		pwr = 0;
		for (i = 0; i < LED_STRIP_SIZE; i++)
		{
			led_t* ptr = &led_strip[i];
			CRGB* dot = &strip_array[i];
			dot->b = led_adjustChan(ptr->r, b);
			dot->g = led_adjustChan(ptr->g, b);
			dot->r = led_adjustChan(ptr->b, b);
			pwr += dot->r;
			pwr += dot->g;
			pwr += dot->b;
		}
		if (pwr > LEDSTRIP_POWER_LIMIT)
		{
			b--;
		}
		else
		{
			break;
		}
	}
	// blank out non-existant LEDs, or leftover LEDs
	for (; i < LED_STRIP_SIZE_VIRTUAL; i++)
	{
		CRGB* dot = &strip_array[i];
		dot->r = 0;
		dot->g = 0;
		dot->b = 0;
		if (i == LED_STRIP_SIZE_VIRTUAL - 1) {
			dot->r = 1; // possible fix for glitchy strip
		}
	}
	#ifdef DEBUG_STRIP
	dbg_strip(led_strip, LED_STRIP_SIZE, global_brightness);
	#endif
	strip->show();
}

uint32_t strip_getPower(void)
{
	uint16_t i;
	uint32_t pwr;
	for (i = 0; i < LED_STRIP_SIZE; i++)
	{
		led_t* ptr = &led_strip[i];
		pwr += led_adjustChan(ptr->r, global_brightness);
		pwr += led_adjustChan(ptr->g, global_brightness);
		pwr += led_adjustChan(ptr->b, global_brightness);
	}
	return pwr;
}

void strip_setBrightness(int16_t x)
{
	if (x < 0) {
		x = 0;
	}
	else if (x > 0xFF) {
		x = 0xFF;
	}
	global_brightness = x;
}

uint8_t strip_getBrightness(void)
{
	return global_brightness;
}

void strip_setColourRGB(uint16_t n, uint8_t r, uint8_t g, uint8_t b)
{
	led_t* ptr;
	if (n < 0 || n > LED_STRIP_SIZE) {
		return;
	}
	ptr = &led_strip[n];
	ptr->r = r;
	ptr->g = g;
	ptr->b = b;
}

uint8_t strip_getColourR(uint16_t n)
{
	return led_strip[n].r;
}

uint8_t strip_getColourG(uint16_t n)
{
	return led_strip[n].g;
}

uint8_t strip_getColourB(uint16_t n)
{
	return led_strip[n].b;
}

uint16_t strip_getColourAny(uint16_t n)
{
	return strip_getColourR(n) + strip_getColourG(n) + strip_getColourB(n);
}

void strip_blank(void)
{
	uint16_t i;
	for (i = 0; i < LED_STRIP_SIZE; i++)
	{
		strip_setColourRGB(i, 0, 0, 0);
	}
}

void strip_fill(uint8_t r, uint8_t g, uint8_t b)
{
	uint16_t i;
	for (i = 0; i < LED_STRIP_SIZE; i++)
	{
		strip_setColourRGB(i, r, g, b);
	}
}

/*
this function multiplies a colour intensity by a brightness
but it has a rule that prevents a LED from going totally off
*/
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
	r += 127; // rounding
	r /= 255;
	if (r <= 0) {
		return 1; // ensure minimum is still visible
	}
	if (r >= b) {
		return b; // snsure never exceed brightness
	}
	return r;
}