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
#include <SmallDate.hpp>
#include <SmallTime.hpp>
#include <SmallTimeAndDate.hpp>

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CSmallDate::CSmallDate(void)
{
    Clear();
}

// -----------------------------------------------------------------------------

CSmallDate::CSmallDate(const CSmallTimeAndDate& timeanddate)
{
    Date.tm_mday = timeanddate.TimeDate.tm_mday;
    Date.tm_mon = timeanddate.TimeDate.tm_mon;
    Date.tm_year = timeanddate.TimeDate.tm_year;
    Date.tm_wday = timeanddate.TimeDate.tm_wday;
    Date.tm_yday = timeanddate.TimeDate.tm_yday;
    Date.tm_sec = 0;
    Date.tm_min = 0;
    Date.tm_hour = 0;
}

// -----------------------------------------------------------------------------

void CSmallDate::GetActualDate(void)
{
    time_t timer;
    tm     *tblock;
    timer = time(NULL);
    tblock = localtime(&timer);
    memcpy(tblock,&Date,sizeof(Date));

    Date.tm_sec = 0;
    Date.tm_min = 0;
    Date.tm_hour = 0;
}

// -----------------------------------------------------------------------------

void CSmallDate::Clear(void)
{
    memset(&Date,0,sizeof(Date));
}

// -----------------------------------------------------------------------------

bool CSmallDate::IsClear(void) const
{
    bool result = true;

    result &= Date.tm_mday == 0;
    result &= Date.tm_mon == 0;
    result &= Date.tm_year == 0;

    return(result);
}

// -----------------------------------------------------------------------------

CSmallString CSmallDate::GetSDate(void) const
{
    char buffer[10];
    sprintf(buffer,"%02d-%02d-%02d",GetYear(),GetMonth(),GetDay());
    return(CSmallString(buffer));
}

// -----------------------------------------------------------------------------

int CSmallDate::GetYear(void) const
{
    return(Date.tm_year);
}

// -----------------------------------------------------------------------------

int CSmallDate::GetYearDay(void) const
{
    return(Date.tm_yday);
}

// -----------------------------------------------------------------------------

int CSmallDate::GetMonth(void) const
{
    return(Date.tm_mon);
}

// -----------------------------------------------------------------------------

int CSmallDate::GetDay(void) const
{
    return(Date.tm_mday);
}

// -----------------------------------------------------------------------------

int CSmallDate::GetWeekDay(void) const
{
    return(Date.tm_wday);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

