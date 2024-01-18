 static void process_kbd_report(hid_keyboard_report_t const *report)
{
     queue_try_add(&hid_keyboard_report_queue, report);
     return;
}