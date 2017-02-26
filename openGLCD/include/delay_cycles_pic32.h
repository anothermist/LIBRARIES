/*
 *  delay_cycles_pic32.h
 *
 * Provides accurate cycle delays for a given number of i/o clocks.
 * This version is for the pic32 and is part of the openGLCD library.
 */


#ifndef _DELAY_CYCLES_PIC32_H_
#define _DELAY_CYCLES_PIC32_H_

#include <inttypes.h>

/*
 *
 *  _delay_cycles(uint32 _IOticks)
 *
 *  Perform an accurate delay of a given number of i/o cycles.
 *  NOTE: i/o cycles are half the processor pic32 clock
 *
 */

#ifdef MAGICDEFINE_GOES_HERE
// Try to figure this out someday
// In the mean time, without this magic define, MPIDE must have Wsystem.c
// which started in the 2013-07-15 release

/*
 * plib is no longer used in newer rleases,
 * for the older releases we use the plib ReadCoreTimer() function
 */

#ifdef __cplusplus
extern "C" {
#endif
#include <peripheral/timer.h>
#ifdef __cplusplus
}
#endif

#define readCoreTimer() ReadCoreTimer()
#endif


static __inline__ void
_delay_cycles(uint32_t _IOticks)
{
#if defined(ARDUINO)
uint32_t startTicks = readCoreTimer();

	while((readCoreTimer() - startTicks) < _IOticks)
	{
		; // do nothing, while spinning
	}
#else

	/*
	 * About the same delay as above
	 */
	_IOticks /= 2;
	while(_IOticks--)
	{
		Nop();
	}
#endif
}

#endif /* _DELAY_CYCLES_PIC32 */
