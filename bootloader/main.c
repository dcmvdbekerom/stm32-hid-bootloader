/*
* STM32 HID Bootloader - USB HID bootloader for STM32F10X
* Copyright (c) 2018 Bruno Freitas - bruno@brunofreitas.com
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stm32f4xx.h>

#include "usb.h"
#include "hid.h"
#include "bitwise.h"

// HID Bootloader takes 4K
#define USER_PROGRAM 0x08001000

int main() {
	uint32_t usrSp = *(volatile uint32_t *)USER_PROGRAM;
	uint32_t usrMain = *(volatile uint32_t *)(USER_PROGRAM + 0x04); /* reset ptr in vector table */

	// Turn GPIOB clock on
	//bit_set(RCC->APB2ENR, RCC_APB2ENR_IOPBEN);
    
    //Turn GPIOC clock on
    bit_set(RCC->AHB1ENR, RCC_AHB1ENR_GPIOCEN);

	// Set B2 as Input Mode Floating
  
	//bit_clear(GPIOB->CRL, GPIO_CRL_MODE2); //input
	//bit_set  (GPIOB->CRL, GPIO_CRL_CNF2_0);
	//bit_clear(GPIOB->CRL, GPIO_CRL_CNF2_1);

    // Set C13 to input (no 'floating' available)
    bit_clear(GPIOC->MODER, GPIO_MODER_MODER13_0);    
    bit_clear(GPIOC->MODER, GPIO_MODER_MODER13_1);    

	// If B2 (BOOT1) is HIGH then go into HID bootloader...
	if(GPIOC->IDR & GPIO_IDR_ID13) {
		USB_Init(HIDUSB_EPHandler, HIDUSB_Reset);
	} else {
		// Turn GPIOC clock off
		bit_clear(RCC->AHB1ENR, RCC_AHB1ENR_GPIOCEN);

		SCB->VTOR = USER_PROGRAM;

		__asm__ volatile(
			"msr msp, %0\n"
			"bx %1\n"
			:: "r" (usrSp), "r" (usrMain));
	}

	for(;;);
}
