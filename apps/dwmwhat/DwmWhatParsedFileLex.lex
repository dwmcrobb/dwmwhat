%{
  #include "DwmWhatParsedFile.hh"
  
  std::vector<Dwm::What::ParsedInfo>  *g_parsedInfoVec = nullptr;
%}

%option noyywrap

%%

\@\(\#\)[^\n\0]*/[\n\0]  { return 1; }
[^\n]
\n
<<EOF>>  { return 0; }

%%

namespace Dwm {

  namespace What {

    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    ParsedFile::ParsedFile(FILE *f, bool unique)
        : _fileName("-"), _infos()
    {
      if (f) {
        yyin = f;
        size_t  bufsiz = 16*1024*1024;
        char  *buf = (char *)malloc(bufsiz);
        if (buf) {
          setvbuf(yyin, buf, _IOFBF, bufsiz);
          
          g_parsedInfoVec = &_infos;
          while (yylex() == 1) {
            _infos.push_back(std::string(yytext));
          }
          yyrestart(yyin);
          free(buf);
        }
      }
    }
          
  }  // namespace What

}  // namespace Dwm

