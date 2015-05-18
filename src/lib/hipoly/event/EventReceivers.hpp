#ifndef EventReceiversH
#define EventReceiversH
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
#include <SimpleList.hpp>
#include <SimpleIterator.hpp>

//------------------------------------------------------------------------------

class CEventManager;

//------------------------------------------------------------------------------

class HIPOLY_PACKAGE CEventRecievers { // popis p��jemc� zpr�vy
public:
    CEventRecievers(const CEventIdentifier& event);
    // registruje funkci pro zpracov�n� ud�losti
    bool    RegisterClient(CEventManager* client);
    // odregistruje funkci pro zpracov�n� ud�losti
    bool    UnregisterClient(CEventManager* client);
    //
    bool    UnregisterClientOnServerSide(CEventManager* server);
    // vyvol� ud�lost nad v�emi registrovan�mi zpracovateli ud�lost�
    void    ThrowEvent(CEventManager* object,void* p_data);
    // ud�lost nem� ��dn�ho zpracovatele (vr�t� FALSE)
    bool                     IsEmpty(void) const;
    // vr�t� p�idru�enou ud�lost
    const CEventIdentifier&  GetEvent(void);
    void SetRemoveAfterThrowEvent(void);

    // sekce dat ------------------------------------------------------
public:
    bool    RemoveAfterThrowEvent;

private:
    const CEventIdentifier&             Event;
    CSimpleList<CEventManager>          Clients;
    CSimpleIterator<CEventManager>      TPH;
    bool                                MoveTPH;
};

//---------------------------------------------------------------------------
#endif
