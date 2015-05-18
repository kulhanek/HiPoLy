#ifndef BaseListH
#define BaseListH
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

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

template<class MemberObject,class ListNode>
class CBaseList {
public:
// constructor/destructor -----------------------------------------------------
     CBaseList(void);
    virtual ~CBaseList(void);

// insert objects -------------------------------------------------------------
    /// insert object to the beginning of list
    void            InsertToBegin(MemberObject *object,int id=0,bool own=false);

    /// insert object to the end of list
    void            InsertToEnd(MemberObject *object,int id=0,bool own=false);

// remove objects -------------------------------------------------------------
    /// remove first object from list
    MemberObject*   RemoveFromBegin(bool destroyifown=true);

    /// remove last object from list
    MemberObject*   RemoveFromEnd(bool destroyifown=true);

    /// remove object specified by ist pointer
    MemberObject*   Remove(MemberObject *object,bool destroyifown=true);

    /// remove all objects from list
    void            RemoveAll(void);

    /// find and remove object
    MemberObject*   FindAndRemove(int id,bool destroyifown=false);

    /// get object and remove it
    MemberObject*   GetAndRemove(int index,bool destroyifown=false);

// manipulation methods -------------------------------------------------------
    /// exchange objects - list nodes are not changed
    void            Swap(MemberObject* obj1,MemberObject* obj2);

// information methods --------------------------------------------------------
    /// is list empty
    inline bool     IsEmpty(void) const;

    /// is object in the list
    bool            IsInList(MemberObject *obj) const;

    /// find object with id
    MemberObject*   Find(int id) const;

    /// get first object
    inline MemberObject* GetFirst(void) const;

    /// get last object
    inline MemberObject* GetLast(void) const;

    /// get object with index
    MemberObject*        operator[](unsigned int index);

    /// get object with index
    const MemberObject*  operator[](unsigned int index) const;

    /// get object with index
    MemberObject*        Get(unsigned int index);

    /// get object with index
    const MemberObject*  Get(unsigned int index) const;

    /// get ID of object with index
    int                  GetID(unsigned int index) const;

    /// number of items in the list
    inline int           NumOfMembers(void) const;

// section of private data ----------------------------------------------------
public:
    ListNode*    FirstNode;
    ListNode*    LastNode;
    int          NumberOfMembers;

    void SetEmpty(void);
    CBaseList(CBaseList<MemberObject,ListNode> &);
};

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

template<class MemberObject,class ListNode>
CBaseList<MemberObject,ListNode>::CBaseList(void)
{
    SetEmpty();
}

// ----------------------------------------------------------------------------

template<class MemberObject,class ListNode>
CBaseList<MemberObject,ListNode>::~CBaseList(void)
{
    RemoveAll();
}

// ----------------------------------------------------------------------------

template<class MemberObject,class ListNode>
void CBaseList<MemberObject,ListNode>::InsertToBegin(MemberObject *object,int id,bool own)
{
    if(object == NULL) return;

    ListNode *p_n;

    p_n=new ListNode;

    p_n->Own  = own;
    p_n->Object = (void*)object;
    p_n->SetIndex(id);

    if( FirstNode != NULL ) FirstNode->PrevNode = p_n;
    else LastNode        = p_n;

    p_n->NextNode = FirstNode;
    p_n->PrevNode  = NULL;

    FirstNode = p_n;
    NumberOfMembers++;
}

// ----------------------------------------------------------------------------

template<class MemberObject,class ListNode>
void CBaseList<MemberObject,ListNode>::InsertToEnd(MemberObject *object,int id,bool own)
{
    if(object == NULL) return;

    ListNode *p_n;

    p_n=new ListNode;

    p_n->Own  = own;
    p_n->Object = (void*)object;
    p_n->SetIndex(id);

    if( LastNode != NULL ) LastNode->NextNode = p_n;
    else  FirstNode      = p_n;
    p_n->NextNode = NULL;
    p_n->PrevNode = LastNode;
    LastNode = p_n;
    NumberOfMembers++;
}

// ----------------------------------------------------------------------------

template<class MemberObject,class ListNode>
MemberObject* CBaseList<MemberObject,ListNode>::RemoveFromBegin(bool destroyifown)
{
    if( IsEmpty() )return(NULL);

    ListNode   *p_n;
    MemberObject *Objectect;

    p_n    = FirstNode;
    Objectect = (MemberObject*)p_n->Object;
    if((FirstNode = FirstNode->NextNode)==NULL) {
        SetEmpty();
    } else {
        FirstNode->PrevNode = NULL;
        NumberOfMembers--;
    }

    if((destroyifown == true)&&(p_n->Own == true) ) {
        delete Objectect;
        Objectect = NULL;
    }

    delete p_n;

    return(Objectect);
}

// ----------------------------------------------------------------------------

template<class MemberObject,class ListNode>
MemberObject* CBaseList<MemberObject,ListNode>::RemoveFromEnd(bool destroyifown)
{
    if( IsEmpty() )return(NULL);

    ListNode  *p_n;
    MemberObject *Objectect;

    p_n    = LastNode;
    Objectect = (MemberObject*)p_n->Object;
    if((LastNode = LastNode->PrevNode)==NULL) {
        SetEmpty();
    } else {
        LastNode->NextNode = NULL;
        NumberOfMembers--;
    }

    if((destroyifown == true)&&(p_n->Own == true) ) {
        delete Objectect;
        Objectect = NULL;
    }

    delete p_n;

    return(Objectect);
}

// ----------------------------------------------------------------------------

template<class MemberObject,class ListNode>
MemberObject* CBaseList<MemberObject,ListNode>::Remove(MemberObject *Objectect,bool destroyifown)
{
    ListNode *pointer = FirstNode;

    if( IsEmpty()) return(NULL);
    while(pointer != NULL) {
        if( Objectect == pointer->Object ) {
            ListNode  *p_n;
            MemberObject         *Objectect;

            if( pointer == FirstNode) {
                return(RemoveFromBegin(destroyifown));
            }
            if( pointer == LastNode ) {
                return(RemoveFromEnd(destroyifown));
            }

            pointer->NextNode->PrevNode = pointer->PrevNode;
            pointer->PrevNode->NextNode = pointer->NextNode;

            Objectect = (MemberObject*)pointer->Object;
            p_n    = pointer;
            if((destroyifown == true)&&(p_n->Own == true) ) {
                delete Objectect;
                Objectect = NULL;
            }

            delete p_n;
            NumberOfMembers--;
            return(Objectect);
        }

        pointer = pointer->NextNode;
    }
    return(NULL);
}

// ----------------------------------------------------------------------------

template<class MemberObject,class ListNode>
void CBaseList<MemberObject,ListNode>::RemoveAll(void)
{
    ListNode      *p_n=FirstNode,*p_n1;
    MemberObject    *o;

    if( IsEmpty() )return;

    while(p_n != NULL) {
        p_n1 = p_n;
        if((o = (MemberObject*)p_n->Object)!=NULL) {
            if( p_n->Own == true ) delete o;
        }
        p_n = p_n->NextNode;
        delete p_n1;
    }

    SetEmpty();
}

// ----------------------------------------------------------------------------

template<class MemberObject,class ListNode>
void CBaseList<MemberObject,ListNode>::Swap(MemberObject* Object1,MemberObject* Object2)
{
    ListNode      *p_n=FirstNode,*p_n1=NULL,*p_n2 = NULL;

    while(p_n != NULL) {
        if( p_n->Object == Object1 ) {
            p_n1 = p_n;
            break;
        }
        p_n = p_n->NextNode;
    }

    if( p_n1 == NULL ) return;

    p_n=FirstNode;

    while(p_n != NULL) {
        if( p_n->Object == Object2 ) {
            p_n2 = p_n;
            break;
        }
        p_n = p_n->NextNode;
    }

    if( p_n2 == NULL ) return;

    p_n1->Object = Object2;
    p_n2->Object = Object1;

    int id = p_n1->GetIndex();
    p_n1->SetIndex(p_n2->GetIndex());
    p_n2->SetIndex(id);

    bool own = p_n1->Own;
    p_n1->Own = p_n2->Own;
    p_n2->Own = own;
}

// ----------------------------------------------------------------------------

template<class MemberObject,class ListNode>
inline bool CBaseList<MemberObject,ListNode>::IsEmpty(void) const
{
    return(NumberOfMembers == 0);
}

// ----------------------------------------------------------------------------

template<class MemberObject,class ListNode>
bool CBaseList<MemberObject,ListNode>::IsInList(MemberObject *Object) const
{
    ListNode *p_n = FirstNode;

    while(p_n != NULL) {
        if( Object == p_n->Object )return(true);
        p_n = p_n->NextNode;
    }
    return(false);
}

// ----------------------------------------------------------------------------

template<class MemberObject,class ListNode>
MemberObject* CBaseList<MemberObject,ListNode>::Find(int id) const
{
    ListNode *p_n = FirstNode;

    while(p_n != NULL) {
        if( id == p_n->GetIndex() )return((MemberObject*)p_n->Object);
        p_n = p_n->NextNode;
    }
    return(NULL);
}

// ----------------------------------------------------------------------------

template<class MemberObject,class ListNode>
MemberObject* CBaseList<MemberObject,ListNode>::FindAndRemove(int id,bool destroyifown)
{
    ListNode *p_n = FirstNode;
    MemberObject* Objectect;

    while(p_n != NULL) {
        if( id == p_n->GetIndex() ) {
            if( p_n == FirstNode) {
                return(RemoveFromBegin(destroyifown));
            }
            if( p_n == LastNode ) {
                return(RemoveFromEnd(destroyifown));
            }

            p_n->NextNode->PrevNode = p_n->PrevNode;
            p_n->PrevNode->NextNode = p_n->NextNode;

            Objectect = (MemberObject*)p_n->Object;
            if((destroyifown == true)&&(p_n->Own == true) ) {
                delete Objectect;
                Objectect = NULL;
            }

            delete p_n;
            NumberOfMembers--;
            return(Objectect);
        }
        p_n = p_n->NextNode;
    }
    return(NULL);
}

// ----------------------------------------------------------------------------

template<class MemberObject,class ListNode>
inline MemberObject* CBaseList<MemberObject,ListNode>::GetFirst(void) const
{
    if(FirstNode == NULL)return(NULL);
    return((MemberObject*)FirstNode->Object);
}

// ----------------------------------------------------------------------------

template<class MemberObject,class ListNode>
inline MemberObject* CBaseList<MemberObject,ListNode>::GetLast(void) const
{
    if(LastNode == NULL)return(NULL);
    return((MemberObject*)LastNode->Object);
}

// ----------------------------------------------------------------------------

template<class MemberObject,class ListNode>
MemberObject* CBaseList<MemberObject,ListNode>::GetAndRemove(int i,bool destroyifown)
{
    ListNode*   p_n = FirstNode;
    int            index=0;
    MemberObject*        Objectect;

    while(p_n != NULL) {
        if( index == i ) {
            if( p_n == FirstNode) {
                return(RemoveFromBegin(destroyifown));
            }
            if( p_n == LastNode ) {
                return(RemoveFromEnd(destroyifown));
            }

            p_n->NextNode->PrevNode = p_n->PrevNode;
            p_n->PrevNode->NextNode = p_n->NextNode;

            Objectect = (MemberObject*)p_n->Object;
            if((destroyifown == true)&&(p_n->Own == true) ) {
                delete Objectect;
                Objectect = NULL;
            }

            delete p_n;
            NumberOfMembers--;
            return(Objectect);
        }
        index++;
        p_n = p_n->NextNode;
    }
    return(NULL);
}

// ----------------------------------------------------------------------------

template<class MemberObject,class ListNode>
MemberObject* CBaseList<MemberObject,ListNode>::operator[](unsigned int index)
{
    return(Get(index));
}

// ----------------------------------------------------------------------------

template<class MemberObject,class ListNode>
const MemberObject* CBaseList<MemberObject,ListNode>::operator[](unsigned int index) const
{
    return(Get(index));
}

// ----------------------------------------------------------------------------

template<class MemberObject,class ListNode>
MemberObject* CBaseList<MemberObject,ListNode>::Get(unsigned int index)
{
    ListNode*      p_n = FirstNode;
    unsigned int   lindex = 0;

    while(p_n != NULL) {
        if( lindex == index ) return((MemberObject*)p_n->Object);
        lindex++;
        p_n = p_n->NextNode;
    }
    return(NULL);
}

// ----------------------------------------------------------------------------

template<class MemberObject,class ListNode>
const MemberObject* CBaseList<MemberObject,ListNode>::Get(unsigned int index) const
{
    ListNode*      p_n = FirstNode;
    unsigned int   lindex = 0;

    while(p_n != NULL) {
        if( lindex == index ) return((MemberObject*)p_n->Object);
        lindex++;
        p_n = p_n->NextNode;
    }
    return(NULL);
}

// ----------------------------------------------------------------------------

template<class MemberObject,class ListNode>
int CBaseList<MemberObject,ListNode>::GetID(unsigned int index) const
{
    ListNode*      p_n = FirstNode;
    unsigned int   lindex = 0;

    while(p_n != NULL) {
        if( lindex == index ) return(p_n->GetIndex());
        lindex++;
        p_n = p_n->NextNode;
    }
    return(-1);
}

// ----------------------------------------------------------------------------

template<class MemberObject,class ListNode>
inline int CBaseList<MemberObject,ListNode>::NumOfMembers(void) const
{
    return(NumberOfMembers);
}

// ----------------------------------------------------------------------------

template<class MemberObject,class ListNode>
inline void CBaseList<MemberObject,ListNode>::SetEmpty(void)
{
    FirstNode = NULL;
    LastNode  = NULL;
    NumberOfMembers   = 0;
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

#endif
