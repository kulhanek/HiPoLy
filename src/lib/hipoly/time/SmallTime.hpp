#ifndef SmallTimeH
#define SmallTimeH
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

#include <HiPoLyMainHeader.hpp>
#include <SmallString.hpp>
#include <time.h>

//------------------------------------------------------------------------------

class CSmallTimeAndDate;

//------------------------------------------------------------------------------

/*! \brief Simple time definition
 *
 *  Define time in seconds. This time can be partitioned in seconds,minutes,hours,
    and days. Full arithmetics is supported.
 */

class HIPOLY_PACKAGE CSmallTime {
public:
    /// Constructor
    /*! Set current time for object.
    */
    CSmallTime(void);

    /// Constructor
    /*! set time to time value
    */
    CSmallTime(const time_t& time);

    /// Constructor
    /*! set time to time from timeanddate value
    */
    CSmallTime(const CSmallTimeAndDate& timeanddate);

// setup --------------------------------------------------------
    /// set from the string in the hh:mm:ss format
    void SetFromString(const CSmallString& stime);

    /// set actual time
    void GetActualTime(void);

    /// set time to zero
    void Clear(void);

    /// check time if it is zero
    bool IsClear(void) const;

    /// check time if it is in past
    /*! Past is defined as negative time
    */
    bool IsInPast(void) const;

    /// check time if it overflows 24 hours
    bool IsOverflow(void) const;

// acces operations ---------------------------------------------------------
    /// return time in string format
    /*! format "hh:mm:ss"
        hh - hour
        mm - minute
        ss - second
    */
    CSmallString GetSTime(void) const;

    /// return time in string format - no 24 hour limits
    /*! format "hh:mm:ss"
        hh - hour
        mm - minute
        ss - second
    */
    CSmallString GetSTimeFull(void) const;

    /// return time in string format
    /*! format "days hh:mm:ss"
        days - day
        hh - hour
        mm - minute
        ss - second
    */
    CSmallString GetSTimeAndDay(void) const;

    /// return day from "days hh:mm:ss" time format
    /*! return day of time e.g time/86400
    */
    int GetDay(void) const;

    /// return hour from "days hh:mm:ss" time format
    /*! return hour e.g (time%86400)/3600
    */
    int GetHour(void) const;

    /// return minute from "days hh:mm:ss" time format
    /*! return hours e.g ((time%86400)%3600)/60
    */
    int GetMinute(void) const;

    /// return second from "days hh:mm:ss" time format
    /*! return second e.g ((time%86400)%3600)%60
    */
    int GetSecond(void) const;

    /// return second from "beginning"
    int GetSecondsFromBeginning(void) const;


// logical operations----------------------------------------------------------
    /// compare times
    bool operator == (const CSmallTime& right) const;
    /// compare times
    bool operator != (const CSmallTime& right) const;
    /// compare times
    bool operator < (const CSmallTime& right) const;
    /// compare times
    bool operator > (const CSmallTime& right) const;
    /// compare times
    bool operator <= (const CSmallTime& right) const;
    /// compare times
    bool operator >= (const CSmallTime& right) const;

// arithmetic operations ------------------------------------------------------
    /// add times
    CSmallTime operator + (const CSmallTime& right) const;
    /// sub times
    CSmallTime operator - (const CSmallTime& right) const;
    /// add times
    void operator += (const CSmallTime& right);
    /// sub times
    void operator -= (const CSmallTime& right);

    /// multiply time in seconds with constant
    CSmallTime operator * (int mult) const;

    /// divide time in seconds with constant
    CSmallTime operator / (int div) const;

// section of private data ----------------------------------------------------
protected:
    /// time in sec elapsed since 00:00:00 GMT, January 1, 1970
    time_t    Time;

    friend class CSmallTimeAndDate;
    friend class CXMLElement;
};

//---------------------------------------------------------------------------

#endif
