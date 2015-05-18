#ifndef OptionsH
#define OptionsH

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

#include "SimpleOptions.hpp"

// -----------------------------------------------------------------------------

class CMyOptions : public CSimpleOptions {
public:

    CSO_LIST_BEGIN
    CSO_OPT(bool,Help)
    CSO_OPT(bool,Version)
    CSO_OPT(bool,Verbose)
    CSO_OPT(CSmallString,ABFAccuFile)
    CSO_OPT(CSmallString,FEFile)
    CSO_LIST_END

// list of options
    CSO_MAP_BEGIN
    CSO_MAP_OPT(CSmallString,                   /* option type */
                ABFAccuFile,                    /* option name */
                "_abf.rst",
                true,
                'i',
                "input",
                "FILE",
                "input FILE name with ABF accumulator")
    CSO_MAP_OPT(CSmallString,
                FEFile,
                "_abf.int",
                true,
                'o',
                "output",
                "FILE",
                "output FILE name with free energy")
    CSO_MAP_OPT(bool,                           /* option type */
                Help,                           /* option name */
                false,                          /* default value */
                false,                          /* is option mandatory */
                'h',                            /* short option name */
                "help",                         /* long option name */
                NULL,                           /* parametr name */
                "display this help and exit")   /* option description */
    CSO_MAP_OPT(bool,
                Verbose,
                false,
                false,
                'v',
                "verbose",
                NULL,
                "increase output verbosity")
    CSO_MAP_OPT(bool,
                Version,
                false,
                false,
                '\0',
                "version",
                NULL,
                "output version information and exit")
    CSO_MAP_END
};

// -----------------------------------------------------------------------------

#endif
