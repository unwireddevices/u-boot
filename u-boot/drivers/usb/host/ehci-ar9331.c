/*
 * Atheros AR933x USB driver initialization
 *
 * Copyright (C) 2014 Mantas Pucka <mantas@8devices.com>
 *
 * SPDX-License-Identifier:GPL-2.0
 */

#include <common.h>
#include <pci.h>
#include <usb.h>
#include <asm/io.h>
#include <usb/ehci-fsl.h>

#include <asm/mipsregs.h>
#include "ar7240_soc.h"

#include "ehci.h"

#define mdelay(_x) udelay((_x) * 1000)

/*
 * Create the appropriate control structures to manage
 * a new EHCI host controller.
 *
 * Excerpts from linux ehci fsl driver.
 */

int ehci_hcd_init(int index, struct ehci_hccr **hccr, struct ehci_hcor **hcor)
{
	struct usb_ehci *ehci;

	#define AR933X_EHCI_BASE		0x1b000000
	#define AR933X_RESET_USB_HOST		BIT(5)
	#define AR933X_RESET_USB_PHY		BIT(4)
	#define AR933X_RESET_USBSUS_OVERRIDE	BIT(3)
	
	ar7240_reg_wr (AR7240_RESET, (ar7240_reg_rd(AR7240_RESET) | AR933X_RESET_USBSUS_OVERRIDE));
	mdelay(10);

	ar7240_reg_wr (AR7240_RESET, (ar7240_reg_rd(AR7240_RESET) & ~(AR933X_RESET_USB_HOST)));
	mdelay(10);

	ar7240_reg_wr (AR7240_RESET, (ar7240_reg_rd(AR7240_RESET) & ~(AR933X_RESET_USB_PHY)));
	mdelay(10);
	
	ehci = (struct usb_ehci *)AR933X_EHCI_BASE;
	*hccr = (struct ehci_hccr *)((uint32_t)&ehci->caplength);
	*hcor = (struct ehci_hcor *)((uint32_t) *hccr +
			HC_LENGTH(ehci_readl(&(*hccr)->cr_capbase)));
	debug("ehci-ar9331: init hccr %x and hcor %x hc_length %d\n",
		(uint32_t)*hccr, (uint32_t)*hcor,
		(uint32_t)HC_LENGTH(ehci_readl(&(*hccr)->cr_capbase)));

	return 0;
}

/*
 * Destroy the appropriate control structures corresponding
 * the the EHCI host controller.
 */
int ehci_hcd_stop(int index)
{
	return 0;
}
