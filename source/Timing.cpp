
#include "../Headers/Timing.h"

struct tms startTime;
struct tms endTime;
float clockTicksPerSecond;
float startTimeSeconds;
float endTimeSeconds;


 void StartTiming(){
     times(&startTime);
 }
 void EndTiming(){
     times(&endTime);
 }

 float ComputeTime(){
     clockTicksPerSecond = (double) sysconf(_SC_CLK_TCK);
     startTimeSeconds = startTime.tms_utime/clockTicksPerSecond;
     endTimeSeconds = endTime.tms_utime/clockTicksPerSecond;
     return endTimeSeconds - startTimeSeconds;
 }