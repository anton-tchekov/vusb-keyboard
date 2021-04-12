#include "usb_kbd.h"
#include <util/delay.h>

#define BTN_PIN 7
#define BTN_IN   PIND
#define BTN_OUT  PORTD

int main(void)
{
	uint8_t state = 0;
	usb_kbd_init();

	/* Enable internal pullup */
	BTN_OUT |= (1 << BTN_PIN);
	DDRB |= (1 << 5);

	while(1)
	{
		usb_kbd_update();
		if(!(BTN_IN & (1 << BTN_PIN)) && state == 0)
		{
			/* Print "Hello World!" */
			usb_kbd_send(KEY_H, MOD_SHIFT_LEFT);
			usb_kbd_send(KEY_E, 0);
			usb_kbd_send(KEY_L, 0);
			usb_kbd_send(KEY_L, 0);
			usb_kbd_send(KEY_O, 0);
			usb_kbd_send(KEY_SPACE, 0);
			usb_kbd_send(KEY_W, MOD_SHIFT_LEFT);
			usb_kbd_send(KEY_O, 0);
			usb_kbd_send(KEY_R, 0);
			usb_kbd_send(KEY_L, 0);
			usb_kbd_send(KEY_D, 0);

			/* Exclamation mark */
			usb_kbd_send(KEY_1, MOD_SHIFT_LEFT);
			usb_kbd_send(KEY_ENTER, 0);

			state = 1;

			/* Debouncing */
			_delay_ms(10);
		}
		else if((BTN_IN & (1 << BTN_PIN)) && state == 1)
		{
			state = 0;

			/* Debouncing */
			_delay_ms(10);
		}
	}

	return 0;
}
