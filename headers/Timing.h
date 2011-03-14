/*______________________________________________________________________________
 _______________________________________________________________________________
 *@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 * Author: Faris Alqadah, Copyright 2008
 * This program is available for only academic use. Commercial use is not allowed.
 * Modification and re-distribution is permited only for academic use.
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 *  Use this class to time different components of your program
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

extern struct tms startTime; //when timing began
extern struct tms endTime; //when timiing ended
extern float clockTicksPerSecond; //what are the units ?
extern float startTimeSeconds; //start time converted to seconds
extern float endTimeSeconds; //end time converted to seconds

/********************************************************************/
// StartTimin(): start the timing process
//  Pre-Condition: none
//  Post-Condition: the startTime variable is set
//  returns: none
//  output: none
/********************************************************************/
 void StartTiming();

/********************************************************************/
// EndTiming(): end the timing process
//  Pre-Condition: none
//  Post-Condition: the endTime variable is set
//  returns: none
//  output: none
/********************************************************************/
 void EndTiming();

 /********************************************************************/
// ComputeTime(): compute the time between a start and end
//  Pre-Condition: StarTiming() and EndTimint() were called
//  Post-Condition: none
//  returns: the time in seconds between StartTiming and EndTiming
//  output: none
/********************************************************************/
 float ComputeTime();
 
#endif	/* _TIMING_H */

