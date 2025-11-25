include ../../Makefile.vars

all: dwmwhat.pdf

dwmwhat.pdf: *.tex
	latexmk -lualatex dwmwhat.tex

tarprep: ${DOCTARDIR}/dwmwhat.pdf

${DOCTARDIR}/dwmwhat.pdf: dwmwhat.pdf
	${INSTALL} -c -m 644 $< $@

clean::
	rm -f *.log *.aux dwmwhat.fdb_latexmk dwmwhat.fls dwmwhat.lof \
	  dwmwhat.lot dwmwhat.out dwmwhat.toc

distclean:: clean

