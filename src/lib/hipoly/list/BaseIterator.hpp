#ifndef BaseIteratorH
#define BaseIteratorH
//==============================================================================
//    Copyright 1998,1999,2000,2001,2004,2005 Petr Kulhanek
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

#include <stdio.h>
#include <HiPoLyMainHeader.hpp>
#include <BaseList.hpp>

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

template<class List,class MemberObject,class ListNode>
class CBaseIterator {
public:
    CBaseIterator(void);
    CBaseIterator(List &l);
    CBaseIterator(List *l);

    inline void ChangeList(List &l);
    inline void ChangeList(List *l);

    inline MemberObject* Current(void);
    inline int           CurrentID(void);
    inline bool          Set(MemberObject* p_obj);

    MemberObject*       SetToBegin(void);
    MemberObject*       SetTo(int pos);
    MemberObject*       SetToEnd(void);

    bool                GoTo(MemberObject* object);

    inline void         operator++(int);
    inline void         operator--(int);

    MemberObject*       Remove(bool destroyifown=true);
    void                InsertAfter(MemberObject *object,int id=0,bool own=false);
    void                InsertBefore(MemberObject *object,int id=0,bool own=false);

    bool                MoveAfter(void);
    bool                MoveBefore(void);

    inline bool         operator==(CBaseIterator<List,MemberObject,ListNode> &it);
    inline bool         operator!=(CBaseIterator<List,MemberObject,ListNode> &a);
    inline bool         IsValid(void);
    void                Invalidate(void);

// section of private data ----------------------------------------------------
private:
    List*       list;
    ListNode*   pointer;
};

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

template<class List,class MemberObject,class ListNode>
CBaseIterator<List,MemberObject,ListNode>::CBaseIterator(void)
{
    list= NULL;
    pointer= NULL;
}

// ----------------------------------------------------------------------------

template<class List,class MemberObject,class ListNode>
CBaseIterator<List,MemberObject,ListNode>::CBaseIterator(List &l)
{
    list= &l;
    pointer= l.FirstNode;
}

// ----------------------------------------------------------------------------

template<class List,class MemberObject,class ListNode>
CBaseIterator<List,MemberObject,ListNode>::CBaseIterator(List *l)
{
    list= l;
    if( list != NULL ) {
        pointer = l->FirstNode;
    } else {
        pointer = NULL;
    }
}

// ----------------------------------------------------------------------------

template<class List,class MemberObject,class ListNode>
inline void CBaseIterator<List,MemberObject,ListNode>::ChangeList(List &l)
{
    list = &l;
    pointer = l.FirstNode;
}

// ----------------------------------------------------------------------------

template<class List,class MemberObject,class ListNode>
inline void CBaseIterator<List,MemberObject,ListNode>::ChangeList(List *l)
{
    if( l != NULL ) {
        list = l;
        pointer = l->FirstNode;
    } else {
        list = NULL;
        pointer = NULL;
    }
}

// ----------------------------------------------------------------------------

template<class List,class MemberObject,class ListNode>
inline MemberObject* CBaseIterator<List,MemberObject,ListNode>::Current(void)
{
    if(pointer==NULL) return(NULL);
    return( (MemberObject*)pointer->Object );
}

// ----------------------------------------------------------------------------

template<class List,class MemberObject,class ListNode>
inline int CBaseIterator<List,MemberObject,ListNode>::CurrentID(void)
{
    if(pointer==NULL) return(-1);
    return( pointer->GetIndex() );
}

// ----------------------------------------------------------------------------

template<class List,class MemberObject,class ListNode>
inline bool CBaseIterator<List,MemberObject,ListNode>::Set(MemberObject* p_Object)
{
    if( pointer==NULL ) return(false);
    pointer->Object = p_Object;
    return(true);
}

// ----------------------------------------------------------------------------

template<class List,class MemberObject,class ListNode>
inline MemberObject* CBaseIterator<List,MemberObject,ListNode>::SetToBegin(void)
{
    if( list == NULL ) return(NULL);
    pointer = list->FirstNode;
    if( pointer == NULL ) return(NULL);
    return( (MemberObject*)pointer->Object );
}

// ----------------------------------------------------------------------------

template<class List,class MemberObject,class ListNode>
MemberObject* CBaseIterator<List,MemberObject,ListNode>::SetTo(int pos)
{
    if( list == NULL ) return(NULL);

    int       index=0;
    pointer = list->FirstNode;

    while(pointer != NULL) {
        if( index == pos ) return((MemberObject*)pointer->Object);
        index++;
        pointer = pointer->NextNode;
    }
    return(NULL);
}

// ----------------------------------------------------------------------------

template<class List,class MemberObject,class ListNode>
inline MemberObject* CBaseIterator<List,MemberObject,ListNode>::SetToEnd(void)
{
    if( list == NULL ) return(NULL);
    pointer = list->LastNode;
    if( pointer == NULL ) return(NULL);
    return( (MemberObject*)pointer->Object );
}

// ----------------------------------------------------------------------------

template<class List,class MemberObject,class ListNode>
bool CBaseIterator<List,MemberObject,ListNode>::GoTo(MemberObject* Objectect)
{
    if( list == NULL ) return(false);

    pointer = list->FirstNode;
    while(pointer != NULL) {
        if( pointer->Object == Objectect ) return(true);
        pointer = pointer->NextNode;
    }

    return(false);
}


// ----------------------------------------------------------------------------

template<class List,class MemberObject,class ListNode>
inline void CBaseIterator<List,MemberObject,ListNode>::operator++(int)
{
    if( pointer!=NULL ) pointer = pointer->NextNode;
}

// ----------------------------------------------------------------------------

template<class List,class MemberObject,class ListNode>
inline void CBaseIterator<List,MemberObject,ListNode>::operator--(int)
{
    if( pointer!=NULL ) pointer = pointer->PrevNode;
}

// ----------------------------------------------------------------------------

template<class List,class MemberObject,class ListNode>
MemberObject* CBaseIterator<List,MemberObject,ListNode>::Remove(bool destroyifown)
{
    if( list == NULL ) return(NULL);

    ListNode     *p_n;
    MemberObject         *Objectect;

    if( pointer == NULL )return(NULL);
    if( list->IsEmpty()) return(NULL);

    if( pointer == list->FirstNode) {
        pointer = pointer->NextNode;
        return(list->RemoveFromBegin(destroyifown));
    }
    if( pointer == list->LastNode ) {
        pointer = pointer->NextNode;
        return(list->RemoveFromEnd(destroyifown));
    }

    pointer->NextNode->PrevNode = pointer->PrevNode;
    pointer->PrevNode->NextNode = pointer->NextNode;

    Objectect = (MemberObject*)pointer->Object;
    p_n    = pointer;

    pointer= pointer->NextNode;

    if( (p_n->Own == true)&&(destroyifown==true) ) {
        delete Objectect;
        Objectect = NULL;
    }

    delete p_n;
    list->NumberOfMembers--;
    return(Objectect);
}

// ----------------------------------------------------------------------------

template<class List,class MemberObject,class ListNode>
void CBaseIterator<List,MemberObject,ListNode>::InsertAfter(MemberObject *Objectect,int id,bool own)
{
    if( list == NULL ) return;

    if(Objectect == NULL) return;

    ListNode     *p_n;

    if( list->IsEmpty() || (pointer == NULL) ) {
        list->InsertToEnd(Objectect,id,own);
        pointer = list->LastNode;
        return;
    }

    p_n=new ListNode;

    p_n->Own  = own;
    p_n->Object = Objectect;
    p_n->SetIndex(id);

    p_n->PrevNode = pointer;

    if( pointer->NextNode == NULL) {
        pointer->NextNode = p_n;
        p_n->NextNode = NULL;
        list->LastNode = p_n;
    } else {
        p_n->NextNode = pointer->NextNode;
        pointer->NextNode->PrevNode = p_n;
        pointer->NextNode = p_n;
    }

    pointer= p_n;

    list->NumberOfMembers++;
}

// ----------------------------------------------------------------------------

template<class List,class MemberObject,class ListNode>
void CBaseIterator<List,MemberObject,ListNode>::InsertBefore(MemberObject *Objectect,int id,bool own)
{
    if( list == NULL ) return;

    if(Objectect == NULL) return;

    ListNode     *p_n;

    if( list->IsEmpty() || (pointer == NULL) ) {
        list->InsertToEnd(Objectect,id,own);
        pointer = list->FirstNode;
    }

    p_n=new ListNode;

    p_n->Own  = own;
    p_n->Object = Objectect;
    p_n->SetIndex(id);

    p_n->NextNode = pointer;

    if( pointer->PrevNode == NULL) {
        pointer->PrevNode = p_n;
        p_n->PrevNode = NULL;
        list->FirstNode = p_n;
    } else {
        p_n->PrevNode = pointer->PrevNode;
        pointer->PrevNode->NextNode = p_n;
        pointer->PrevNode = p_n;
    }

    pointer= p_n;

    list->NumberOfMembers++;
}

// ----------------------------------------------------------------------------

template<class List,class MemberObject,class ListNode>
bool CBaseIterator<List,MemberObject,ListNode>::MoveAfter(void)
{
    if( list == NULL ) return(false);

    if( (pointer == NULL) || (pointer == list->LastNode) ) return(false);

    if( pointer->NextNode != NULL ) {
        pointer->NextNode->PrevNode = pointer->PrevNode;
    } else {
        return(false);
    }

    if( pointer->PrevNode != NULL ) {
        pointer->PrevNode->NextNode = pointer->NextNode;
    } else {
        list->FirstNode = pointer->NextNode;
    }

    ListNode    *p_n = pointer;

    pointer = pointer->NextNode;

    p_n->PrevNode = pointer;

    if( pointer->NextNode == NULL) {
        pointer->NextNode = p_n;
        p_n->NextNode = NULL;
        list->LastNode = p_n;
    } else {
        p_n->NextNode = pointer->NextNode;
        pointer->NextNode->PrevNode = p_n;
        pointer->NextNode = p_n;
    }

    pointer = p_n;
    return(true);
}

// ----------------------------------------------------------------------------

template<class List,class MemberObject,class ListNode>
bool CBaseIterator<List,MemberObject,ListNode>::MoveBefore(void)
{
    if( list == NULL ) return(false);

    if( (pointer == NULL) || (pointer == list->FirstNode) ) return(false);

    if( pointer->PrevNode != NULL ) {
        pointer->PrevNode->NextNode = pointer->NextNode;
    } else {
        return(false);
    }

    if( pointer->NextNode != NULL ) {
        pointer->NextNode->PrevNode = pointer->PrevNode;
    } else {
        list->LastNode = pointer->PrevNode;
    }

    ListNode    *p_n = pointer;

    pointer = pointer->PrevNode;

    p_n->NextNode = pointer;

    if( pointer->PrevNode == NULL) {
        pointer->PrevNode = p_n;
        p_n->PrevNode = NULL;
        list->FirstNode = p_n;
    } else {
        p_n->PrevNode = pointer->PrevNode;
        pointer->PrevNode->NextNode = p_n;
        pointer->PrevNode = p_n;
    }

    pointer = p_n;
    return(true);
}

// ----------------------------------------------------------------------------

template<class List,class MemberObject,class ListNode>
inline bool CBaseIterator<List,MemberObject,ListNode>::operator==(CBaseIterator<List,MemberObject,ListNode> &a)
{
    if( (pointer == NULL ) || (a.pointer == NULL) ) return(false);
    return( pointer->Object == a.pointer->Object );
}

// ----------------------------------------------------------------------------

template<class List,class MemberObject,class ListNode>
inline bool CBaseIterator<List,MemberObject,ListNode>::operator!=(CBaseIterator<List,MemberObject,ListNode> &a)
{
    if( (pointer == NULL ) || (a.pointer == NULL) ) return(true);
    return( pointer->Object != a.pointer->Object );
}

// ----------------------------------------------------------------------------

template<class List,class MemberObject,class ListNode>
inline bool CBaseIterator<List,MemberObject,ListNode>::IsValid(void)
{
    return( pointer!=NULL );
}

// ----------------------------------------------------------------------------

template<class List,class MemberObject,class ListNode>
inline void CBaseIterator<List,MemberObject,ListNode>::Invalidate(void)
{
    pointer = NULL;
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

#endif
