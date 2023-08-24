//*****************************************************************************
//  Copyright (c) 2014  Daniel D Miller
//  This class is released into the public domain.
//  class for handling string list
//*****************************************************************************

//***********************************************************************
//  list-of-strings struct
//***********************************************************************
typedef struct cstr_list_s {
   struct cstr_list_s *next ;
   char *sptr ; //  pointer to string
   uint slen ;  //  needed for binary "strings"
   u16 flags ;  //  application-specific flags
   uint count ; //  application-specific counter
   uint ref_count ; //  reference counter, incremented by str[n]_exists compares
} cstr_list_t, *cstr_list_p ;

//***********************************************************************
//  base class struct
//***********************************************************************
class CStrList {
private:   
   cstr_list_p top ;
   cstr_list_p tail ;
   uint element_count ;

   //  bypass the assignment operator and copy constructor
   CStrList &operator=(const CStrList &src) ;
   CStrList(const CStrList&);
   
public:   
   CStrList();
   ~CStrList();
   void add(char *str);
   void add(u8 *bfr, uint bfrlen);
   cstr_list_p get_top(void);
   bool remove(char *del_str);
   void delete_list(void);
   bool str_exists(char *cbentry);
   bool strn_exists(char *cbentry);
   void clear_counters(void);
   cstr_list_p get_next(cstr_list_p curr_element);
   uint write_to_file(FILE *fd);
   uint get_element_count(void) const
      { return element_count ; }
} ;

