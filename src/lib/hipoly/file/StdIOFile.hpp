#ifndef StdIOFileH
#define StdIOFileH
//==============================================================================
//    Copyright 2011 Petr Kulhanek
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
#include <stdio.h>

//------------------------------------------------------------------------------

/// cstdio FILE wrapper

class HIPOLY_PACKAGE CStdIOFile {
public:
    CStdIOFile(void);
    ~CStdIOFile(void);

// input/output methods --------------------------------------------------------
    /// open file
    /*! Open file with name and mode. If the name is "-" then the standard stdin or
        stdout file is used. Which one will be used is determined by used mode.
        The mode can be either "r" (read) or "w" (write) */
    bool Open(const CSmallString& name,const CSmallString& mode);

    /// close file
    void Close(void);

// access methods --------------------------------------------------------------
    /// get access to FILE object
    operator FILE* ();

// section of private data -----------------------------------------------------
private:
    bool    IsOwned;
    FILE*   File;
};

//------------------------------------------------------------------------------

#endif
