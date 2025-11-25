# dwmwhat

dwmwhat is a small header-only C++ library for embedding package
information in compiled code as a delimited string literal, and a
utility to search for the information in compiled code in a form
similar to the very old `what` utility from SCCS (Marc Rochkind,
Bell Labs, 1970's).

The `what` utility was part of POSIX.1 (2001).  It is optional, and is
not in the base of most (any?) Linux distros.  It is installed as part
of the base of FreeBSD, and is on my Apple systems via Xcode command
line utilities.  It is useful, but requires developers to follow one
convention: embed string literals starting with '@(#)' in files we want
to be able to identify.  I like the convention, despite the fact that
what follows the '@(#)' today would not come from SCCS.

Few are using a revision control system that automatically changes
keyword values in files that are under revision control.  We're all left
to our own devices for revision names, package names and how we embed
(or do not embed) any version information in binaries.

I am using Dwm::What::Info to bring a little consistency and
structure to embedded version strings in my own software.

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

## Building
Once you have the requirements, just:
```
  ./configure
  make package
```
This should create a package in the current directory.

## Using the Dwm::What::Info and Dwm::What::SegmentedLiteral classes

See [dwmwhat.pdf](doc/manual/dwmwhat.pdf) for more information.

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
