/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
/* STEP 4 - Include the header file of the logger module */
#include <zephyr/logging/log.h>

#define MAX_NUMBER_FACT 10
#define SLEEP_TIME_MS	10 * 60 * 1000

#define SW0_NODE DT_ALIAS(sw0)
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(SW0_NODE, gpios);

#define LED0_NODE DT_ALIAS(led0)
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

/* STEP 5 - Register your code with the logger module */
LOG_MODULE_REGISTER(Less4_Exer2, LOG_LEVEL_DBG);

/* STEP 7 - Replace the callback function button_pressed() */
void button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
 int i;
 long int factorial = 1;  
 
 LOG_INF("Calculating the factorials of numbers 1 to %d:",MAX_NUMBER_FACT);
 for (i = 1; i <= MAX_NUMBER_FACT; i++) {
	factorial = factorial * i;
	LOG_INF("The factorial of %2d = %ld",i,factorial);
 }
 /*Important note!
 Code in ISR runs at a high priority, therefore, 
 it should be written with timing in mind.
 Too lengthy or too complex tasks should not be performed by an ISR,
 they should be deferred	to a thread.
 */
}


static struct gpio_callback button_cb_data;

int main(void)
{
	int ret;
	/* STEP 6 - Print logging information */
	int exercise_num = 2;
	uint8_t data[] = {0x00, 0x01, 0x02, 0x03,
		0x04, 0x05, 0x06, 0x07,
		'H', 'e', 'l', 'l','o'};
	// Printf-like messages
	LOG_INF("nRF Connect SDK Fundamentals");
	LOG_INF("Exercise %d",exercise_num); 
	LOG_DBG("A log message in debug level");
	LOG_WRN("A log message in warning level!");
	LOG_ERR("A log message in Error level!");
	// Hexdump some data
	LOG_HEXDUMP_INF(data, sizeof(data),"Sample Data!"); 


	/* Only checking one since led.port and button.port point to the same device, &gpio0 */
	if (!device_is_ready(led.port)) {
		return -1;
	}

	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return -1;
	}

	ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
	if (ret < 0) {
		return -1;
	}

	ret = gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_TO_ACTIVE);

	gpio_init_callback(&button_cb_data, button_pressed, BIT(button.pin));

	gpio_add_callback(button.port, &button_cb_data);
	while (1) {
		k_msleep(SLEEP_TIME_MS);
	}
}