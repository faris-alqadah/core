/*______________________________________________________________________________
 _______________________________________________________________________________
 *@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 * Author: Faris Alqadah, Copyright 2008
 * This program is available for only academic use. Commercial use is not allowed.
 * Modification and re-distribution is permited only for academic use.
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 *  Timing
 *
 *
 *
 *______________________________________________________________________________
 *_____________________________________________________________________________*/
#ifndef _TIMING_H
#define	_TIMING_H
#include<time.h>
#include <sys/times.h>
#include <unistd.h>

extern struct tms startTime;
extern struct tms endTime;
extern float clockTicksPerSecond;
extern float startTimeSeconds;
extern float endTimeSeconds;

 
 void StartTiming();
 void EndTiming();
 
 float ComputeTime();
#endif	/* _TIMING_H */

