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
//!  @file DwmWhatParsedInfo.cc
//!  @author Daniel W. McRobb
//!  @brief Dwm::What::ParsedInfo class implementation
//---------------------------------------------------------------------------

#include <numeric>
#include <regex>
#include <stdexcept>

#include "DwmWhatInfo.hh"
#include "DwmWhatParsedInfo.hh"

namespace Dwm {

  namespace What {

    //------------------------------------------------------------------------
    ParsedInfo::ParsedInfo(const std::string & sccsString)
        : _buffer(sccsString), _parsed(false)
    {
      if ((sccsString.size() < 4)
          || (sccsString[0] != '@')
          || (sccsString[1] != '(')
          || (sccsString[2] != '#')
          || (sccsString[3] != ')')) {
        throw std::invalid_argument("Invalid SCCS string");
      }
      
      memset(_seglengths, 0, sizeof(_seglengths));
      static const Dwm::What::SegmentedLiteral rgxlit(DWM_WHAT_DELIM,
                                                      "@\\(#\\)",
                                                      "(.+)",    // type(s)
                                                      "(.+)",    // status
                                                      "(.+)",    // name
                                                      "(.+)",    // version
                                                      "(.+)",    // copyright
                                                      "(.+)");   // other
      static const std::regex  rgx(rgxlit.buffer(),
                                   std::regex::ECMAScript|std::regex::optimize);
      std::smatch sm;
      if (std::regex_match(sccsString, sm, rgx) && (sm.size() == 7)) {
        _seglengths[0] = sizeof("@(#)") - 1;
        for (size_t i = 1; i < 7; ++i) {
          _seglengths[i] = sm[i].str().size();
        }
        _parsed = true;
      }
      else {
        _seglengths[0] = sccsString.size();
      }
    }

    //------------------------------------------------------------------------
    std::string_view ParsedInfo::nth(std::size_t n) const
    {
      assert(n < 7);

      if (_parsed) {
        std::size_t  off = std::accumulate(_seglengths, &_seglengths[n], 0);
        off += n * (sizeof(DWM_WHAT_DELIM)-1);  // delimiters
        if (off >= _buffer.size()) {
          return std::string_view(_buffer.data() + _buffer.size(), 0);
        }
        else {
          return std::string_view(_buffer.data() + off, _seglengths[n]);
        }
      }
      else {
        if (n == 0) {
          return std::string_view(_buffer.data(), _buffer.size());
        }
        else {
          return std::string_view(_buffer.data() + _buffer.size(), 0);
        }
      }
    }

    //------------------------------------------------------------------------
    std::string_view ParsedInfo::view() const
    {
      return std::string_view(_buffer.data(), _buffer.size());
    }

    //------------------------------------------------------------------------
    std::string_view ParsedInfo::data_view() const
    {
      std::string_view  sv = view();
      std::size_t       sz = sv.size();
      std::size_t       skip = ((sizeof("@(#)") - 1)
                                + (sizeof(DWM_WHAT_DELIM) - 1));
      
      if (_parsed) {
        const char       *p = sv.data() + skip;
        if (sv.size() >= skip) {
          sz -= skip;
        }
        else {
          sz = 0;
        }
        return std::string_view(p, sz);
      }
      else {
        return std::string_view(sv.data() + sv.size(), 0);
      }
    }

    //------------------------------------------------------------------------
    std::ostream & operator << (std::ostream & os, const ParsedInfo & pi)
    {
      if (pi._parsed) { os << pi.data_view(); }
      else            { os << pi.view(); }
      return os;
    }

    static std::string json_escape(std::string_view sv)
    {
      static const std::regex
        rgx("\"", std::regex::ECMAScript|std::regex::optimize);
      
      std::string  rc(sv);
      return std::regex_replace(rc, rgx, "\\\"");
    }
    
    //------------------------------------------------------------------------
    std::string ParsedInfo::as_json() const
    {
      std::string  s("{");
      if (_parsed) {
        s += "\"type\":\"";
        s += json_escape(type());
        s += "\",\"status\":\"";
        s += json_escape(status());
        s += "\",\"name\":\"";
        s += json_escape(name());
        s += "\",\"version\":\"";
        s += json_escape(version());
        s += "\",\"copyright\":\"";
        s += json_escape(copyright());
        s += "\",\"other\":\"";
        s += json_escape(other());
        s += "\",\"view\":\"";
        s += json_escape(view());
        s += "\"";
      }
      else {
        s += "\"view\":\"";
        s += json_escape(view());
        s += "\"";
      }
      s += "}";
      return s;
    }

    //------------------------------------------------------------------------
    std::string_view ParsedInfo::type() const
    { return nth(1); }

    //------------------------------------------------------------------------
    std::string_view ParsedInfo::status() const
    { return nth(2); }

    //------------------------------------------------------------------------
    std::string_view ParsedInfo::name() const
    { return nth(3); }

    //------------------------------------------------------------------------
    std::string_view ParsedInfo::version() const
    { return nth(4); }

    //------------------------------------------------------------------------
    std::string_view ParsedInfo::copyright() const
    { return nth(5); }

    //------------------------------------------------------------------------
    std::string_view ParsedInfo::other() const
    { return nth(6); }
    
    
  }  // namespace What

}  // namespace Dwm
