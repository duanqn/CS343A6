CXX = u++					# compiler
CXXFLAGS = -g -Wall -MMD -std=c++11      # compiler flags
OPT:=-O2 -multi
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}	# makefile name

ifneq (${TEST},)
  MAINOBJ = test/driver${TEST}.o
else
  MAINOBJ = driver.o # the driver object
endif

OBJECTS2 = ${MAINOBJ} # list of object files
EXEC2 = soda

OBJECTS = ${OBJECTS2}				# all object files
DEPENDS = ${OBJECTS:.o=.d}			# substitute ".o" with ".d"
EXECS = ${EXEC2}				# all executables

#############################################################

.PHONY : all clean

all : ${EXECS}					# build all executables

ifneq (${TEST},)
.PHONY : test
test : ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OPT} $^ -o $@${TEST}
else
.PHONY : ${EXEC2}
${EXEC2} : ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OPT} $^ -o $@ 
endif

#############################################################

${OBJECTS} : ${MAKEFILE_NAME}			# OPTIONAL : changes to this file => recompile

-include ${DEPENDS}				# include *.d files containing program dependences

clean :						# remove files that can be regenerated
	rm -f *.d *.o ${EXECS}
