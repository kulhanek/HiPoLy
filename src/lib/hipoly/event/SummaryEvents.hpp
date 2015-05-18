#ifndef SummaryEventsH
#define SummaryEventsH
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

#include <HiPoLyMainHeader.hpp>
#include <EventIdentifier.hpp>
#include <EventManager.hpp>

//---------------------------------------------------------------------------

/// reduce duplication of events

class HIPOLY_PACKAGE CSummaryEvents : public CEventManager {
public:
    CSummaryEvents(void);

    void StartEvents(void);
    void FinishEvents(void);

private:
    int     Depth;
};

//---------------------------------------------------------------------------

extern CSummaryEvents HIPOLY_PACKAGE  SummaryEvents;

//---------------------------------------------------------------------------

DECLARE_EVENT(HIPOLY_PACKAGE,SummaryEvents_Start);
DECLARE_EVENT(HIPOLY_PACKAGE,SummaryEvents_Finish);

//---------------------------------------------------------------------------

#endif
