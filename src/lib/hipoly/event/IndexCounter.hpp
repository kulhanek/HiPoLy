#ifndef IndexCounterH
#define IndexCounterH
//==============================================================================
//    Copyright 1998,1999,2000,2001,2004,2005 Petr Kulhanek
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

//------------------------------------------------------------------------------

/// unique index support

class HIPOLY_PACKAGE CIndexCounter {
public:
    /// constructor
    /*! first index is zero
    */
    CIndexCounter(void);

    /// return index and update next index
    int  GetIndex(void);

    /// return next free index
    int  GetTopIndex(void) const;

    /// set next free index if specified topindex is higher than it
    void SetTopIndex(int topindex);

    /// set next free index to newindex
    void SetIndex(int newindex);

// section of private data ----------------------------------------------------
private:
    /// first free index
    int Index;
};

//------------------------------------------------------------------------------

/// reindex unique indexes

class HIPOLY_PACKAGE CIndexArray {
public:
    /// constructor
    /*! init index array of specified size
    */
    CIndexArray(int size);
    ~CIndexArray(void);

    /// fill array with old indexes
    int& operator[](int i);

    /// get position of oldindex in index array (e.g. it returns new shaken index)
    int GetNewIndex(int oldindex);

private:
    /// number of all indexes
    int Size;
    /// array of all indexes
    int *Array;
};

// -----------------------------------------------------------------------------
#endif
