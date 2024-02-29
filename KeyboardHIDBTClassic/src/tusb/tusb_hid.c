#include "header/tusb_lib.h"
#include "../header/pico_queue_lib.h"

#define MAX_REPORT 4

// Each HID instance can has multiple reports
static struct
{
  uint8_t report_count;
  tuh_hid_report_info_t report_info[MAX_REPORT];
} hid_info[CFG_TUH_HID];

//--------------------------------------------------------------------+
// TinyUSB Callbacks
//--------------------------------------------------------------------+

// Invoked when device with hid interface is mounted
// Report descriptor is also available for use. tuh_hid_parse_report_descriptor()
// can be used to parse common/simple enough descriptor.
// Note: if report descriptor length > CFG_TUH_ENUMERATION_BUFSIZE, it will be skipped
// therefore report_desc = NULL, desc_len = 0
void tuh_hid_mount_cb(uint8_t dev_addr, uint8_t instance, uint8_t const *desc_report, uint16_t desc_len)
{
  printf("HID device address = %d, instance = %d is mounted\r\n", dev_addr, instance);

  // Interface protocol (hid_interface_protocol_enum_t)
  const char *protocol_str[] = {"None", "Keyboard", "Mouse"};
  uint8_t const itf_protocol = tuh_hid_interface_protocol(dev_addr, instance);

  printf("HID Interface Protocol = %s\r\n", protocol_str[itf_protocol]);

  // By default host stack will use activate boot protocol on supported interface.
  // Therefore for this simple example, we only need to parse generic report descriptor (with built-in parser)
  if (itf_protocol == HID_ITF_PROTOCOL_NONE)
  {
    hid_info[instance].report_count = tuh_hid_parse_report_descriptor(hid_info[instance].report_info, MAX_REPORT, desc_report, desc_len);
    printf("HID has %u reports \r\n", hid_info[instance].report_count);
  }

  // request to receive report
  // tuh_hid_report_received_cb() will be invoked when report is available
  if (!tuh_hid_receive_report(dev_addr, instance))
  {
    printf("Error: cannot request to receive report\r\n");
  }
}

// Invoked when device with hid interface is un-mounted
void tuh_hid_umount_cb(uint8_t dev_addr, uint8_t instance)
{
  printf("HID device address = %d, instance = %d is unmounted\r\n", dev_addr, instance);
}

// Invoked when received report from device via interrupt endpoint
void tuh_hid_report_received_cb(uint8_t dev_addr, uint8_t instance, uint8_t const *report, uint16_t len)
{
  uint8_t const itf_protocol = tuh_hid_interface_protocol(dev_addr, instance);
  if (itf_protocol == HID_ITF_PROTOCOL_KEYBOARD)
  {
    process_kbd_report((hid_keyboard_report_t const *)report);
    TU_LOG2("HID receive boot keyboard report\r\n");
  }

  // continue to request to receive report
  if (!tuh_hid_receive_report(dev_addr, instance))
  {
    printf("Error: cannot request to receive report\r\n");
  }
}

static void process_kbd_report(hid_keyboard_report_t const *report)
{
  printf("------------------TUSB Keyboard Report ID starting ------------\n");
  printf("process Report modifier: %d, reserved: %d", report->modifier, report->reserved);
  for (int i = 0; i < 6; i++)
  {
    printf("keyboard keycode: %x\n", report->keycode[0]);
    printf("Keyboard Report ID: %x\n", report->keycode[1]);
    printf("Keyboard Report ID: %x\n", report->keycode[2]);
    printf("Keyboard Report ID: %x\n", report->keycode[3]);
    printf("Keyboard Report ID: %x\n", report->keycode[4]);
    printf("Keyboard Report ID: %x\n", report->keycode[5]);
    printf("------------------ TUSB Keyboard Report ID ending ------------\n");
  }
  queue_try_add(&hid_keyboard_report_queue, report);
  return;
}