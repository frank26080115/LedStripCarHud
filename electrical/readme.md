Everything was done on a perfboard "on the fly". The schematic here is provided as an afterthought. It may not accurately represent what I've actually built, and I forgot some of the component values.

## Part List ##

This is a rough list, may not be complete. Check the schematic, and also fill in any missing parts yourself.

 * Teensy 3.2 , Microcontroller
 * MCP2562, CAN bus transceiver, plus relevant decoupling capacitors and chip socket
 * 2x 60 ohm resistors for the CAN bus termination, plus 1x 4700pF capacitor, use in a split termination circuit
 * "DotStar" LED Strip from Adafruit (APA102C)
 * A generic DC/DC step-down voltage converter from Amazon that outputs 5V from a 12V input, need to handle a few amps
 * 2x SN74AHCT1G125 buffer, used as level shifter. Remember to add decoupling capacitors
 * 6x Potentiometers, add some filter capacitors to these
 * NSL-5112 Photoresistor, forms a voltage divider with a 47Kohm resistor and 3.6V zener diode for protection. Also add a filter capacitor
 * A voltage divider of some sort to divide 14V down to something we can measure with the ADC, add a filter capacitor. Honestly I used some random values of resistors that I had laying around, we don't need to be accurate and the potentiometers can adjust stuff.
 * Power Switch is a S802C21S207Q
 * 3mm LED for heartbeat, plus a 1K resistor to limit the current to this LED
 * Wires, passive components, pin headers, chip socket

## Notes ##

 * Remove the LED from the Teensy. It is shared with SCK (SPI clock), removing the LED will remove some load and capacitance from that signal. Do this if you find the clock signal to be weak (low amplitude and/or too much slope). I honestly don't know if this is even a real problem but I've gotten into this habit (remove LEDs) on all Arduino products, the resistors used on the LEDs always have too low of a resistance and causes problems.