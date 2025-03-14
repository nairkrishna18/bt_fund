/*
 * Copyright (c) 2023 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

/* STEP 3 - Include the header file of the Bluetooth LE stack
Include the following header files needed for enabling the stack,
populating the advertising data, and starting advertising.*******/
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/gap.h>

#include <dk_buttons_and_leds.h>

LOG_MODULE_REGISTER(Lesson2_Exercise1, LOG_LEVEL_INF);

#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)

#define RUN_STATUS_LED DK_LED1
#define RUN_LED_BLINK_INTERVAL 1000

/* STEP 4.1.1 - Declare the advertising packet */
static const struct bt_data ad[] =
	{
		/* STEP 4.1.2 - Set the advertising flags */
		BT_DATA_BYTES(BT_DATA_FLAGS, BT_LE_AD_NO_BREDR),

		/* STEP 4.1.3 - Set the advertising packet data  */
		BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN),

};

/* STEP 4.2.2 - Declare the URL data to include in the scan response
0x17 = https: from URI Scheme Name String Mapping to URI Scheme Prefixes
Helps us to save 6 bytes while advertising the URL
The URL is	https://academy.nordicsemi.com
******************************************************************************/
static unsigned char url_data[] = {0x17, '/', '/', 'a', 'c', 'a', 'd', 'e', 'm', 'y', '.',
								   'n', 'o', 'r', 'd', 'i', 'c', 's', 'e', 'm', 'i', '.',
								   'c', 'o', 'm'};

/* STEP 4.2.1 - Declare the scan response packet */
static const struct bt_data sd[] =
	{
		/* 4.2.3 Include the URL data in the scan response packet*/
		BT_DATA(BT_DATA_URI, url_data, sizeof(url_data)),

};

int main(void)
{
	int blink_status = 0;
	int err;

	LOG_INF("Starting Lesson 2 - Exercise 1 \n");

	err = dk_leds_init();
	if (err)
	{
		LOG_ERR("LEDs init failed (err %d)\n", err);
		return -1;
	}
	/* STEP 5 - Enable the Bluetooth LE stack ****************************************************************
	The function bt_enable() is used to enable the Bluetooth LE stack in the application.
	This function must be called before any other calls that require communication with
	the Bluetooth LE hardware (for example, start advertising).
	bt_enable() is blocking when passing NULL to it, and non-blocking if you pass a bt_ready_cb_t callback.
	**********************************************************************************************************/
	err = bt_enable(NULL);
	if (err)
	{
		LOG_ERR("Bluetooth init failed (err %d)\n", err);
		return -1;
	}
	LOG_INF("Bluetooth initialized\n");

	/* STEP 6 - Start advertising **************************************************************************
	The first parameter this function expects is the advertising parameters.
	Here, we can either use predefined macros that cover the most common cases.
	Or we can declare a variable of type bt_le_adv_param and set the parameters manually.
	For now, we will use one of the predefined macros.
	In exercise 2, we will set this parameter manually.
	Available macros are documented here.
	In our case, we will be using
	BT_LE_ADV_NCONN – non-connectable advertising with a minimum advertising interval
					  of 100 ms and a maximum advertising interval of 150 ms.

	The second and third parameters are the advertising packet (created in step 4.1) and
	its size, while the fourth and fifth parameters are the scan response (created in step 4.2) and its size.
	*******************************************************************************************************/
	err = bt_le_adv_start(BT_LE_ADV_NCONN, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
	if (err)
	{
		LOG_ERR("Advertising failed to start (err %d)\n", err);
		return -1;
	}

	LOG_INF("Advertising successfully started\n");

	for (;;)
	{
		dk_set_led(RUN_STATUS_LED, (++blink_status) % 2);
		k_sleep(K_MSEC(RUN_LED_BLINK_INTERVAL));
	}
}
