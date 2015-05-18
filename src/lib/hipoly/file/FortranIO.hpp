#ifndef FortranIOH
#define FortranIOH
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
#include <stdio.h>
#include <SmallString.hpp>

//------------------------------------------------------------------------------

#define LINE_BUFFER 255

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

class HIPOLY_PACKAGE CFortranIO {
public:
    CFortranIO(FILE* p_file,bool silent_error=false);

    /// set FORTRAN format
    /*! currently recognized formats are as follows: xAy, xIy, xEy.z, xFy.z
    */
    bool SetFormat(const CSmallString& format);

    /// change FORTRAN format, e.g. new section is not forced as in SetFormat
    /*! currently recognized formats are as follows: xAy, xIy, xEy.z, xFy.z
    */
    bool ChangeFormat(const CSmallString& format);

    /// try once again from beginning of line
    bool ReturnBack(void);

    /// read real value from stream
    bool ReadReal(double& value);

    /// read real value from stream
    bool ReadReal(float& value);

    /// read integer value from stream
    bool ReadInt(int& value);

    /// read string from stream - the p_value must point at allocated memory
    bool ReadString(char* p_value);

    /// read string from stream
    bool ReadString(CSmallString& string);

    /// write real value to stream
    bool WriteReal(const double value);

    /// write integer to stream
    bool WriteInt(const int value);

    /// write string to stream
    bool WriteString(const char* p_value);

    /// write end of section (e.g. end of line)
    bool WriteEndOfSection(void);

    /// get format of current section
    /*! If current line is empty it is skipped. This occurs when previous
        section does not contain data.
        Format is returned only if current section name has same name
        as p_section_name.
    */
    char* GetFormatOfSection(const CSmallString& section_name);

    /// get name of current section
    /*! If current line is empty it is skipped. This occurs when previous
        section does not contain data.
    */
    char* GetNameOfSection(void);

    /// find next section
    /*! it reads new lines until new section is found
    */
    char* FindNewSection(void);

// section of private data -------------------------------------------------
private:
    FILE*   File;

    char    Line[LINE_BUFFER];     // currently only line with 80 characters are supported
    char*   RecPos;
    int     RecPerLine;
    int     ActualRec;
    int     RecLength;
    char    FormatType;
    int     TotalWrittenRecords;
    bool    SilentError;

    CSmallString WriteStringCFormat;
    CSmallString WriteIntCFormat;
    CSmallString WriteRealCFormat;

    bool LoadLine(void);
    bool WriteNewLine(void);
    bool IsNewLine(void);

    char* PrepareAmber7Format(char* p_buffer);
};

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

#endif
