#ifndef SmallDateH
#define SmallDateH
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

/*! \brief Simple date definition
 * This is short version of CSmallTimeAndDate, which support only dates.
 */

class HIPOLY_PACKAGE CSmallDate {
public:
    /// Constructor
    /*! Set current date for object.
    */
    CSmallDate(void);

    /// Constructor
    /*! set date to date from timeanddate value
    */
    CSmallDate(const CSmallTimeAndDate& timeanddate);

// setup --------------------------------------------------------
    /// set actual date
    void GetActualDate(void);

    /// set date to zero
    void Clear(void);

    /// check if date is zero
    bool IsClear(void) const;

// acces operations ---------------------------------------------------------
    /// return date in string format
    /*! format "yy-mo-dd"
        yy - year
        mo - month
        dd - day
    */
    CSmallString GetSDate(void) const;

    /// return year
    int GetYear(void) const;

    /// return year day
    int GetYearDay(void) const;

    /// return month
    int GetMonth(void) const;

    /// return day
    int GetDay(void) const;

    /// return week day
    int GetWeekDay(void) const;

// section of private data ----------------------------------------------------
protected:
    /// time in elapsed since 00:00:00 GMT, January 1, 1970
    tm    Date;

    friend class CXMLElement;
};

//---------------------------------------------------------------------------

#endif
