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
//!  @file DwmWhatSegmentedLiteral.hh
//!  @author Daniel W. McRobb
//!  @brief Dwm::What::SegmentedLiteral class template
//---------------------------------------------------------------------------

#ifndef _DWMWHATSEGMENTEDLITERAL_HH_
#define _DWMWHATSEGMENTEDLITERAL_HH_

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <limits>
#include <numeric>
#include <ranges>
#include <string_view>
#include <type_traits>

namespace Dwm {

  namespace What {

    //------------------------------------------------------------------------
    //!  Class template to build a string literal from a delimiter string
    //!  liternal and one or more other string literals.
    //------------------------------------------------------------------------
    template <std::size_t DelimLen, std::size_t FirstLen, std::size_t ...SegLen>
    class SegmentedLiteral
    {
    public:
      //----------------------------------------------------------------------
      //!  The number of bytes we need in our character array.  @c D is
      //!  the size of the delimiter that is deduced from the first argument
      //!  of the constructor (a string literal).  Hence the size we need
      //!  for each delimiter is actually D-1 (we don't need the terminating
      //!  null).  @c Size is a pack of the sizes of all other arguments
      //!  from the constructor (also string literals and again we don't
      //!  need the terminating null).  The number of delimiters is one
      //!  less than the number of segments.  We add 1 so we have a
      //!  terminating null (which we don't need but it's useful for tools
      //!  that search binaries for strings).
      //----------------------------------------------------------------------
      template <std::size_t D, std::size_t ...Size>
      struct CalcNumChars {
        static constexpr size_t sz =
          ((D - 1) * (sizeof...(Size) - 1)) + ((Size - 1) + ...) + 1;
      };

      //----------------------------------------------------------------------
      //!  Returns the number of characters we need in our character array.
      //----------------------------------------------------------------------
      static consteval size_t NumChars() noexcept
      { return CalcNumChars<DelimLen, FirstLen, SegLen...>::sz; }
        
      //----------------------------------------------------------------------
      //!  The minimum sized type we need for our array of segment lengths.
      //----------------------------------------------------------------------
      using SegLenType =
        std::conditional<(NumChars() <= 256),
                         uint8_t,
                         typename std::conditional<(NumChars() <= 65536),
                                                   uint16_t,
                                                   uint32_t>::type>::type;

      //----------------------------------------------------------------------
      //!  Just an alias for our buffer type.
      //----------------------------------------------------------------------
      using BufType = const char(&)[NumChars()];

      //----------------------------------------------------------------------
      //!  Constructor.  Since we require a delimiter and at least one
      //!  segment, we have @c firstSeg as a required argument (the first
      //!  segment).  @c moreSegs is the pack of all other segments (which
      //!  may be empty).
      //----------------------------------------------------------------------
      consteval SegmentedLiteral(const char (&delim)[DelimLen],
                                 const char (&firstSeg)[FirstLen],
                                 const char (&...moreSegs)[SegLen]) noexcept
      {
        static_assert(NumChars() <= std::numeric_limits<SegLenType>::max());

        //  handle the first segment
        auto  it = std::ranges::copy_n(firstSeg, FirstLen - 1, _buffer).out;
        std::size_t  si = 0;
        _seglengths[si++] = FirstLen - 1;
        //  and the remaining segments
        ((_seglengths[si++] = SegLen - 1,
          it = std::ranges::copy_n(delim, DelimLen - 1, it).out,
          it = std::ranges::copy_n(moreSegs, SegLen-1, it).out), ...);
        *it = '\0';
      }

      //----------------------------------------------------------------------
      //!  Returns the buffer.
      //----------------------------------------------------------------------
      constexpr BufType buffer() const noexcept
      { return _buffer; }
        
      //----------------------------------------------------------------------
      //!  Returns a view of the whole buffer, minus the terminating null.
      //----------------------------------------------------------------------
      constexpr std::string_view view() const noexcept
      { return std::string_view(_buffer,NumChars() - 1); }
        
      //----------------------------------------------------------------------
      //!  Returns the number of segments in the buffer (1 or more).
      //----------------------------------------------------------------------
      constexpr std::size_t num_segments() const noexcept
      { return sizeof...(SegLen) + 1; }
      
      //----------------------------------------------------------------------
      //!  Returns a view of the nth segment.
      //----------------------------------------------------------------------
      constexpr std::string_view nth(std::size_t n) const noexcept
      {
        assert(n < (sizeof...(SegLen) + 1));
        std::size_t  off = std::accumulate(_seglengths, &_seglengths[n], 0);
        off += n * (DelimLen - 1);
        return std::string_view(_buffer + off, _seglengths[n]);
      }
      
      //----------------------------------------------------------------------
      //!  Returns the size of the type used to hold segment lengths.
      //----------------------------------------------------------------------
      constexpr std::size_t size_of_seg_lengths() const noexcept
      { return sizeof(SegLenType); }

      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      constexpr bool delims_in_content() const noexcept
      {
        if ((sizeof...(SegLen) > 0) && (DelimLen > 1)) {
          std::string_view  d(_buffer + _seglengths[0], DelimLen - 1);
          for (size_t i = 0; i < (sizeof...(SegLen) + 1); ++i) {
            auto  v = this->nth(i);
            if (v.find(d) != v.npos) {
              return true;
            }
          }
        }
        return false;
      }
      
    protected:
      char        _buffer[NumChars()] {};
      SegLenType  _seglengths[sizeof...(SegLen) + 1] {};
    };
    
  }  // namespace What

}  // namespace Dwm

#endif  // _DWMWHATSEGMENTEDLITERAL_HH_
