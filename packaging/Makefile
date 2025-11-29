include ../Makefile.vars

ifeq ("${OSNAME}", "freebsd")
PKGPCDIR = libdata/pkgconfig
else
PKGPCDIR = lib/pkgconfig
endif

TARTARGS = ${TARDIR}/${PKGPCDIR}/dwmwhat.pc

all::
	echo "nothing to be done for 'all' target."

tarprep: ${TARTARGS}

${TARDIR}/${PKGPCDIR}/dwmwhat.pc: dwmwhat.pc
	../install-sh -c -m 444 $< $@

distclean::
	rm -f debcontrol fbsd_manifest dwmwhat.pc
