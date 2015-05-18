#ifndef RunnableH
#define RunnableH
//==============================================================================
//    Copyright 2010 Petr Kulhanek
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

/*! \brief runnable object via thread, see CThreadPool
 *
 */

class HIPOLY_PACKAGE CRunnable {
public:
    CRunnable(void);
    virtual  ~CRunnable(void);

// main methods ---------------------------------------------------------------
    /// main execution method
    virtual void Run(void) = 0;

    /// set autodelete flag
    void SetAutoDelete(bool autoDelete);

    /// get autodelete flag
    bool ShouldBeAutoDeleted(void);

// section of private data ----------------------------------------------------
private:
    bool    AutoDelete;
};

//------------------------------------------------------------------------------
#endif
