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

#include <stdarg.h>
#include <ctype.h>
#include <stdio.h>
#include <Terminal.hpp>
#include <ErrorSystem.hpp>
#include <cstring>

#if ! (defined _WIN32 || defined __CYGWIN__)
#include <sys/ioctl.h>
#include <unistd.h>
#endif

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CTerminal::CTerminal(void)
{
    FOut = stdout;
    DetectTerminal();
    ResetFormattedStream();
    SetDefault();
}

//------------------------------------------------------------------------------

CTerminal::CTerminal(FILE* fout)
{
    FOut = fout;
    DetectTerminal();
    ResetFormattedStream();
    SetDefault();
}

//------------------------------------------------------------------------------

bool CTerminal::IsTerminal(FILE* p_f)
{
#if ! (defined _WIN32 || defined __CYGWIN__)
    if( p_f == NULL ) return(false);

    // is it terminal?
    int file_id = fileno(p_f);
    if( isatty(file_id) != 0 ) return(true);

#endif

    return(false);
}

//------------------------------------------------------------------------------

//! set output stream
void CTerminal::SetOutputStream(FILE* fout)
{
    FOut = fout;
    DetectTerminal();
    SetDefault();
}

//------------------------------------------------------------------------------

void CTerminal::SetNumberOfColumns(int columns)
{
    NColumns = columns;
}

//------------------------------------------------------------------------------

void CTerminal::DetectTerminal(void)
{
    // default values
    ColorsAvailable = false;
    ColorsEnabled = true;
    NRows=40;
    NColumns=80;

#if ! (defined _WIN32 || defined __CYGWIN__)
    if( FOut == NULL ) return;

    // is it terminal?
    int file_id = fileno(FOut);

    if( isatty(file_id) == 0 ) return; // no

    // file is connected to terminal get its properties

    // dimensions
    struct winsize winsize;
    if( ioctl(file_id,TIOCGWINSZ,&winsize) != 0 ) return;

    NRows = winsize.ws_row;
    NColumns = winsize.ws_col;
    ColorsAvailable = true;
#endif
}
//------------------------------------------------------------------------------

bool CTerminal::GetSize(int &nrows,int &ncolumns)
{
    nrows = 1;
    ncolumns = 80;

    // is it terminal?
    int file_id = fileno(stdin);

    if( isatty(file_id) == 0 ) return(false); // no

    // file is connected to terminal get its properties

    // dimensions
    struct winsize winsize;
    if( ioctl(file_id,TIOCGWINSZ,&winsize) != 0 ) return(false);

    nrows = winsize.ws_row;
    ncolumns = winsize.ws_col;
    return(true);
}

//------------------------------------------------------------------------------

void CTerminal::ForceColors(void)
{
    // default values
    ColorsAvailable = true;
    ColorsEnabled = true;
    NRows=40;
    NColumns=80;

#if ! (defined _WIN32 || defined __CYGWIN__)

    // is it terminal?
    int file_id = fileno(stdin);

    if( isatty(file_id) == 0 ) return; // no

    // file is connected to terminal get its properties

    // dimensions
    struct winsize winsize;
    if( ioctl(file_id,TIOCGWINSZ,&winsize) != 0 ) return;

    NRows = winsize.ws_row;
    NColumns = winsize.ws_col;
#endif
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

int CTerminal::GetNumberOfColumns(void) const
{
    return(NColumns);
}

//------------------------------------------------------------------------------

int CTerminal::GetNumberOfRows(void) const
{
    return(NRows);
}

//------------------------------------------------------------------------------

bool CTerminal::AreColorsAvailable(void) const
{
    return(ColorsAvailable);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

bool CTerminal::SetColors(int foreground,int background) const
{
    if( FOut == NULL ) return(false);

    if( ColorsAvailable && ColorsEnabled && ((foreground >= 0) || (background >=0)) ) {
        fprintf(FOut,"\033[");
        if( foreground >= 0 ) fprintf(FOut,"%d",foreground+30);
        if( (foreground >= 0) && (background >=0) ) fprintf(FOut,";");
        if( background >= 0 ) fprintf(FOut,"%d",background+40);
        fprintf(FOut,"m");
    }
    return(true);
}

//------------------------------------------------------------------------------

bool CTerminal::SetColors(EColor foreground,EColor background) const
{
    return(SetColors((int)foreground,(int)background));
}

//------------------------------------------------------------------------------

bool CTerminal::SetBold(bool set) const
{
    if( FOut == NULL ) return(false);

    if( set ) {
        if( ColorsAvailable && ColorsEnabled ) fprintf(FOut,"\033[1m");
    } else {
        if( ColorsAvailable && ColorsEnabled ) fprintf(FOut,"\033[22m");
    }
    return(true);
}

//------------------------------------------------------------------------------

bool CTerminal::SetUnderline(bool set) const
{
    if( FOut == NULL ) return(false);

    if( set ) {
        if( ColorsAvailable && ColorsEnabled ) fprintf(FOut,"\033[4m");
    } else {
        if( ColorsAvailable && ColorsEnabled ) fprintf(FOut,"\033[24m");
    }
    return(true);
}

//------------------------------------------------------------------------------

bool CTerminal::SetItalic(bool set) const
{
    if( FOut == NULL ) return(false);

    if( set ) {
        if( ColorsAvailable && ColorsEnabled ) fprintf(FOut,"\033[3m");
    } else {
        if( ColorsAvailable && ColorsEnabled ) fprintf(FOut,"\033[23m");
    }
    return(true);
}

//------------------------------------------------------------------------------

bool CTerminal::SetDefault(bool only_colors) const
{
    if( FOut == NULL ) return(false);

    if( only_colors ) {
        if( ColorsAvailable && ColorsEnabled ) fprintf(FOut,"\033[39m\033[49m");
    } else {
        if( ColorsAvailable && ColorsEnabled ) fprintf(FOut,"\033[0m");
    }

    return(true);
}

//------------------------------------------------------------------------------

void CTerminal::EnableColors(bool set)
{
    ColorsEnabled = set;
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

void CTerminal::AddItem(const CSmallString& text)
{
    List.push_back(text);
}

//------------------------------------------------------------------------------

void CTerminal::ClearList(void)
{
    List.clear();
}

//------------------------------------------------------------------------------

bool CTerminal::PrintColumnSortedList(unsigned int column_width)
{
    if( FOut == NULL ) return(false);

    if( column_width <= 0 ) {
        column_width = 0;
        for(unsigned int i=0; i < List.size(); i++) {
            if( List[i].GetLength() > column_width ) column_width = List[i].GetLength();
        }
        column_width++; // add one character for space among words
    }

    int num_col = NColumns / column_width;
    int num_ite = List.size();
    int num_row = num_ite / num_col;
    if( num_ite % num_col > 0) num_row++;

    for(int i=0; i < num_row; i++) {
        for(int j=0; j < num_col; j++ ) {
            int index = i + j*num_row;
            if( index < num_ite ) fprintf(FOut,"%-*s",column_width,(const char*)List[index]);
        }
        fprintf(FOut,"\n");
    }

    return(true);
}

//------------------------------------------------------------------------------

bool CTerminal::PrintPlainList(void)
{
    if( FOut == NULL ) return(false);

    for(unsigned int i=0; i < List.size(); i++) fprintf(FOut,"%s\n",(const char*)List[i]);

    return(true);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

bool CTerminal::PrintTitle(const CSmallString& title,
                                        char delimiter,int beginning)
{
    if( FOut == NULL ) return(false);

    for(int i=0; i < beginning; i++) {
        fprintf(FOut,"%c",delimiter);
    }
    fprintf(FOut," %s ",(const char*)title);
    for(int i=title.GetLength()+2+beginning; i < NColumns; i++) {
        fprintf(FOut,"%c",delimiter);
    }

    return(true);
}

//------------------------------------------------------------------------------

bool CTerminal::PrintText(const CSmallString& text,
                                       int column_width,const char* p_leading)
{
    if( FOut == NULL ) return(false);

    if( column_width <= 0 ) column_width = NColumns;

    const char* white_list = " \t\n/";

    int indent = 0;
    int lpos = indent;
    int len  = text.GetLength();
    int maxlen = column_width;

    if( p_leading != NULL ) Printf("%s",p_leading);

    for(int i=0; i < len; i++) {
        // find next non-white character
        int wc  = text.Scan(white_list,i+1);
        if( wc < 0 ) wc = len;

        if( (wc - i + lpos >= maxlen)  ) {
            Printf("\n");
            if( p_leading != NULL ) Printf("%s",p_leading);
            lpos = indent;
            if( isspace(text[i]) == 0 ) Printf("%c",text[i]);
        } else {
            if( text[i] != '\n' ) {
                Printf("%c",text[i]);
            } else {
                Printf("%c",' ');
            }
            lpos++;
        }
    }
    Printf("\n");

    return(true);
}

//------------------------------------------------------------------------------

int CTerminal::Printf(const char* format,...)
{
    if( FOut == NULL ) return(false);

    va_list args;

    va_start(args,format);
    int result = vfprintf(FOut,format,args);
    va_end( args );

    return(result);
}

//------------------------------------------------------------------------------

bool CTerminal::PrintFormatted(const char* text)
{
    bool result = true;
    for(unsigned int i=0; i < std::strlen(text); i++) {
        result &= PrintFormattedChar(text[i]);
    }
    return(result);
}

//------------------------------------------------------------------------------

void CTerminal::ResetFormattedStream(void)
{
    CommandLevel = 0;
    Command.str("");
    Command.clear();
    BlockMode = false;
    LeftOffset = 0;
}

//------------------------------------------------------------------------------

int CTerminal::FlushStream(void)
{
    if( FOut == NULL ) return(0);
    if(fflush ( FOut ) == EOF) {
        return(-1);
    }
    return(0);
}

//------------------------------------------------------------------------------

bool CTerminal::PrintFormattedChar(int c)
{
    if( FOut == NULL ) return(false);

    // levels
    // 12
    // <c>
    // </c>
    // 134

    if( c == '<' ) {
        if( CommandLevel == 1 ) {
            fprintf(FOut,"%c",(char)c);
            Command.str("");
            Command.clear();
            CommandLevel = 0;
            return(true);
        }
        if( CommandLevel != 0 ) {
            fprintf(FOut,"%s","!formating error!");
            Command.str("");
            Command.clear();
            CommandLevel = 0;
            return(false);
        }
        CommandLevel = 1;
        return(true);
    }

    if( c == '/' ) {
        if( CommandLevel == 0 ) {
            fprintf(FOut,"%c",(char)c);
            return(true);
        }
        if( CommandLevel != 1 ) {
            fprintf(FOut,"%s","!formating error1!");
            Command.str("");
            Command.clear();
            CommandLevel = 0;
            return(false);
        }
        CommandLevel = 3;
        return(true);
    }
    if( c == '>' ) {
        if( CommandLevel == 0 ) {
            fprintf(FOut,"%c",(char)c);
            return(true);
        }
        if( (CommandLevel != 2) && (CommandLevel != 4) ) {
            fprintf(FOut,"%s","!formating error2!");
            Command.str("");
            Command.clear();
            CommandLevel = 0;
            return(false);
        }

        std::string cmd = Command.str();

        // apply command
        if( CommandLevel == 2 ) {
            if( cmd == "b" ) {
                SetBold(true);
            } else if ( cmd == "u" ) {
                SetUnderline(true);
            } else if ( cmd == "i" ) {
                SetItalic(true);

            } else if ( cmd == "black" ) {
                SetColors(EC_BLACK);
            } else if ( cmd == "red" ) {
                SetColors(EC_RED);
            } else if ( cmd == "green" ) {
                SetColors(EC_GREEN);
            } else if ( cmd == "yellow" ) {
                SetColors(EC_YELLOW);
            } else if ( cmd == "blue" ) {
                SetColors(EC_BLUE);
            } else if ( cmd == "purple" ) {
                SetColors(EC_PURPLE);
            } else if ( cmd == "cyan" ) {
                SetColors(EC_CYAN);
            } else if ( cmd == "white" ) {
                SetColors(EC_WHITE);
            } else if ( cmd.find("block") == 0 ) {
                BlockMode = true;
                std::string tmp;
                LeftOffset = 0;
                Command >> tmp >> LeftOffset;
                for(int i=0; i < LeftOffset; i++ ) fprintf(FOut," ");
            }
        }

        // revers command
        if( CommandLevel == 4 ) {
            if( cmd == "b" ) {
                SetBold(false);
            } else if ( cmd == "u" ) {
                SetUnderline(false);
            } else if ( cmd == "i" ) {
                SetItalic(false);

            } else if ( cmd == "black" ) {
                SetDefault();
            } else if ( cmd == "red" ) {
                SetDefault();
            } else if ( cmd == "green" ) {
                SetDefault();
            } else if ( cmd == "yellow" ) {
                SetDefault();
            } else if ( cmd == "blue" ) {
                SetDefault();
            } else if ( cmd == "purple" ) {
                SetDefault();
            } else if ( cmd == "cyan" ) {
                SetDefault();
            } else if ( cmd == "white" ) {
                SetDefault();
            } else if ( cmd == "block" ) {
                BlockMode = false;
                LeftOffset = 0;
            }
        }

        Command.str("");
        Command.clear();
        CommandLevel = 0;
        return(true);
    }
    if( (CommandLevel == 1) || (CommandLevel == 2) ) {
        Command << (char)c;
        CommandLevel = 2;
        return(true);
    }
    if( (CommandLevel == 3) || (CommandLevel == 4) ) {
        Command << (char)c;
        CommandLevel = 4;
        return(true);
    }
    if( CommandLevel != 0 ) {
        fprintf(FOut,"%s","!formating error4!");
        Command.str("");
        Command.clear();
        CommandLevel = 0;
        return(false);
    }
    if( BlockMode == false ) {
        fprintf(FOut,"%c",(char)c);
    } else {
        if( c == '\n' ) {
            for(int i=0; i < LeftOffset; i++ ) fprintf(FOut," ");
            fprintf(FOut,"\n");
        } else {
            fprintf(FOut,"%c",(char)c);
        }
    }
    return(true);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

