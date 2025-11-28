extern "C" {
  #include <sys/types.h>
  #include <sys/uio.h>
  #include <unistd.h>
}

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#include "DwmWhatFileParseState.hh"

namespace Dwm {

  namespace What {

    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    FileParseState::FileParseState(std::vector<ParsedInfo> & infos)
        : _infos(infos), _state(&FileParseState::LookingForAtSign)
    { }
      
    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    void FileParseState::ProcessBuffer(const char *buf, size_t bufsize)
    {
      (this->*_state)(buf, bufsize);
      return;
    }
      
    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    void FileParseState::LookingForAtSign(const char *buf, size_t bufsize)
    {
      size_t  i = 0;
      while ((i < bufsize) && ('@' != buf[i])) {
        ++i;
      }
      if (i < bufsize) {
        _string += '@';
        _state = &FileParseState::LookingForOpenParen;
        (this->*_state)(&(buf[i+1]),bufsize - (i+1));
      }
      return;
    }
    
    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    void FileParseState::LookingForOpenParen(const char *buf, size_t bufsize)
    {
      if (buf[0] == '(') {
        _state = &FileParseState::LookingForHashSign;
        _string += '(';
      }
      else {
        _string.clear();
        _state = &FileParseState::LookingForAtSign;
      }
      if (bufsize > 1) {
        (this->*_state)(&(buf[1]), bufsize-1);
      }
      return;
    }
      
    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    void FileParseState::LookingForHashSign(const char *buf, size_t bufsize)
    {
      if (buf[0] == '#') {
        _string += '#';
        _state = &FileParseState::LookingForCloseParen;
      }
      else {
        _string.clear();
        _state = &FileParseState::LookingForAtSign;
      }
      if (bufsize > 1) {
        (this->*_state)(&(buf[1]), bufsize-1);
      }
      return;
    }
      
    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    void FileParseState::LookingForCloseParen(const char *buf, size_t bufsize)
    {
      if (buf[0] == ')') {
        _string += ')';
        _state = &FileParseState::LookingForNewlineOrNull;
      }
      else {
        _string.clear();
        _state = &FileParseState::LookingForAtSign;
      }
      if (bufsize > 1) {
        (this->*_state)(&(buf[1]), bufsize-1);
      }
      return;
    }
    
    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    void
    FileParseState::LookingForNewlineOrNull(const char *buf, size_t bufsize)
    {
      size_t  i = 0;
      for ( ; i < bufsize; ++i) {
        if ((buf[i] == '\0') || (buf[i] == '\n')) {
          break;
        }
      }
      _string += std::string(buf, i);
      if (i < bufsize) {
        _infos.push_back(_string);
        _string.clear();
        _state = &FileParseState::LookingForAtSign;
      }
      
      if (_state != &FileParseState::LookingForNewlineOrNull) {
        if (i < bufsize) {
          (this->*_state)(&(buf[i]), bufsize-i);
        }
      }
      return;
    }
    
  }  // namespace What

}  // namespace Dwm

#if 0
//----------------------------------------------------------------------------
//!  
//----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
  std::string test1("@(#) Test1\0",11);
  Dwm::What::FileParseState  fps;
  size_t  bufsiz = 512*1024;
  char  *buf = (char *)malloc(bufsiz);
  if (buf) {
    std::size_t  b;
    while ((b = ::read(STDIN_FILENO, buf, bufsiz)) > 0) {
      fps.ProcessBuffer(buf, b);
    }
    for (const auto & s : fps.strings()) {
      std::cout << s << '\n';
    }
  }
  
}
#endif
