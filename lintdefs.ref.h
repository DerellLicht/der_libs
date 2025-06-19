//  PcLint warnings/errors caused by PcLint not supporting STL, C++11 standard, TCHAR

//  this is a long-unexplained message
//lint -e10    Expecting ';'

//lint -esym(14,  __created)   Symbol not referenced
//lint -esym(401, __created)   Symbol not referenced
//lint -esym(528, __created)   Symbol not referenced
//lint -esym(843, __created)   Variable could be declared as const

//  warnings/errors caused by PcLint not supporting STL, or C++11 standard
//lint -e2     Unclosed Quote
//lint -e18    Symbol redeclared (basic) 
//lint -e19    Useless Declaration
//lint -e26    Expected an expression, found ')'
//lint -e30    Expected an integer constant
//lint -e31    Redefinition of symbol compare with line ...
//lint -e32    Field size (member 'std::_Vector_base<int,int>::_Vector_impl::_Tp_alloc_type')
//lint -e36    redefining the storage class of symbol 
//lint -e38    Offset of symbol 'std::_Vector_base<int,int>::_Vector_impl_data::pointer'
//lint -e40    Undeclared identifier 'make_unique'
//lint -e42    Expected a statement (TCHAR)
//lint -e46    field type should be an integral or enumeration type
//lint -e48    Bad type
//lint -e52    Expected an lvalue
//lint -e53    Expected a scalar
//lint -e55    Bad type
//lint -e58    Bad type
//lint -e61    Bad type
//lint -e63    Expected an lvalue
//lint -e64    Type mismatch (initialization) (struct ffdata * = int)
//lint -e78    Symbol ... typedef'ed at line ... used in expression
//lint -e110   Attempt to assign to void
//lint -e121   Attempting to initialize an object of undefined type 'void'
//lint -e129   declaration expected, identifier ignored
//lint -e151   Token 'flist' inconsistent with abstract type
//lint -e155   Ignoring { }'ed sequence within an expression, 0 assumed
//lint -e401   symbol not previously declared static at location unknown
//lint -e438   Last value assigned to variable not used
//lint -e503   Boolean argument to relational
//lint -e515   Symbol has arg. count conflict 
//lint -e516   Symbol has arg. type conflict 
//lint -e521   Highest operation, a 'constant', lacks side-effects
//lint -e522   Highest operation, operator '!=', lacks side-effects
//lint -e526   Symbol not defined
//lint -e530   Symbol not initialized
//lint -e550   Symbol not accessed
//lint -e559   Size of argument no. 2 inconsistent with format
//lint -e560   argument no. 4 should be a pointer
//lint -e592   Non-literal format specifier used without arguments
//lint -e628   no argument information provided for function
//lint -e681   Loop is not entered
//lint -e707   Mixing narrow and wide string literals in concatenation
//lint -e719   Too many arguments for format (1 too many)
//lint -e746   call to function not made in the presence of a prototype
//lint -e808   No explicit type given symbol 'file', int assumed
//lint -e1013  Symbol not a member of class ''
//lint -e1015  Symbol '_M_allocate' not found in class
//lint -e1039  Symbol is not a member of
//lint -e1040  Symbol is not a legal
//lint -e1054  template variable declaration expects a type, int assumed
//lint -e1055  Symbol undeclared, assumed to return int
//lint -e1057  Member '__gnu_cxx::__is_integer_nonstrict<<1>>::__value' cannot be used
//lint -e1062  template must be either a class or a function
//lint -e1070  No scope in which to find symbol 'pointer'
//lint -e1072  Reference variable 'file' must be initialized
//lint -e1077  Could not evaluate default template parameter '=typename _Alloc::value_type'
//lint -e1087  Previous declaration of '__gnu_cxx::__is_signed' (line 88) is incompatible
//lint -e1098  Function template specialization does not match any function template
//lint -e1514  Creating temporary to copy 'int' to 'struct ffdata &' (context: arg. no. 1)
//lint -e1712  default constructor not defined for class 'ffdata'
//lint -e1025  No function matches invocation 
//lint -e1066  Symbol declared as "C" conflicts ...
//lint -e1702  operator 'operator+' is both an ordinary function and something else??
//lint -e1776  Converting a string literal is not const safe (initialization)
//lint -e1778  Assignment of string literal to variable is not const safe

