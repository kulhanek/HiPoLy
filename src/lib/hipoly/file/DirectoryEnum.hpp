#ifndef DirectoryEnumH
#define DirectoryEnumH
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
#include <FileName.hpp>

#if defined _WIN32 || defined __CYGWIN__
#include <windows.h>
#else
#include <dirent.h>
#include <vector>
#endif

//------------------------------------------------------------------------------

/// enumerate files and subdirectories in a given directory
class HIPOLY_PACKAGE CDirectoryEnum {
public:
// constructor and destructor -------------------------------------------------
    /// init object
    CDirectoryEnum(void);
    /// init object and set directory name
    CDirectoryEnum(const CFileName& dirname);
    virtual ~CDirectoryEnum(void);

// setup methods --------------------------------------------------------------
    /// set new directory name, previous search is closed
    bool                SetDirName(const CFileName& dirname);
    const CFileName&    GetDirName(void) const;

// enum methods ---------------------------------------------------------------
    /// it releases previous search and prepare for new one
    bool StartFindFile(const CFileName& filter);

    /// find file
    bool FindFile(CFileName& file);

    /// close file search
    bool EndFindFile(void);

// private data and methods ---------------------------------------------------
private:
    CFileName       DirName;    // directory name
    CSmallString    Filter;
#if defined _WIN32 || defined __CYGWIN__
    HANDLE          DirFD;
    WIN32_FIND_DATA Data;
    BOOL            Result;
#else
    DIR*            DirFD;
#endif
};

//------------------------------------------------------------------------------
#endif
