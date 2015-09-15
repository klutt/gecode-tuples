GCDISABLE=--disable-examples --disable-thread --disable-float-vars --disable-set-vars
GCVISFILE_EXACT=exact/intpair.vis
GCVISFILE_APPROX=approx/intpair.vis

TUPLED=$(CURDIR)
#INSTD=$(patsubst %src,%external/gecode, $(TUPLED))
INSTD=/usr/local
GCSRC=$(patsubst %src,%external/gecode-4.3.2, $(TUPLED))

LD_LIBRARY_PATH=/usr/local/lib

CXX=g++
LIBS=-L$(INSTD)/lib
INCLUDES=-I$(INSTD)/include

#../external/gecodeprefix:
#	mkdir ../external/gecodeprefix

#../external/gecodeeprefix:
#	mkdir ../external/gecodeeprefix

#../external/gecodedatarootdir:
#	mkdir ../external/gecodedatarootdir

#FLAGS=-lgecodedriver -lgecodegist -lgecodesearch -lgecodeminimodel -lgecodeint -lgecodekernel -lgecodesupport
FLAGS=-lgecodedriver -lgecodesearch -lgecodeminimodel -lgecodeint -lgecodekernel -lgecodesupport

trivial:
	/usr/bin/g++ -c trivial.cpp $(INCLUDES) -fdiagnostics-color=always
	/usr/bin/g++ -o trivial trivial.o $(FLAGS) $(LIBS) -fdiagnostics-color=always

debug:
	/usr/bin/g++ -g -c trivial.cpp $(INCLUDES) -fdiagnostics-color=always
	/usr/bin/g++ -g -o trivial trivial.o $(FLAGS) $(LIBS) -fdiagnostics-color=always

testingeqprop:
	/usr/bin/g++ -g -c testingeqprop.cpp $(INCLUDES) -fdiagnostics-color=always
	/usr/bin/g++ -g -o testingeqprop testingeqprop.o $(FLAGS) $(LIBS) -fdiagnostics-color=always

test/%: testsrc/%.cpp *.hh
	/usr/bin/g++ -g -c testsrc/$*.cpp -o test/$*.o $(INCLUDES) -fdiagnostics-color=always -std=c++11
	/usr/bin/g++ -g -o test/$* test/$*.o $(FLAGS) $(LIBS) -fdiagnostics-color=always
	test/$* -threads 1

test: \
	test/3statedfawithcostint \
	test/3statedfawithcost \
	test/3statedfanocostint \
	test/queens01test \
	test/testtemplate \
	test/zerosolutions \
	test/simpleintpairarray \
	test/simpleintpair \
	test/simpledfa \
	test/3statedfanocost 
	@echo "All tests passed"

configure:
	echo $(INSTD)
	cd $(GCSRC); \
	./configure --disable-qt --prefix=$(INSTD) $(GCDISABLE) --with-vis=$(TUPLED)/$(GCVISFILE_EXACT)
	#./configure $(GCDISABLE) --with-vis=$(TUPLED)/$(GCVISFILE)

#./configure --prefix=$(INSTD)prefix --exec-prefix=$(INSTD)execprefix $(GCDISABLE) --with-vis=$(TUPLED)/$(GCVISFILE)

configapprox:
	echo $(INSTD)
	cd $(GCSRC); \
	./configure --disable-qt --prefix=$(INSTD) $(GCDISABLE) --with-vis=$(TUPLED)/$(GCVISFILE_APPROX)


exact: configure
	cd $(GCSRC) ; make -j12; make install
	echo run "sudo make install" in "$(GCSRC)"

appros: configapprox
	cd $(GCSRC) ; make -j12; make install
	echo run "sudo make install" in "$(GCSRC)"

clean:
	rm *.o *~ a.out trivial test/*


