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
//!  @file DwmWhatParsedFile.cc
//!  @author Daniel W. McRobb
//!  @brief NOT YET DOCUMENTED
//---------------------------------------------------------------------------

extern "C" {
  #include <sys/mman.h>  // for mmap()
  #include <sys/stat.h>  // for stat()
  #include <fcntl.h>     // for open()
  #include <libgen.h>    // for basename()
  #include <unistd.h>
}

#include <algorithm>
#include <utility>

#include "DwmWhatParsedFile.hh"

namespace Dwm {

  namespace What {

    //------------------------------------------------------------------------
    static std::pair<const char *,size_t>
    MapFile(const std::string & fileName)
    {
      std::pair<const char *,size_t>  rc(nullptr,0);
      int  fd = open(fileName.c_str(), O_RDONLY);
      if (fd >= 0) {
        struct stat  statbuf;
        if (stat(fileName.c_str(), &statbuf) == 0) {
          rc.second = statbuf.st_size;
          rc.first = (const char *)mmap(0, statbuf.st_size, PROT_READ,
                                        MAP_FILE|MAP_SHARED, fd, 0);
          if (rc.first == MAP_FAILED) {
            rc.first = 0;
            rc.second = 0;
          }
        }
        close(fd);
      }
      return rc;
    }

    //------------------------------------------------------------------------
    ParsedFile::ParsedFile(std::string_view fileName, bool unique)
    {
      _fileName = std::string(fileName);
      
      auto         mappedFile = MapFile(_fileName);
      auto         map = mappedFile.first;
      std::size_t  size = mappedFile.second;
      if (map && size) {
        std::size_t  i = 0;
        while (i < (size - 5)) {
          if ((map[i] == '@') && (map[i+1] == '(') && (map[i+2] == '#')
              && (map[i+3] == ')')) {
            std::string::size_type  startidx = i;
            i += 4;
            while ((map[i] != '\0') && (map[i] != '\n') && (i < size)) {
              ++i;
            }
            if ((i < size) && (i > (startidx + 4))) {
              _infos.push_back(ParsedInfo(std::string(&map[startidx], &map[i])));
            }
          }
          else {
            ++i;
          }
        }
      }
      std::sort(_infos.begin(), _infos.end());
      if (unique) {
        auto last = std::unique(_infos.begin(), _infos.end());
        _infos.erase(last, _infos.end());
      }
    }

    //------------------------------------------------------------------------
    std::ostream & operator << (std::ostream & os, const ParsedFile & pf)
    {
      os << pf._fileName << ":\n";
      for (const auto & pi : pf._infos) {
        os << pi << '\n';
      }
      return os;
    }

    //------------------------------------------------------------------------
    std::string ParsedFile::as_json() const
    {
      std::string  rc("{\"file\": \"" + _fileName + "\"");
      if (! _infos.empty()) {
        rc += ",\"infos\":[";
        std::string  comma;
        for (const auto & pi : _infos) {
          rc += comma + pi.as_json();
          comma = ",";
        }
        rc += "]";
      }
      rc += "}";
      return rc;
    }
    
  }  // namespace What

}  // namespace Dwm
