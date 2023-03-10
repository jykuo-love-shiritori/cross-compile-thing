#include "util.h"

#define PIN0 396
#define PIN1 392
#define PIN2 255
#define PIN3 254

void LED_ON(unsigned int pin) {
	gpio_export(pin);
	gpio_set_dir(pin, "out");
	gpio_set_value(pin, "1");
}

void LED_OFF(unsigned int pin) {
	gpio_set_value(pin, "0");
	gpio_unexport(pin);
}

int main(int argc, char **argv) {
	int input;
	int PIN;
	int i;

	/* basic
	switch (argv[1][3]) {
		case '0':
			PIN = PIN0;
			break;
		case '1':
			PIN = PIN1;
			break;
		case '2':
			PIN = PIN2;
			break;
		case '3':
			PIN = PIN3;
			break;
	}

	if (argv[2][1] == 'n') {
		LED_ON(PIN);
	} else if (argv[2][1] == 'f') {
		LED_OFF(PIN);
	}
	*/

	/* advanced
	for (i = 0; i < 5; ++i) {
		LED_ON(PIN0);
		LED_ON(PIN1);
		sleep(1);
		LED_OFF(PIN0);
		LED_OFF(PIN1);

		LED_ON(PIN2);
		LED_ON(PIN3);
		sleep(1);
		LED_OFF(PIN2);
		LED_OFF(PIN3);
	}
	*/

	return 0;
}
