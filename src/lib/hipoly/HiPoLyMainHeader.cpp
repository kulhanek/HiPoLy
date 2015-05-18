//==============================================================================
//    Copyright 1998,1999,2000,2001,2005 Petr Kulhanek
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

//------------------------------------------------------------------------------

const char* LibBuildVersion_HiPoLy = HIPOLY_VERSION;

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

#if defined _WIN32 || defined __CYGWIN__

#include <windows.h>

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    return(TRUE);
}

//==============================================================================
#else
//==============================================================================

#include <stdio.h>
#include <unistd.h>

// under UNIX we try to make library as executable object that will
// print copyright notice and build number

// you need to compile shared library with following options

// -e __hipoly_main                         <- define entry point
// --dynamic-loader /ld/ld-linux.so.2       <- redefine dynamic loader
// -pie                                     <- make position independent executable
// --export-dynamic                         <- export symbols

// declare entry point
extern "C" void __hipoly_main(void) __attribute__ ((noreturn));

// define entry point
extern "C" void __hipoly_main(void)
{
    printf("\n");
    printf("===========================================================\n");
    printf(" ************ HiPoLy - Highly Portable Library *********** \n");
    printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    printf(" (c) 1998-2009                                             \n");
    printf("     Library maintainer: Petr Kulhanek                     \n");
    printf("                         kulhanek@chemi.muni.cz            \n");
    printf("     HiPoLy is licenced under GNU Lesser GPL.              \n");
    printf("===========================================================\n");
    printf(" Build: %s\n",LibBuildVersion_HiPoLy);
    printf("===========================================================\n");
    printf("\n");
    _exit(0);
}

#endif

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
