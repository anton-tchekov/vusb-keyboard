#include "usb_kbd.h"
#include <util/delay.h>

#ifdef BLA
const char usbHidReportDescriptor[35] PROGMEM =
{
	0x05, 0x01, /* USAGE_PAGE */
	0x09, 0x06, /* USAGE */
	0xa1, 0x01, /* COLLECTION */
	0x05, 0x07, /* USAGE_PAGE */
	0x19, 0xe0, /* USAGE_MINIMUM */
	0x29, 0xe7, /* USAGE_MAXIMUM */
	0x15, 0x00, /* LOGICAL_MINIMUM */
	0x25, 0x01, /* LOGICAL_MAXIMUM */
	0x75, 0x01, /* REPORT_SIZE */
	0x95, 0x08, /* REPORT_COUNT */
	0x81, 0x02, /* INPUT */
	0x95, BUFFER_SIZE - 1, /* REPORT_COUNT */
	0x75, 0x08, /* REPORT_SIZE */
	0x25, 0x65, /* LOGICAL_MAXIMUM */
	0x19, 0x00, /* USAGE_MINIMUM */
	0x29, 0x65, /* USAGE_MAXIMUM */
	0x81, 0x00, /* INPUT */
	0xc0 /* END_COLLECTION */
};
#endif

const char usbHidReportDescriptor[] PROGMEM = {
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x06,                    // USAGE (Keyboard)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x95, 0x08,                    //   REPORT_COUNT (8)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)(Key Codes)
    0x19, 0xe0,                    //   USAGE_MINIMUM (Keyboard LeftControl)(224)
    0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)(231)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs) ; Modifier byte
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x81, 0x03,                    //   INPUT (Cnst,Var,Abs) ; Reserved byte
    0x95, 0x05,                    //   REPORT_COUNT (5)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x05, 0x08,                    //   USAGE_PAGE (LEDs)
    0x19, 0x01,                    //   USAGE_MINIMUM (Num Lock)
    0x29, 0x05,                    //   USAGE_MAXIMUM (Kana)
    0x91, 0x02,                    //   OUTPUT (Data,Var,Abs) ; LED report
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x03,                    //   REPORT_SIZE (3)
    0x91, 0x03,                    //   OUTPUT (Cnst,Var,Abs) ; LED report padding
    0x95, 0x06,                    //   REPORT_COUNT (6)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x65,                    //   LOGICAL_MAXIMUM (101)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)(Key Codes)
    0x19, 0x00,                    //   USAGE_MINIMUM (Reserved (no event indicated))(0)
    0x29, 0x65,                    //   USAGE_MAXIMUM (Keyboard Application)(101)
    0x81, 0x00,                    //   INPUT (Data,Ary,Abs)
    0xc0                           // END_COLLECTION
};

uint8_t report[BUFFER_SIZE];

void usb_kbd_init(void)
{
	PORTD = 0;
	DDRD |= ~USBMASK;
	asm volatile("cli");
	usbDeviceDisconnect();
	usbDeviceConnect();
	usbInit();
	asm volatile("sei");
	memset(report, 0, sizeof(report));
	usbSetInterrupt(report, sizeof(report));
	usbDeviceDisconnect();
	_delay_ms(250);
	usbDeviceConnect();
}

void usb_kbd_update(void)
{
	usbPoll();
}

void usb_kbd_send(uint8_t key, uint8_t modifiers)
{
	while(!usbInterruptIsReady()) ;
	memset(report, 0, sizeof(report));
	report[0] = modifiers;
	report[1] = key;
	usbSetInterrupt(report, sizeof(report));
	while(!usbInterruptIsReady()) ;
	memset(report, 0, sizeof(report));
	usbSetInterrupt(report, sizeof(report));
}

uint8_t usbFunctionSetup(uint8_t data[8]) 
{
	usbMsgPtr = report;
	return 0;
}
