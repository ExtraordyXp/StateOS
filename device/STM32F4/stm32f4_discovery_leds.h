/******************************************************************************
 * @file    stm32f4_discovery_leds.h
 * @author  Rajmund Szymanski
 * @date    24.12.2018
 * @brief   This file contains definitions for STM32F4-Discovery Kit.
 ******************************************************************************/

#ifndef __STM32F4_DISCOVERY_LEDS_H
#define __STM32F4_DISCOVERY_LEDS_H

#include <stdbool.h>
#include <stm32f4_io.h>

#ifdef  __cplusplus
extern "C" {
#endif//__cplusplus

#define GRN      USB_LED
#define GRN_Init USB_LED_Init

/* -------------------------------------------------------------------------- */

#define     LED (BITBAND(GPIOD->ODR)+12) // leds array
#define     LEDG BITBAND(GPIOD->ODR)[12] // green led
#define     LEDO BITBAND(GPIOD->ODR)[13] // orange led
#define     LEDR BITBAND(GPIOD->ODR)[14] // red led
#define     LEDB BITBAND(GPIOD->ODR)[15] // blue led

struct    __LEDs { uint16_t: 12; volatile uint16_t f: 4; uint16_t: 0; uint32_t alignment_for_gcc8; };

#define     LEDs (((struct __LEDs *)&(GPIOD->ODR))->f)

#define USB_LED  BITBAND(GPIOA->ODR)[ 9] // usb green led

/* -------------------------------------------------------------------------- */

// init leds (PD12..PD15) as pushpull output

static inline
void LED_Init( void )
{
	GPIO_Init(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15, GPIO_Output_PushPull);
}

/* -------------------------------------------------------------------------- */

// get user led state

static inline
bool LED_Get( unsigned nr )
{
	switch (nr)
	{
	case  0: return (GPIOD->ODR & GPIO_ODR_OD12) != 0;
	case  1: return (GPIOD->ODR & GPIO_ODR_OD13) != 0;
	case  2: return (GPIOD->ODR & GPIO_ODR_OD14) != 0;
	case  3: return (GPIOD->ODR & GPIO_ODR_OD15) != 0;
	default: return false;
	}
}

/* -------------------------------------------------------------------------- */

// set user led state

static inline
void LED_Set( unsigned nr )
{
	switch (nr)
	{
	case  0: GPIOD->BSRR = GPIO_BSRR_BS12; break;
	case  1: GPIOD->BSRR = GPIO_BSRR_BS13; break;
	case  2: GPIOD->BSRR = GPIO_BSRR_BS14; break;
	case  3: GPIOD->BSRR = GPIO_BSRR_BS15; break;
	default: break;
	}
}

/* -------------------------------------------------------------------------- */

// reset user led state

static inline
void LED_Reset( unsigned nr )
{
	switch (nr)
	{
	case  0: GPIOD->BSRR = GPIO_BSRR_BR12; break;
	case  1: GPIOD->BSRR = GPIO_BSRR_BR13; break;
	case  2: GPIOD->BSRR = GPIO_BSRR_BR14; break;
	case  3: GPIOD->BSRR = GPIO_BSRR_BR15; break;
	default: break;
	}
}

/* -------------------------------------------------------------------------- */

// toggle user led state

static inline
void LED_Toggle( unsigned nr )
{
	switch (nr)
	{
	case  0: GPIOD->ODR ^= GPIO_ODR_OD12; break;
	case  1: GPIOD->ODR ^= GPIO_ODR_OD13; break;
	case  2: GPIOD->ODR ^= GPIO_ODR_OD14; break;
	case  3: GPIOD->ODR ^= GPIO_ODR_OD15; break;
	default: break;
	}
}

/* -------------------------------------------------------------------------- */

// rotate leds

static inline
void LED_Tick( void )
{
	unsigned leds = (GPIOD->ODR << 1) & 0xE000;
	GPIOD->BSRR = 0xF0000000;
	GPIOD->BSRR = leds ? leds : 0x1000;
}

/* -------------------------------------------------------------------------- */

// init usb green led (PA9)

static inline
void USB_LED_Init( void )
{
	GPIO_Init(GPIOA, GPIO_Pin_9, GPIO_Output_PushPull);
}

/* -------------------------------------------------------------------------- */

// get usb green led state

static inline
bool USB_LED_Get( void )
{
	return (GPIOA->ODR & GPIO_ODR_OD9) != 0;
}

/* -------------------------------------------------------------------------- */

// set usb green led state

static inline
void USB_LED_Set( void )
{
	GPIOA->BSRR = GPIO_BSRR_BS9;
}

/* -------------------------------------------------------------------------- */

// reset usb green led state

static inline
void USB_LED_Reset( void )
{
	GPIOA->BSRR = GPIO_BSRR_BR9;
}

/* -------------------------------------------------------------------------- */

// toggle usb green led state

static inline
void USB_LED_Toggle( void )
{
	GPIOA->ODR ^= GPIO_ODR_OD9;
}

/* -------------------------------------------------------------------------- */

#ifdef  __cplusplus
}
#endif//__cplusplus

/* -------------------------------------------------------------------------- */

#ifdef  __cplusplus

#define GreenLed USB_Led

/* -------------------------------------------------------------------------- */

struct Led
{
	Led( void ) { LED_Init(); }

	bool get   ( unsigned nr ) { return LED_Get(nr);    }
	void set   ( unsigned nr ) {        LED_Set(nr);    }
	void reset ( unsigned nr ) {        LED_Reset(nr);  }
	void toggle( unsigned nr ) {        LED_Toggle(nr); }
	void tick  ( void )        {        LED_Tick();     }

	unsigned   operator = ( const unsigned status ) { return LEDs = status; }
	unsigned & operator []( const unsigned number ) { return (unsigned &)LED[number]; }
};

/* -------------------------------------------------------------------------- */

struct USB_Led
{
	USB_Led( void ) { USB_LED_Init(); }

	bool get   ( void ) { return USB_LED_Get   (); }
	void set   ( void ) {        USB_LED_Set   (); }
	void reset ( void ) {        USB_LED_Reset (); }
	void toggle( void ) {        USB_LED_Toggle(); }

	operator   unsigned & ( void )                  { return (unsigned &) GRN; }
	unsigned   operator = ( const unsigned status ) { return USB_LED = status; }
};

/* -------------------------------------------------------------------------- */

#endif//__cplusplus

#endif//__STM32F4_DISCOVERY_LEDS_H
