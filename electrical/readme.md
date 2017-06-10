Sorry, but I do not have a circuit diagram of any kind for this project yet. Everything was done on a perfboard "on the fly". The pin assignments are listed in the Arduino sketch, and you should be able to figure out everything using that and the relevant datasheets of the parts used.

## Part List ##

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