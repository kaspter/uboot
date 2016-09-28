/*
 * (C) Copyright 2016 Kaspter Ju <kaspter@kmatt.net>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <spl.h>

void board_boot_order(u32 *spl_boot_list)
{
	/* eMMC prior to sdcard. */
	spl_boot_list[0] = BOOT_DEVICE_MMC1;
	spl_boot_list[1] = BOOT_DEVICE_MMC2;
}
