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

#include <FormatSpec.hpp>
#include <ErrorSystem.hpp>
#include <ctype.h>

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CFormatSpec::CFormatSpec(void)
{

}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

void CFormatSpec::SetFormat(const CSmallString& format)
{
    FormatSpec = format;
}

//------------------------------------------------------------------------------

const CSmallString& CFormatSpec::GetFormat(void) const
{
    return(FormatSpec);
}

//------------------------------------------------------------------------------

bool CFormatSpec::IsFormatValid(EFormatType type)
{
    if( FormatSpec.GetLength() <= 0 ) {
        ES_ERROR("format is empty string");
        return(false);
    }

    int position = 0;

    if( FormatSpec[position] != '%' ) {
        char c = FormatSpec[position];
        CSmallString error;
        error << "format does not start with '%' but '" << c << "'";
        ES_ERROR(error);
        return(false);
    }
    position++;

    if( FormatSpec.GetLength() - position < 1 ) {
        CSmallString error;
        error << "incomplete format (specification is too short after "
              << position+1  << " character)";
        ES_ERROR(error);
        return(false);
    }

    // read modifiers
    if( (FormatSpec[position] == '+') ||
            (FormatSpec[position] == '-') ||
            (FormatSpec[position] == '0') ||
            (FormatSpec[position] == ' ') ) {
        position++;
    }

    if( FormatSpec.GetLength() - position < 1 ) {
        CSmallString error;
        error << "incomplete format (specification is too short after "
              << position+1  << " character)";
        ES_ERROR(error);
        return(false);
    }

    int tot_length = 0;
    int dec_length = 0;

    // is it a digit?
    if( isdigit(FormatSpec[position]) != 0 ) {
        sscanf(&FormatSpec[position],"%d",&tot_length);
        // find the end
        for(; position < (int)FormatSpec.GetLength(); position++) {
            if( isdigit(FormatSpec[position]) == 0 ) break;
        }
    }

    if( (int)FormatSpec.GetLength() - position < 1 ) {
        CSmallString error;
        error << "incomplete format (specification is too short after "
              << position+1  << " character)";
        ES_ERROR(error);
        return(false);
    }

    // dot?
    if( FormatSpec[position] == '.' ) {
        if( tot_length <= 0 ) {
            ES_ERROR("record length is not specified so usege of '.' is not valid");
            return(false);
        }
        position++;
    }
    if( (int)FormatSpec.GetLength() - position < 1 ) {
        CSmallString error;
        error << "incomplete format (specification is too short after "
              << position+1  << " character)";
        ES_ERROR(error);
        return(false);
    }

    // is it a digit?
    if( isdigit(FormatSpec[position]) != 0 ) {
        sscanf(&FormatSpec[position],"%d",&dec_length);
        // find the end
        for(; position < (int)FormatSpec.GetLength(); position++) {
            if( isdigit(FormatSpec[position]) == 0 ) break;
        }
    }
    if( (int)FormatSpec.GetLength() - position < 1 ) {
        CSmallString error;
        error << "incomplete format (specification is too short after "
              << position+1  << " character)";
        ES_ERROR(error);
        return(false);
    }

    // format specifier
    if( (FormatSpec[position] == 's') ||
            (FormatSpec[position] == 'd') ||
            (FormatSpec[position] == 'f') ||
            (FormatSpec[position] == 'e') ) {
        return(true);
    } else {
        CSmallString error;
        error << "unsupported data type '" << FormatSpec[position] << "'";
        ES_ERROR(error);
        return(false);
    }
}

//------------------------------------------------------------------------------

unsigned int CFormatSpec::GetRecordLength(void)
{
    if( FormatSpec.GetLength() <= 0 ) {
        ES_ERROR("format is empty string");
        return(false);
    }

    int position = 0;

    if( FormatSpec[position] != '%' ) {
        char c = FormatSpec[position];
        CSmallString error;
        error << "format does not start with '%' but '" << c << "'";
        ES_ERROR(error);
        return(false);
    }
    position++;

    if( FormatSpec.GetLength() - position < 1 ) {
        CSmallString error;
        error << "incomplete format (specification is too short after "
              << position+1  << " character)";
        ES_ERROR(error);
        return(false);
    }

    // read modifiers
    if( (FormatSpec[position] == '+') ||
            (FormatSpec[position] == '-') ||
            (FormatSpec[position] == '0') ||
            (FormatSpec[position] == ' ') ) {
        position++;
    }

    if( FormatSpec.GetLength() - position < 1 ) {
        CSmallString error;
        error << "incomplete format (specification is too short after "
              << position+1  << " character)";
        ES_ERROR(error);
        return(false);
    }

    unsigned int   tot_length = 0;

    // is it a digit?
    if( isdigit(FormatSpec[position]) != 0 ) {
        sscanf(&FormatSpec[position],"%u",&tot_length);
        // find the end
        for(; position < (int)FormatSpec.GetLength(); position++) {
            if( isdigit(FormatSpec[position]) == 0 ) break;
        }
    }

    if( FormatSpec.GetLength() - position < 1 ) {
        CSmallString error;
        error << "incomplete format (specification is too short after "
              << position+1  << " character)";
        ES_ERROR(error);
        return(false);
    }

    return(tot_length);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
