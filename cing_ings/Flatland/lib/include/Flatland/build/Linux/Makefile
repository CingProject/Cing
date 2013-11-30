# Linux Makefile for Flatland by Philip Rideout.

EXEC = flatland-demo
LIBS = -lXi -lXmu -lX11 -lm -lstdc++ -lGLU -lGL -L. -lFlatland
IFLAGS = -I ../.. -I ../../..
CXXFLAGS = $(IFLAGS) -O3
CFLAGS = $(IFLAGS) -O3
DEMO_OBJS = demo.o osLinux.o

FLATLAND_OBJS = \
	shapes.o \
	vector.o \
	flatland.o \
	intersection.o \
	circle-circle.o \
	quad-quad.o \
	quad-circle.o \
	terrain-all.o \
	composite-all.o \

ODE_OBJS = array.o joint.o mass.o matrix.o misc.o ode.o quickstep.o step.o testing.o \
	util.o error.o lcp.o mat.o memory.o obstack.o odemath.o rotation.o stepfast.o timer.o \
	fastdot.o fastldlt.o fastlsolve.o fastltsolve.o

all: complex-demo simple-demo

complex-demo: $(DEMO_OBJS) complex.o libFlatland.a
	$(CXX) -o $@ $(DEMO_OBJS) complex.o $(LIBS)

simple-demo: $(DEMO_OBJS) simple.o libFlatland.a
	$(CXX) -o $@ $(DEMO_OBJS) simple.o $(LIBS)

libFlatland.a: $(FLATLAND_OBJS) $(ODE_OBJS)
	ar rcs $@ $+

doc:
	cd ../../doc ; perl main-page.pl ; doxygen doxygen.cfg ; cp readme.html html ; cp file_list.html html; mv html doxygen

%.o: ../../tests/%.cpp
	$(CXX) -c $+ $(CXXFLAGS)

%.o: ../../tests/%.c
	$(CXX) -c $+ $(CFLAGS)

%.o: ../../source/%.cpp
	$(CXX) -c $+ $(CXXFLAGS)

%.o: ../../ode/source/%.c
	$(CXX) -c $+ $(CFLAGS)

%.o: ../../ode/source/%.cpp
	$(CXX) -c $+ $(CXXFLAGS)

clean:
	-rm -f $(OBJS) core *~ ../../tests/*~ *.o *~ ../../ode/*~ ../../flatland/*~ ../../source/*~

release: clobber
	-rm -f ../../../flatland.tar.gz
	-rm -f ../../../flatland.tar
	tar -cvf ../../../flatland.tar \
		../../../flatland/*.hpp \
		../../../flatland/codegen \
		../../../flatland/build \
		../../../flatland/ode \
		../../../flatland/source \
		../../../flatland/tests
	tar --delete --file=../../../flatland.tar flatland/build/VisualStudio/clean.bat
	gzip ../../../flatland.tar

clobber: clean
	-rm -f complex-demo
	-rm -f simple-demo
	-rm -f libFlatland.a
