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
//!  \file DwmWhatSegmentedLiteral.hh
//!  \author Daniel W. McRobb
//!  \brief NOT YET DOCUMENTED
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
    //!  
    //------------------------------------------------------------------------
    template <std::size_t DelimLen, std::size_t FirstLen, std::size_t ...N>
    class SegmentedLiteral
    {
    public:
      template <std::size_t D, std::size_t ...Size>
      struct CalcNumChars {
        static constexpr size_t sz =
          ((D - 1) * (sizeof...(Size) - 1)) + ((Size - 1) + ...) + 1;
      };

      static constexpr size_t NumChars =
        CalcNumChars<DelimLen, FirstLen, N...>::sz;

      static constexpr size_t NumSegs = sizeof...(N) + 1;

      using SegLenType =
        std::conditional<(NumChars <= 256),
                         uint8_t,
                         typename std::conditional<(NumChars <= 65536),
                                                   uint16_t,
                                                   uint32_t>::type>::type;
      using BufType = const char(&)[NumChars];

      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      consteval SegmentedLiteral(const char (&delim)[DelimLen],
                                 const char (&f)[FirstLen],
                                 const char (&...s)[N])
      {
        static_assert(NumChars <= std::numeric_limits<SegLenType>::max());

        //  'f' is just 'first'
        auto  it = std::ranges::copy_n(f,FirstLen - 1, _buffer).out;
        std::size_t  si = 0;
        _seglengths[si++] = FirstLen - 1;
        ((_seglengths[si++] = N - 1,
          it = std::ranges::copy_n(delim,DelimLen - 1, it).out,
          it = std::ranges::copy_n(s, N-1, it).out), ...);
        *it = '\0';
      }

      //----------------------------------------------------------------------
      //!  Returns a view of the whole buffer, minus the terminating null.
      //----------------------------------------------------------------------
      consteval operator std::string_view () const noexcept
      { return std::string_view(_buffer,NumChars - 1); }

      //----------------------------------------------------------------------
      //!  Returns the buffer.
      //----------------------------------------------------------------------
      constexpr operator BufType () const noexcept
      { return _buffer; }

      //----------------------------------------------------------------------
      //!  Returns a view of the whole buffer, minus the terminating null.
      //----------------------------------------------------------------------
      constexpr std::string_view view() const noexcept
      { return std::string_view(_buffer,NumChars - 1); }
        
      //----------------------------------------------------------------------
      //!  Returns the number of segments in the buffer (1 or more).
      //----------------------------------------------------------------------
      constexpr std::size_t num_segments() const noexcept
      { return NumSegs; }
      
      //----------------------------------------------------------------------
      //!  Returns a view of the nth segment.
      //----------------------------------------------------------------------
      constexpr std::string_view nth(std::size_t n) const noexcept
      {
        assert(n < NumSegs);
        std::size_t  off = std::accumulate(_seglengths, &_seglengths[n], 0);
        off += n * (_delimLen - 1);
        return std::string_view(_buffer + off, _seglengths[n]);
      }
      
      //----------------------------------------------------------------------
      //!  Returns the size of the type used to hold segment lengths.
      //----------------------------------------------------------------------
      constexpr std::size_t size_of_seg_lengths() const noexcept
      { return sizeof(SegLenType); }
      
    private:
      char         _buffer[NumChars] {};
      SegLenType   _seglengths[NumSegs] {};
      std::size_t  _delimLen = DelimLen;

    };
    
  }  // namespace What

}  // namespace Dwm

#endif  // _DWMWHATSEGMENTEDLITERAL_HH_
