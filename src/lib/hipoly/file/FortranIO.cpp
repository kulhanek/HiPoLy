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

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <FortranIO.hpp>
#include <ErrorSystem.hpp>

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CFortranIO::CFortranIO(FILE* p_file,bool silent_error)
{
    RecPerLine=0;
    ActualRec=0;
    RecLength=0;
    RecPos=0;
    File = p_file;
    FormatType = ' ';
    TotalWrittenRecords = 0;
    Line[LINE_BUFFER-1]='\0';
    SilentError = silent_error;
}

//------------------------------------------------------------------------------

bool CFortranIO::SetFormat(const CSmallString& format)
{
    if( format == NULL ) {
        if( SilentError == false ) ES_ERROR("format cannot be NULL string");
        return(false);
    }

    CSmallString work_buffer(format);

    char* p_endpos = NULL;
    RecPerLine = 0;
    RecPerLine = strtol(work_buffer.GetBuffer(),&p_endpos,10);
    if( (RecPerLine <= 0) && (p_endpos != NULL) ) {
        if( SilentError == false ) {
            ES_ERROR("Number of records per line in CFortranIO::SetFormat is lesser than or equal zero!");
            ES_ERROR("Format structure: " + format);
        }
        return(false);
    }
    RecLength = 0;
    FormatType = *p_endpos++; // skip format specifier
    RecLength = strtol(p_endpos,&p_endpos,10);
    if( (RecLength <= 0) && (p_endpos != NULL) ) {
        if( SilentError == false ) {
            ES_ERROR("Length of record in CFortranIO::SetFormat is lesser than or equal zero!");
            ES_ERROR("Format structure: " + format);
        }
        return(false);
    }

    int precision = 0;
    if( (FormatType == 'E')||(FormatType == 'e') ) {
        // extract
        p_endpos++; // skip point
        precision = strtol(p_endpos,&p_endpos,10);
        if( (precision <= 0) && (p_endpos != NULL) ) {
            if( SilentError == false ) {
                ES_ERROR("Precision of record in CFortranIO::SetFormat is less or equal zero!");
                ES_ERROR("Format structure: " + format);
            }
            return(false);
        }
    }

    if( (FormatType == 'F')||(FormatType == 'f') ) {
        // extract
        p_endpos++; // skip point
        precision = strtol(p_endpos,&p_endpos,10);
        if( (precision <= 0) && (p_endpos != NULL) ) {
            if( SilentError == false ) {
                ES_ERROR("Precision of record in CFortranIO::SetFormat is less or equal zero!");
                ES_ERROR("Format structure: " + format);
            }
            return(false);
        }
    }

    // prepare C formats for writing
    CSmallString slen, sprec;

    slen.IntToStr(RecLength);
    sprec.IntToStr(precision);

    switch(FormatType) {
    case 'i':
        FormatType = 'I';
    case 'I':
        WriteIntCFormat = "%" + slen + "d";
        break;
    case 'a':
        FormatType = 'A';
    case 'A':
        WriteStringCFormat = "%" + slen + "s";
        break;
    case 's':
        FormatType = 'S';
    case 'S':
        WriteStringCFormat = "%-" + slen + "s";
        break;
    case 'e':
        FormatType = 'E';
    case 'E':
        WriteRealCFormat = "%" + slen + "." + sprec + "lE";
        break;
    case 'f':
        FormatType = 'F';
    case 'F':
        WriteRealCFormat = "%" + slen + "." + sprec + "lf";
        break;
    default:
        if( SilentError == false ) {
            ES_ERROR("Illegal format type in CFortranIO::SetFormat!");
            ES_ERROR("Format structure: " + format);
        }
        return(false);
    };

    ActualRec = 0;
    RecPos = NULL;
    TotalWrittenRecords = 0;

    return(true);
}

//------------------------------------------------------------------------------

bool CFortranIO::ChangeFormat(const CSmallString& format)
{
    if( format == NULL ) {
        if( SilentError == false ) ES_ERROR("Format cannot be NULL string in CFortranIO::ChangeFormat!");
        return(false);
    }

    CSmallString work_buffer(format);

    char* p_endpos = NULL;
    RecPerLine = 0;
    RecPerLine = strtol(work_buffer.GetBuffer(),&p_endpos,10);
    if( (RecPerLine <= 0) && (p_endpos != NULL) ) {
        if( SilentError == false ) {
            ES_ERROR("Number of records per line in CFortranIO::ChangeFormat is lesser than or equal zero!");
            ES_ERROR("Format structure: " + format);
        }
        return(false);
    }
    RecLength = 0;
    FormatType = *p_endpos++; // skip format specifier
    RecLength = strtol(p_endpos,&p_endpos,10);
    if( (RecLength <= 0) && (p_endpos != NULL) ) {
        if( SilentError == false ) {
            ES_ERROR("Length of record in CFortranIO::ChangeFormat is lesser than or equal zero!");
            ES_ERROR("Format structure: " + format);
        }
        return(false);
    }

    int precision = 0;
    if( (FormatType == 'E')||(FormatType == 'e') ) {
        // extract
        p_endpos++; // skip point
        precision = strtol(p_endpos,&p_endpos,10);
        if( (precision <= 0) && (p_endpos != NULL) ) {
            if( SilentError == false ) {
                ES_ERROR("Precision of record in CFortranIO::ChangeFormat is less or equal zero!");
                ES_ERROR("Format structure: " + format);
            }
            return(false);
        }
    }

    if( (FormatType == 'F')||(FormatType == 'f') ) {
        // extract
        p_endpos++; // skip point
        precision = strtol(p_endpos,&p_endpos,10);
        if( (precision <= 0) && (p_endpos != NULL) ) {
            if( SilentError == false ) {
                ES_ERROR("Precision of record in CFortranIO::ChangeFormat is less or equal zero!");
                ES_ERROR("Format structure: " + format);
            }
            return(false);
        }
    }

    // prepare C formats for writing
    CSmallString slen, sprec;

    slen.IntToStr(RecLength);
    sprec.IntToStr(precision);

    switch(FormatType) {
    case 'i':
        FormatType = 'I';
    case 'I':
        WriteIntCFormat = "%" + slen + "d";
        break;
    case 'a':
        FormatType = 'A';
    case 'A':
        WriteStringCFormat = "%" + slen + "s";
        break;
    case 's':
        FormatType = 'S';
    case 'S':
        WriteStringCFormat = "%-" + slen + "s";
        break;
    case 'e':
        FormatType = 'E';
    case 'E':
        WriteRealCFormat = "%" + slen + "." + sprec + "lE";
        break;
    case 'f':
        FormatType = 'F';
    case 'F':
        WriteRealCFormat = "%" + slen + "." + sprec + "lf";
        break;
    default:
        if( SilentError == false ) {
            ES_ERROR("Illegal format type in CFortranIO::ChangeFormat!");
            ES_ERROR("Format structure: " + format);
        }
        return(false);
    };

    TotalWrittenRecords = 0;
    ActualRec = 0;
    return(true);
}

//------------------------------------------------------------------------------

bool CFortranIO::ReturnBack(void)
{
    RecPos = Line;
    ActualRec = 0;
    return(true);
}

//------------------------------------------------------------------------------

bool CFortranIO::LoadLine(void)
{
    RecPos = Line;
    ActualRec = 0;

    if( File == NULL ) return(false);

    bool result = fgets(Line,LINE_BUFFER,File) != NULL;

    if( result != false ) {
        // remove trailing \n character if present
        int len = strlen(Line);
        if( len > 0 ) {
            if( Line[len-1] == '\n' ) Line[len-1] = '\0';
        }
    } else {
        Line[0] = '\0';
    }

    return(result);
}

//------------------------------------------------------------------------------

bool CFortranIO::WriteNewLine(void)
{
    RecPos = Line;
    ActualRec = 0;
    if( File != NULL ) return( fprintf(File,"\n") > 0 );
    return(false);
}

//------------------------------------------------------------------------------

bool CFortranIO::ReadReal(double& value)
{
    if( (FormatType != 'E') && (FormatType != 'F') ) {
        if(SilentError == false) ES_ERROR("Conflict in format in CFortranIO::ReadReal!");
        return(false);
    }

    if( IsNewLine() == true ) {
        if( LoadLine() == false ) {
            if(SilentError == false) ES_ERROR("Unable to load data line in CFortranIO::ReadReal!");
            return(false);
        }
    }
    char backup = RecPos[RecLength];
    RecPos[RecLength] = '\0';

    // char* p_endpos=NULL;
    // value = strtod(RecPos,&p_endpos);
    // if( p_endpos == NULL ) return(false);

    if( sscanf(RecPos," %lf",&value) != 1 ) {
        if( SilentError == false ) ES_ERROR("Unable to load double in CFortranIO::ReadReal!");
        return(false);
    }

    RecPos+=RecLength;
    ActualRec++;
    *RecPos = backup;

    return(true);
}

//------------------------------------------------------------------------------

bool CFortranIO::ReadReal(float& value)
{
    if( (FormatType != 'E') && (FormatType != 'F') ) {
        if( SilentError == false ) ES_ERROR("Conflict in format in CFortranIO::ReadReal!");
        return(false);
    }

    if( IsNewLine() == true ) {
        if( LoadLine() == false ) {
            if( SilentError == false ) ES_ERROR("Unable to load data line in CFortranIO::ReadReal!");
            return(false);
        }
    }

    char backup = RecPos[RecLength];
    RecPos[RecLength] = '\0';

    // char* p_endpos=NULL;
    // value = strtod(RecPos,&p_endpos);
    // if( p_endpos == NULL ) return(false);

    if( sscanf(RecPos," %f",&value) != 1 ) {
        *RecPos = backup;
        if( SilentError == false ) ES_ERROR("Unable to load float in CFortranIO::ReadReal!");
        return(false);
    }

    RecPos+=RecLength;
    ActualRec++;
    *RecPos = backup;

    return(true);
}

//------------------------------------------------------------------------------

bool CFortranIO::ReadInt(int& value)
{
    if( FormatType != 'I' ) {
        if( SilentError == false ) ES_ERROR("Conflict in format in CFortranIO::ReadInt!");
        return(false);
    }

    if( IsNewLine() == true ) {
        if( LoadLine() == false ) {
            if( SilentError == false ) ES_ERROR("Unable to load data line in CFortranIO::ReadInt!");
            return(false);
        }
    }

    char backup = RecPos[RecLength];
    RecPos[RecLength] = '\0';

    // char* p_endpos=NULL;  - problematic if record starts with spaces
    // value = strtol(RecPos,&p_endpos,10);
    // if( p_endpos == NULL ) return(false);

    if( sscanf(RecPos," %d",&value) != 1 ) {
        *RecPos = backup;
        CSmallString error;
        error << "Unable to load integer in CFortranIO::ReadInt (" << RecPos << ")!";
        if( SilentError == false ) ES_ERROR(error);
        return(false);
    }

    RecPos+=RecLength;
    ActualRec++;
    *RecPos = backup;

    return(true);
}

//------------------------------------------------------------------------------

bool CFortranIO::ReadString(char* p_value)
{
    if( FormatType != 'A' ) {
        if( SilentError == false ) ES_ERROR("Conflict in format in CFortranIO::ReadString!");
        return(false);
    }

    if( IsNewLine() == true ) {
        if( LoadLine() == false ) {
            if( SilentError == false ) ES_ERROR("Unable to load data line in CFortranIO::ReadString!");
            return(false);
        }
    }

    memcpy(p_value,RecPos,RecLength);
    ActualRec++;
    RecPos+=RecLength;

    return(true);
}

//------------------------------------------------------------------------------

bool CFortranIO::ReadString(CSmallString& string)
{
    if( FormatType != 'A' ) {
        if( SilentError == false ) ES_ERROR("Conflict in format in CFortranIO::ReadString!");
        return(false);
    }

    if( IsNewLine() == true ) {
        if( LoadLine() == false ) {
            if( SilentError == false ) ES_ERROR("Unable to load data line in CFortranIO::ReadString!");
            return(false);
        }
    }

    string.SetLength(RecLength);
    memcpy(string.GetBuffer(),RecPos,RecLength);
    string.GetBuffer()[RecLength] = '\0';

    ActualRec++;
    RecPos+=RecLength;

    return(true);
}

//------------------------------------------------------------------------------

bool CFortranIO::WriteReal(const double value)
{
    if( (FormatType != 'E') && (FormatType != 'F') ) {
        ES_ERROR("Conflict in format in CFortranIO::WriteReal!");
        return(false);
    }

    if( ActualRec == RecPerLine ) {
        if( WriteNewLine() == false ) return(false);
    }

    int num = 0;
    if( File != NULL ) {
        num = fprintf(File,WriteRealCFormat,value);
    }

    ActualRec++;

    TotalWrittenRecords++;
    return(num > 0);
}

//------------------------------------------------------------------------------

bool CFortranIO::WriteInt(const int value)
{
    if( FormatType != 'I' ) {
        ES_ERROR("Conflict in format in CFortranIO::WriteInt!");
        return(false);
    }

    if( ActualRec == RecPerLine ) {
        if( WriteNewLine() == false ) return(false);
    }

    int num = 0;
    if( File != NULL ) {
        num = fprintf(File,WriteIntCFormat,value);
    }

    ActualRec++;

    TotalWrittenRecords++;
    return(num > 0);
}

//------------------------------------------------------------------------------

bool CFortranIO::WriteString(const char* p_value)
{
    if( (FormatType != 'A') && (FormatType != 'S') ) {
        ES_ERROR("Conflict in format in CFortranIO::WriteString!");
        return(false);
    }

    if( ActualRec == RecPerLine ) {
        if( WriteNewLine() == false ) return(false);
    }

    int num=0;
    if( File != NULL ) {
        num = fprintf(File,WriteStringCFormat,p_value);
    }

    ActualRec++;

    TotalWrittenRecords++;
    return(num > 0);
}

//------------------------------------------------------------------------------

bool CFortranIO::WriteEndOfSection(void)
{
    int num = 1;
    if( (ActualRec != 0) || (TotalWrittenRecords == 0) ) {
        if( File != NULL ) {
            num = fprintf(File,"\n");
        }
    }
    ActualRec = 0;
    RecPos = NULL;
    TotalWrittenRecords = 0;
    return(num > 0);
}

//------------------------------------------------------------------------------

bool CFortranIO::IsNewLine(void)
{
    return((ActualRec == RecPerLine)||(RecPos==NULL));
}

//------------------------------------------------------------------------------

char* CFortranIO::PrepareAmber7Format(char* p_buffer)
{
    // return from %FORMAT(10I8) only fortran format
    // e.g. 10I8 , original string is demaged

    int len = strlen(p_buffer);

    char* p_format = NULL;

    for(int i=0; i<len; i++) {
        if( p_buffer[i] == '(' ) {
            p_format = &p_buffer[i+1];
            break;
        }
    }
    if( p_format == NULL ) return(NULL);

    for(int i=0; i<len; i++) {
        if( p_buffer[i] == ')' ) {
            p_buffer[i]='\0';
            break;
        }
    }
    return(p_format);
}

//------------------------------------------------------------------------------

char* CFortranIO::GetFormatOfSection(const CSmallString& section_name)
{
    CSmallString    working_buffer(section_name);

    //  bool empty;
    //  do{
    //     if( LoadLine() == false ){
    //         if( SilentError == false ) ES_ERROR("Unable to load line from file in CFortranIO::GetFormatOfSection!");
    //         return(NULL);
    //         }
    //     empty = true;
    //     for(unsigned int i=0;i<strlen(Line);i++){
    //         if( isalpha(Line[i]) > 0 ){
    //             empty = false;
    //             break;
    //             }
    //         }
    //     }while( empty == true );

    if( strstr(Line,working_buffer.GetBuffer()) == NULL ) {
        if( SilentError == false ) ES_ERROR("Expecting section: '" + section_name + "' but current section is: '" + Line + "'");
        return(NULL);
    }

    if( LoadLine() == false ) {
        if( SilentError == false ) ES_ERROR("Unable to load line from file!");
        return(NULL);
    }

    return(PrepareAmber7Format(Line));
}

//------------------------------------------------------------------------------

char* CFortranIO::GetNameOfSection(void)
{
    bool empty;
    do {
        if( LoadLine() == false ) {
            if( SilentError == false )  ES_ERROR("unable to load line from file!");
            return(NULL);
        }
        empty = true;
        for(unsigned int i=0; i<strlen(Line); i++) {
            if( isalpha(Line[i]) > 0 ) {
                empty = false;
                break;
            }
        }
    } while( empty == true );

    // strip white characters from the end
    int len = strlen(Line);
    for(int i=len-1; i >= 0; i--) {
        if( isspace(Line[i]) != 0 ) {
            Line[i] = '\0';
        } else {
            break;
        }
    }

    return(Line);
}

//------------------------------------------------------------------------------

char* CFortranIO::FindNewSection(void)
{
    bool empty;
    do {
        if( LoadLine() == false ) {
            return(NULL);
        }
        empty = true;
        for(unsigned int i=0; i<strlen(Line); i++) {
            if( isalpha(Line[i]) > 0 ) {
                empty = false;
                break;
            }
        }
    } while( (empty == true) || (Line[0] != '%') );

    // strip white characters from the end
    int len = strlen(Line);
    for(int i=len-1; i >= 0; i--) {
        if( isspace(Line[i]) != 0 ) {
            Line[i] = '\0';
        } else {
            break;
        }
    }

    return(Line);
}

//------------------------------------------------------------------------------


