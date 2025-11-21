include ../Makefile.vars

.PHONY : clean clean-install clean-tarprep install tarprep uninstall

html/index.html: ../classes/include/*.hh
	-doxygen Doxyfile
	cp mcplex_net_blue_shadowed.svg html

www: html/index.html
	tar --exclude=\*.md5 -zcvf html.tgz html

tarprep: html/index.html ${TARDIR}/share/doc/dwmwhat.pdf
	mkdir -p ${TARDIR}/share/doc/dwmwhat
	find html -name \*.md5 -exec rm {} \;
	cp html/* ${TARDIR}/share/doc/dwmwhat

${TARDIR}/share/doc/dwmwhat.pdf:
	${MAKE} -C manual tarprep

clean:: clean-tarprep clean-manual
	rm -Rf html latex man

clean-manual::
	${MAKE} -C manual clean

clean-tarprep::
	rm -Rf ${TARDIR}/share/doc/dwmwhat

distclean-manual::
	${MAKE} -C manual distclean

distclean:: distclean-manual
	rm -f Doxyfile
