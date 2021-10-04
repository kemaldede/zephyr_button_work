/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <drivers/gpio.h>
#include <sys/printk.h>

#define SLEEP_TIME_MS 2000

#define GPIO_PORT	"GPIO_0"
// #define GPIO_PORT DT_LABEL(DT_NODELABEL(gpio0))

#define	BUTTON1_PIN	13
#define BUTTON2_PIN	14

#define LED1_PIN	17
#define LED2_PIN	18	

static struct gpio_callback button1_cb_data;
static struct gpio_callback button2_cb_data;

static struct k_work button1_work;
static struct k_work button2_work;

const struct device *my_gpio_port;


void button1_pressed(const struct device *port,
					struct gpio_callback *cb,
					gpio_port_pins_t pins)
{
	int ret;
	printk("Button1 work is pressed\n");
	ret = k_work_submit(&button1_work);
	if(ret == 0)
	{
		printk("Button1 work is already in queue\n");
	}
}


void button2_pressed(const struct device *port,
					struct gpio_callback *cb,
					gpio_port_pins_t pins)
{
	int ret;
	printk("Button2 work is pressed\n");
	ret = k_work_submit(&button2_work);
	if(ret == 0)
	{
		printk("Button2 work is already in queue\n");
	}
}

void button1_work_handler(struct k_work *work)
{
	gpio_pin_toggle(my_gpio_port,LED1_PIN);
}

void button2_work_handler(struct k_work *work)
{
	gpio_pin_toggle(my_gpio_port,LED2_PIN);
}


void configure_gpios()
{
	int ret;

	my_gpio_port = device_get_binding(GPIO_PORT);

	gpio_pin_configure(my_gpio_port,BUTTON1_PIN , GPIO_INPUT | GPIO_PULL_UP);
	gpio_pin_configure(my_gpio_port,BUTTON2_PIN , GPIO_INPUT | GPIO_PULL_UP);

	gpio_pin_configure(my_gpio_port, LED1_PIN , GPIO_OUTPUT | GPIO_OUTPUT_ACTIVE);
	gpio_pin_configure(my_gpio_port, LED2_PIN , GPIO_OUTPUT | GPIO_OUTPUT_ACTIVE);

	gpio_init_callback(&button1_cb_data, button1_pressed, BIT(BUTTON1_PIN));
	gpio_add_callback(my_gpio_port, &button1_cb_data);
	gpio_init_callback(&button2_cb_data, button2_pressed, BIT(BUTTON2_PIN));
	gpio_add_callback(my_gpio_port, &button2_cb_data);

	gpio_pin_interrupt_configure(my_gpio_port, BUTTON1_PIN, GPIO_INT_EDGE_TO_INACTIVE);
	gpio_pin_interrupt_configure(my_gpio_port, BUTTON2_PIN, GPIO_INT_EDGE_TO_INACTIVE);

	k_work_init(&button1_work, button1_work_handler);
	k_work_init(&button2_work, button2_work_handler);
}

void main(void)
{
	printk("Hello World! %s\n", CONFIG_BOARD);

	configure_gpios();
}
