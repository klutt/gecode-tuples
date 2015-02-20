GCDISABLE=--disable-examples --disable-thread --disable-float-vars --disable-set-vars
GCVISFILE=intpair.vis

TUPLED=$(CURDIR)
INSTD=$(patsubst %src, %external/gecode, $(TUPLED))
GCSRC=$(patsubst %src, %external/gecode-4.3.2, $(TUPLED))

LD_LIBRARY_PATH=/usr/local/lib

CXX=g++

#../external/gecodeprefix:
#	mkdir ../external/gecodeprefix

#../external/gecodeeprefix:
#	mkdir ../external/gecodeeprefix

#../external/gecodedatarootdir:
#	mkdir ../external/gecodedatarootdir

FLAGS=-lgecodedriver -lgecodegist -lgecodesearch -lgecodeminimodel -lgecodeint -lgecodekernel -lgecodesupport

trivial:
	/usr/bin/g++ -c trivial.cpp -I/usr/local/include
	/usr/bin/g++ -o trivial trivial.o $(FLAGS) -L /usr/local/lib

configure: ../external/gecodeprefix ../external/gecodeeprefix ../external/gecodedatarootdir
	cd $(GCSRC) ; \
	./configure $(GCDISABLE) --with-vis=$(TUPLED)/$(GCVISFILE)

#	./configure --prefix=$(INSTD)prefix --exec-prefix=$(INSTD)execprefix $(GCDISABLE) --with-vis=$(TUPLED)/$(GCVISFILE)

pair: configure
	cd $(GCSRC) ; make ; sudo make install

clean:
	rm *.o *~ a.out


