#ifndef FormatSpecH
#define FormatSpecH
//==============================================================================
//    Copyright 2008 Petr Kulhanek
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

//------------------------------------------------------------------------------

/// types

enum EFormatType {
    EFT_INTEGER,
    EFT_DOUBLE,
    EFT_STRING
};

//------------------------------------------------------------------------------

/// C format specification

class HIPOLY_PACKAGE CFormatSpec {
public:
    CFormatSpec(void);

    /// set format specification
    void SetFormat(const CSmallString& format);

    /// get format specification
    const CSmallString& GetFormat(void) const;

    /// is format valid?
    bool IsFormatValid(EFormatType type);

    /// get length of record
    unsigned int GetRecordLength(void);

// section of private data ----------------------------------------------------
private:
    CSmallString    FormatSpec;
};

//------------------------------------------------------------------------------

#endif
