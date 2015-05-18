/***************************************************************************
 *   Copyright (C) 2007 by Petr Kulhanek                                   *
 *                         kulhanek@chemi.muni.cz                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "options.hpp"
#include "SmallGetOpt.hpp"

//------------------------------------------------------------------------------

int main(int argc, char* argv[])
{
    //  CSmallGetOpt    getopt;
    //
    //  getopt.SetCmdLine(argc,argv);
    //  getopt.SetShortOptList("cde:");
    //  getopt.GetOpt();
    //  getopt.PrintCmdLine();
    //
    //  for(int i=0; i < getopt.GetNumberOfProgArgs(); i++){
    //     printf("arg %d is %s\n",i+1,(char*)getopt.GetProgArg(i));
    //     }
    //
    //  getopt.Restart();
    //  getopt.PrintCmdLine();

//    CSmallString test("   po kus  \t ");
//    printf("'%s'\n",(const char*)test);
//    test.Trim();
//    printf("'%s'\n",(const char*)test);

    CMyOptions opt;

    opt.ParseCmdLine(argc,argv);
    opt.PrintCmdLine();
    if( opt.GetOptHelp()) opt.PrintUsage();

    return(0);
}

//------------------------------------------------------------------------------
