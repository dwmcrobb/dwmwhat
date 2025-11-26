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
//!  @file DwmWhatMacros.hh
//!  @author Daniel W. McRobb
//!  @brief Macros
//---------------------------------------------------------------------------

#ifndef _DWMWHATMACROS_HH_
#define _DWMWHATMACROS_HH_

//----------------------------------------------------------------------------
//!  Just some macros for UTF-8 encodings of some unicode characters
//----------------------------------------------------------------------------
#define DWM_WHAT_SYM_NB_SPACE          "\xC2\xA0"        // non-breaking space
#define DWM_WHAT_SYM_ALARM_CLOCK       "\xE2\x8F\xB0"                   // ⏰
#define DWM_WHAT_SYM_ALIEN             "\xF0\x9F\x91\xBD"               // 👽
#define DWM_WHAT_SYM_BLUE_CIRCLE       "\xF0\x9F\x94\xB5"               // 🔵
#define DWM_WHAT_SYM_BOOKS             "\xF0\x9F\x93\x9A"               // 📚
#define DWM_WHAT_SYM_CALENDAR          "\xF0\x9F\x93\x86"               // 📆
#define DWM_WHAT_SYM_CAUTION_SIGN      "\xE2\x98\xA1"                   // ☡
#define DWM_WHAT_SYM_CHECK_MARK        "\xE2\x9C\x93"                   // ✓
#define DWM_WHAT_SYM_CONSTRUCT_WORKER  "\xF0\x9F\x91\xB7"               // 👷
#define DWM_WHAT_SYM_CONSTRUCT_ZONE    "\xF0\x9F\x9A\xA7"               // 🚧
#define DWM_WHAT_SYM_COPYRIGHT         "\xC2\xA9"                       // ©
#define DWM_WHAT_SYM_CYCLONE           "\xF0\x9F\x8C\x80"               // 🌀
#define DWM_WHAT_SYM_DOT               "\xE2\x97\x8F"                   // ●
#define DWM_WHAT_SYM_FILE_FOLDER       "\xF0\x9F\x93\x81"               // 📁
#define DWM_WHAT_SYM_FULLHASH          "\xEF\xBC\x83"                   // ＃
#define DWM_WHAT_SYM_GEAR              "\xE2\x9A\x99"                   // ⚙
#define DWM_WHAT_SYM_GHOST             "\xF0\x9F\x91\xBB"               // 👻
#define DWM_WHAT_SYM_HEAVY_CHECK       "\xE2\x9C\x94"                   // ✔
#define DWM_WHAT_SYM_HEAVY_CIRCLE      "\xE2\xAD\x95"                   // ⭕
#define DWM_WHAT_SYM_HEAVY_EXCL        "\xE2\x9D\x97"                   // ❗
#define DWM_WHAT_SYM_HEAVY_PLUS        "\xE2\x9E\x95"                   // ➕
#define DWM_WHAT_SYM_JACKOLANTERN      "\xF0\x9F\x8E\x83"               // 🎃
#define DWM_WHAT_SYM_OPEN_BOOK         "\xF0\x9F\x93\x96"               // 📖
#define DWM_WHAT_SYM_OPEN_FOLDER       "\xF0\x9F\x93\x82"               // 📂
#define DWM_WHAT_SYM_PACKAGE           "\xF0\x9F\x93\xA6"               // 📦
#define DWM_WHAT_SYM_PAGE              "\xF0\x9F\x93\x84"               // 📄
#define DWM_WHAT_SYM_ROBOT             "\xF0\x9F\xA4\x96"               // 🤖
#define DWM_WHAT_SYM_ROCKET            "\xF0\x9F\x9A\x80"               // 🚀
#define DWM_WHAT_SYM_STAR              "\xE2\xAD\x90"                   // ⭐
#define DWM_WHAT_SYM_RP_TRIANGLE       "\xE2\x96\xB6"                   // ▶
#define DWM_WHAT_SYM_WARNING_SIGN      "\xE2\x9A\xA0"                   // ⚠
#define DWM_WHAT_SYM_WHITE_CHECK       "\xE2\x9C\x85"                   // ✅

//----------------------------------------------------------------------------
//!  The three characters I use for 'status' in Dwm::What::Info.
//!  Development, release candidate or release.
//----------------------------------------------------------------------------
#define DWM_WHAT_STATUS_DEV  DWM_WHAT_SYM_HEAVY_EXCL
#define DWM_WHAT_STATUS_RC   DWM_WHAT_SYM_CONSTRUCT_WORKER
#define	DWM_WHAT_STATUS_REL  DWM_WHAT_SYM_WHITE_CHECK

//----------------------------------------------------------------------------
//!  The characters I use for 'package type' in Dwm::What::Info, one or more
//!  of 'header files', 'library', 'executable' or 'documentation'.
//----------------------------------------------------------------------------
#define DWM_WHAT_TYPE_HDR  DWM_WHAT_SYM_FULLHASH
#define DWM_WHAT_TYPE_LIB  DWM_WHAT_SYM_BOOKS
#define DWM_WHAT_TYPE_EXE  DWM_WHAT_SYM_ROBOT
#define DWM_WHAT_TYPE_DOC  DWM_WHAT_SYM_PAGE

//----------------------------------------------------------------------------
//!  Delimiter used in Dwm::What::Info.
//----------------------------------------------------------------------------
#define DWM_WHAT_DELIM DWM_WHAT_SYM_NB_SPACE

//----------------------------------------------------------------------------
//!  DWM_WHAT_MK_LINE_ARG(__LINE__) can be used to get a string literal of
//!  __LINE__.
//----------------------------------------------------------------------------
#define DWM_WHAT_MK_LINE_ARG(x) DWM_WHAT_MK_LINE_ARG2(x)
#define DWM_WHAT_MK_LINE_ARG2(x) #x

//----------------------------------------------------------------------------
//!  If all you need is a version string and you're using C instead of C++,
//!  the following macro can be used to create a string of the same format
//!  as used by Dwm::What::Info, for example:
//!
//!  const char  my_prog_version[] =
//!     DWM_WHAT_INFO("🤖","✅","myprog","1.0.0","© My Name 2025","my prog");
//----------------------------------------------------------------------------
#define DWM_WHAT_INFO(type,status,name,version,copyright,other)         \
  "@(#) " type " " status " " name " " version " " copyright " " other

#endif  // _DWMWHATMACROS_HH_
