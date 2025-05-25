#ifndef __CONFIG_H
#define __CONFIG_H

// #if defined STM32F103xx


#if defined STM32F466xx

    #define BOOT_PORT
    #define BOOT_PIN

    #define LED1_PORT 
    #define LED1_PIN

    #define USB_PORT    PORTA
    #define USB_PIN_DP  GPIO_P
    #define USB_PIN_DM

// #elif defined STM32G0B1xx



#else
	#error "No config for this target"
#endif