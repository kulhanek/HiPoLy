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
#include <SmallTime.hpp>
#include <SmallTimeAndDate.hpp>

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CSmallTime::CSmallTime(void)
{
    Clear();
}

// -----------------------------------------------------------------------------

CSmallTime::CSmallTime(const time_t& time)
{
    Time = time;
}

// -----------------------------------------------------------------------------

CSmallTime::CSmallTime(const CSmallTimeAndDate& timeanddate)
{
    Time = timeanddate.TimeDate.tm_sec + timeanddate.TimeDate.tm_min*60 + timeanddate.TimeDate.tm_hour*3600;
}

// -----------------------------------------------------------------------------

void CSmallTime::GetActualTime(void)
{
    time_t timer;
    tm     *tblock;
    timer = time(NULL);
    tblock = localtime(&timer);
    Time = tblock->tm_sec + tblock->tm_min*60 + tblock->tm_hour*3600;
}

//---------------------------------------------------------------------------

void CSmallTime::Clear(void)
{
    Time = 0;
}

//---------------------------------------------------------------------------

bool CSmallTime::IsClear(void) const
{
    return( Time == 0 );
}

//---------------------------------------------------------------------------

bool CSmallTime::IsInPast(void) const
{
    return( Time < 0 );
}

//---------------------------------------------------------------------------

bool CSmallTime::IsOverflow(void) const
{
    return( (Time / 86400) > 0 );
}

//---------------------------------------------------------------------------

CSmallString CSmallTime::GetSTime(void) const
{
    char buffer[10];
    sprintf(buffer,"%02d:%02d:%02d",GetHour(),GetMinute(),GetSecond());
    return(CSmallString(buffer));
}

//---------------------------------------------------------------------------

CSmallString CSmallTime::GetSTimeAndDay(void)const
{
    char buffer[50];
    sprintf(buffer,"% 2dd %02d:%02d:%02d",GetDay(),GetHour(),GetMinute(),GetSecond());
    return(CSmallString(buffer));
}

// -----------------------------------------------------------------------------

int CSmallTime::GetDay(void) const
{
    return(Time / 86400);
}

// -----------------------------------------------------------------------------

int CSmallTime::GetHour(void) const
{
    return((Time % 86400)/3600);
}

// -----------------------------------------------------------------------------

int CSmallTime::GetMinute(void) const
{
    return(((Time % 86400)%3600)/60);
}

// -----------------------------------------------------------------------------

int CSmallTime::GetSecond(void) const
{
    return(((Time % 86400)%3600)%60);
}

// -----------------------------------------------------------------------------

int CSmallTime::GetSecondsFromBeginning(void) const
{
    return(Time);
}

// -----------------------------------------------------------------------------

bool CSmallTime::operator == (const CSmallTime& right) const
{
    return(Time == right.Time);
}

// -----------------------------------------------------------------------------

bool CSmallTime::operator != (const CSmallTime& right) const
{
    return(Time != right.Time);
}

// -----------------------------------------------------------------------------

bool CSmallTime::operator < (const CSmallTime& right) const
{
    return(Time < right.Time);
}

// -----------------------------------------------------------------------------

bool CSmallTime::operator > (const CSmallTime& right) const
{
    return(Time > right.Time);
}

// -----------------------------------------------------------------------------

bool CSmallTime::operator <= (const CSmallTime& right) const
{
    return(Time <= right.Time);
}

// -----------------------------------------------------------------------------

bool CSmallTime::operator >= (const CSmallTime& right) const
{
    return(Time >= right.Time);
}

// -----------------------------------------------------------------------------

CSmallTime CSmallTime::operator + (const CSmallTime& right) const
{
    return(Time + right.Time);
}

// -----------------------------------------------------------------------------

CSmallTime CSmallTime::operator - (const CSmallTime& right) const
{
    return(Time - right.Time);
}

// -----------------------------------------------------------------------------

void CSmallTime::operator += (const CSmallTime& right)
{
    Time = Time + right.Time;
}

// -----------------------------------------------------------------------------

void CSmallTime::operator -= (const CSmallTime& right)
{
    Time = Time - right.Time;
}

// -----------------------------------------------------------------------------

CSmallTime CSmallTime::operator * (int mult) const
{
    return(Time*mult);
}

CSmallTime CSmallTime::operator / (int div) const
{
    return(Time/div);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================


