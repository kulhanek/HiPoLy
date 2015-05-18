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

#include <EventManager.hpp>

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CEventManager::CEventManager(void)
{
    EnabledEvents = true;
    RunningEventReceiver = NULL;
}

//------------------------------------------------------------------------------

CEventManager::~CEventManager(void)
{
    // unregister all events on other servers
    UnregisterAllEvents();

    // unregister all events on this server
    CSimpleIterator<CEventRecievers> R(RegisteredEvents);
    CEventRecievers* p_r;

    while((p_r = R.Current())!=NULL) {
        p_r->UnregisterClientOnServerSide(this);
        R++;
    }
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

bool CEventManager::RegisterEventForServer(const CEventIdentifier& event,CEventManager* server)
{
    if( server == NULL ) return(false);

    // zaregistruje se server
    if( Servers.IsInList(server) == false ) Servers.InsertToEnd(server);

    // registrace ud�losti
    CSimpleIterator<CEventRecievers> R(server->RegisteredEvents);
    CEventRecievers* p_r;

    while((p_r = R.Current())!=NULL) {
        if( p_r->GetEvent() == event) {
            // ud�lost ji� existuje - registruje �adatele
            return(p_r->RegisterClient(this));
            // vrac� false pokud ji� existuje
        }
        R++;
    }
    p_r = new CEventRecievers(event);
    p_r->RegisterClient(this);
    server->RegisteredEvents.InsertToEnd(p_r,0,true);
    return(true);
}

//------------------------------------------------------------------------------

bool CEventManager::UnregisterEventForServer(const CEventIdentifier& event,CEventManager* server)
{
    if( server == NULL ) return(false);

    // odregistruje se server
    Servers.Remove(server);

    // odregistruje se ud�lost
    CSimpleIterator<CEventRecievers> R(server->RegisteredEvents);
    CEventRecievers* p_r;

    while((p_r = R.Current())!=NULL) {
        if( p_r->GetEvent() == event) {
            // ud�lost ji� existuje - odregistruje �adatele
            p_r->UnregisterClient(this);
            if( p_r->IsEmpty() == true ) {
                if( p_r == RunningEventReceiver ) {
                    p_r->RemoveAfterThrowEvent=true;
                } else {
                    R.Remove(); // odstran� cel� objekt
                }
            }
            return(true);
        }
        R++;
    }
    return(false);  // ud�lost pro odregistrovan� neexistuje
}

//------------------------------------------------------------------------------

bool CEventManager::UnregisterAllEventsForClient(CEventManager* client)
{
    if( client == NULL ) return(false);

    client->Servers.Remove(this);

    CSimpleIterator<CEventRecievers> R(RegisteredEvents);
    CEventRecievers* p_r;

    while((p_r = R.Current())!=NULL) {
        p_r->UnregisterClient(client);
        if( p_r->IsEmpty() == true ) {
            if( p_r == RunningEventReceiver ) {
                p_r->RemoveAfterThrowEvent=true;
                R++;
            } else {
                R.Remove(); // odstran� cel� objekt
            }
        } else {
            R++;
        }
    }
    return(false);
}

//------------------------------------------------------------------------------

bool CEventManager::UnregisterAllEvents(void)
{
    CEventManager* p_r;

    while((p_r = Servers.GetFirst())!=NULL) {
        p_r->UnregisterAllEventsForClient(this);
    }
    return(true);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

void CEventManager::ThrowEvent(const CEventIdentifier& event)
{
    ThrowEvent(event,NULL);
}

//------------------------------------------------------------------------------

void CEventManager::ThrowEvent(const CEventIdentifier& event,void* p_data)
{
    if( EnabledEvents == false ) return;
    if( RegisteredEvents.IsEmpty()==true )return;

    CSimpleIterator<CEventRecievers> R(RegisteredEvents);
    RunningEventReceiver=NULL;
    CEventRecievers* p_r;
    while((p_r = R.Current())!=NULL) {
        if( p_r->GetEvent() == event) {
            // ud�lost existuje - vyvol� ud�lost
            RunningEventReceiver=p_r;
            p_r->RemoveAfterThrowEvent = false;
            p_r->ThrowEvent(this,p_data);
            if( p_r->RemoveAfterThrowEvent == true ) {
                R.Remove();
            }
            return;
        }
        R++;
    }
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

bool CEventManager::ProcessEvent(const CEventIdentifier& event,CEventManager* Sender,void* p_data)
{
    return(false);
}

//------------------------------------------------------------------------------

void CEventManager::EnableEvents(bool status)
{
    EnabledEvents = status;
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

