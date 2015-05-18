#ifndef SmallStringH
#define SmallStringH
//==============================================================================
//    Copyright 1998,1999,2000,2001,2004,2006,2008 Petr Kulhanek
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
#include <stdio.h>
#include <string>

#ifdef HAVE_VCL
#include "system.hpp"
#endif

#ifdef HAVE_QT5
#include <QString>
#endif

//------------------------------------------------------------------------------

/// return "true" or "false"
HIPOLY_PACKAGE const char* bool_to_str(bool data);

//------------------------------------------------------------------------------

/*
 IMPORTANT NOTES:

 NULL and empty string are always represent as NULL

 e.g.
 CSmallString str;
 str = "";

 str[0] will rise an exception!

*/

/*! \brief string manipulation support
 *
 *  string manipulation support
 */

class HIPOLY_PACKAGE CSmallString {
public:
//  constructors ---------------------------------------------------------------
    /// Constructor
    /*! empty string
    */
    CSmallString(void);

    /// Constructor
    /*! string from copystring
    */
    CSmallString(const CSmallString& copystring);

    /// Constructor
    /*! string from \0 terminated p_string
    */
    CSmallString(const char* p_string);

    /// Constructor
    /*! string from STL string
    */
    CSmallString(const std::string& string);

    /// Constructor
    /*! string from one character
    */
    CSmallString(char character);

    /// Constructor
    /*! string from one character
    */
    CSmallString(unsigned char character);

    /// Constructor
    /*! string from number
    */
    CSmallString(int number);

    /// Constructor
    /*! string from number
    */
    CSmallString( unsigned int number);

    /// Constructor
    /*! string from number
    */
    CSmallString(long int number);

    /// Constructor
    /*! string from number
    */
    CSmallString(long unsigned int number);

    /// destructor
    ~CSmallString(void);

// length operation ------------------------------------------------------------

    /// allocate fixed space for string of maximum length
    bool          SetLength(int length);

    /// get current lenght of string
    unsigned int  GetLength(void) const;

    /// is string empty ?
    bool          IsEmpty(void) const;

// retype operators ------------------------------------------------------------
    /// get string of const char* type
    operator const char*() const;

    /// get character at i position
    /* do not check string boundaries
    */
    char operator[](const unsigned int i) const;

    /// get character at i position
    /* do not check string boundaries
    */
    char  operator[](const int i) const;

    /// get character at i position
    /* do not check string boundaries
    */
    char&  operator[](const unsigned int i);

    /// get character at i position
    /* do not check string boundaries
    */
    char&  operator[](const int i);

    /// get access to string buffer
    char*  GetBuffer(void);

    /// get access to string buffer
    const char*  GetBuffer(void) const;

    /// set string from fortran string
    void SetFromFortran(char* str,unsigned int str_len,bool do_not_trim=false);

    /// convert to fortran string
    void ConvertToFortran(char* str,unsigned int str_len);

// logical operation ----------------------------------------------------------
    /// compare two strings
    bool  operator==(const char *p_string) const;
    /// compare two strings
    bool  operator==(const CSmallString& string) const;

    /// compare two strings
    bool  operator!=(const char *p_string) const;
    /// compare two strings
    bool  operator!=(const CSmallString& string) const;

    /// compare two strings
    bool  operator<(const CSmallString& string) const;

// add, assign operation ------------------------------------------------------
    /// assign new string p_string
    void  operator=(const char* p_string);
    /// assign new string string
    void  operator=(const CSmallString& string);

    /// add string p_string
    void  operator+=(const char *p_string);
    /// add character ch
    void  operator+=(const char ch);
    /// add string cs
    void  operator+=(const CSmallString& cs);

    /// add two strings
    CSmallString        operator+(const char *p_string) const;
    /// add two strings
    CSmallString        operator+(const CSmallString& string) const;
    /// add two strings
    friend CSmallString HIPOLY_PACKAGE operator+(const char* p_string1,
            const CSmallString& string2);

// stream operator -------------------------------------------------------------
    CSmallString&  operator << (const char* p_string);
    CSmallString&  operator << (const CSmallString& cs);
    CSmallString&  operator << (const char ch);
    CSmallString&  operator << (const unsigned char ch);
    CSmallString&  operator << (const int number);
    CSmallString&  operator << (const unsigned int number);
    CSmallString&  operator << (const long int number);
    CSmallString&  operator << (const long unsigned int number);

// numeric operations ----------------------------------------------------------
    /// convert int to string
    /*! total length of string is limited to 256 characters
    */
    bool   IntToStr(int value,const char* p_format="%d");
    /// convert unsigned int to string
    /*! total length of string is limited to 256 characters
    */
    bool   UIntToStr(unsigned int value,const char* p_format="%u");

    /// convert long int to string
    /*! total length of string is limited to 256 characters
    */
    bool   LIntToStr(long int value,const char* p_format="%d");
    /// convert long unsigned int to string
    /*! total length of string is limited to 256 characters
    */
    bool   LUIntToStr(long unsigned int value,const char* p_format="%u");

    /// string to int
    int                 ToInt(void) const;

    /// string to unsigned int
    unsigned int        ToUInt(void) const;

    /// string to long int
    long int            ToLInt(void) const;

    /// string to long unsigned int
    long unsigned int   ToLUInt(void) const;

    /// string to int
    bool   ToInt(int& num) const;

    /// string to unsigned int
    bool   ToInt(unsigned int& num) const;

    /// string to long int
    bool   ToInt(long int& num) const;

    /// string to long unsigned int
    bool   ToInt(unsigned long int& num) const;

    /// check if string is integer number
    bool   IsInt(void) const;

    /// convert bool to string (true/false)
    bool   BoolToStr(bool value);

    /// convert string (true/false) to bool
    /*! only first letter is checked
        't' = true
        otherwise false
    */
    bool   ToBool(void) const;

    /// convert float to string
    /*! total length of string is limited to 256 characters
    */
    bool   FloatToStr(float value,const char* p_format="%f");

    /// convert string to float
    float  ToFloat(void) const;

    /// convert double to string
    /*! total length of string is limited to 256 characters
    */
    bool   DoubleToStr(double value,const char* p_format="%lf");

    /// convert string to double
    double ToDouble(void) const;

    /// convert string to double
    bool ToDouble(double& value) const;

// string manipulation ---------------------------------------------------------
    /// convert every character to uppercase
    void ToUpperCase(void);

    /// convert every character to uppercase and return as new string
    CSmallString GetUpperCase(void) const;

    /// convert every character to lowercase
    void ToLowerCase(void);

    /// convert every character to lowercase and return as new string
    CSmallString GetLowerCase(void) const;

    /// trim white spaces
    void Trim(void);

    /// substitute all characters 'before' with characters 'after' and return as new string
    CSmallString GetSubstitute(char before,char after) const;

    /// substitute all characters 'before' with characters 'after'
    void Substitute(char before,char after) const;

    /// return index of the first occurence of characer 'c'
    /*! boundary of offset are not controlled
    */
    int IndexOf(const char c,int offset=0) const;

    /// return index of the first character which is not in p_vlist string
    int Verify(const char* p_vlist,int from=-1,int to=-1,bool back=false) const;

    /// return index of the first character which is in p_vlist string
    int Scan(const char* p_vlist,int from=-1,int to=-1,bool back=false) const;

// operations with substrings --------------------------------------------------
    /// find substring
    /*! return index of substring
        -1 if substring is not found
        search start at offset index of string
        do not check string boundaries
    */
    int FindSubString(const char* p_substring, int offset=0) const;

    /// find substring
    /*! return index of substring
        -1 if substring is not found
        search start at offset index of string
        do not check string boundaries
    */
    int FindSubString(const CSmallString& substring, int offset=0) const;

    /// return substring
    /*! substring start at from index and has specified length
        do not check string boundaries
    */
    CSmallString GetSubString(unsigned int from, unsigned int length) const;

    /// return substring
    /*! substring start at from index and to index
        do not check string boundaries
    */
    CSmallString GetSubStringFromTo(unsigned int from, unsigned int to) const;

    // read string from file ---------------------------------------------------
    /// it reads string from file - string length is fully dynamic
    bool ReadStringFromFile(FILE* p_file);

    /// it reads whole line from file (it reads \n that is stored in string) - string length is fully dynamic
    bool ReadLineFromFile(FILE* p_file,bool do_not_store_nl=false,bool clear_string=false);

// VCL support -----------------------------------------------------------------
#ifdef HAVE_VCL
    /// constructor from AnsiString
    CSmallString(const AnsiString& string);

    /// assign new string string
    void operator = (const AnsiString& string);

    /// retype CSmallString as AnsiString
    operator const AnsiString() const;

    /// compare two strings
    bool  operator==(const AnsiString& string) const;

    /// compare two strings
    bool  operator!=(const AnsiString& string) const;
#endif

// QT5 support -----------------------------------------------------------------
#ifdef HAVE_QT5
    /// constructor from QString
    CSmallString(const QString& string);

    /// assign new string
    void operator = (const QString& string);

    /// compare two strings
    bool  operator==(const QString& string) const;

    /// compare two strings
    bool  operator!=(const QString& string) const;
#endif

// section of private data -----------------------------------------------------
private:
    /// string buffer
    char*       Buffer;
    /// length of buffer
    unsigned    LengthOfBuffer;

    friend class CFileName;
};

//------------------------------------------------------------------------------

#endif


