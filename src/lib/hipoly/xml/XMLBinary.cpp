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

#include <XMLBinary.hpp>

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CBXMLHeader::CBXMLHeader(void)
{
    Trac[0] = 'B';
    Trac[1] = 'X';
    Trac[2] = 'M';
    Trac[3] = 'L';
    Version.Set(BXML_VERSION);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

bool CBXMLHeader::IsBXML(void)
{
    return( Trac[0] == 'B' && Trac[1] == 'X' && Trac[2] == 'M' && Trac[3] == 'L' );
}

//------------------------------------------------------------------------------

unsigned int CBXMLHeader::GetVersion(void)
{
    return( Version.Get() );
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
