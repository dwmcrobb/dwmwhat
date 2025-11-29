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
//!  @file DwmWhatParsedFiles.cc
//!  @author Daniel W. McRobb
//!  @brief Dwm::What::ParsedFiles class implementation
//---------------------------------------------------------------------------

extern "C" {
  #include <unistd.h>
}

#include "DwmWhatParsedFiles.hh"

namespace Dwm {

  namespace What {

    //------------------------------------------------------------------------
    ParsedFiles::ParsedFiles(int numFiles, char * const files[], bool unique)
    {
      for (int i = 0; i < numFiles; ++i) {
        if (std::string(files[i]) == "-") {
          _parsedFiles.push_back(ParsedFile(STDIN_FILENO, unique));
        }
        else {
          _parsedFiles.push_back(ParsedFile(files[i], unique));
        }
      }
    }

    //------------------------------------------------------------------------
    std::ostream & operator << (std::ostream & os, const ParsedFiles & pfs)
    {
      if (! pfs._parsedFiles.empty()) {
        for (const auto & pf : pfs._parsedFiles) {
          os << pf;
        }
      }
      return os;
    }
    
    //------------------------------------------------------------------------
    std::string ParsedFiles::as_json() const
    {
      std::string  rc;
      if (! _parsedFiles.empty()) {
        rc += "[";
        std::string  comma;
        for (const auto & pf : _parsedFiles) {
          rc += comma + pf.as_json();
          comma = ",";
        }
        rc += "]";
      }
      
      return rc;
    }
    
    
  }  // namespace What

}  // namespace Dwm
