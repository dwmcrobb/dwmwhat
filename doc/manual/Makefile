include ../../Makefile.vars

all: dwmwhat.pdf

dwmwhat.pdf: *.tex
	lualatex dwmwhat.tex
	lualatex dwmwhat.tex

tarprep: ${DOCINSTALLDIR}/dwmwhat.pdf

${DOCINSTALLDIR}/dwmwhat.pdf: dwmwhat.pdf
	${INSTALL} -c -m 644 $< $@

clean::
	rm -f *.log *.aux dwmwhat.fdb_latexmk dwmwhat.fls dwmwhat.lof \
	  dwmwhat.lot dwmwhat.out dwmwhat.pdf dwmwhat.toc

