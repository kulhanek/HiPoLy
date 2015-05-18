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

#include <stdlib.h>
#include <IndexCounter.hpp>

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CIndexCounter::CIndexCounter(void)
{
    Index = 0;
}

//------------------------------------------------------------------------------

int CIndexCounter::GetIndex(void)
{
    int temp = Index;
    Index++;
    return(temp);
}

//------------------------------------------------------------------------------

int CIndexCounter::GetTopIndex(void) const
{
    return(Index);
}

//------------------------------------------------------------------------------

void CIndexCounter::SetTopIndex(int topindex)
{
    if( Index > topindex ) return;
    Index = topindex+1;
}

//------------------------------------------------------------------------------

void CIndexCounter::SetIndex(int newindex)
{
    Index = newindex;
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CIndexArray::CIndexArray(int size)
{
    Size = size;
    Array = new int[size];
}

//------------------------------------------------------------------------------

CIndexArray::~CIndexArray(void)
{
    delete Array;
    Array = NULL;
}

//------------------------------------------------------------------------------

int& CIndexArray::operator[](int i)
{
    return(Array[i]);
}

//------------------------------------------------------------------------------

int CIndexArray::GetNewIndex(int oldindex)
{
    for(int i=0; i<Size; i++) {
        if(oldindex == Array[i]) return(i);
    }
    return(0);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
