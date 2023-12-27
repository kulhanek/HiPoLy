#ifndef FileNameH
#define FileNameH
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
#include <type_traits>

//------------------------------------------------------------------------------

/*! \brief file name string
 *
 *  uniform methods over file and directory names
 */

class HIPOLY_PACKAGE CFileName : public CSmallString {
public:
// constructor and destructors ------------------------------------------------
    /// Constructor
    /*! empty path
    */
    CFileName(void);

    /// Constructor
    /*! string from copypath
    */
    CFileName(const CFileName& copypath);

    /// Constructor
    /*! string from copystring
    */
    CFileName(const CSmallString& copystring);

    /// Constructor
    /*! string from STL string
    */
    CFileName(const std::string& string);

    /// Constructor
    /*! string from p_string
    */
    CFileName(const char* p_string);

    /// Constructor
    /*! range copy constructor
    */
    template<class InputIterator>
    CFileName(InputIterator first, InputIterator last) {
        size_t len = std::distance(first,last);
        LengthOfBuffer = 0;
        Buffer = NULL;
        SetLength(len);
        size_t i=0;
        while(first != last){
             GetBuffer()[i] = *first;
             first++;
             i++;
        }
    }

    /// destructor
    ~CFileName(void);

    /// init from UNIX style path
    void SetFromUNIX(const CSmallString& path);

//  path operators ------------------------------------------------------
    /// add cs to path without path delimiter
    void  operator+=(const CFileName& cs);

    /// add p_string to path with path delimiter
    void  operator/=(const char *p_string);

    /// add cs to path with path delimiter
    void  operator/=(const CFileName& cs);

    /// add cs to path with path delimiter
    void  operator/=(const CSmallString& cs);

    /// add two paths with path delimiter
    CFileName        operator/(const char *p_string) const;

    /// add two paths with path delimiter
    CFileName        operator/(const CFileName& string) const;

    /// add two paths with path delimiter
    CFileName        operator/(const CSmallString& string) const;

    /// add two paths with path delimiter
    friend CFileName HIPOLY_PACKAGE operator/(const char* p_string1,
            const CFileName& string2);

// stream operator ------------------------------------------------------
    /// append string to path
    CFileName&  operator << (const char* p_string);

    /// append string to path
    CFileName&  operator << (const CSmallString& cs);

    /// append path to path without path delimiter
    CFileName&  operator << (const CFileName& cs);

// file name operations ------------------------------------------------------
    /// return file directory
    CFileName   GetFileDirectory(void) const;

    /// return file name
    CFileName   GetFileName(void) const;

    /// return file name extension (with point)
    CFileName   GetFileNameExt(void) const;

    /// return file name without its extension
    CFileName   GetFileNameWithoutExt(void) const;

    /// return relative path
    CFileName RelativeTo(const CFileName& path_to);

    /// absolutize path
    void AbsolutizePath(void);

    /// absolutize path
    void AbsolutizePath(const CFileName& base);

// information methods --------------------------------------------------------
    static char GetPathDelimiter(void);

#ifdef HAVE_QT5
    /// constructor from QString
    CFileName(const QString& string);

    /// add two paths with path delimiter
    CFileName   operator/(const QString& string) const;
#endif

// section of private data ----------------------------------------------------
private:
    static const char  PathDelimiter;  // character used to divide the names of subdirectories
};

//------------------------------------------------------------------------------

#endif
