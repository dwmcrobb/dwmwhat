# DwmWhat : bringing an old standard back into modern C++

DwmWhat is a small header-only C++ library for embedding package
information in compiled code, and a utility to search for the
information in compiled code much like the very old `what` utility
from SCCS.

The `what` utility was part of POSIX.1 (2001).  It is optional, and is
not in the base of most (any?) Linux distros.  It is installed as
part of the base of FreeBSD, and is on my Apple systems via Xcode
command line utilities.  It is useful, but requires developers to
follow a convention very few follow today.  I like the convention
(embed strings starting with '@(#)' in files), despite the fact
that no one in their right mind uses SCCS for revision control today
(even for single local files, RCS makes more sense).

## Platforms
- macOS
- FreeBSD
- Ubuntu
- Raspberry Pi OS

## Requirements
- [`dwm_gmk`](https://github.com/dwmcrobb/dwm_gmk) GNU make plugin
- GNU make
- [`mkfbsdmnfst`](https://github.com/dwmcrobb/mkfbsdmnfst) for FreeBSD
  packaging
- [`mkdebcontrol`](https://github.com/dwmcrobb/mkdebcontrol) for Ubuntu
  and other Debian package platforms

## `class Dwm::What::Info`
This is the primary class template in the library, and normally the
only type a user will instantiate from the library.  A concise version
of its interface is shown below.  Note that `Dwm::What::Info` inherits
from `Dwm::What::SegmentedLiteral`.

```cpp
template <std::size_t P, std::size_t S, std::size_t N,
          std::size_t V, std::size_t C, std::size_t O>
class Info {
public:
   consteval Info(const char (&whattype)[P], const char (&status)[S],
                  const char (&name)[N], const char (&version)[V],
                  const char (&cpyright)[C], const char (&other)[O]);

   constexpr std::string_view type() const noexcept;
   constexpr std::string_view status() const noexcept;
   constexpr std::string_view name() const noexcept;
   constexpr std::string_view version() const noexcept;
   constexpr std::string_view copyright() const noexcept;
   constexpr std::string_view date() const noexcept;
   constexpr std::string_view other() const noexcept;
   constexpr std::string_view data_view() const noexcept;
   constexpr std::string_view view() const noexcept;
};
```
### Usage
Typical usage for a library is to just add a single instance
of a `Dwm::What::Info` in a header file you expect to be included
by users of your library.  It can be deep in a nested namespace,
and of course shouldn't be in the top level global namespace.
For example, for version 1.0.0 of a library package "MyPackageName":

```
namespace MyPackageName {
  namespace pkg {
    inline constexpr const Dwm::What::Info __attribute__((used))
       info(DWM_WHAT_TYPE_LIB, DWM_WHAT_STATUS_REL, "MyPackageName",
            "1.0.0", "My Name", "other stuff");
  }
}
```

Things of note here:
- We declared our instance `inline` so that the compiler and linker
  will in the end only produce a single instance.  It's `constexpr`
  because it can be and should be.
- `__attribute__((used))` tells the compiler and linker to not throw
  away this instance when optimizing, even if no code uses it.  The
  whole idea is just to embed a useful string literal in output,
  regardless of whether or not the linked code accesses it.

#### Constructor arguments, in order
- **`whattype`**
    > The type of the package.  See the supported package types below.
- **`status`**
    > The status of the package.  See the support package status choices
    below.
- **`name`**
    > The name of the package.
- **`version`**
    > The version of the package.
- **`cpyright`**
    > The copyright holder (usually just your name).
- **`other`**
    > Any other package information.

Note that all arguments are string literals.  The whole idea is to build
a string literal at compile time, without using the preprocessor to
assemble the string literal.

#### Package types
Package types are just string literals, but I provide macros for
some useful ones, which are UTF-8 code points for symbols I find
useful in my own software.  Note that currently `dwmwhat` depends
on the use of only these package types if you want canonical JSON
output from `dwmwhat` for embedded instances of `Dwm::What::Info`.

- **`DWM_WHAT_TYPE_LIB`** (📚)
    > A library package, containing compiled or otherwise executable content.
- **`DWM_WHAT_TYPE_HDR`** (＃)
    > A header package, common in the C++ world.
- **`DWM_WHAT_TYPE_EXE`** (🤖)
    > A package for one or more executables.
- **`DWM_WHAT_TYPE_DOC`** (📄)
    > A package of documentation.

More than one package type can be concatenated to form the first
argument to the `Dwm::What::Info` constructor.  For example, the
DwmWhat package contains header files and the `dwmwhat` utility,
so I use both `DWM_WHAT_TYPE_EXE` and `DWM_WHAT_TYPE_HDR`:

```
    inline constexpr const Info __attribute__((used))
    info(DWM_WHAT_TYPE_EXE DWM_WHAT_TYPE_HDR, DWM_WHAT_STATUS_DEV, "DwmWhat",
         "0.0.5", "Daniel McRobb " DWM_WHAT_SYM_GHOST, "mcplex.net");
```

#### Package status
- **`DWM_WHAT_STATUS_DEV`** (❗)
    > Not tagged, not reproducible... should not be used in production.
- **`DWM_WHAT_STATUS_RC`** (👷)
    > A release candidate.  Reproducible (presumably tagged too).
- **`DWM_WHAT_STATUS_REL`** (✅)
    > An official release.

## `Dwm::What::SegmentedLiteral`
This class template is the more generic segmented string literal class
template.  `Dwm::What::Info` inherits from `Dwm::What::SegmentedLiteral`.
The idea here is to provide a means of constructing a contiguous
string literal at compile time from N other string literals and a
delimiter that is placed between each given string literal, without
resorting to using the preprocessor.  In code that instantiates a
`SegmentedLiteral`, the segments from which it was constructed are
accessible via the `nth(std::size_t n)` member, where `n` is from 0 to
`num_segments() - 1`.  A view of the entire constructed string literal
is available via the `view()` member.

The tricks to such a thing...
- Passing the string literals as `const char (&)[N]` so we can use
  template parameter deduction in the constructor to deduce the
  size of the encapsulated character buffer.
- a correct deduction guide.
  

```cpp
template <std::size_t DelimLen, std::size_t NumSegs, std::size_t NumChars>
class SegmentedLiteral
{
public:
   template <std::size_t D, std::size_t F, std::size_t ...Ns>
   consteval SegmentedLiteral(const char (&delim)[D], const char (&f)[F],
                              const char (&...s)[Ns]);
   constexpr std::string_view view() const noexcept;
   constexpr std::size_t num_segments() const noexcept;      
   constexpr std::string_view nth(std::size_t n) const noexcept;
};

```
The following would produce the same string literals:
```
    inline constexpr const Dwm::What::Info __attribute__((used))
    myinfo(DWM_WHAT_TYPE_LIB, DWM_WHAT_STATUS_DEV, "MyPackage",
         "1.0.5", "John Doe 2025", "Other stuff");

    inline constexpr const Dwm::What::SegmentedLiteral __attribute__((used))
    myseg(" ", "@(#)", DWM_WHAT_TYPE_LIB, DWM_WHAT_STATUS_DEV, "MyPackage",
          "1.0.5", DWM_WHAT_SYM_COPYRIGHT, "John Doe 2025",
	  DWM_WHAT_SYM_OTHER, "Other stuff");
```
This is kind of important to note.  The compiler is going to produce two
string literals of the same size (hence the same type, `const char (&)[63]`),
with the same content:

"@(#) 📚 ❗ MyPackage 1.0.5 ©️  John Doe 2025  Other stuff"

In fact, since both are available at compile time, these static
assertions pass:
```
  static_assert(myinfo.view() == myseg.view());
  static_assert(myinfo.num_segments() == myseg.num_segments());
  static_assert(sizeof(decltype(myinfo)::BufType)
                == sizeof(decltype(myseg)::BufType));
  static_assert(std::is_same_v<decltype(myinfo)::BufType,                    
                               decltype(myseg)::BufType>);                 
```
And these runtime assertions pass:
```
  for (size_t i = 0; i < myinfo.num_segments(); ++i) {
     assert(myinfo.nth(i) == myseg.nth(i));
  }                                                                             

```
However, `dwmwhat` will coalesce these into one instance when scanning
a file, unlike `what`.  This just helps remove duplicate reporting of
the same information.

## dwmwhat
dwmwhat searches one or more files for strings starting with @(#) and
displays the strings on stdout, one per line.  It is similar to the old
`what(1)` utility from SCCS.
     
```
% dwmwhat `which dwmwhat`
🤖＃ ✅ DwmWhat 0.0.3 ©️  Daniel McRobb 👻  mcplex.net
```

```
% dwmwhat `which bash`
Bash version 5.2.37(1) release GNU
```

It can also produce JSON output for the strings it finds.  If the
strings came from instances of `Dwm::What::Info`, they will be parsed
and presented in decomposed form.

```
% dwmwhat -j `which dwmwhat` | json_pp
[
   {
      "file" : "/usr/local/bin/dwmwhat",
      "pkgs" : [
         {
            "copyright" : "Daniel McRobb 👻",
            "name" : "DwmWhat",
            "other" : "mcplex.net",
            "status" : "✅",
            "type" : "🤖＃",
            "version" : "0.0.7"
         }
      ]
   }
]
```
