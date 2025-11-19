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
//!  @file DwmWhatParsedInfo.hh
//!  @author Daniel W. McRobb
//!  @brief Dwm::What::ParsedInfo class definition
//---------------------------------------------------------------------------

#ifndef _DWMWHATPARSEDINFO_HH_
#define _DWMWHATPARSEDINFO_HH_

#include <cstdint>
#include <iostream>
#include <string>
#include <string_view>

namespace Dwm {

  namespace What {

    //------------------------------------------------------------------------
    //!  Provides an interface very similar to Dwm::What::Info, but can
    //!  be constructed at runtime from a string and will handle SCCS-like
    //!  strings that do not conform to the Dwm::What::Info format.  When
    //!  a string does not conform to the Dwm::What::Info format, we'll
    //!  only populate the first segment, with the entire string (retrievable
    //!  with view() or nth(0)).
    //!  This is only useful for utilities like dwmwhat(1) that want to
    //!  process SCCS-like strings found in files.
    //------------------------------------------------------------------------
    class ParsedInfo
    {
    public:
      //----------------------------------------------------------------------
      //!  Default our constructors, assignment and destructor.
      //----------------------------------------------------------------------
      ParsedInfo() = default;
      ParsedInfo(const ParsedInfo &) = default;
      ParsedInfo(ParsedInfo &&) = default;
      ParsedInfo & operator = (const ParsedInfo &) = default;
      ParsedInfo & operator = (ParsedInfo &&) = default;
      ~ParsedInfo() = default;
      
      //----------------------------------------------------------------------
      //!  Construct from the given SCCS string @c sccsString, which must
      //!  start with "@(#)".  If @c sccsString does not start with "@(#)",
      //!  throws std::invalid_argument.
      //----------------------------------------------------------------------
      ParsedInfo(const std::string & sccsString);

      //----------------------------------------------------------------------
      //!  Returns true if the ParsedInfo was parsed as a string in the form
      //!  used by Dwm::What::Info.  If @c true, all 7 (0 through 6) segments
      //!  are valid.  If @c false, the entire SCCS string is stored in the
      //!  first segment.
      //----------------------------------------------------------------------
      bool parsed() const
      { return _parsed; }
        
      //----------------------------------------------------------------------
      //!  Returns a view of the nth segment.
      //----------------------------------------------------------------------
      std::string_view nth(std::size_t n) const;

      //----------------------------------------------------------------------
      //!  Returns a view of the entire string.
      //----------------------------------------------------------------------
      std::string_view view() const;

      //----------------------------------------------------------------------
      //!  Returns a view of the type segment, which will be empty if
      //!  @c parsed() is false.
      //----------------------------------------------------------------------
      std::string_view type() const;

      //----------------------------------------------------------------------
      //!  Returns a view of the status segment, which will be empty if
      //!  @c parsed() is false.
      //----------------------------------------------------------------------
      std::string_view status() const;

      //----------------------------------------------------------------------
      //!  Returns a view of the name segment, which will be empty if
      //!  @c parsed() is false.
      //----------------------------------------------------------------------
      std::string_view name() const;

      //----------------------------------------------------------------------
      //!  Returns a view of the version segment, which will be empty if
      //!  @c parsed() is false.
      //----------------------------------------------------------------------
      std::string_view version() const;

      //----------------------------------------------------------------------
      //!  Returns a view of the copyright segment, which will be empty if
      //!  @c parsed() is false.
      //----------------------------------------------------------------------
      std::string_view copyright() const;

      //----------------------------------------------------------------------
      //!  Returns a view of the other segment, which will be empty if
      //!  @c parsed() is false.
      //----------------------------------------------------------------------
      std::string_view other() const;

      //----------------------------------------------------------------------
      //!  Returns a view of the sring from the type segment to the other
      //!  segment (inclusive) if parsed() is true, else returns an empty
      //!  view.
      //----------------------------------------------------------------------
      std::string_view data_view() const;

      //----------------------------------------------------------------------
      //!  Returns the info in JSON form.
      //----------------------------------------------------------------------
      std::string as_json() const;

      //----------------------------------------------------------------------
      //!  Less-than operator just so we can sort ParsedInfo instances.
      //----------------------------------------------------------------------
      bool operator < (const ParsedInfo & pi) const
      { return view() < pi.view(); }
        
      //----------------------------------------------------------------------
      //!  Equality operator.
      //----------------------------------------------------------------------
      bool operator == (const ParsedInfo & pi) const
      { return view() == pi.view(); }

      //----------------------------------------------------------------------
      //!  Print to an ostream.
      //----------------------------------------------------------------------
      friend std::ostream &
      operator << (std::ostream & os, const ParsedInfo & pi);

    private:
      uint32_t     _seglengths[7];
      std::string  _buffer;
      bool         _parsed;
    };
    
  }  // namespace What

}  // namespace Dwm

#endif  // _DWMWHATPARSEDINFO_HH_
