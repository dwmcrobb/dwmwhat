include ../../Makefile.vars

dwmwhat.pdf: *.tex
	lualatex dwmwhat.tex
	lualatex dwmwhat.tex

tarprep: ${DOCINSTALLDIR}/dwmwhat.pdf

${DOCINSTALLDIR}/dwmwhat.pdf: dwmwhat.pdf
	${INSTALL} -c -m 644 $< $@
