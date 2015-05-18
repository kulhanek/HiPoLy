#ifndef UUIDClassH
#define UUIDClassH
//==============================================================================
//    Copyright 2000,2004,2008 Petr Kulhanek
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

/// universal unique identifier

class HIPOLY_PACKAGE CUUID {
public:
    /// Constructor - invalid UUID
    CUUID(void);
    /// Constructor - initialized form string
    CUUID(const CSmallString& data);

    /// compare two UUID
    bool operator==(const CUUID& left) const;

    /// compare two UUID
    bool operator!=(const CUUID& left) const;

    /// convert UUID from string form
    bool         SetFromStringForm(const CSmallString& data);

    /// convert UUID to string
    CSmallString GetStringForm(void) const;

    /// check if it is valid UUID
    bool         IsValidUUID(void) const;

    /// create new UUID
    bool         CreateUUID(void);

    /// clear UUID
    void         Clear(void);

    /// get name of UUID generator
    CSmallString GetGeneratorType(void) const;

    /// comparison operator
    bool         operator < (const CUUID& left) const;

// section of private data ----------------------------------------------------
private:
    unsigned int    Main;
    unsigned short  First;
    unsigned short  Second;
    unsigned short  Third;
    unsigned char   Extended[6];
};

//------------------------------------------------------------------------------

#endif
