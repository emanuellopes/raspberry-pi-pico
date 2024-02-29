/* This file was modified from btstack hid_keyboard_demo.c example*/

/*
 * Copyright (C) 2014 BlueKitchen GmbH
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holders nor the names of
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 * 4. Any redistribution, use, or modification is done solely for
 *    personal benefit and not for any commercial purpose or for
 *    monetary gain.
 *
 * THIS SOFTWARE IS PROVIDED BY BLUEKITCHEN GMBH AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL BLUEKITCHEN
 * GMBH OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * Please inquire about commercial licensing options at
 * contact@bluekitchen-gmbh.com
 *
 */

// *****************************************************************************
/* EXAMPLE_START(hid_keyboard_demo): HID Keyboard Classic
 *
 * @text This HID Device example demonstrates how to implement
 * an HID keyboard. Without a HAVE_BTSTACK_STDIN, a fixed demo text is sent
 * If HAVE_BTSTACK_STDIN is defined, you can type from the terminal
 */
// *****************************************************************************

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "pico/cyw43_arch.h"

#include "btstack.h"
#include "header/btstack_config.h"

#include "header/hid_descriptor.h"
#include "header/btstack_hid.h"

#include "../header/pico_queue_lib.h"
#include "../config/keyboard_config.h"

// When not set to 0xffff, sniff and sniff subrating are enabled
static uint16_t hid_ssr_host_max_latency = 1600;
static uint16_t hid_ssr_host_min_timeout = 3200;
static uint16_t hid_supervision_timeout = 3200;

// STATE

static uint8_t hid_service_buffer[300];
static uint8_t device_id_sdp_service_buffer[100];
static const char hid_device_name[] = BLE_NAME;
static btstack_packet_callback_registration_t hci_event_callback_registration;
static uint16_t hid_cid;
static uint8_t hid_boot_device = 0;

// HID Report sending
static uint8_t send_modifier;
static uint8_t send_keycode;

// HID
static btstack_timer_source_t text_timer;

static enum {
    APP_BOOTING,
    APP_NOT_CONNECTED,
    APP_CONNECTING,
    APP_CONNECTED
} app_state = APP_BOOTING;

//=======Code Modified   Begin=================
static void text_timer_handler(btstack_timer_source_t *ts)
{
    struct
    {
        uint8_t modifier;   /**< Keyboard modifier (KEYBOARD_MODIFIER_* masks). */
        uint8_t reserved;   /**< Reserved for OEM use, always set to 0. */
        uint8_t keycode[6]; /**< Key codes of the currently pressed keys. */
    } report_q; //same structure as hid_keyboard_report_t from tusb.h

    if (queue_try_remove(&hid_keyboard_report_queue, &report_q))
    {
        // for (enum ReportIds reportId = REPORT_ID_KEYBOARD; reportId <= REPORT_ID_VENDOR2; reportId++)
        // {
        //     uint8_t report[] = {
        //         0xa1, // I'm not sure if I need to use this value or another value from descriptor
        //         reportId,
        //         report_q.modifier,
        //         0,
        //         report_q.keycode[0],
        //         report_q.keycode[1],
        //         report_q.keycode[2],
        //         report_q.keycode[3],
        //         report_q.keycode[4],
        //         report_q.keycode[5]};

        //     printf("------------------ Keyboard Report ID starting ------------\n");
        //     printf("Keyboard Report ID: %x\n", report[0]);
        //     printf("Keyboard Report ID: %x\n", report[1]);
        //     printf("Keyboard Report ID: %x\n", report[2]);
        //     printf("Keyboard Report ID: %x\n", report[3]);
        //     printf("Keyboard Report ID: %x\n", report[4]);
        //     printf("Keyboard Report ID: %x\n", report[5]);
        //     printf("Keyboard Report ID: %x\n", report[6]);
        //     printf("Keyboard Report ID: %x\n", report[7]);
        //     printf("Keyboard Report ID: %x\n", report[8]);
        //     printf("Keyboard Report ID: %x\n", report[9]);
        //     printf("------------------ Keyboard Report ID ending ------------\n");

        // }
        uint8_t report[] = {
                0xa1, // I'm not sure if I need to use this value or another value from descriptor
                REPORT_ID_KEYBOARD,
                report_q.modifier,
                0,
                report_q.keycode[0],
                report_q.keycode[1],
                report_q.keycode[2],
                report_q.keycode[3],
                report_q.keycode[4],
                report_q.keycode[5]};

        hid_device_send_interrupt_message(hid_cid, &report[0], sizeof(report));

    }
    hid_device_request_can_send_now_event(hid_cid);
}

//=======Code Modified   End===============
static void packet_handler(uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t packet_size)
{
    UNUSED(channel);
    UNUSED(packet_size);
    uint8_t status;
    switch (packet_type)
    {
    case HCI_EVENT_PACKET:
        switch (hci_event_packet_get_type(packet))
        {
        case BTSTACK_EVENT_STATE:
            if (btstack_event_state_get_state(packet) != HCI_STATE_WORKING)
                return;
            app_state = APP_NOT_CONNECTED;
            break;

        case HCI_EVENT_USER_CONFIRMATION_REQUEST:
            // ssp: inform about user confirmation request
            log_info("SSP User Confirmation Request with numeric value '%06" PRIu32 "'\n", hci_event_user_confirmation_request_get_numeric_value(packet));
            log_info("SSP User Confirmation Auto accept\n");
            break;

        case HCI_EVENT_HID_META:
            switch (hci_event_hid_meta_get_subevent_code(packet))
            {
            case HID_SUBEVENT_CONNECTION_OPENED:
                status = hid_subevent_connection_opened_get_status(packet);
                if (status != ERROR_CODE_SUCCESS)
                {
                    // outgoing connection failed
                    printf("Connection failed, status 0x%x\n", status);
                    app_state = APP_NOT_CONNECTED;
                    hid_cid = 0;
                    return;
                }
                app_state = APP_CONNECTED;
                hid_cid = hid_subevent_connection_opened_get_hid_cid(packet);
                btstack_run_loop_set_timer_handler(&text_timer, text_timer_handler);
                btstack_run_loop_set_timer(&text_timer, TEXT_PERIOD_MS);
                btstack_run_loop_add_timer(&text_timer);
                cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
                break;
            case HID_SUBEVENT_CONNECTION_CLOSED:
                printf("HID Disconnected\n");
                app_state = APP_NOT_CONNECTED;
                hid_cid = 0;
                cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
                break;
            case HID_SUBEVENT_CAN_SEND_NOW:
                btstack_run_loop_set_timer_handler(&text_timer, text_timer_handler);
                btstack_run_loop_set_timer(&text_timer, TEXT_PERIOD_MS);
                btstack_run_loop_add_timer(&text_timer);

                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}

void btstack_main()
{
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
    sleep_ms(200);
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
    sleep_ms(200);
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);

    // allow to get found by inquiry
    gap_discoverable_control(1);
    // use Limited Discoverable Mode; Peripheral; Keyboard as CoD
    gap_set_class_of_device(HID_DEVICE_SUBCLASS);
    // Name of device
    gap_set_local_name(hid_device_name);
    // allow for role switch in general and sniff mode
    gap_set_default_link_policy_settings(LM_LINK_POLICY_ENABLE_ROLE_SWITCH | LM_LINK_POLICY_ENABLE_SNIFF_MODE);
    // allow for role switch on outgoing connections - this allow HID Host to become master when we re-connect to it
    gap_set_allow_role_switch(true);

    // L2CAP
    l2cap_init();

    // SDP Server
    sdp_init();
    memset(hid_service_buffer, 0, sizeof(hid_service_buffer));

    uint8_t hid_virtual_cable = 0;
    uint8_t hid_remote_wake = 1;
    uint8_t hid_reconnect_initiate = 1;
    uint8_t hid_normally_connectable = 1;

    hid_sdp_record_t hid_params = {
        // hid sevice subclass 2540 Keyboard, hid counntry code 33 US
        HID_DEVICE_SUBCLASS, // hid_device_subclass
        KEYBOARD_COUNTRY,    // hid_country_code
        hid_virtual_cable,
        hid_remote_wake,
        hid_reconnect_initiate,
        hid_normally_connectable,
        hid_boot_device,
        hid_ssr_host_max_latency,
        hid_ssr_host_min_timeout,
        hid_supervision_timeout,
        hid_descriptor_keyboard,
        sizeof(hid_descriptor_keyboard),
        hid_device_name};

    hid_create_sdp_record(hid_service_buffer, 0x10001, &hid_params);

    printf("HID service record size: %u\n", de_get_len(hid_service_buffer));
    sdp_register_service(hid_service_buffer);

    // See https://www.bluetooth.com/specifications/assigned-numbers/company-identifiers if you don't have a USB Vendor ID and need a Bluetooth Vendor ID
    // device info: BlueKitchen GmbH, product 1, version 1
    device_id_create_sdp_record(device_id_sdp_service_buffer, 0x10003, DEVICE_ID_VENDOR_ID_SOURCE_BLUETOOTH, BLUETOOTH_COMPANY_ID_BLUEKITCHEN_GMBH, 1, 1);
    printf("Device ID SDP service record size: %u\n", de_get_len((uint8_t *)device_id_sdp_service_buffer));
    sdp_register_service(device_id_sdp_service_buffer);

    // HID Device
    hid_device_init(hid_boot_device, sizeof(hid_descriptor_keyboard), hid_descriptor_keyboard);

    // register for HCI events
    hci_event_callback_registration.callback = &packet_handler;
    hci_add_event_handler(&hci_event_callback_registration);

    // register for HID events
    hid_device_register_packet_handler(&packet_handler);

    // turn on!
    hci_power_control(HCI_POWER_ON);
}