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
CFLAGS=-g -fdiagnostics-color=always -std=c++11 -fmax-errors=5
FLAGS=-lgecodedriver -lgecodesearch -lgecodeminimodel -lgecodeint -lgecodekernel -lgecodesupport -lgecodegist


test/%: testsrc/%.cpp *.hh
	/usr/bin/g++ -c testsrc/$*.cpp -o test/$*.o $(INCLUDES) $(CFLAGS)
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


testexact: \
	test/3statedfawithcost \
	test/queens01test \
	test/testtemplate \
	test/zerosolutions \
	test/simpleintpairarray \
	test/simpleintpair \
	test/simpledfa \
	test/3statedfanocost 
	@echo "All tests passed"

testint: \
	test/3statedfawithcostint \
	test/3statedfanocostint 
	@echo "All tests passed"

tripletest/%:	tripletestsrc/%.cpp *.hh
	/usr/bin/g++ -c tripletestsrc/$*.cpp -o tripletest/$*.o $(INCLUDES) $(CFLAGS)
	/usr/bin/g++ -g -o tripletest/$* tripletest/$*.o $(FLAGS) $(LIBS) -fdiagnostics-color=always
	cp tripletestsrc/testrun.py tripletestsrc/comparesolutions.py tripletest

triple:	tripletest/exact \
	tripletest/approx \
	tripletest/int
	cd tripletest; \
	python testrun.py
	@echo "All tests passed"

performance/%:	performancesrc/%.cpp *.hh
	/usr/bin/g++ -c performancesrc/$*.cpp -o performance/$*.o $(INCLUDES) $(CFLAGS)
	/usr/bin/g++ -g -o performance/$* performance/$*.o $(FLAGS) $(LIBS) -fdiagnostics-color=always
	cp performancesrc/performancetestrun.py performancesrc/comparesolutions.py performance


performance: \
	performance/exact \
	performance/approx \
	performance/int
	cd performance; \
	python performancetestrun.py
	@echo "All tests passed"

test:	testint testapprox testexact triple performance

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
	performance/* performancesrc/*~ \
	tripletest/* tripletestsrc/*~ 


