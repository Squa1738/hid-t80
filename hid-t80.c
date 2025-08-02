#include <linux/input-event-codes.h>
#include <linux/module.h>
#include <linux/hid.h>
#include <linux/input.h>
#include "hid-t80.h"

MODULE_AUTHOR("Gabriell Simic");
MODULE_DESCRIPTION("Thrustmaster T80 Wheel Driver");

static int t80_input_configured(struct hid_device *hdev, struct hid_input *hidinput)
{
    struct input_dev *input = hidinput->input;

    __clear_bit(EV_ABS, input->evbit);
    memset(input->evbit, 0, sizeof(input->evbit));
    memset(input->absbit, 0, sizeof(input->absbit));
    memset(input->keybit, 0, sizeof(input->keybit));

    input_set_abs_params(input, ABS_X, 0, 65535, 0, 0);
    input_set_abs_params(input, ABS_Y, 0, 65535, 0, 0);
    input_set_abs_params(input, ABS_Z, 0, 65535, 0, 0);

    __set_bit(EV_ABS, input->evbit);
    __set_bit(EV_KEY, input->evbit);

    __set_bit(ABS_X, input->absbit);
    __set_bit(ABS_Y, input->absbit);
    __set_bit(ABS_Z, input->absbit);

    __set_bit(BTN_SOUTH,  input->keybit);
    __set_bit(BTN_EAST,   input->keybit);
    __set_bit(BTN_WEST,   input->keybit);
    __set_bit(BTN_NORTH,  input->keybit);

    __set_bit(BTN_TL,     input->keybit);
    __set_bit(BTN_TR,     input->keybit);
    __set_bit(BTN_TL2,    input->keybit);
    __set_bit(BTN_TR2,    input->keybit);

    __set_bit(BTN_THUMBL, input->keybit);
    __set_bit(BTN_THUMBR, input->keybit);

    __set_bit(BTN_START,  input->keybit);
    __set_bit(BTN_SELECT, input->keybit);

    __set_bit(BTN_DPAD_UP,    input->keybit);
    __set_bit(BTN_DPAD_DOWN,  input->keybit);
    __set_bit(BTN_DPAD_LEFT,  input->keybit);
    __set_bit(BTN_DPAD_RIGHT, input->keybit);

    hid_info(hdev, "T80 racing wheel configured\n");
    return 0;
}

static int t80_raw_event(struct hid_device *hdev, struct hid_report *report,
                         u8 *data, int size)
{
    struct hid_input *hidinput;
    struct input_dev *input;
    u16 steering, gas, brake;

    if (size < 49)
        return 0;

    if (list_empty(&hdev->inputs)) {
        hid_err(hdev, "no inputs found\n");
        return -ENODEV;
    }

    hidinput = list_first_entry(&hdev->inputs, struct hid_input, list);
    input = hidinput->input;

    // Original steering logic
    steering = (s16)((data[44] << 8) | data[43]);
    gas = (data[46] << 8) | data[45];
    brake = (data[48] << 8) | data[47];

    input_report_abs(input, ABS_X, steering);
    input_report_abs(input, ABS_Y, gas);
    input_report_abs(input, ABS_Z, brake);

    u8 b5 = data[5], b6 = data[6], b7 = data[7], b8 = data[8], b9 = data[9];

    // DEBUG: Uncomment this to see what buttons do what
     if (b5 || b6 || b7 || b8 || b9) {
         hid_info(hdev, "Buttons: b5=%02x b6=%02x b7=%02x b8=%02x b9=%02x\n", 
                  b5, b6, b7, b8, b9);
     }
    // Parse D-pad as hat switch
    u8 dpad = b5 & 0x0F;  // 0 = up, 1 = up+right, ..., 7 = up+left, 8 = neutral
    input_report_key(input, BTN_DPAD_UP,    dpad == 0 || dpad == 1 || dpad == 7);
    input_report_key(input, BTN_DPAD_RIGHT, dpad == 1 || dpad == 2 || dpad == 3);
    input_report_key(input, BTN_DPAD_DOWN,  dpad == 3 || dpad == 4 || dpad == 5);
    input_report_key(input, BTN_DPAD_LEFT,  dpad == 5 || dpad == 6 || dpad == 7);

    input_report_key(input, BTN_WEST,  b5 & 0x80);
    input_report_key(input, BTN_SOUTH, b5 & 0x20);
    input_report_key(input, BTN_EAST,  b5 & 0x40);
    input_report_key(input, BTN_NORTH, b5 & 0x10);

    input_report_key(input, BTN_TL,     b6 & 0x01);
    input_report_key(input, BTN_TR,     b6 & 0x02);
    input_report_key(input, BTN_TL2,    b6 & 0x04);
    input_report_key(input, BTN_TR2,    b6 & 0x08);
    input_report_key(input, BTN_SELECT, b6 & 0x10);
    input_report_key(input, BTN_START,  b6 & 0x20);
    input_report_key(input, BTN_THUMBR, b6 & 0x80);
    input_report_key(input, BTN_THUMBL, b6 & 0x40);

    input_sync(input);
    return 1;
}

static struct hid_driver t80_driver = {
    .name = "t80",
    .id_table = t80_devices,
    .input_configured = t80_input_configured,
    .raw_event = t80_raw_event,
};

module_hid_driver(t80_driver);

