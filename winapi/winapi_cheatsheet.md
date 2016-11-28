# Winapi Cheatsheet

## Char Types 

Wide characters in C are based on the **wchar_t** data type, which is
defined in several header files, including WCHAR.H, like so:

```cpp
typedef unsigned short wchar_t;
```

To define a variable containing a single wide character, use the
following statement:

```cpp
wchar_t ch = 'A';
```

You can also define an initialized pointer to a wide-character string:

```cpp
wchar_t *p = L"Hello!";
```

Similarly, you can define an array of wide characters this way:

```cpp
wchar_t a[] = L"Hello!";
```

You can also use the **L** prefix in front of single character 
literals. But it's usually not necessary.

```cpp
wchar_t ch = L'A';
```

**strlen** function is declared like this:


```cpp
size_t __cdecl strlen (const char*);
```

The wide-character version of the **strlen** function is called 
**wcslen** and the **wcslen** function looks like this:

```cpp
size_t __cdecl wcslen (const wchar_t *);
```

TCHAR type definitions better describe as follows:

```cpp
#ifdef _UNICODE
typedef wchar_t TCHAR;
#define _tcslen wcslen
#else
typedef char TCHAR;
#define _tcslen strlen
#endif
```

Now it's time to address that sticky **L** problem with the string
literals. If the **_UNICODE** identifier is defined, a macro called 
**__T** is defined like this:

```cpp
#define __T(x) L##x
```

This is fairly obscure syntax, but it's in the ANSI C standard for
the C preprocessor. That pair of number signs is called a 
"token paste," and it causes the letter L to be appended to the macro
parameter. Thus, if the macro parameter is "Hello!", then **L##x** 
is L"Hello!".

If the **_UNICODE** identifier is not defined, the **__T** macro is 
simply defined in the following way:


```cpp
#define __T(x) x
```

Regardless, two other macros are defined to be the same as **__T**:

```cpp
#define _T(x) __T(x)
#define _TEXT(x) __T(x)
```

Which one you use for your Win32 console programs depends on how 
concise or verbose you'd like to be. Basically, you must define your 
string literals inside the **_T** or **_TEXT** macro in the following
way:

```cpp
_TEXT("Hello!")
```

Doing so causes the string to be interpreted as composed of wide 
characters if the **_UNICODE** identifier is defined and as 8-bit 
characters if not.


### Windows Header File Types

A Windows program includes the header file **WINDOWS.H**. This file
includes a number of other header files, including **WINDEF.H**, 
which has many of the basic type definitions used in Windows and which
itself includes **WINNT.H**. **WINNT.H** handles the basic Unicode 
support.

**WINNT.H** begins by including the C header file **CTYPE.H**, which 
is one of many C header files that have a definition of wchar_t. 
**WINNT.H** defines new data types named CHAR and WCHAR:

```cpp
typedef char CHAR ;
typedef wchar_t WCHAR ; // wc
```

CHAR and WCHAR are the data types recommended for your use in a 
Windows program when you need to define an 8-bit character or a 
16-bit character. (but I won't).

The **WINNT.H** header file goes on to define six data types you can 
use as pointers to 8-bit character strings and four data types you 
can use as pointers to const 8-bit character strings. I've condensed 
the actual header file statements a bit to show the data types here:

```cpp
typedef CHAR * PCHAR, * LPCH, * PCH, * NPSTR, * LPSTR, * PSTR ;
typedef CONST CHAR * LPCCH, * PCCH, * LPCSTR, * PCSTR ;
```

The **N** and **L** prefixes stand for "near" and "long" and refer to 
the two different sizes of pointers in 16-bit Windows. There is no 
differentiation between near and long pointers in Win32.

Similarly, **WINNT.H** defines six data types you can use as pointers 
to 16-bit character strings and four data types you can use as 
pointers to const 16-bit character strings:

```cpp
typedef WCHAR * PWCHAR, * LPWCH, * PWCH, * NWPSTR, * LPWSTR, * PWSTR;
typedef CONST WCHAR * LPCWCH, * PCWCH, * LPCWSTR, * PCWSTR;
```

So far, we have the data types CHAR (which is an 8-bit char) and 
WCHAR (which is a 16-bit **wchar_t**) and pointers to CHAR and WCHAR. 

As in **TCHAR.H**, **WINNT.H** defines TCHAR to be the generic
character type. If the identifier **UNICODE** (without the underscore)
is defined, TCHAR and pointers to **TCHAR** are defined based on 
WCHAR and pointers to WCHAR; if the identifier UNICODE is not
defined, TCHAR and pointers to TCHAR are defined based on char and 
pointers to char:

```cpp
#ifdef  UNICODE                   
typedef WCHAR TCHAR, * PTCHAR ;
typedef LPWSTR LPTCH, PTCH, PTSTR, LPTSTR ;
typedef LPCWSTR LPCTSTR ;
#else 
typedef char TCHAR, * PTCHAR ;
typedef LPSTR LPTCH, PTCH, PTSTR, LPTSTR ;
typedef LPCSTR LPCTSTR ;
#endif
```

**LPCTSTR**: **PC** just means pointer to const TCHAR[].

Both the WINNT.H and WCHAR.H header files are protected against 
redefinition of the TCHAR data type if it's already been defined by 
one or the other of these header files. However, whenever you're
using other header files in your program, you should include 
WINDOWS.H before all others.

The WINNT.H header file also defines a macro that appends the L to the
first quotation mark of a character string. If the **UNICODE** 
identifier is defined, a macro called **__TEXT** is defined as 
follows:

```cpp
#define __TEXT(quote) L##quote 
```

If the identifier UNICODE is not defined, the **__TEXT** macro is 
defined like so:

```cpp
#define __TEXT(quote) quote
```

Regardless, the **TEXT** macro is defined like this:

```cpp
#define TEXT(quote) __TEXT(quote) 
```

This is very similar to the way the **_TEXT** macro is defined in 
TCHAR.H, except that you need not bother with the underscore. 

These definitions let you mix ASCII and Unicode characters strings in 
the same program or write a single program that can be compiled for 
either ASCII or Unicode. 

If you want to explicitly define 8-bit character variables and 
strings, use CHAR, PCHAR (or one of the others), and strings with
quotation marks. For explicit 16-bit character variables and strings, 
use WCHAR, PWCHAR, and append an L before quotation marks. For 
variables and characters strings that will be 8 bit or 16 bit
depending on the definition of the UNICODE identifier, use TCHAR, 
PTCHAR, and the TEXT macro.

For me the best approach is to disable **UNICODE**:

```cpp
#undef UNICODE
```

# Registry Operation

We will write **regquery** utility to read and write values 
to keys

```cpp
LONG RegOpenKeyEx(
  _In_     HKEY    hKey,
  _In_opt_ LPCTSTR lpSubKey,
  _In_     DWORD   ulOptions,
  _In_     REGSAM  samDesired,
  _Out_    PHKEY   phkResult
);
```

Best theory description in **Windows System Programming 4th Edition**
book

Let's start with opening and closing keys:

```cpp
#undef UNICODE
#include <windows.h>
#include <cstdio>

using namespace std;


int main(int arc, char *argv[])
{
	HKEY hkey;
	LONG ret = RegOpenKeyEx(HKEY_CURRENT_USER, NULL, 0, 
								KEY_QUERY_VALUE, &hkey);
	if(ret == ERROR_SUCCESS) {
		
		printf("Key successfully opened\n");

		ret = RegCloseKey(hkey);

		if(ret == ERROR_SUCCESS) {
			printf("registry key successfully closed\n");
		}
		else {
			fprintf(stderr, "registry key close failed\n");
			return -1;
		}
	}
	else {
		fprintf(stderr, "failed to read registry key!\n");
		return -1;
	}
}
```

Now we will enumerates subkey names of an open registry key with
**RegEnumKeyEx** function.

Notes about name convention: most function names for 32-bit versions 
of Windows and higher has suffix **Ex** which stands for **Extended**.

For example, **RegEnumKey** function is provided only for 
compatibility with 16-bit versions of Windows. Applications should 
use the **RegEnumKeyEx**.
function.

```cpp
LONG RegEnumKeyEx(
  _In_        HKEY      hKey,
  _In_        DWORD     dwIndex,
  _Out_       LPTSTR    lpName,
  _Inout_     LPDWORD   lpcName,
  _Reserved_  LPDWORD   lpReserved,
  _Inout_     LPTSTR    lpClass,
  _Inout_opt_ LPDWORD   lpcClass,
  _Out_opt_   PFILETIME lpftLastWriteTime
);
```

**dwIndex** should be on the first call and then should be 
incremented on each subsequent call. The value name **lpName** and 
its size **lpcName**,  along with the class **lpClass** string and 
its size **lpcClass**, are returned. 

Note that there are two count parameters **lpcName**, (the subkey
name) and **lpcClass**, which are used for both input and output for 
buffer size.

**lpClass** and **lpcClass** are, however, rarely used and should 
almost always be **NULL**.

The function returns **ERROR_SUCCESS** or an error number.


