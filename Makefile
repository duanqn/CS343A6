CXX = u++					# compiler
CXXFLAGS = -g -Wall -MMD -std=c++14      # compiler flags
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}	# makefile name

ifneq (${TEST},)
  MAINOBJ = test/driver${TEST}.o Printer.o
  CXXFLAGS += -DDEBUG
  OBJECTS2 = ${TEST}.o ${MAINOBJ} # list of object files
else
  MAINOBJ = driver.o # the driver object
  OBJECTS2 = Truck.o ${MAINOBJ} # list of object files <ADD NEW FILE NAMES HERE>
  OPT:=-O2 -multi
endif

EXEC2 = soda

OBJECTS = ${OBJECTS2}				# all object files
DEPENDS = ${OBJECTS:.o=.d}			# substitute ".o" with ".d"
EXECS = ${EXEC2}				# all executables

#############################################################

.PHONY : all clean

all : ${EXECS}					# build all executables

.PHONY : ${EXEC2}
${EXEC2} : ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OPT} $^ -o $@${TEST} 

#############################################################

${OBJECTS} : ${MAKEFILE_NAME}			# OPTIONAL : changes to this file => recompile

-include ${DEPENDS}				# include *.d files containing program dependences

clean :						# remove files that can be regenerated
	rm -f *.d *.o test/*.d test/*.o ${EXECS}
	rm soda*
