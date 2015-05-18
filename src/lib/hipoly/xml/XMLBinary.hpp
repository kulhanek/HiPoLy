#ifndef XMLBinaryH
#define XMLBinaryH
//==============================================================================
//    Copyright 2000,2004,2008,2011 Petr Kulhanek
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

// -----------------------------------------------------------------------------

#define BXML_VERSION            0x02

// BXML section codes
#define BXML_DECLARATION_TAG    0xD0

#define BXML_ELEMENT_STAG       0xE0
#define BXML_BIN_DATA_STAG      0xB0
#define BXML_ELEMENT_ETAG       0xE1
#define BXML_ATTRIBUTE_TAG      0xA0

#define BXML_UNKNOWN_TAG        0x10
#define BXML_COMMENT_TAG        0x20
#define BXML_TEXT_TAG           0x30
#define BXML_BIN_ARRAY_TAG      0x40

// -----------------------------------------------------------------------------

class CBXMLUShort {
public:
// constructor ----------------------------------------------------------------
    inline CBXMLUShort(void);

// access methods -------------------------------------------------------------
    inline unsigned short Get(void) const;
    inline void           Set(unsigned short number);

// section of private data ----------------------------------------------------
private:
    unsigned char Data[2];
};

// -----------------------------------------------------------------------------

class CBXMLUInt {
public:
// constructor ----------------------------------------------------------------
    inline CBXMLUInt(void);

// access methods -------------------------------------------------------------
    inline unsigned int Get(void) const;
    inline void         Set(unsigned int number);

// section of private data ----------------------------------------------------
private:
    unsigned char Data[4];
};

// -----------------------------------------------------------------------------

class CBXMLHeader {
public:
    CBXMLHeader(void);

// control methods ------------------------------------------------------------
    /// check if it is binary XML
    bool IsBXML(void);

    /// return BXML version
    unsigned int GetVersion(void);

// section of private data ----------------------------------------------------
private:
    unsigned char   Trac[4];    // leading sequence
    CBXMLUShort     Version;    // version
    CBXMLUShort     Control;    // control flags
    CBXMLUInt       Reserved;   // reserved part
};

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CBXMLUShort::CBXMLUShort(void)
{
    Data[0] = 0;
    Data[1] = 0;
}

// -----------------------------------------------------------------------------

inline unsigned short CBXMLUShort::Get(void) const
{
    return( Data[0] + Data[1]*256 );
}

// -----------------------------------------------------------------------------

inline void CBXMLUShort::Set(unsigned short number)
{
    Data[0] = number % 256;
    Data[1] = (number >> 8) % 256;
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

inline CBXMLUInt::CBXMLUInt(void)
{
    Data[0] = 0;
    Data[1] = 0;
    Data[2] = 0;
    Data[3] = 0;
}

//------------------------------------------------------------------------------

inline unsigned int CBXMLUInt::Get(void) const
{
    return( Data[0] + Data[1]*256 + Data[2]*65536 + Data[3]*16777216 );
}

//------------------------------------------------------------------------------

inline void CBXMLUInt::Set(unsigned int number)
{
    Data[0] = number % 256;
    Data[1] = (number >> 8) % 256;
    Data[2] = (number >> 16) % 256;
    Data[3] = (number >> 24) % 256;
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

#endif

