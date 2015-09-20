GCDISABLE=--disable-examples --disable-thread --disable-float-vars --disable-set-vars
GCVISFILE_EXACT=exact/intpair.vis
GCVISFILE_APPROX=approx/intpairapprox.vis

TUPLED=$(CURDIR)
#INSTD=$(patsubst %src,%external/gecode, $(TUPLED))
INSTD=/usr/local
GCSRC=$(patsubst %src,%external/gecode-4.3.2, $(TUPLED))

LD_LIBRARY_PATH=/usr/local/lib

CXX=g++
LIBS=-L$(INSTD)/lib
INCLUDES=-I$(INSTD)/include

#FLAGS=-lgecodedriver -lgecodegist -lgecodesearch -lgecodeminimodel -lgecodeint -lgecodekernel -lgecodesupport
FLAGS=-lgecodedriver -lgecodesearch -lgecodeminimodel -lgecodeint -lgecodekernel -lgecodesupport -lgecodegist


test/%: testsrc/%.cpp *.hh
	/usr/bin/g++ -g -c testsrc/$*.cpp -o test/$*.o $(INCLUDES) -fdiagnostics-color=always -std=c++11 -fmax-errors=5
	/usr/bin/g++ -g -o test/$* test/$*.o $(FLAGS) $(LIBS) -fdiagnostics-color=always
	test/$* -threads 1

testapprox:	\
	test/xeqapprox \
	test/simpleintpairapprox \
	test/nqapprox \
	test/arraybranchapprox \
	test/simpledfaapprox \
	test/3statedfanocostapprox \
	test/3statedfawithcostapprox 
	@echo "All tests passed"


test:	test/3statedfawithcostint \
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

tripletest/exact: tripletest/exact.cpp
	/usr/bin/g++ -g -c tripletest/exact.cpp -o tripletest/exact.o $(INCLUDES) -fdiagnostics-color=always -std=c++11 -fmax-errors=5
	/usr/bin/g++ -g -o tripletest/exact tripletest/exact.o $(FLAGS) $(LIBS) -fdiagnostics-color=always

tripletest/approx: tripletest/approx.cpp
	/usr/bin/g++ -g -c tripletest/approx.cpp -o tripletest/approx.o $(INCLUDES) -fdiagnostics-color=always -std=c++11 -fmax-errors=5
	/usr/bin/g++ -g -o tripletest/approx tripletest/approx.o $(FLAGS) $(LIBS) -fdiagnostics-color=always


tripletest/int: tripletest/int.cpp
	/usr/bin/g++ -g -c tripletest/int.cpp -o tripletest/int.o $(INCLUDES) -fdiagnostics-color=always -std=c++11 -fmax-errors=5
	/usr/bin/g++ -g -o tripletest/int tripletest/int.o $(FLAGS) $(LIBS) -fdiagnostics-color=always

triple:	tripletest/exact \
	tripletest/approx \
	tripletest/int

performance/exact: performance/exact.cpp
	/usr/bin/g++ -g -c performance/exact.cpp -o performance/exact.o $(INCLUDES) -fdiagnostics-color=always -std=c++11 -fmax-errors=5
	/usr/bin/g++ -g -o performance/exact performance/exact.o $(FLAGS) $(LIBS) -fdiagnostics-color=always

performance/approx: performance/approx.cpp
	/usr/bin/g++ -g -c performance/approx.cpp -o performance/approx.o $(INCLUDES) -fdiagnostics-color=always -std=c++11 -fmax-errors=5
	/usr/bin/g++ -g -o performance/approx performance/approx.o $(FLAGS) $(LIBS) -fdiagnostics-color=always


performance/int: performance/int.cpp
	/usr/bin/g++ -g -c performance/int.cpp -o performance/int.o $(INCLUDES) -fdiagnostics-color=always -std=c++11 -fmax-errors=5
	/usr/bin/g++ -g -o performance/int performance/int.o $(FLAGS) $(LIBS) -fdiagnostics-color=always

performance: \
	performance/exact \
	performance/approx \
	performance/int

configure:
	echo $(INSTD)
	cd $(GCSRC); \
	./configure --disable-qt --prefix=$(INSTD) $(GCDISABLE) --with-vis=$(TUPLED)/$(GCVISFILE_EXACT)

configapprox:
	echo $(INSTD)
	cd $(GCSRC); \
	./configure --disable-qt --prefix=$(INSTD) $(GCDISABLE) --with-vis=$(TUPLED)/$(GCVISFILE_APPROX)

configboth:
	echo $(INSTD)
	cd $(GCSRC); \
	./configure --disable-qt --prefix=$(INSTD) $(GCDISABLE) --with-vis=$(TUPLED)/$(GCVISFILE_APPROX),$(TUPLED)/$(GCVISFILE_EXACT)

exact: configure
	cd $(GCSRC) ; make -j12; make install
	echo run "sudo make install" in "$(GCSRC)"

approx: configapprox
	cd $(GCSRC) ; make -j12; make install
	echo run "sudo make install" in "$(GCSRC)"

both:	configboth
	cd $(GCSRC) ; make -j12; make install
	echo run "sudo make install" in "$(GCSRC)"


clean:
	rm -f *.o *~ testsrc/*~ test/* exact/*~ approx/*~ \
	performance/approx performance/int performance/exact performance/*.o performance/*~ \
	tripletest/approx tripletest/int tripletest/exact tripletest/*.o tripletest/*~ 


