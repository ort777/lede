/*
 * Ruijie RG-EST310
 *
 * Copyright (c) 2013-2015 The Linux Foundation. All rights reserved.
 * Copyright (c) 2012 Gabor Juhos <juhosg@openwrt.org>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 */

#include <linux/platform_device.h>
#include <linux/gpio.h>
#include <linux/ath9k_platform.h>
#include <linux/ar8216_platform.h>

#include <asm/mach-ath79/ath79.h>
#include <asm/mach-ath79/ar71xx_regs.h>

#include "common.h"
#include "dev-ap9x-pci.h"
#include "dev-eth.h"
#include "dev-gpio-buttons.h"
#include "dev-leds-gpio.h"
#include "dev-m25p80.h"
#include "dev-spi.h"
#include "dev-usb.h"
#include "dev-wmac.h"
#include "machtypes.h"
#include "pci.h"

// #define EST310_GPIO_LED_WLAN		0
#define EST310_GPIO_BTN_RESET		1

#define EST310_KEYS_POLL_INTERVAL	20	/* msecs */
#define EST310_KEYS_DEBOUNCE_INTERVAL	(3 * EST310_KEYS_POLL_INTERVAL)

#define EST310_MAC0_OFFSET		0x0000
// #define EST310_MAC1_OFFSET		0x0006
#define EST310_WMAC_OFFSET	0x000C
#define EST310_CALDATA_OFFSET	0x5000

static struct gpio_keys_button est310_gpio_keys[] __initdata = {
	{
		.desc		= "Reset",
		.type		= EV_KEY,
		.code		= KEY_WPS_BUTTON,
		.debounce_interval = EST310_KEYS_DEBOUNCE_INTERVAL,
		.gpio		= EST310_GPIO_BTN_RESET,
		.active_low	= 1,
	},
};

// static struct gpio_led est310_leds_gpio[] __initdata = {
// 	{
// 		.name		= "est310:green:wlan",
// 		.gpio		= EST310_GPIO_LED_WLAN,
// 		.active_low	= 1,
// 	}
// };

static void __init est310_gpio_key_setup(void)
{
	ath79_register_gpio_keys_polled(-1, EST310_KEYS_POLL_INTERVAL,
			ARRAY_SIZE(est310_gpio_keys),
			est310_gpio_keys);
}

// static void __init est310_gpio_led_setup(void)
// {
// 	ath79_gpio_direction_select(EST310_GPIO_LED_WAN, true);

// /* Mute LEDs on boot */
// gpio_set_value(EST310_GPIO_LED_WLAN, 1);

//	ath79_gpio_output_select(EST310_GPIO_LED_WLAN,0);

// 	ath79_register_leds_gpio(-1, ARRAY_SIZE(est310_leds_gpio),
// 			est310_leds_gpio);
// }

static void __init est310_setup(void)
{
	u8 *art = (u8 *) KSEG1ADDR(0x1fff0000);

	/* Disable JTAG, enabling GPIOs 0-3 */
	ath79_gpio_function_setup(AR934X_GPIO_FUNC_JTAG_DISABLE, 0);

	ath79_register_m25p80(NULL);

	est310_gpio_key_setup();

// 	est310_gpio_led_setup();

// 	ath79_register_usb();

// 	ath79_wmac_set_led_pin(EST310_GPIO_LED_WLAN);

//	ath79_register_wmac(art + EST310_CALDATA_OFFSET, art + EST310_WMAC_OFFSET);

	ath79_setup_ar933x_phy4_switch(false, false);

	ath79_register_mdio(0, 0x0);

// 	ath79_init_mac(ath79_eth0_data.mac_addr, art + EST310_MAC0_OFFSET, 0);
 	ath79_init_mac(ath79_eth1_data.mac_addr, art + EST310_MAC0_OFFSET, 0);
	ap91_pci_init(art + EST310_CALDATA_OFFSET, art + EST310_WMAC_OFFSET);

	/* WAN port */
// 	ath79_eth0_data.phy_if_mode = PHY_INTERFACE_MODE_MII;
// 	ath79_eth0_data.speed = SPEED_100;
// 	ath79_eth0_data.duplex = DUPLEX_FULL;
// 	ath79_eth0_data.phy_mask = BIT(4);
// 	ath79_register_eth(0);

	/* LAN ports */
	ath79_eth1_data.phy_if_mode = PHY_INTERFACE_MODE_GMII;
	ath79_eth1_data.speed = SPEED_1000;
	ath79_eth1_data.duplex = DUPLEX_FULL;
// 	ath79_switch_data.phy_poll_mask |= BIT(4);
// 	ath79_switch_data.phy4_mii_en = 1;
	ath79_register_eth(1);
}

MIPS_MACHINE(ATH79_MACH_RG_EST310, "RG-EST310", "Ruijie RG-EST310", est310_setup);
