//==============================================================================
//    Copyright 1998,1999,2000,2001,2004 Petr Kulhanek
//
//    This file is part of HiPoLy library.
//
//    This library is free software; you can redistribute it and/or
//    modify it under the terms of the GNU Lesser General Public
//    License as published by the Free Software Foundation; either
//    version 2.1 of the License, or (at your option) any later version.
//
//    This library is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//    Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public
//    License along with this library; if not, write to the Free Software
//    Foundation, Inc., 51 Franklin Street, Fifth Floor,
//    Boston, MA  02110-1301  USA
//==============================================================================

#include <stdio.h>
#include <string.h>
#include <SmallTimeAndDate.hpp>
#include <SmallDate.hpp>
#include <SmallTime.hpp>

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CSmallTimeAndDate::CSmallTimeAndDate(void)
{
    Clear();
}

// -----------------------------------------------------------------------------

CSmallTimeAndDate::CSmallTimeAndDate(const tm& timedate)
{
    TimeDate = timedate;
}

//------------------------------------------------------------------------------

CSmallTimeAndDate::CSmallTimeAndDate(const int seconds_from_beginning)
{
    time_t timer = seconds_from_beginning;
    tm     *tblock;
    tblock = localtime(&timer);
    memcpy(&TimeDate,tblock,sizeof(TimeDate));
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

void CSmallTimeAndDate::GetActualTimeAndDate(void)
{
    time_t timer;
    tm     *tblock;
    timer = time(NULL);
    tblock = localtime(&timer);
    memcpy(&TimeDate,tblock,sizeof(TimeDate));
}

// -----------------------------------------------------------------------------

void CSmallTimeAndDate::Clear(void)
{
    memset(&TimeDate,0,sizeof(TimeDate));
}

// -----------------------------------------------------------------------------

bool CSmallTimeAndDate::IsClear(void) const
{
    bool result = true;

    result &= TimeDate.tm_sec == 0;
    result &= TimeDate.tm_min == 0;
    result &= TimeDate.tm_hour == 0;
    result &= TimeDate.tm_mday == 0;
    result &= TimeDate.tm_mon == 0;
    result &= TimeDate.tm_year == 0;

    return(result);
}

// -----------------------------------------------------------------------------

CSmallString CSmallTimeAndDate::GetSDatePart(void) const
{
    char buffer[10];
    sprintf(buffer,"%02d-%02d-%02d",GetYear(),GetMonth(),GetDay());
    return(CSmallString(buffer));
}

// -----------------------------------------------------------------------------

CSmallString CSmallTimeAndDate::GetSTimePart(void) const
{
    char buffer[10];
    sprintf(buffer,"%02d:%02d:%02d",GetHour(),GetMinute(),GetSecond());
    return(CSmallString(buffer));
}

// -----------------------------------------------------------------------------

CSmallString CSmallTimeAndDate::GetSDateAndTime(void) const
{
    char buffer[25];
    sprintf(buffer,"%02d-%02d-%02d %02d:%02d:%02d",GetYear(),GetMonth(),GetDay(),GetHour(),GetMinute(),GetSecond());
    return(CSmallString(buffer));
}

// -----------------------------------------------------------------------------

int CSmallTimeAndDate::GetYear(void) const
{
    return(TimeDate.tm_year+1900);
}

// -----------------------------------------------------------------------------

int CSmallTimeAndDate::GetYearDay(void) const
{
    return(TimeDate.tm_yday);
}

// -----------------------------------------------------------------------------

int CSmallTimeAndDate::GetMonth(void) const
{
    return(TimeDate.tm_mon+1);
}

// -----------------------------------------------------------------------------

int CSmallTimeAndDate::GetDay(void) const
{
    return(TimeDate.tm_mday);
}

// -----------------------------------------------------------------------------

int CSmallTimeAndDate::GetWeekDay(void) const
{
    return(TimeDate.tm_wday);
}

// -----------------------------------------------------------------------------

int CSmallTimeAndDate::GetHour(void) const
{
    return(TimeDate.tm_hour);
}

// -----------------------------------------------------------------------------

int CSmallTimeAndDate::GetMinute(void) const
{
    return(TimeDate.tm_min);
}

// -----------------------------------------------------------------------------

int CSmallTimeAndDate::GetSecond(void) const
{
    return(TimeDate.tm_sec);
}

// -----------------------------------------------------------------------------

bool CSmallTimeAndDate::operator == (const CSmallTimeAndDate& right) const
{
    bool result = true;

    result &= TimeDate.tm_sec == right.TimeDate.tm_sec;
    result &= TimeDate.tm_min == right.TimeDate.tm_min;
    result &= TimeDate.tm_hour == right.TimeDate.tm_hour;
    result &= TimeDate.tm_mday == right.TimeDate.tm_mday;
    result &= TimeDate.tm_mon == right.TimeDate.tm_mon;
    result &= TimeDate.tm_year == right.TimeDate.tm_year;

    return(result);
}

// -----------------------------------------------------------------------------

bool CSmallTimeAndDate::operator != (const CSmallTimeAndDate& right) const
{
    bool result = false;

    result |= TimeDate.tm_sec != right.TimeDate.tm_sec;
    result |= TimeDate.tm_min != right.TimeDate.tm_min;
    result |= TimeDate.tm_hour != right.TimeDate.tm_hour;
    result |= TimeDate.tm_mday != right.TimeDate.tm_mday;
    result |= TimeDate.tm_mon != right.TimeDate.tm_mon;
    result |= TimeDate.tm_year != right.TimeDate.tm_year;

    return(result);
}

// -----------------------------------------------------------------------------

bool CSmallTimeAndDate::operator < (const CSmallTimeAndDate& right) const
{
    tm loctime;
    loctime = TimeDate;
    time_t timel = mktime(&loctime);
    loctime = right.TimeDate;
    time_t timer = mktime(&loctime);
    return( timel < timer );
}

// -----------------------------------------------------------------------------

bool CSmallTimeAndDate::operator > (const CSmallTimeAndDate& right) const
{
    tm loctime;
    loctime = TimeDate;
    time_t timel = mktime(&loctime);
    loctime = right.TimeDate;
    time_t timer = mktime(&loctime);
    return( timel > timer );
}

// -----------------------------------------------------------------------------

bool CSmallTimeAndDate::operator <= (const CSmallTimeAndDate& right) const
{
    tm loctime;
    loctime = TimeDate;
    time_t timel = mktime(&loctime);
    loctime = right.TimeDate;
    time_t timer = mktime(&loctime);
    return( timel <= timer );
}

// -----------------------------------------------------------------------------

bool CSmallTimeAndDate::operator >= (const CSmallTimeAndDate& right) const
{
    tm loctime;
    loctime = TimeDate;
    time_t timel = mktime(&loctime);
    loctime = right.TimeDate;
    time_t timer = mktime(&loctime);
    return( timel >= timer );
}

// -----------------------------------------------------------------------------

CSmallTimeAndDate CSmallTimeAndDate::operator + (const CSmallTime& right) const
{
    tm loctime = TimeDate;
    time_t time = mktime(&loctime);
    time = time + right.Time;
    tm     *tblock;
    tblock = localtime(&time);
    return(*tblock);
}

// -----------------------------------------------------------------------------

CSmallTimeAndDate CSmallTimeAndDate::operator - (const CSmallTime& right) const
{
    tm loctime = TimeDate;
    time_t time = mktime(&loctime);
    time = time - right.Time;
    tm     *tblock;
    tblock = localtime(&time);
    return(*tblock);
}

// -----------------------------------------------------------------------------

CSmallTime CSmallTimeAndDate::operator - (const CSmallTimeAndDate& right) const
{
    tm loctime1 = TimeDate;
    time_t time1 = mktime(&loctime1);
    tm loctime2 = right.TimeDate;
    time_t time2 = mktime(&loctime2);
    time_t time = time1 - time2;
    return(time);
}

// -----------------------------------------------------------------------------

void CSmallTimeAndDate::operator += (const CSmallTime& right)
{
    tm loctime = TimeDate;
    time_t time = mktime(&loctime);
    time = time + right.Time;
    tm     *tblock;
    tblock = localtime(&time);
    memcpy(tblock,&TimeDate,sizeof(TimeDate));
}

// -----------------------------------------------------------------------------

void CSmallTimeAndDate::operator -= (const CSmallTime& right)
{
    tm loctime = TimeDate;
    time_t time = mktime(&loctime);
    time = time - right.Time;
    tm     *tblock;
    tblock = localtime(&time);
    memcpy(tblock,&TimeDate,sizeof(TimeDate));
}


// -----------------------------------------------------------------------------

int CSmallTimeAndDate::GetSecondsFromBeginning(void) const
{
    tm loctime = TimeDate;
    time_t time = mktime(&loctime);
    return(time);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================


