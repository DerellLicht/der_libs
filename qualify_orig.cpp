/******************************************************************/
/*  qualify() modifies a string as required to generate a         */
/*  "fully-qualified" filename, which is a filename that          */
/*  complete drive specifier and path name.                       */
/*                                                                */
/*  input:  argptr: the input filename.                           */
/*                                                                */
/*  output: qresult, a bit-mapped unsigned int with the           */
/*                   following definitions:                       */
/*                                                                */
/*          bit 0 == 1 if wildcards are present.                  */
/*          bit 1 == 1 if no wildcards and path does not exist.   */
/*          bit 2 == 1 if no wildcards and path exists as a file. */
/*          bit 7 == 1 if specified drive is invalid.             */
/*                                                                */
/******************************************************************/

//lint -esym(714, qualify)  Symbol 'qualify' is both a function and a variable
//lint -esym(765, qualify)  Symbol 'qualify' is both a function and a variable
//lint -esym(1051, qualify)  Symbol 'qualify' is both a function and a variable

#include <windows.h>
#include <stdio.h>
#ifndef  UNICODE      
#include <direct.h>     //  only needed for non-Unicode
#endif
#include <ctype.h>      //  tolower()
#include <shlwapi.h>
#include <tchar.h>
#include <string>
#include <memory> //  unique_ptr

#include "common.h"
#include "qualify.h"
#ifdef  UNICODE      
#ifndef _lint
#include "conio_min.h"
#endif
#endif

// #define  LOOP_FOREVER   true

#ifdef  UNICODE      
//lint -e129  declaration expected, identifier ignored
static std::unique_ptr<conio_min> console ;
#endif

/******************************************************************/
unsigned qualify (TCHAR *argptr)
{
static TCHAR path[MAX_PATH_LEN];
   TCHAR *pathptr = &path[0];
   TCHAR *strptr, *srchptr ;
   DWORD plen;
   unsigned len, qresult = 0;

   //******************************************************
   //  first, determine requested drive number,            
   //  in "A: = 1" format.                                 
   //******************************************************
   //  if arg len == 0 or arg is "."
   if (_tcslen (argptr) == 0) {   /*  no arguments given  */
      _tcscpy(argptr, _T(".")) ;
   }

   //******************************************************
   //  strings in quotes will also foil the DOS routines;
   //  strip out all double quotes
   //******************************************************
   strptr = argptr;
   while (LOOP_FOREVER) {
      srchptr = _tcschr (strptr, '"');
      if (srchptr == 0)
         break;
      _tcscpy (srchptr, srchptr + 1);
      strptr = ++srchptr;
   }
   // console->dputsf(_T("after quotes removed: [%s]\n"), argptr);

   //  check for raw drive spec (q: , etc)
   if (_tcslen(argptr) == 2  &&  *(argptr+1) == L':') {
      uint drive = towlower(*argptr) ; //lint !e732
      drive -= (97 - 1) ;  //  97 = 'a', +1 converts drive to start at 1 vs 0
      // console->dputsf(L"drive spec [%d]: [%s]\n", drive, input_path.c_str());
#ifdef  UNICODE      
      wchar_t *p = _wgetdcwd(drive, pathptr, MAX_PATH_LEN);
#else      
      char *p = _getdcwd(drive, pathptr, MAX_PATH_LEN);  //lint !e713
#endif      
      if (p == NULL) {
         return QUAL_INV_DRIVE;
      }
   }
   else {
      //******************************************************
      //  get expanded path (this doesn't support UNC)
      //******************************************************
      plen = GetFullPathName (argptr, MAX_PATH_LEN, (LPTSTR) pathptr, NULL);
      if (plen == 0) {
         return QUAL_NO_PATH;
      }
      // console->dputsf(L"gfpn [%u]: [%s]\n", plen, pathptr);
   }
   // input_path = pathptr ;
   
   len = _tcslen (pathptr);
   if (len == 3) {
      _tcscat (pathptr, _T("*"));
      qresult |= QUAL_WILDCARDS;
   }
   else {
      //  see if there are wildcards in argument.
      //  If not, see whether path is a directory or a file,
      //  or nothing.  If directory, append wildcard char
      if (_tcspbrk (pathptr, _T("*?")) == NULL) {
         if (*(pathptr + len - 1) == '\\') {
            len--;
            *(pathptr + len) = 0;
         }

         //*********************************************************
         //  what *should* I do with UNC paths ??
         //*********************************************************
         // if (PathIsUNC(pathptr)) {
         //    if (PathIsDirectory(pathptr)) {
         //       _tcscpy (pathptr + len, _T("\\*"));   //lint !e669  possible overrun
         //       qresult |= QUAL_WILDCARDS; //  wildcards are present.
         //    } else if (PathFileExists(pathptr)) {
         //       qresult |= QUAL_IS_FILE;   //  path exists as a normal file.
         //    } else {
         //       _tcscpy (pathptr + len, _T("\\*"));   //lint !e669  possible overrun
         //       qresult |= QUAL_WILDCARDS; //  wildcards are present.
         //    }
         // } 
         //  process drive-oriented (non-UNC) paths
         // else {
         if (PathIsDirectory(pathptr)) {
            _tcscpy (pathptr + len, _T("\\*"));   //lint !e669  possible overrun
            qresult |= QUAL_WILDCARDS; //  wildcards are present.
         } else if (PathFileExists(pathptr)) {
            qresult |= QUAL_IS_FILE;   //  path exists as a normal file.
         } else {
            qresult = QUAL_NO_PATH ;
         }
         // }
      }
   }

   //**********************************************
   //  copy string back to input, then return
   //**********************************************
   // console->dputsf(_T("leaving qualify: [%s]\n"), pathptr);
   _tcscpy (argptr, pathptr);
   return (qresult); //lint !e438  drive
}

#ifdef  STANDALONE
//********************************************************************************
//  this solution is from:
//  https://github.com/coderforlife/mingw-unicode-main/
//********************************************************************************
#if defined(__GNUC__) && defined(_UNICODE)

#ifndef __MSVCRT__
#error Unicode main function requires linking to MSVCRT
#endif

#include <wchar.h>
#include <stdlib.h>

extern int _CRT_glob;
extern 
#ifdef __cplusplus
"C" 
#endif
void __wgetmainargs(int*,wchar_t***,wchar_t***,int,int*);

#ifdef MAIN_USE_ENVP
int wmain(int argc, wchar_t *argv[], wchar_t *envp[]);
#else
int wmain(int argc, wchar_t *argv[]);
#endif

int main() 
{
   wchar_t **enpv, **argv;
   int argc, si = 0;
   __wgetmainargs(&argc, &argv, &enpv, _CRT_glob, &si); // this also creates the global variable __wargv
#ifdef MAIN_USE_ENVP
   return wmain(argc, argv, enpv);
#else
   return wmain(argc, argv);
#endif
}

#endif //defined(__GNUC__) && defined(_UNICODE)

//**********************************************************************************
//lint -esym(765, test_vectors)  external could be made static
static TCHAR *test_vectors[] = {
   L"", 
   L".", 
   L"..",
   L"string\"with\"quotes",
   L"c:",   //  c: without backslash, should use relative path
   L"q:",   //  invalid drive should be detected
   L"d:\\",
   L"d:\\*",
   L"f:\\Games",
   NULL };

//**********************************************************************************
//  NOTE: STANDALONE program must be built with UNICODE
//**********************************************************************************
//lint -esym(529, file)  Symbol not subsequently referenced

int wmain(int argc, wchar_t *argv[])
{
   console = std::make_unique<conio_min>() ;
   if (!console->init_okay()) {  //lint !e530
      wprintf(L"console init failed\n");
      return 1 ;
   }
   
   TCHAR test_path[MAX_FILE_LEN+1];
   for (int idx=0; test_vectors[idx] != NULL; idx++) {
      wchar_t *testptr = test_vectors[idx] ;
      wcscpy(test_path, testptr);
      
      console->dputsf(L"input file spec: %s\n", test_path);
      unsigned qresult = qualify(test_path) ;
      if (qresult == QUAL_INV_DRIVE) {
         console->dputsf(L"error: invalid drive: %s\n", test_path);
      }
      else if (qresult == QUAL_NO_PATH) {
         console->dputsf(L"error: path does not exist: %s\n", test_path);
      }
      else {
         console->dputsf(L"qualified file spec: %s\n", test_path);
      }
      console->dputs(L"\n");
   }
   return 0;
}  //lint !e715 !e818 !e533
#endif
