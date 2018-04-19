//*****************************************************************************
//  Copyright (c) 2014  Daniel D Miller
//  This class is released into the public domain.
//  class for handling string list
//*****************************************************************************

#include <windows.h>
#include <stdio.h>

//lint -sem(CStrList::delete_list,cleanup)

//lint -esym(1714, CStrList::add, CStrList::remove, CStrList::str_exists)
//lint -esym(1714, CStrList::get_next, CStrList::write_to_file, CStrList::get_element_count)
//lint -esym(1714, CStrList::get_top)

//lint -esym(1704, CStrList::CStrList)

#include "common.h"
#include "str_list.h"

//*****************************************************************************
void CStrList::delete_list(void)
{
   cstr_list_p cptr = top;
   top = NULL ;
   tail = NULL ;
   while (cptr != NULL) {
      cstr_list_p skill = cptr ;
      cptr = cptr->next ;
      delete[] skill->sptr ;
      delete skill ;
   }
}

//*****************************************************************************
CStrList::CStrList() :
   top(NULL),
   tail(NULL),
   element_count(0)
{
   
}

//*****************************************************************************
CStrList::~CStrList()
{
   // Warning 1551: Function may throw exception '...' in destructor 'CStrList::~CStrList(void)'
   delete_list(); //lint !e1551
}

//*****************************************************************************
void CStrList::add(char *str)
{
   cstr_list_p cptr = new cstr_list_t ;
   ZeroMemory((char *) cptr, sizeof(cstr_list_t)) ;
   cptr->slen = strlen(str) ;
   cptr->sptr = new char[cptr->slen+1] ;
   strcpy(cptr->sptr, str) ;
   element_count++ ;

   //  add new list entry to list
   if (top == NULL)
      top = cptr ;
   else
      tail->next = cptr ;  //lint !e613
   tail = cptr ;  //lint !e613
}

//*****************************************************************************
void CStrList::add(u8 *bfr, uint bfrlen)
{
   cstr_list_p cptr = new cstr_list_t ;
   ZeroMemory((char *) cptr, sizeof(cstr_list_t)) ;
   cptr->sptr = new char[bfrlen] ;
   cptr->slen = bfrlen ;
   memcpy(cptr->sptr, bfr, bfrlen) ;
   element_count++ ;

   //  add new list entry to list
   if (top == NULL)
      top = cptr ;
   else
      tail->next = cptr ;  //lint !e613
   tail = cptr ;  //lint !e613
}

//*****************************************************************************
//  get_top() *removes* the top item from list !!!
//*****************************************************************************
cstr_list_p CStrList::get_top(void)
{
   cstr_list_p cptr = top ;
   if (top != NULL)
      top = top->next ;
   return cptr;
}

//*****************************************************************************
cstr_list_p CStrList::get_next(cstr_list_p curr_element)
{
   if (curr_element == NULL) 
      return top;
   return curr_element->next ;
}

//*****************************************************************************
bool CStrList::remove(char *del_str)
{
   if (top == NULL)
      return false;
   bool elemented_deleted = false ;
   cstr_list_p cptr  ;
   //  see if target item is at start of list
   if (strcasecmp(del_str, top->sptr) == 0) {
      cptr = top ;
      top = top->next ;
      delete cptr->sptr ;
      delete cptr ;
      elemented_deleted = true ;
   } else {
      cptr = top ;
      cstr_list_p tptr = cptr->next ;
      while (LOOP_FOREVER) {
         if (tptr == NULL)
            break;
         if (strcasecmp(del_str, tptr->sptr) == 0) {
            cptr->next = tptr->next ;  //  point past the tptr element
            delete tptr->sptr ;
            delete tptr ;
            elemented_deleted = true ;
            break;
         }
         cptr = tptr ;
         tptr = tptr->next ;
      }
   }
   if (elemented_deleted) {
      element_count-- ;
   }
   return elemented_deleted ;
}

//*****************************************************************************
uint CStrList::write_to_file(FILE *fd)
{
   uint ecount = 0 ;
   cstr_list_p cptr = top;
   while (cptr != NULL) {
      fprintf(fd, "%s\n", cptr->sptr) ;
      ecount++ ;
      cptr = cptr->next ;
   }
   return ecount ;
}

//****************************************************************************
//  increment reference counter in string struct
//****************************************************************************
bool CStrList::str_exists(char *cbentry)
{
   cstr_list_p cptr  ;
   for (cptr=top; cptr != 0; cptr = cptr->next) {
      if (strcmp(cbentry, cptr->sptr) == 0)
      {
         cptr->ref_count++ ;
         return true;
      }
   }
   return false;
}

//****************************************************************************
//  This function is similar to str_exists(), except that it compares only
//  against length of reference string; this allows compares against input
//  strings which have not been terminated yet.
//  increment reference counter in string struct
//****************************************************************************
bool CStrList::strn_exists(char *cbentry)
{
   cstr_list_p cptr  ;
   for (cptr=top; cptr != 0; cptr = cptr->next) {
      if (strncmp(cbentry, cptr->sptr, strlen(cptr->sptr)) == 0)
      {
         cptr->ref_count++ ;
         return true;
      }
   }
   return false;
}

//****************************************************************************
//  this function will clear the counters in the data structs
//****************************************************************************
void CStrList::clear_counters(void)
{
    cstr_list_p cptr  ;
    for (cptr=top; cptr != 0; cptr = cptr->next) {
        cptr->ref_count = 0 ;
    }
}

