include ../../Makefile.vars

all: dwmwhat.pdf

dwmwhat.pdf: *.tex
	lualatex dwmwhat.tex
	lualatex dwmwhat.tex

tarprep: ${DOCTARDIR}/dwmwhat.pdf

${DOCTARDIR}/dwmwhat.pdf: dwmwhat.pdf
	${INSTALL} -c -m 644 $< $@

clean::
	rm -f *.log *.aux dwmwhat.fdb_latexmk dwmwhat.fls dwmwhat.lof \
	  dwmwhat.lot dwmwhat.out dwmwhat.pdf dwmwhat.toc

distclean:: clean
	rm -f TitlePage.tex
