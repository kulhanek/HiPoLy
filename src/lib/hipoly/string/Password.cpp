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

#include <Password.hpp>

#if defined _WIN32 || defined __CYGWIN__
#include <Windows.h>
#else
#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#endif

#include <errno.h>
#include <string.h>
#include <ErrorSystem.hpp>

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================


CPassword::CPassword(void)
{

}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

bool CPassword::ReadFromCmdLine(void)
{
    Password = NULL;

#if defined _WIN32 || defined __CYGWIN__

    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    if( hStdin == INVALID_HANDLE_VALUE ) return(false);

    DWORD mode;

    // disable echo
    GetConsoleMode(hStdin, &mode);
    mode &= ~ENABLE_ECHO_INPUT;
    SetConsoleMode(hStdin, mode);

    // read password
    char ch;
    for(;;){
        DWORD rl = 0;
        if( ReadFile(hStdin,&ch,1,&rl,NULL) != TRUE ) break;
        if( rl != 1 ) break;
        if( ch == '\n' ) continue;
        if( ch == '\r' ) break;
        Password += ch;
    }

    // enable echo
    mode |= ENABLE_ECHO_INPUT;
    SetConsoleMode(hStdin, mode);

    // write \n
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    if( hStdout != INVALID_HANDLE_VALUE ){
        const char* nl = "\n";
        DWORD wl = 0;
        WriteFile(hStdout,nl,strlen(nl),&wl,NULL);
    }

    return(true);
#else
    // open tty
    int ftty;
    ftty = open("/dev/tty",O_RDWR);
    if( ftty == -1 ) {
        CSmallString error;
        error << "unable to open /dev/tty (" << strerror(errno) << ")";
        ES_ERROR(error);
        return(false);
    }

    termios oterm;  // old term setup
    termios nterm;  // new term setup

    // supress echos on terminal

    // get current termnal setup
    tcgetattr(ftty, &oterm);
    memcpy(&nterm,&oterm,sizeof(termios));

    // disable echos
    nterm.c_lflag &= ~(ECHO | ECHONL);

    // set new terminal setup
    tcsetattr(ftty,TCSAFLUSH, &nterm);

    // now read password
    char ch;
    while( read(ftty,&ch,sizeof(ch)) > 0 ) {
        if( (ch == '\n') ||  (ch == '\r') ) break;
        Password += ch;
    }

    // restore old terminal setup
    tcsetattr(ftty, TCSAFLUSH, &oterm);

    // write \n to tty
    if( write(ftty,"\n",1) == 0 ) {
        // error - intentionaly not handled
    }

    // close tty
    close(ftty);

    return(true);
#endif
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

bool CPassword::ReadFromFile(const CSmallString& name)
{
    Password = NULL;   // clear current password

    FILE* fin;

    // open file
    fin = fopen(name,"r");
    if( fin == NULL ) {
        CSmallString error;
        error << "unable to open password file '" << name << "' (" << strerror(errno) << ")";
        ES_ERROR(error);
        return(false);
    }

    bool result;

    // read password as single word
    result = ReadFromFile(fin);

    // close file
    fclose(fin);

    return(result);
}

//------------------------------------------------------------------------------

bool CPassword::ReadFromFile(FILE* fin)
{
    Password = NULL;   // clear current password

    bool result = Password.ReadStringFromFile(fin);
    if( result == false ) {
        ES_ERROR("unable to read password from file stream");
    }
    return(result);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

const CSmallString& CPassword::GetPassword(void)
{
    return(Password);
}

//------------------------------------------------------------------------------

bool CPassword::IsStrongPassword(void)
{
    // FIXME
    if( Password.GetLength() > 4 ) return(true);
    return(false);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
