#ifndef SmallTimeAndDateH
#define SmallTimeAndDateH
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

class CSmallTime;

//------------------------------------------------------------------------------

/*! \brief Simple time and date definition
 *
 *  Define time and date.
 */

class HIPOLY_PACKAGE CSmallTimeAndDate {
public:
    /// Constructor
    /*! Set current date and time for object.
    */
    CSmallTimeAndDate(void);

    /// Constructor
    /*! set time and date to timedate value
    */
    CSmallTimeAndDate(const tm& timedate);

    /// Constructor
    /*! set time and date from seconds from beginning
    */
    CSmallTimeAndDate(const int seconds_from_beginning);

// setup --------------------------------------------------------
    /// set actual time and date
    void GetActualTimeAndDate(void);

    /// set time and date to zero
    void Clear(void);

    /// check if time and date is zero
    bool IsClear(void) const;

// acces operations ---------------------------------------------------------
    /// return time in string format
    /*! format "hh:mm:ss"
        hh - hours
        mm - minutes
        ss - seconds
    */
    CSmallString GetSDatePart(void) const;

    /// return date in string format
    /*! format "yy-mo-dd"
        yy - year
        mo - month
        dd - day
    */
    CSmallString GetSTimePart(void) const;

    /// return time in string format
    /*! format "yy-mo-dd hh:mm:ss"
        yy - year
        mo - month
        dd - day
        hh - hours
        mm - minutes
        ss - seconds
    */
    CSmallString GetSDateAndTime(void) const;

    /// return year
    /*! return year
    */
    int GetYear(void) const;

    /// return year day
    /*! return year day
    */
    int GetYearDay(void) const;

    /// return month
    /*! return month
    */
    int GetMonth(void) const;

    /// return day
    /*! return day
    */
    int GetDay(void) const;

    /// return week day
    /*! return week day
    */
    int GetWeekDay(void) const;

    /// return hour
    /*! return hour
    */
    int GetHour(void) const;

    /// return minute
    /*! return minute
    */
    int GetMinute(void) const;

    /// return second
    /*! return second
    */
    int GetSecond(void) const;

    /// return number of seconds from beginning
    /*! return number of seconds from beginning, e.g. 00:00:00 GMT, January 1, 1970
    */
    int GetSecondsFromBeginning(void) const;

// logical operations----------------------------------------------------------
    /// compare times and dates
    bool operator == (const CSmallTimeAndDate& right) const;

    /// compare times and dates
    bool operator != (const CSmallTimeAndDate& right) const;

    /// compare times and dates
    bool operator < (const CSmallTimeAndDate& right) const;

    /// compare times and dates
    bool operator > (const CSmallTimeAndDate& right) const;

    /// compare times and dates
    bool operator <= (const CSmallTimeAndDate& right) const;

    /// compare times and dates
    bool operator >= (const CSmallTimeAndDate& right) const;


// arithmetic operations ------------------------------------------------------
    /// add time
    CSmallTimeAndDate operator + (const CSmallTime& right) const;

    /// sub time
    CSmallTimeAndDate operator - (const CSmallTime& right) const;

    /// sub date and get time difference
    CSmallTime        operator - (const CSmallTimeAndDate& right) const;

    /// add time
    void operator += (const CSmallTime& right);

    /// sub time
    void operator -= (const CSmallTime& right);

// section of private data ----------------------------------------------------
protected:
    /// time in elapsed since 00:00:00 GMT, January 1, 1970
    tm    TimeDate;

    friend class CSmallTime;
    friend class CSmallDate;
    friend class CXMLElement;
};

//---------------------------------------------------------------------------

#endif
