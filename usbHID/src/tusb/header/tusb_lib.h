#include "bsp/board.h"
#include "tusb_config.h"
#include "tusb.h"

static void process_kbd_report(hid_keyboard_report_t const *report);