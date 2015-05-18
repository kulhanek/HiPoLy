//==============================================================================
//    Copyright 2001,2002,2003,2004,2006,2008 Petr Kulhanek
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

#include <SummaryEvents.hpp>

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CSummaryEvents  SummaryEvents;

DEFINE_EVENT(SummaryEvents_Start);
DEFINE_EVENT(SummaryEvents_Finish);

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CSummaryEvents::CSummaryEvents(void)
//    : CExtComObject(NULL,NULL)
{
    Depth = 0;
}

//---------------------------------------------------------------------------

void CSummaryEvents::StartEvents(void)
{
    if( Depth == 0 ) ThrowEvent(SummaryEvents_Start);
    Depth++;
}

//---------------------------------------------------------------------------

void CSummaryEvents::FinishEvents(void)
{
    Depth--;
    if( Depth == 0 ) ThrowEvent(SummaryEvents_Finish);
}

//---------------------------------------------------------------------------
