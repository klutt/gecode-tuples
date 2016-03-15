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
FLAGS=-lgecodedriver -lgecodegist -lgecodesearch -lgecodeminimodel -lgecodeint -lgecodekernel -lgecodesupport -fdiagnostics-color=always


test/%: testsrc/%.cpp *.hh
	$(CXX) -c testsrc/$*.cpp -o test/$*.o $(INCLUDES) $(CFLAGS)
	$(CXX) -g -o test/$* test/$*.o $(FLAGS) $(LIBS) 
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
	test/3statedfawithcostintopt \
	test/3statedfanocostint 
	@echo "All tests passed"

tripletest/%:	tripletestsrc/%.cpp *.hh
	$(CXX) -c tripletestsrc/$*.cpp -o tripletest/$*.o $(INCLUDES) $(CFLAGS)
	$(CXX) -g -o tripletest/$* tripletest/$*.o $(FLAGS) $(LIBS)

triple:	tripletest/exact \
	tripletest/approx \
	tripletest/intopt \
	tripletest/int
	cp tripletestsrc/testrun.py tripletestsrc/comparesolutions.py tripletest
	cd tripletest; \
	python testrun.py
	@echo "All tests passed"

multisteptest/%:	multisteptestsrc/%.cpp *.hh multisteptestsrc/_main.cpp
	$(CXX) -c multisteptestsrc/$*.cpp -o multisteptest/$*.o $(INCLUDES) $(CFLAGS)
	$(CXX) -g -o multisteptest/$* multisteptest/$*.o $(FLAGS)  

multisteptest: \
	multisteptest/exact \
	multisteptest/approx \
	multisteptest/int \
	multisteptest/intopt
	cp multisteptestsrc/mstestrun.py multisteptestsrc/comparesolutions.py multisteptest
	cd multisteptest; \
#	python mstestrun.py
	@echo "All tests passed"


gist/%:	gistsrc/%.cpp *.hh gistsrc/_main.cpp
	$(CXX) -c gistsrc/$*.cpp -o gist/$*.o $(INCLUDES) $(CFLAGS)
	$(CXX) -g -o gist/$* $(LIBS) gist/$*.o $(FLAGS) 

gist: \
	gist/test \
	gist/exact \
	gist/approx \


test:	testint testapprox testexact triple gist

performance/%:	performancesrc/%.cpp *.hh performancesrc/_main.cpp
	$(CXX) -c performancesrc/$*.cpp -o performance/$*.o $(INCLUDES) $(CFLAGS)
	$(CXX) -g -o performance/$* performance/$*.o $(FLAGS) 
#	cp tripletestsrc/testrun.py tripletestsrc/comparesolutions.py tripletest

performance: \
	performance/exact \
	performance/approx \
	performance/intopt
	cp performancesrc/run.sh performance/

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
	rm -f *.o *~ testsrc/*~ test/* exact/*~ approx/*~ gist/* \
	gist/* gistsrc/*~ \
	multisteptest/* multitestsrc/*~ \
	tripletest/* tripletestsrc/*~ \
	performance/*.o performance/exact performance/approx performance/intopt performance/int performancesrc/*~


