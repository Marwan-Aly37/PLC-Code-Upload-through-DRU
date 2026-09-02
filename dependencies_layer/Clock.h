//
//                       FILE NAME : Clock.h
//
//                       MODULE    : PLatform
//
//                       PROJECT   : 3P_Meter
//
//=============================================================================
//
//    This is a SEWEDY's document. Reproduction and Disclosure are forbidden
//
//=============================================================================
#ifndef Clock_H
#define Clock_H

#include "config.h"

typedef enum
{
  begin,
  end
} tEnumDayLightSavingBeginEnd;


extern TstDateTime DateTime;

void UpdateDateTime(void);
void GetTimeStamp(TstDateTime*);
TU08 Clock_Get_TimeZone(TS16*);
TU08 Clock_Get_DateTimeOfDayLightSaving(tEnumDayLightSavingBeginEnd, TstDateTime*);
TU08 Clock_Get_DayLightSavingDeviation(TS08*);
TU08 Clock_Get_DayLightSavingEnable(TBOOL*);
TU08 Clock_Get_ClockBase(TClockBase*);

TU08 Clock_Set_TimeZone(TS16);
TU08 Clock_Set_DateTimeOfDayLightSaving(tEnumDayLightSavingBeginEnd, TstDateTime*);
TU08 Clock_Set_DayLightSavingDeviation(TS08 pDayLightSavingDeviation);
TU08 Clock_Set_DayLightSavingEnable(TBOOL);

TU08 Set_DateTimeIntoRTC(TstDateTime *stDateTime);
TU08 Get_DateTimeFromRTC(TstDateTime *pstDateTime);

TU08 Clock_dateTimeCompare(TstDateTime FirstDateTime, TstDateTime SecondDateTime);
TS64 Clock_dateTimeToSeconds(TstDateTime dateTime);
TU08 Clock_TimeCompare(TstTime FirstTime, TstTime SecondTime);
TU32 Clock_TimeToSeconds(TstTime Time);
void Clock_Init(void);
void Clock_gGetDateTime(TstDateTime* pstDateTime);

void Clock_Manager(void);
#endif /* Clock_H */
