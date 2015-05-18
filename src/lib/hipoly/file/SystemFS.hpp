#ifndef SystemFSH
#define SystemFSH
//==============================================================================
//    Copyright 2008,2009 Petr Kulhanek
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

//------------------------------------------------------------------------------

class HIPOLY_PACKAGE CSystemFS {
public:
// directory methods ----------------------------------------------------------
    /// return path to program root directory (PREFIX)
    static const CFileName GetProgramDir(void);

    /// return path to program subdirectory
    static const CFileName GetProgramSubDir(const CFileName& sub_dir);

    /// return path to directory for user configuration files
    static const CFileName GetUserSetupDir(void);

    /// return path to directory for user documents
    static const CFileName GetUserDocumentsDir(void);

// file extensions ------------------------------------------------------------
    /// return prefix for dynamic libraries
    static const CFileName  GetLibraryPrefix(void);

    /// return affix for dynamic libraries
    static const CFileName  GetLibraryAffix(void);

// environments ---------------------------------------------------------------
    /// get system variable
    static const CSmallString GetSystemVariable(const CSmallString& name);
};

//------------------------------------------------------------------------------

#endif
