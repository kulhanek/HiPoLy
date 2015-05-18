#ifndef EventManagerH
#define EventManagerH
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
#include <EventReceivers.hpp>

//------------------------------------------------------------------------------

class HIPOLY_PACKAGE CEventManager {
public:
// constructors and destructors -----------------------------------------------
    CEventManager(void);
    virtual ~CEventManager(void);

    bool RegisterEventForServer(const CEventIdentifier& event,
            CEventManager* server);
    bool UnregisterEventForServer(const CEventIdentifier& event,
            CEventManager* server);
    bool UnregisterAllEvents(void);

    bool UnregisterAllEventsForClient(CEventManager* client);

    void ThrowEvent(const CEventIdentifier& event);
    void ThrowEvent(const CEventIdentifier& event,void* p_data);

    virtual bool ProcessEvent(const CEventIdentifier& event,
                                           CEventManager* Sender,void* p_data);

    /// disable events - no events are emmited or received
    void EnableEvents(bool status);

// section of private data ---------------------------------------------------
private:
    CSimpleList<CEventRecievers>    RegisteredEvents;
    CSimpleList<CEventManager>      Servers;
    CEventRecievers*                RunningEventReceiver;
    bool                            EnabledEvents;

    friend class CEventRecievers;
};

//------------------------------------------------------------------------------

#endif
