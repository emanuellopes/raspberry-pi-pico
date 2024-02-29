#include <stdint.h>

enum ReportIds
{
    REPORT_ID_KEYBOARD = 0x01,
    REPORT_ID_SYS_CONTROL = 0x02,
    REPORT_ID_COLLECTION = 0x03,
    REPORT_ID_VENDOR1 = 0x04,
    REPORT_ID_VENDOR2 = 0x05
};

// close to USB HID Specification 1.1, Appendix B.1
const uint8_t hid_descriptor_keyboard[] = {
    /********************** keyboard ****************************/
    0x05, 0x01,               // Usage Page (Generic Desktop Ctrls)
    0x09, 0x06,               // Usage (Keyboard)
    0xA1, 0x01,               // Collection (Application)
    0x85, REPORT_ID_KEYBOARD, //   Report ID (5)
    0x95, 0x70,               //   Report Count (112)
    0x75, 0x01,               //   Report Size (1)
    0x15, 0x00,               //   Logical Minimum (0)
    0x25, 0x01,               //   Logical Maximum (1)

    0x05, 0x07, //   Usage Page (Kbrd/Keypad)
    0x19, 0xE0, //   Usage Minimum (0xE0)
    0x29, 0xE7, //   Usage Maximum (0xE7)

    0x19, 0x00, //   Usage Minimum (0x00)
    0x29, 0x67, //   Usage Maximum (0x67)
    0x81, 0x02, //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)

    // Reserved byte (input) copied from original

    // 0x75, 0x01,                    //   Report Size (1)
    // 0x95, 0x08,                    //   Report Count (8)
    // 0x81, 0x03,                    //   Input (Constant, Variable, Absolute)

    0x95, 0x08, //   Report Count (8)
    0x81, 0x01, //   Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0xC0,       // End Collection
    /********************** keyboard ****************************/
    /********************** Sys Control *************************/

    /*               */
    0x05, 0x01,                  // Usage Page (Generic Desktop Ctrls)
    0x09, 0x80,                  // Usage (Sys Control)
    0xA1, 0x01,                  // Collection (Application)
    0x85, REPORT_ID_SYS_CONTROL, //   Report ID (2)
    0x19, 0x81,                  //   Usage Minimum (Sys Power Down)
    0x29, 0x83,                  //   Usage Maximum (Sys Wake Up)
    0x15, 0x00,                  //   Logical Minimum (0)
    0x25, 0x01,                  //   Logical Maximum (1)
    0x95, 0x03,                  //   Report Count (3)
    0x75, 0x01,                  //   Report Size (1)
    0x81, 0x02,                  //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x95, 0x01,                  //   Report Count (1)
    0x75, 0x05,                  //   Report Size (5)
    0x81, 0x01,                  //   Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0xC0,                        // End Collection
    /********************** Sys Control **********************************/

    /********************** Consumer Media Keys **************************/
    0x05, 0x0C,                 // Usage Page (Consumer)
    0x09, 0x01,                 // Usage (Consumer Control)
    0xA1, 0x01,                 // Collection (Application)
    0x85, REPORT_ID_COLLECTION, //   Report ID (3)
    0x19, 0x00,                 //   Usage Minimum (Unassigned)
    0x2A, 0xFF, 0x02,           //   Usage Maximum (0x02FF)
    0x15, 0x00,                 //   Logical Minimum (0)
    0x26, 0xFF, 0x7F,           //   Logical Maximum (32767)
    0x95, 0x01,                 //   Report Count (1)
    0x75, 0x10,                 //   Report Size (16)
    0x81, 0x00,                 //   Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0xC0,                       // End Collection
    /********************** Consumer Media Keys **************************/

    /********************** other vendor **************************/
    0x06, 0x00, 0xFF,        // Usage Page (Vendor Defined 0xFF00)
    0x09, 0x01,              // Usage (0x01)
    0xA1, 0x01,              // Collection (Application)
    0x85, REPORT_ID_VENDOR1, //   Report ID (6)
    0x15, 0x00,              //   Logical Minimum (0)
    0x26, 0xFF, 0x00,        //   Logical Maximum (255)
    0x09, 0x2F,              //   Usage (0x2F)
    0x95, 0x02,              //   Report Count (2)
    0x75, 0x08,              //   Report Size (8)
    0x81, 0x02,              //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0xC0,                    // End Collection
    /********************** other vendor **************************/
    /********************** other vendor 2 **************************/

    0x06, 0x01, 0xFF,        // Usage Page (Vendor Defined 0xFF01)
    0x09, 0x01,              // Usage (0x01)
    0xA1, 0x01,              // Collection (Application)
    0x85, REPORT_ID_VENDOR2, //   Report ID (7)
    0x15, 0x00,              //   Logical Minimum (0)
    0x26, 0xFF, 0x00,        //   Logical Maximum (255)
    0x09, 0x20,              //   Usage (0x20)
    0x75, 0x08,              //   Report Size (8)
    0x95, 0x07,              //   Report Count (7)
    0xB1, 0x02,              //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0xC0,                    // End Collection
    /********************** other vendor 2 **************************/
};