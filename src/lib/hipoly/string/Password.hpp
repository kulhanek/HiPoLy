#ifndef PasswordH
#define PasswordH
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

/*! \brief password operation
 *
 */

class HIPOLY_PACKAGE CPassword {
public:
    CPassword(void);

    /// read password from command line
    bool ReadFromCmdLine(void);

    /// read password from file
    bool ReadFromFile(const CSmallString& name);

    /// read password from file
    bool ReadFromFile(FILE* fin);

    /// return stored password
    const CSmallString& GetPassword(void);

    /// test if password is strong (len > 4)
    bool IsStrongPassword(void);

// section of private data ----------------------------------------------------
private:
    CSmallString    Password;
};

//------------------------------------------------------------------------------

#endif
