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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <SmallString.hpp>
#include <ErrorSystem.hpp>

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

const char* bool_to_str(bool data)
{
    if( data == true ) return("true");
    return("false");
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CSmallString::CSmallString(void)
{
    Buffer = NULL;
    LengthOfBuffer = 0;
}

//---------------------------------------------------------------------------

CSmallString::CSmallString(const CSmallString& copystring)
{
    LengthOfBuffer = copystring.LengthOfBuffer;

    if( LengthOfBuffer != 0 ) {
        Buffer = (char*)malloc(LengthOfBuffer*sizeof(char));
        memcpy(Buffer,copystring.Buffer,LengthOfBuffer);
    } else {
        Buffer = NULL;
    }
}

//---------------------------------------------------------------------------

CSmallString::CSmallString(const char* string)
{
    LengthOfBuffer = 0;

    if( (string == NULL)||((LengthOfBuffer = strlen(string))==0) ) {
        Buffer = NULL;
        return;
    }

    LengthOfBuffer++;  // +1 pro znak \0

    Buffer = (char*)malloc(LengthOfBuffer*sizeof(char));
    memcpy(Buffer,string,LengthOfBuffer);
}

//---------------------------------------------------------------------------

CSmallString::CSmallString(const std::string& string)
{
    LengthOfBuffer = string.size();

    if( LengthOfBuffer == 0 ) {
        Buffer = NULL;
        return;
    }

    LengthOfBuffer++;  // +1 pro znak \0

    Buffer = (char*)malloc(LengthOfBuffer*sizeof(char));
    memcpy(Buffer,string.c_str(),LengthOfBuffer);
}

//---------------------------------------------------------------------------

CSmallString::CSmallString(char character)
{
    LengthOfBuffer = 2; // char + \0
    Buffer = (char*)malloc(LengthOfBuffer*sizeof(char));
    Buffer[0] = character;
    Buffer[1] = '\0';
}

//---------------------------------------------------------------------------

CSmallString::CSmallString(unsigned char character)
{
    LengthOfBuffer = 2; // char + \0
    Buffer = (char*)malloc(LengthOfBuffer*sizeof(char));
    Buffer[0] = character;
    Buffer[1] = '\0';
}

//---------------------------------------------------------------------------

CSmallString::CSmallString(int number)
{
    Buffer = NULL;
    LengthOfBuffer = 0;
    IntToStr(number);
}

//---------------------------------------------------------------------------

CSmallString::CSmallString(unsigned int number)
{
    Buffer = NULL;
    LengthOfBuffer = 0;
    UIntToStr(number);
}

//---------------------------------------------------------------------------

CSmallString::CSmallString(long int number)
{
    Buffer = NULL;
    LengthOfBuffer = 0;
    IntToStr(number);
}

//---------------------------------------------------------------------------

CSmallString::CSmallString(long unsigned int number)
{
    Buffer = NULL;
    LengthOfBuffer = 0;
    UIntToStr(number);
}

// -----------------------------------------------------------------------------

CSmallString::~CSmallString(void)
{
    if( Buffer != NULL ) free(Buffer);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

bool CSmallString::SetLength(unsigned int length)
{
    if( Buffer ) free(Buffer);

    if( length ==0 ) {
        Buffer = NULL;
        LengthOfBuffer = 0;
        return(true);
    }

    LengthOfBuffer = length+1;
    Buffer = (char*)malloc(LengthOfBuffer*sizeof(char));
    if( Buffer == NULL ) {
        LengthOfBuffer = 0;
        return(false);
    }
    memset(Buffer,' ',length);
    Buffer[length] = '\0';
    return(true);
}

// -----------------------------------------------------------------------------

unsigned int CSmallString::GetLength(void) const
{
    if( LengthOfBuffer != 0) return(LengthOfBuffer-1);
    else  return(0);
}

// -----------------------------------------------------------------------------

bool CSmallString::IsEmpty(void) const
{
    return( Buffer == NULL );
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CSmallString::operator const char*() const
{
    static const char* p_empty = "";
    if( Buffer == NULL ) {
        return(p_empty);
    }
    return(Buffer);
}

// -----------------------------------------------------------------------------

char CSmallString::operator[](const unsigned int i) const
{
    return(Buffer[i]);
}

// -----------------------------------------------------------------------------

char CSmallString::operator[](const int i) const
{
    return(Buffer[i]);
}

//------------------------------------------------------------------------------

char&  CSmallString::operator[](const unsigned int i)
{
    return(Buffer[i]);
}

//------------------------------------------------------------------------------

char&  CSmallString::operator[](const int i)
{
    return(Buffer[i]);
}

// -----------------------------------------------------------------------------

char* CSmallString::GetBuffer(void)
{
    return(Buffer);
}

// -----------------------------------------------------------------------------

const char* CSmallString::GetBuffer(void) const
{
    return(Buffer);
}

//------------------------------------------------------------------------------

void CSmallString::SetFromFortran(char* str,unsigned int str_len,bool do_not_trim)
{
    if( str == NULL ){
        INVALID_ARGUMENT("str is NULL");
    }

    // detect the end
    unsigned int end = str_len;
    if( ! do_not_trim ){
        for(int i=str_len-1; i >= 0; i--) {
            if( isspace(str[i]) == 0 ) {
                end = i;
                break;
            }
        }
        str_len = end + 1;
    }

    // allocate buffer
    SetLength(str_len);

    if( str_len == 0 ) return; // string is empty

    // copy string
    strncpy(Buffer,str,str_len);

    // terminate string
    Buffer[str_len] = '\0';
}

//------------------------------------------------------------------------------

void CSmallString::ConvertToFortran(char* str,unsigned int str_len)
{
    if( str == NULL ){
        INVALID_ARGUMENT("str is NULL");
    }

    if( str_len == 0 ) return;
    memset(str,' ',str_len);
    unsigned int len = str_len;
    if( GetLength() < len) len = GetLength();
    strncpy(str,Buffer,len);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

bool CSmallString::operator==(const char* p_string) const
{
    if( Buffer == NULL ) return( (p_string == NULL) || (strcmp(p_string,"")==0) );
    if( p_string == NULL ) return( (Buffer == NULL) || (strcmp(Buffer,"")==0) );
    return( strcmp(Buffer,p_string) == 0);
}

// -----------------------------------------------------------------------------

bool CSmallString::operator==(const CSmallString& string) const
{
    if( LengthOfBuffer != string.LengthOfBuffer ) return(false);
    if( (Buffer == NULL) || (string.Buffer == NULL ) ) return( Buffer == string.Buffer );
    return( strcmp(Buffer,string.Buffer) == 0);
}

//---------------------------------------------------------------------------

bool CSmallString::operator!=(const char* p_string) const
{
    if( Buffer == NULL ) return( !((p_string == NULL) || (strcmp(p_string,"")==0)) );
    if( p_string == NULL ) return( !((Buffer == NULL) || (strcmp(Buffer,"")==0)) );
    return( strcmp(Buffer,p_string) != 0);
}

// -----------------------------------------------------------------------------

bool CSmallString::operator!=(const CSmallString& string) const
{
    if( LengthOfBuffer != string.LengthOfBuffer ) return(true);
    if( (Buffer == NULL) || (string.Buffer == NULL ) ) return( Buffer != string.Buffer );
    return( strcmp(Buffer,string.Buffer) != 0);
}

// -----------------------------------------------------------------------------

bool CSmallString::operator<(const CSmallString& string) const
{
    if( (Buffer == NULL) && (string.Buffer == NULL ) ) return( false );
    if( Buffer == NULL ) return( true );
    if( string.Buffer == NULL ) return( false );
    return( strcmp(Buffer,string.Buffer) < 0);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

void CSmallString::operator=(const char* p_string)
{
    if( Buffer ) free(Buffer);

    if( (p_string == NULL)||( (LengthOfBuffer = strlen(p_string)) ==0) ) {
        LengthOfBuffer = 0;
        Buffer = NULL;
        return;
    } else {
        LengthOfBuffer++;
        Buffer = (char*)malloc(LengthOfBuffer*sizeof(char));
        memcpy(Buffer,p_string,LengthOfBuffer);
        return;
    }
}

// -----------------------------------------------------------------------------

void CSmallString::operator=(const CSmallString& string)
{
    if( Buffer ) free(Buffer);

    if( string.Buffer == NULL ) {
        LengthOfBuffer = 0;
        Buffer = NULL;
        return;
    } else {
        LengthOfBuffer = string.LengthOfBuffer;
        Buffer = (char*)malloc(LengthOfBuffer*sizeof(char));
        memcpy(Buffer,string.Buffer,LengthOfBuffer);
        return;
    }
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

void CSmallString::operator+=(const char* p_string)
{
    int addlen;
    if( (p_string == NULL) || ( (addlen = strlen(p_string)) ==0) || (strcmp(p_string,"")==0) ) return;

    if( LengthOfBuffer == 0 ) LengthOfBuffer++;

    Buffer = (char*)realloc(Buffer,LengthOfBuffer+addlen);
    memcpy(&Buffer[LengthOfBuffer-1],p_string,addlen+1);
    LengthOfBuffer += addlen;
}

//---------------------------------------------------------------------------

void  CSmallString::operator+=(const char ch)
{
    if( LengthOfBuffer == 0 ) LengthOfBuffer++; // terminal \0
    LengthOfBuffer++;

    Buffer = (char*)realloc(Buffer,LengthOfBuffer);
    Buffer[LengthOfBuffer-2] = ch;
    Buffer[LengthOfBuffer-1] = '\0';
}

//---------------------------------------------------------------------------

void CSmallString::operator+=(const CSmallString& string)
{
    if( string.LengthOfBuffer == 0 ) return;

    if( LengthOfBuffer == 0 ) LengthOfBuffer++;

    Buffer = (char*)realloc(Buffer,LengthOfBuffer+string.LengthOfBuffer-1);
    memcpy(&Buffer[LengthOfBuffer-1],string.Buffer,string.LengthOfBuffer);
    LengthOfBuffer += string.LengthOfBuffer-1;
}
//---------------------------------------------------------------------------

CSmallString CSmallString::operator+(const char* p_string) const
{
    CSmallString ret_string;

    if( (p_string == NULL) || (strcmp(p_string,"")==0) ) {
        ret_string.LengthOfBuffer = LengthOfBuffer;
        if( LengthOfBuffer != 0 ) {
            ret_string.Buffer = (char*)malloc(LengthOfBuffer*sizeof(char));
            memcpy(ret_string.Buffer,Buffer,LengthOfBuffer);
        }
    } else {
        int addlen = strlen(p_string);
        int prelen = LengthOfBuffer;
        if( prelen == 0 ) prelen++;
        ret_string.LengthOfBuffer = prelen + addlen;
        ret_string.Buffer = (char*)malloc(ret_string.LengthOfBuffer*sizeof(char));
        if( prelen > 1 ) memcpy(ret_string.Buffer,Buffer,prelen-1);
        memcpy(&ret_string.Buffer[prelen-1],p_string,addlen+1);
    }

    return(ret_string);
}

//---------------------------------------------------------------------------

CSmallString CSmallString::operator+(const CSmallString &string) const
{
    CSmallString ret_string;

    if( (LengthOfBuffer == 0) && (string.LengthOfBuffer==0) ) return(ret_string);

    int addlen = string.LengthOfBuffer;
    int prelen = LengthOfBuffer;
    ret_string.LengthOfBuffer = prelen + addlen;
    if( (prelen != 0) && (addlen != 0) ) ret_string.LengthOfBuffer--;
    ret_string.Buffer = (char*)malloc(ret_string.LengthOfBuffer*sizeof(char));
    if( prelen != 0 ) memcpy(ret_string.Buffer,Buffer,prelen);
    else prelen = 1;
    if( addlen != 0 ) memcpy(&ret_string.Buffer[prelen-1],string.Buffer,addlen);
    else ret_string.Buffer[prelen-1]='\0';
    return(ret_string);
}

// -----------------------------------------------------------------------------

CSmallString HIPOLY_PACKAGE operator+(const char* string1,const CSmallString& string2)
{
    CSmallString ret_string;

    if( (string1 == NULL) || (strcmp(string1,"")==0) ) {
        ret_string.LengthOfBuffer = string2.LengthOfBuffer;
        if( string2.LengthOfBuffer != 0 ) {
            ret_string.Buffer = (char*)malloc(string2.LengthOfBuffer*sizeof(char));
            memcpy(ret_string.Buffer,string2.Buffer,string2.LengthOfBuffer);
        }
    } else {
        int prelen = strlen(string1);
        int addlen = string2.LengthOfBuffer;
        if( addlen == 0 ) addlen++;
        ret_string.LengthOfBuffer = prelen + addlen;
        ret_string.Buffer = (char*)malloc(ret_string.LengthOfBuffer*sizeof(char));
        memcpy(ret_string.Buffer,string1,prelen);
        if( addlen > 1 ) memcpy(&ret_string.Buffer[prelen],string2.Buffer,addlen);
        else ret_string.Buffer[prelen] = '\0';
    }

    return(ret_string);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CSmallString& CSmallString::operator << (const char* p_string)
{
    *this += p_string;
    return(*this);
}

//------------------------------------------------------------------------------

CSmallString& CSmallString::operator << (const CSmallString& cs)
{
    *this += cs;
    return(*this);
}

//------------------------------------------------------------------------------

CSmallString& CSmallString::operator << (const char ch)
{
    *this += ch;
    return(*this);
}

//------------------------------------------------------------------------------

CSmallString& CSmallString::operator << (const unsigned char ch)
{
    *this += ch;
    return(*this);
}

//------------------------------------------------------------------------------

CSmallString& CSmallString::operator << (const int number)
{
    CSmallString snumber(number);
    *this += snumber;
    return(*this);
}

//------------------------------------------------------------------------------

CSmallString& CSmallString::operator << (const unsigned int number)
{
    CSmallString snumber(number);
    *this += snumber;
    return(*this);
}

//------------------------------------------------------------------------------

CSmallString& CSmallString::operator << (const long int number)
{
    CSmallString snumber(number);
    *this += snumber;
    return(*this);
}

//------------------------------------------------------------------------------

CSmallString& CSmallString::operator << (const long unsigned int number)
{
    CSmallString snumber(number);
    *this += snumber;
    return(*this);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

bool CSmallString::IntToStr(int value,const char* p_format)
{
    char string[256];
    string[0]='\0';
    sprintf(string,p_format,value);

    if( Buffer ) free(Buffer);

    if( (string == NULL)||(strcmp(string,"")==0) ) {
        LengthOfBuffer = 0;
        Buffer = NULL;
        return(false);
    } else {
        LengthOfBuffer = strlen(string);
        LengthOfBuffer++;
        Buffer = (char*)malloc(LengthOfBuffer*sizeof(char));
        if( Buffer == NULL ) {
            LengthOfBuffer = 0;
            return(false);
        }
        memcpy(Buffer,string,LengthOfBuffer);
        return(true);
    }
}

// -----------------------------------------------------------------------------

bool CSmallString::UIntToStr(unsigned int value,const char* p_format)
{
    char string[256];
    string[0]='\0';
    sprintf(string,p_format,value);

    if( Buffer ) free(Buffer);

    if( (string == NULL)||(strcmp(string,"")==0) ) {
        LengthOfBuffer = 0;
        Buffer = NULL;
        return(false);
    } else {
        LengthOfBuffer = strlen(string);
        LengthOfBuffer++;
        Buffer = (char*)malloc(LengthOfBuffer*sizeof(char));
        if( Buffer == NULL ) {
            LengthOfBuffer = 0;
            return(false);
        }
        memcpy(Buffer,string,LengthOfBuffer);
        return(true);
    }
}

// -----------------------------------------------------------------------------

bool CSmallString::IsInt(void) const
{
    int d;
    if( Buffer == NULL ) return(false);
    if( isdigit(Buffer[0]) == 0 ){
        if( (Buffer[0] != '+') && (Buffer[0] != '-') ) return(false);
    }
    return( sscanf(Buffer,"%d",&d) != 0 );
}

// -----------------------------------------------------------------------------

int CSmallString::ToInt(void) const
{
    int d=0;
    if( Buffer == NULL ) return(0);
    sscanf(Buffer,"%d",&d);
    return(d);
}

// -----------------------------------------------------------------------------

unsigned int CSmallString::ToUInt(void) const
{
    unsigned int d=0;
    if( Buffer == NULL ) return(0);
    sscanf(Buffer,"%u",&d);
    return(d);
}

// -----------------------------------------------------------------------------

long int CSmallString::ToLInt(void) const
{
    long int d=0;
    if( Buffer == NULL ) return(0);
    sscanf(Buffer,"%ld",&d);
    return(d);
}

// -----------------------------------------------------------------------------

long unsigned int CSmallString::ToLUInt(void) const
{
    long unsigned int d=0;
    if( Buffer == NULL ) return(0);
    sscanf(Buffer,"%lu",&d);
    return(d);
}

// -----------------------------------------------------------------------------

bool CSmallString::ToInt(int& num) const
{
    if( Buffer == NULL ) return(false);
    return( sscanf(Buffer,"%d",&num) == 1);
}

// -----------------------------------------------------------------------------

bool CSmallString::ToInt(unsigned int& num) const
{
    if( Buffer == NULL ) return(false);
    return( sscanf(Buffer,"%u",&num) == 1);
}

// -----------------------------------------------------------------------------

bool CSmallString::ToInt(long int& num) const
{
    if( Buffer == NULL ) return(false);
    return( sscanf(Buffer,"%ld",&num) == 1);
}

// -----------------------------------------------------------------------------

bool CSmallString::ToInt(long unsigned int& num) const
{
    if( Buffer == NULL ) return(false);
    return( sscanf(Buffer,"%lu",&num) == 1);
}

// -----------------------------------------------------------------------------

bool CSmallString::BoolToStr(bool value)
{
    if( Buffer ) free(Buffer);

    if( value == true ) {
        LengthOfBuffer = 5;
        Buffer = (char*)malloc(LengthOfBuffer*sizeof(char));
        if( Buffer == NULL ) {
            LengthOfBuffer = 0;
            return(false);
        }
        strcpy(Buffer,"true");
        return(true);
    } else {
        LengthOfBuffer = 6;
        Buffer = (char*)malloc(LengthOfBuffer*sizeof(char));
        if( Buffer == NULL ) {
            LengthOfBuffer = 0;
            return(false);
        }
        strcpy(Buffer,"false");
        return(true);
    }
}

// -----------------------------------------------------------------------------

bool  CSmallString::ToBool(void) const
{
    if( Buffer == NULL ) return(false);
    if( Buffer[0] == 't' ) return(true);
    return(false);
}

// -----------------------------------------------------------------------------

bool CSmallString::FloatToStr(float value,const char* p_format)
{
    char string[256];
    string[0]='\0';
    sprintf(string,p_format,value);

    if( Buffer ) free(Buffer);

    if( (string == NULL)||(strcmp(string,"")==0) ) {
        LengthOfBuffer = 0;
        Buffer = NULL;
        return(false);
    } else {
        LengthOfBuffer = strlen(string);
        LengthOfBuffer++;
        Buffer = (char*)malloc(LengthOfBuffer*sizeof(char));
        if( Buffer == NULL ) {
            LengthOfBuffer = 0;
            return(false);
        }
        memcpy(Buffer,string,LengthOfBuffer);
        return(true);
    }
}

// -----------------------------------------------------------------------------

float  CSmallString::ToFloat(void) const
{
    float d=0;
    if( Buffer == NULL ) return(0);
    sscanf(Buffer,"%f",&d);
    return(d);
}

// -----------------------------------------------------------------------------

bool CSmallString::DoubleToStr(double value,const char* p_format)
{
    char string[256];
    string[0]='\0';
    sprintf(string,p_format,value);

    if( Buffer ) free(Buffer);

    if( (string == NULL)||(strcmp(string,"")==0) ) {
        LengthOfBuffer = 0;
        Buffer = NULL;
        return(false);
    } else {
        LengthOfBuffer = strlen(string);
        LengthOfBuffer++;
        Buffer = (char*)malloc(LengthOfBuffer*sizeof(char));
        if( Buffer == NULL ) {
            LengthOfBuffer = 0;
            return(false);
        }
        memcpy(Buffer,string,LengthOfBuffer);
        return(true);
    }
}

// -----------------------------------------------------------------------------

double  CSmallString::ToDouble(void) const
{
    double d=0;
    if( Buffer == NULL ) return(0);
    sscanf(Buffer,"%lf",&d);
    return(d);
}

// -----------------------------------------------------------------------------

bool  CSmallString::ToDouble(double& value) const
{
    if( Buffer == NULL ) return(false);
    int rec = sscanf(Buffer,"%lf",&value);
    return(rec == 1);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

int CSmallString::FindSubString(const char* p_string,int offset) const
{
    if((Buffer == NULL)||(p_string==NULL))return(-1);
    char* newstr = strstr(&Buffer[offset],p_string);
    if( newstr == NULL ) return(-1);

    return(newstr-Buffer);
}

// -----------------------------------------------------------------------------

int CSmallString::FindSubString(const CSmallString& string,int offset) const
{
    if((Buffer == NULL)||(string.Buffer==NULL))return(-1);
    char* newstr = strstr(&Buffer[offset],string.Buffer);
    if( newstr == NULL ) return(-1);

    return(newstr-Buffer);
}

// -----------------------------------------------------------------------------

CSmallString CSmallString::GetSubString(unsigned int from,
        unsigned int length) const
{
    CSmallString ret_string;
    // extract substring

    if( (Buffer != NULL) && (length != 0) ) {
        ret_string.LengthOfBuffer = length+1;
        ret_string.Buffer = (char*)malloc(ret_string.LengthOfBuffer*sizeof(char));
        memcpy(ret_string.Buffer,&Buffer[from],ret_string.LengthOfBuffer);
        ret_string.Buffer[ret_string.LengthOfBuffer-1]='\0';
    }

    return(ret_string);
}

// -----------------------------------------------------------------------------

CSmallString CSmallString::GetSubStringFromTo(unsigned int from,
        unsigned int to) const
{
    CSmallString ret_string;
    // extract substring

    if( (Buffer != NULL) && (to - from >= 0) ) {
        ret_string.LengthOfBuffer = to - from + 2;
        ret_string.Buffer = (char*)malloc(ret_string.LengthOfBuffer*sizeof(char));
        memcpy(ret_string.Buffer,&Buffer[from],ret_string.LengthOfBuffer);
        ret_string.Buffer[ret_string.LengthOfBuffer-1]='\0';
    }

    return(ret_string);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

void CSmallString::ToUpperCase(void)
{
    for(unsigned int i=0; i < LengthOfBuffer; i++) {
        Buffer[i] = toupper(Buffer[i]);
    }
}

//------------------------------------------------------------------------------

CSmallString CSmallString::GetUpperCase(void) const
{
    CSmallString str;

    if( Buffer == NULL ) return(str);

    str.LengthOfBuffer = LengthOfBuffer;
    str.Buffer = (char*)malloc(LengthOfBuffer*sizeof(char));

    for(unsigned int i=0; i < LengthOfBuffer; i++) {
        str.Buffer[i] = toupper(Buffer[i]);
    }

    return(str);
}

//------------------------------------------------------------------------------

void CSmallString::ToLowerCase(void)
{
    for(unsigned int i=0; i < LengthOfBuffer; i++) {
        Buffer[i] = tolower(Buffer[i]);
    }
}

//------------------------------------------------------------------------------

CSmallString CSmallString::GetLowerCase(void) const
{
    CSmallString str;

    if( Buffer == NULL ) return(str);

    str.LengthOfBuffer = LengthOfBuffer;
    str.Buffer = (char*)malloc(LengthOfBuffer*sizeof(char));

    for(unsigned int i=0; i < LengthOfBuffer; i++) {
        str.Buffer[i] = tolower(Buffer[i]);
    }

    return(str);
}

//------------------------------------------------------------------------------

void CSmallString::Trim(void)
{
    unsigned int start=0;
    int stop=GetLength()-1;
    for(;start < GetLength(); start++){
        if( isspace(Buffer[start]) == false ) break;
    }
    for(;stop >= 0; stop--){
        if( isspace(Buffer[stop]) == false ) break;
    }
    *this = GetSubStringFromTo(start,stop);
}

//------------------------------------------------------------------------------

CSmallString CSmallString::GetSubstitute(char before,char after) const
{
    CSmallString str;

    if( Buffer == NULL ) return(str);

    str.LengthOfBuffer = LengthOfBuffer;
    str.Buffer = (char*)malloc(LengthOfBuffer*sizeof(char));

    for(unsigned int i=0; i < LengthOfBuffer; i++) {
        if( Buffer[i] == before ) str.Buffer[i] = after;
        else  str.Buffer[i] = Buffer[i];
    }

    return(str);
}

//------------------------------------------------------------------------------

void CSmallString::Substitute(char before,char after) const
{
    for(unsigned int i=0; i < LengthOfBuffer; i++) {
        if( Buffer[i] == before ) Buffer[i] = after;
    }
}

//------------------------------------------------------------------------------

int CSmallString::IndexOf(const char c,int offset) const
{
    for(unsigned int i=offset; i < LengthOfBuffer; i++) {
        if( Buffer[i] == c ) return(i);
    }
    return(-1);
}

//------------------------------------------------------------------------------

int CSmallString::Verify(const char* p_vlist,int from,int to,bool back) const
{
    if( from == -1 ) from = 0;
    if( to == -1 ) to = LengthOfBuffer-2;   // -1 for \0 and -1 <=

    if( p_vlist == NULL ) return(-1);
    unsigned int len = strlen(p_vlist);
    if( len == 0 ) return(-1);

    if( back == false ) {
        for(int i=from; i <= to; i++) {
            bool found = false;
            for(unsigned int j=0; j < len; j++) {
                if( Buffer[i] == p_vlist[j] ) {
                    found = true;
                    break;
                }
            }
            if( found == false ) return(i);
        }
    } else {
        for(int i=to; i >= from; i--) {
            bool found = false;
            for(unsigned int j=0; j < len; j++) {
                if( Buffer[i] == p_vlist[j] ) {
                    found = true;
                    break;
                }
            }
            if( found == false ) return(i);
        }
    }

    return(-1);
}

//------------------------------------------------------------------------------

int CSmallString::Scan(const char* p_vlist,int from,int to,bool back) const
{
    if( from == -1 ) from = 0;
    if( to == -1 ) to = LengthOfBuffer-2;  // -1 for \0 and -1 <=

    if( p_vlist == NULL ) return(-1);
    unsigned int len = strlen(p_vlist);
    if( len == 0 ) return(-1);

    if( back == false ) {
        for(int i=from; i <= to; i++) {
            bool found = false;
            for(unsigned int j=0; j < len; j++) {
                if( Buffer[i] == p_vlist[j] ) {
                    found = true;
                    break;
                }
            }
            if( found == true ) return(i);
        }
    } else {
        for(int i=to; i >= from; i--) {
            bool found = false;
            for(unsigned int j=0; j < len; j++) {
                if( Buffer[i] == p_vlist[j] ) {
                    found = true;
                    break;
                }
            }
            if( found == true ) return(i);
        }
    }

    return(-1);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

bool CSmallString::ReadStringFromFile(FILE* p_file)
{
    // skip white characters
    int c;
    do {
        c = fgetc(p_file);
        if( feof(p_file) != 0 ) return(false);
    } while(isspace(c) != 0);

    ungetc(c,p_file);

    char buffer[256];
    buffer[255] = '\0';

    // read string
    bool end;
    do {
        end = true;
        if( fscanf(p_file,"%255s",buffer) == 1 ) {
            *this += buffer;
        } else {
            return(false);
        }
        c = fgetc(p_file);
        if( isspace(c) != 0 ) end = false;
        ungetc(c,p_file);
    } while(end == true);

    return(true);
}

//------------------------------------------------------------------------------

bool CSmallString::ReadLineFromFile(FILE* p_file,bool do_not_store_nl,bool clear_string)
{
    if( clear_string ){
        SetLength(0);
    }

    // skip white characters
    char buffer[256];
    buffer[255] = '\0';

    // read string
    bool end;
    do {
        end = false;
        if( fgets(buffer,254,p_file) != NULL ) {
            *this += buffer;
        } else {
            return(false);
        }
        int len = strlen(buffer);
        if( len == 254 ) {
            end = buffer[len-1] == '\n';
        } else {
            break;
        }
    } while(end == false);

    if( do_not_store_nl ){
        // remove trailing \n
        if( LengthOfBuffer > 0 ){
            if( Buffer[LengthOfBuffer-2] == '\n' ){
                LengthOfBuffer--;
                if( LengthOfBuffer > 0 ){
                    Buffer = (char*)realloc(Buffer,LengthOfBuffer);
                    Buffer[LengthOfBuffer-1] = '\0';
                } else {
                    free(Buffer);
                    Buffer = NULL;
                }
            }
        }
    }

    return(true);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

#ifdef HAVE_VCL

CSmallString::CSmallString(const AnsiString& string)
{
    LengthOfBuffer = string.Length();

    if( LengthOfBuffer == 0  ) {
        Buffer = NULL;
        return;
    }

    LengthOfBuffer++;  // +1 pro znak \0

    Buffer = (char*)malloc(LengthOfBuffer*sizeof(char));
    memcpy(Buffer,string.c_str(),LengthOfBuffer);
}

//------------------------------------------------------------------------------

void CSmallString::operator = (const AnsiString& string)
{
    if( Buffer ) free(Buffer);
    LengthOfBuffer = string.Length();

    if( LengthOfBuffer ==0 ) {
        Buffer = NULL;
        return;
    } else {
        LengthOfBuffer++;
        Buffer = (char*)malloc(LengthOfBuffer*sizeof(char));
        memcpy(Buffer,string.c_str(),LengthOfBuffer);
        return;
    }
}

//------------------------------------------------------------------------------

CSmallString::operator const AnsiString() const
{
    AnsiString retstring;
    if( Buffer ) retstring = Buffer;
    return(retstring);
}

//------------------------------------------------------------------------------

bool  CSmallString::operator==(const AnsiString& string) const
{
    if( LengthOfBuffer != string.Length() ) return(false);
    if( (Buffer == NULL) || (string.c_str() == NULL ) ) return( Buffer == string.c_str() );
    return( strcmp(Buffer,string.c_str()) == 0);
}

//------------------------------------------------------------------------------

bool  CSmallString::operator!=(const AnsiString& string) const
{
    if( LengthOfBuffer != string.Length() ) return(true);
    if( (Buffer == NULL) || (string.c_str() == NULL ) ) return( Buffer != string.c_str() );
    return( strcmp(Buffer,string.c_str()) != 0);
}

#endif

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

#ifdef HAVE_QT5

CSmallString::CSmallString(const QString& string)
{
    if( string.toLatin1().constData() != NULL ) {
        LengthOfBuffer = strlen(string.toLatin1().constData());
    }

    if( LengthOfBuffer == 0  ) {
        Buffer = NULL;
        return;
    }

    LengthOfBuffer++;  // +1 pro znak \0

    Buffer = (char*)malloc(LengthOfBuffer*sizeof(char));
    memcpy(Buffer,string.toLatin1().constData(),LengthOfBuffer);
}

//------------------------------------------------------------------------------

void CSmallString::operator = (const QString& string)
{
    if( Buffer ) free(Buffer);
    if( string.toLatin1().constData() != NULL ) {
        LengthOfBuffer = strlen(string.toLatin1().constData());
    } else {
        LengthOfBuffer = 0;
    }

    if( LengthOfBuffer ==0 ) {
        Buffer = NULL;
        return;
    } else {
        LengthOfBuffer++;
        Buffer = (char*)malloc(LengthOfBuffer*sizeof(char));
        memcpy(Buffer,string.toLatin1().constData(),LengthOfBuffer);
        return;
    }
}

//------------------------------------------------------------------------------

bool CSmallString::operator==(const QString& string) const
{
    if( (Buffer == NULL) || (string.toLatin1().constData() == NULL ) ) {
        return( Buffer == string.toLatin1().constData() );
    }
    return( strcmp(Buffer,string.toLatin1().constData()) == 0);
}

//------------------------------------------------------------------------------

bool CSmallString::operator!=(const QString& string) const
{
    if( (Buffer == NULL) || (string.toLatin1().constData() == NULL ) ) {
        return( Buffer != string.toLatin1().constData() );
    }
    return( strcmp(Buffer,string.toLatin1().constData()) != 0);
}

//------------------------------------------------------------------------------

#endif

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================


