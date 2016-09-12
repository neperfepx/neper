# This file is part of the Neper software package.
# Copyright (C) 2003-2016 Romain Quey
# See the COPYING file in the top-level directory.

pdf:
	texi2pdf neper.texi 
	cp neper.pdf ..

info:
	makeinfo neper.texi
	cp neper.info ..

html:
	makeinfo --html neper.texi

clean:
	rm -f *.cp* *.fn* *.ky* *.pg* *.tp* *.vr* *.mv*\
          *.log *.toc *.aux *.dvi *~ *.pdf *.info
install:
	su -c "mkdir -p /usr/local /usr/local/info;\
	cp -f neper.info /usr/local/info/"
