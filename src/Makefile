GECODESRC=~/gecode-4.3.2
GECODEINSTALLDIR=$(HOME)/gecodelib
GECODEDISABLE=--disable-examples --disable-thread --disable-float-vars --disable-set-vars
GECODETUPLEDIR=~/gecode-tuples/src
GECODEVISFILE=intpair.vis

configure:
	cd $(GECODESRC) ; \
	./configure --prefix=$(GECODEINSTALLDIR) $(GECODEDISABLE) --with-vis=$(GECODETUPLEDIR)/$(GECODEVISFILE)

pair:
	cd $(GECODESRC) ; make ; make install

clean:
	rm *.o *~ a.out


