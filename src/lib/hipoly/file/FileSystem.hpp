#ifndef FileSystemH
#define FileSystemH
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

/*! \brief file system methods
 *
 *  Basic methods over file systems.
 */

class HIPOLY_PACKAGE CFileSystem {
public:
// directory methods ----------------------------------------------------------
    /// populate currdir with the name of current working directory
    static bool GetCurrentDir(CSmallString& currdir);

    /// change working direcotry to currdir
    static bool SetCurrentDir(const CSmallString& currdir);

    /// create directory - dir could contain nested directories
    static bool CreateDir(const CSmallString& dir);

    /// remove empty directory, use recursive for non-empty directory
    static bool RemoveDir(const CSmallString& dir,bool recursive=false);

    /// remove contents of directory
    static bool RemoveDirContents(const CSmallString& dir,bool recursive=false);

// file methods ---------------------------------------------------------------
    /// copy a file, it fails for a directory
    static bool CopyFile(const CSmallString& source,
                                      const CSmallString& dest,bool overwrite=false);

    /// remove a file, it fails for a directory
    static bool RemoveFile(const CSmallString& name,bool silent=false);

// file and directory methods -------------------------------------------------
    /// rename a file or directory
    static bool Rename(const CSmallString& old_name,
                                    const CSmallString& new_name);

    /// copy a file or directory
    static bool Copy(const CSmallString& source,
                                  const CSmallString& dest,bool overwrite=false,
                                  bool recursive=true);

    /// remove a file or empty directory
    static bool Remove(const CSmallString& file,bool recursive=false);

    /// is directory?
    static bool IsDirectory(const CSmallString& file);

    /// is file?
    static bool IsFile(const CSmallString& file);

    /// is socket?
    static bool IsSocket(const CSmallString& file);

    // permisions --------------------------------------------------------------
    /// set posix mode
    static bool SetPosixMode(const CSmallString& file,int mode);

    /// get posix mode
    static int  GetPosixMode(const CSmallString& file);
};

//------------------------------------------------------------------------------

#endif
