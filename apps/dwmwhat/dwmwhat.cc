//===========================================================================
// @(#) $DwmPath$
//===========================================================================
//  Copyright (c) Daniel W. McRobb 2016, 2025
//  All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions
//  are met:
//
//  1. Redistributions of source code must retain the above copyright
//     notice, this list of conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright
//     notice, this list of conditions and the following disclaimer in the
//     documentation and/or other materials provided with the distribution.
//  3. The names of the authors and copyright holders may not be used to
//     endorse or promote products derived from this software without
//     specific prior written permission.
//
//  IN NO EVENT SHALL DANIEL W. MCROBB BE LIABLE TO ANY PARTY FOR
//  DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES,
//  INCLUDING LOST PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE,
//  EVEN IF DANIEL W. MCROBB HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH
//  DAMAGE.
//
//  THE SOFTWARE PROVIDED HEREIN IS ON AN "AS IS" BASIS, AND
//  DANIEL W. MCROBB HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT,
//  UPDATES, ENHANCEMENTS, OR MODIFICATIONS. DANIEL W. MCROBB MAKES NO
//  REPRESENTATIONS AND EXTENDS NO WARRANTIES OF ANY KIND, EITHER
//  IMPLIED OR EXPRESS, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//  WARRANTIES OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE,
//  OR THAT THE USE OF THIS SOFTWARE WILL NOT INFRINGE ANY PATENT,
//  TRADEMARK OR OTHER RIGHTS.
//===========================================================================

//---------------------------------------------------------------------------
//!  \file svnwhat.cc
//!  \brief Specialized version of 'what' command
//---------------------------------------------------------------------------

extern "C" {
  #include <unistd.h>  // for getopt()
}

#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <vector>

#include "DwmWhat.hh"
#include "DwmWhatParsedFiles.hh"

using namespace std;

#define DWMWHAT_COPYRIGHT  "Daniel McRobb 2025 " DWM_WHAT_SYM_JACKOLANTERN \
  DWM_WHAT_SYM_GHOST " "

#if defined(DWM_WHAT_CAN_USE_REFLECTION)

//----------------------------------------------------------------------------
//!  
//----------------------------------------------------------------------------
static auto GetPackages()
{
  auto pkgs = Dwm::What::get_what_infos<^^Dwm>();
  std::ranges::sort(pkgs);
  auto u = std::ranges::unique(pkgs);
  pkgs.erase(u.begin(), u.end());
  return pkgs;
}

//----------------------------------------------------------------------------
//!  
//----------------------------------------------------------------------------
static void DumpPackagesJson(const char *argv0)
{
  auto pkgs = GetPackages();
  
  if (! pkgs.empty()) {
    cout << "{ \"file\": \"" << argv0 << "\", \"infos\": [";
    bool  first = true;
    for (auto & pkg : pkgs) {
      if (! first) {
        cout << "," << pkg.second.second;
      }
      else {
        cout << "  " << pkg.second.second;
        first = false;
      }
    }
    cout << "] }\n";
  }
  return;
}

//----------------------------------------------------------------------------
//!  
//----------------------------------------------------------------------------
static void DumpPackagesPlain()
{
  auto pkgs = GetPackages();
  
  if (! pkgs.empty()) {
    for (auto & pkg : pkgs) {
      cout << pkg.second.first << '\n';
    }
  }
  return;
}

#endif

//----------------------------------------------------------------------------
//!  
//----------------------------------------------------------------------------
static void Usage(const char *argv0)
{
  std::cerr << "Usage: " << argv0 << " [-v|-V] [-j] [-a] files...\n";
  return;
}

//----------------------------------------------------------------------------
//!  
//----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
  bool  showVersion = false, showVerbose = false, showAsJson = false,
    unique = true;
  int  optChar;
  while ((optChar = getopt(argc, argv, "ajvV")) != -1) {
    switch (optChar) {
      case 'a':        unique = false;                               break;        
      case 'j':        showAsJson = true;                            break;
      case 'v':        showVersion = true;                           break;
      case 'V':        showVersion = true, showVerbose = true;       break;
      default:
        Usage(argv[0]);
        return 1;
        break;
    }
  }

  if (showVersion) {
#if defined(DWM_WHAT_CAN_USE_REFLECTION)
    cerr << "Using reflection...\n";
    
    if (showVerbose) {
      DumpPackagesJson(argv[0]);
    }
    else {
      DumpPackagesPlain();
    }
#else
    if (showVerbose) {
      std::cout << "{\"file\":\"" << argv[0] << "\",\"infos\":["
                << Dwm::What::info().as_json()
                << "]}\n";
    }
    else {
      std::cout << Dwm::What::info().data_view() << '\n';
    }
#endif
    return 0;
  }

  if (optind >= argc) {
    Usage(argv[0]);
    return 1;
  }

  Dwm::What::ParsedFiles  pfs(argc - optind, &argv[optind], unique);
  if (showAsJson) {
    std::cout << pfs.as_json() << '\n';
  }
  else {
    std::cout << pfs;
  }
  return 0;
}
