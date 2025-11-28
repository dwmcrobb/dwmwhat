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
//!  @file DwmWhatFileParseState.hh
//!  @author Daniel W. McRobb
//!  @brief Dwm::What::FileParseState class declaration
//---------------------------------------------------------------------------

#ifndef _DWMWHATFILEPARSESTATE_HH_
#define _DWMWHATFILEPARSESTATE_HH_

#include <vector>

#include "DwmWhatParsedInfo.hh"

namespace Dwm {

  namespace What {

    //------------------------------------------------------------------------
    //!  A small state machine that can be used to find an SCCS identifier
    //!  string.  We give consecutive chunks of data to the state machine
    //!  via ProcessBuffer(), and the state machine will populate the @c infos
    //!  given to it in the constructor with any SCCS identifier strings
    //!  found in the chunks of data.
    //!  The only reason this class exists: to support searching for SCCS
    //!  identifiers in a generic way that externalizes the reading of the
    //!  data from its original source.  This allows dwmwhat to reasonably
    //!  search in what is read from stdin, for example.
    //------------------------------------------------------------------------
    class FileParseState
    {
    public:
      //----------------------------------------------------------------------
      //!  Construct with a reference to @c infos.  We don't copy @c infos;
      //!  we keep a reference so we can modify it while processing data.
      //----------------------------------------------------------------------
      FileParseState(std::vector<ParsedInfo> & infos);

      //----------------------------------------------------------------------
      //!  Process the next buffer @c buf of size @c bufsiz.
      //----------------------------------------------------------------------
      void ProcessBuffer(const char *buf, size_t bufsiz);

    private:
      //----------------------------------------------------------------------
      //!  Pointer to current state.  Always points to one of the member
      //!  functions below.
      //----------------------------------------------------------------------
      void (FileParseState::*_state)(const char *buf, size_t bufsize);
      
      //----------------------------------------------------------------------
      //!  String used to hold SCCS identifier string while we're in the
      //!  process of building it.
      //----------------------------------------------------------------------
      std::string               _string;
      
      //----------------------------------------------------------------------
      //!  Reference to the infos given to us in the constructor.  We push
      //!  back on this vector as we find SCCS identifier strings.
      //----------------------------------------------------------------------
      std::vector<ParsedInfo> & _infos;
      
      //----------------------------------------------------------------------
      //!  States, which are just member functions.
      //----------------------------------------------------------------------
      void LookingForAtSign(const char *buf, size_t bufsize);
      void LookingForOpenParen(const char *buf, size_t bufsize);
      void LookingForHashSign(const char *buf, size_t bufsize);
      void LookingForCloseParen(const char *buf, size_t bufsize);
      void LookingForNewlineOrNull(const char *buf, size_t bufsize);
      
    };
    
  }  // namespace What

}  // namespace Dwm

#endif  // _DWMWHATFILEPARSESTATE_HH_
