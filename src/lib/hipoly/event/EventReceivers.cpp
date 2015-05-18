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

#include <EventReceivers.hpp>
#include <EventManager.hpp>

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

 CEventRecievers::CEventRecievers(const CEventIdentifier& event)
    : Event(event),TPH(Clients)
{
    RemoveAfterThrowEvent=false;
    MoveTPH=false;
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

bool CEventRecievers::RegisterClient(CEventManager* client)
{
    CSimpleIterator<CEventManager> H(Clients);
    CEventManager* p_h;

    while((p_h = H.Current())!=NULL) {
        if( p_h == client) {
            // handler ji� existuje - neregistruje se
            return(false);
            // vrac� false pokud ji� existuje
        }
        H++;
    }
    Clients.InsertToEnd(client);
    return(true);
}

//------------------------------------------------------------------------------

bool CEventRecievers::UnregisterClient(CEventManager* client)
{
    CSimpleIterator<CEventManager> H(Clients);
    CEventManager* p_h;

    while((p_h = H.Current())!=NULL) {
        if( p_h == client) {
            if( p_h == TPH.Current() ) {
                TPH++;
                MoveTPH=true;
            }
            H.Remove();
            return(true);
            // vrac� false pokud ji� existuje
        }
        H++;
    }
    return(false);
}

//------------------------------------------------------------------------------

bool CEventRecievers::UnregisterClientOnServerSide(CEventManager* server)
{
    CSimpleIterator<CEventManager> H(Clients);
    CEventManager* p_h;

    while((p_h = H.Current())!=NULL) {
        H++;
        p_h->Servers.Remove(server);
    }

    return(true);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

void CEventRecievers::ThrowEvent(CEventManager* object,void* p_data)
{
    CEventManager* p_h;
    TPH.SetToBegin();
    MoveTPH = false;
    while((p_h = TPH.Current())!=NULL) {
        if( p_h->EnabledEvents ) {
            p_h->ProcessEvent(Event,object,p_data);
        }
        if( MoveTPH == false ) TPH++;
        else   MoveTPH = false;
    }
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

bool CEventRecievers::IsEmpty(void) const
{
    return(Clients.IsEmpty());
}

//---------------------------------------------------------------------------

const CEventIdentifier& CEventRecievers::GetEvent(void)
{
    return(Event);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
