//===========================================================================
// @(#) $DwmPath$
//===========================================================================
//  Copyright (c) Daniel W. McRobb 2025
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
//!  \file TestInfo.cc
//!  \author Daniel W. McRobb
//!  \brief Unit tests for Dwm::What::Info
//---------------------------------------------------------------------------

#include <cassert>
#include <iostream>
#include <regex>

#include "DwmWhatInfo.hh"

//----------------------------------------------------------------------------
//!  
//----------------------------------------------------------------------------
inline constexpr const Dwm::What::Info __attribute__((used))
g_info1(DWM_WHAT_TYPE_HDR, DWM_WHAT_STATUS_RC, "g_info1", "0.0.1",
         "Daniel McRobb " DWM_WHAT_SYM_GHOST, "mcplex.net");

//----------------------------------------------------------------------------
//!  
//----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
  assert(Dwm::What::info().name() == "DwmWhat");
  assert(! Dwm::What::info().type().empty());
  assert(! Dwm::What::info().version().empty());
  assert(! Dwm::What::info().status().empty());
  assert(! Dwm::What::info().copyright().empty());
  assert(Dwm::What::info().other() == "mcplex.net");
  
  assert(g_info1.type() == DWM_WHAT_TYPE_HDR);
  assert(g_info1.status() == DWM_WHAT_STATUS_RC);
  assert(g_info1.name() == "g_info1");
  assert(g_info1.version() == "0.0.1");
  assert(g_info1.copyright() == "Daniel McRobb " DWM_WHAT_SYM_GHOST);
  assert(g_info1.other() == "mcplex.net");

  static constexpr const Dwm::What::Info __attribute__((used))
    maininfo1(DWM_WHAT_TYPE_EXE, DWM_WHAT_STATUS_DEV, "maininfo1", "1.0.0",
              "Daniel McRobb " DWM_WHAT_SYM_GHOST,
              DWM_WHAT_SYM_RP_TRIANGLE " mcplex.net");

  assert(maininfo1.type() == DWM_WHAT_TYPE_EXE);
  assert(maininfo1.status() == DWM_WHAT_STATUS_DEV);
  assert(maininfo1.name() == "maininfo1");
  assert(maininfo1.version() == "1.0.0");
  assert(maininfo1.copyright()
         == "Daniel McRobb " DWM_WHAT_SYM_GHOST);
  assert(maininfo1.other() == DWM_WHAT_SYM_RP_TRIANGLE " mcplex.net");
  assert(maininfo1.data_view() ==
         DWM_WHAT_TYPE_EXE " " DWM_WHAT_STATUS_DEV " maininfo1 1.0.0 "
         DWM_WHAT_SYM_COPYRIGHT " Daniel McRobb " DWM_WHAT_SYM_GHOST " "
         DWM_WHAT_SYM_OTHER " " DWM_WHAT_SYM_RP_TRIANGLE " mcplex.net");
  
  assert(maininfo1 != g_info1);
  assert(g_info1 < maininfo1);

  static constexpr const Dwm::What::SegmentedLiteral __attribute__((used))
    mainsegstr(" ","@(#)",DWM_WHAT_TYPE_EXE,DWM_WHAT_STATUS_DEV,
               "maininfo1", "1.0.0", DWM_WHAT_SYM_COPYRIGHT,
               "Daniel McRobb " DWM_WHAT_SYM_GHOST, DWM_WHAT_SYM_OTHER,
               DWM_WHAT_SYM_RP_TRIANGLE " mcplex.net");
  static_assert(mainsegstr.view() == maininfo1.view());
  static_assert(maininfo1.num_segments() == mainsegstr.num_segments());
  static_assert(sizeof(decltype(maininfo1)::BufType)
                == sizeof(decltype(mainsegstr)::BufType));
  static_assert(std::is_same_v<decltype(maininfo1)::BufType,
                               decltype(mainsegstr)::BufType>);

  for (size_t i = 0; i < maininfo1.num_segments(); ++i) {
    assert(maininfo1.nth(i) == mainsegstr.nth(i));
  }

#if 0
  auto infoCopy = Dwm::What::info();
  assert(infoCopy.view().data()
         != Dwm::What::DwmWhat_2025_11_14.view().data());
  assert(infoCopy.view() == Dwm::What::DwmWhat_2025_11_14.view());
#endif
  
  return 0;
}
