####### Compiler, tools and options

CC	=	g++
CFLAGS	=	-O3 -g -Wno-deprecated
INCPATH	=	
LINK	=	g++
LFLAGS	=	
LIBS	=	

####### Directories

SOURCES=

OBJ = objs
SOURCE = source
HEADER = headers

OBJ_CORE = $(OBJ)/core
SOURCE_CORE = $(SOURCE)/core
HEADER_CORE = $(HEADER)/core


ALGOS_HELPERS = algos_helpers
# objects
#main objects
MAIN_OBJECTS =  $(OBJ_CORE)/IOSet.o \
		$(OBJ_CORE)/NameMap.o \
		$(OBJ_CORE)/NCluster.o \
		$(OBJ_CORE)/PreProcess.o \
		$(OBJ_CORE)/Ops.o \
		$(OBJ_CORE)/Context.o \
		$(OBJ_CORE)/RelationGraph.o \
		$(OBJ_CORE)/LatticeOps.o \
		$(OBJ_CORE)/LatticeAlgos.o \
		$(OBJ_CORE)/QualityMeasures.o \
		$(OBJ_CORE)/RSet.o \
		$(OBJ_CORE)/RContext.o \
		$(OBJ_CORE)/NRCluster.o \
		$(OBJ_CORE)/OpsR.o \
		$(OBJ_CORE)/RelationGraphR.o \
		$(OBJ_CORE)/BasicStats.o \
		$(OBJ_CORE)/MiscOps.o \
		$(OBJ_CORE)/Timing.o

ALGOS_HELPER_OBJECTS = $(OBJ_CORE)/basic.o \
			$(OBJ_CORE)/topk.o



#targets
.cpp.o:
	$(CC) -c $(CFLAGS) $< -o $@

core_lib: $(MAIN_OBJECTS) $(ALGOS_HELPER_OBJECTS)
	ar cr libcorelib.a $(MAIN_OBJECTS) $(ALGOS_HELPER_OBJECTS)

#install and setup scripts
install:
		mkdir -p $(OBJ_CORE)

clean:
		rm -rf $(OBJ_CORE)
# main data structures and functionality

$(OBJ_CORE)/IOSet.o: $(SOURCE_CORE)/IOSet.cpp
	$(CC) $(CFLAGS) -c $(SOURCE_CORE)/IOSet.cpp -o $@
$(OBJ_CORE)/NameMap.o: $(SOURCE_CORE)/NameMap.cpp
	$(CC) $(CFLAGS) -c $(SOURCE_CORE)/NameMap.cpp -o $@
$(OBJ_CORE)/Ops.o: $(SOURCE_CORE)/Ops.cpp
	$(CC) $(CFLAGS) -c $(SOURCE_CORE)/Ops.cpp -o $@
$(OBJ_CORE)/RelationGraph.o: $(SOURCE_CORE)/RelationGraph.cpp
	 $(CC) $(CFLAGS) -c  $(SOURCE_CORE)/RelationGraph.cpp -o $@
$(OBJ_CORE)/RelationGraphR.o: $(SOURCE_CORE)/RelationGraphR.cpp
	 $(CC) $(CFLAGS) -c  $(SOURCE_CORE)/RelationGraphR.cpp -o $@
$(OBJ_CORE)/Context.o: $(SOURCE_CORE)/Context.cpp
	 $(CC) $(CFLAGS) -c  $(SOURCE_CORE)/Context.cpp -o $@
$(OBJ_CORE)/Timing.o: $(SOURCE_CORE)/Timing.cpp
	 $(CC) $(CFLAGS) -c  $(SOURCE_CORE)/Timing.cpp -o $@
$(OBJ_CORE)/LatticeOps.o: $(SOURCE_CORE)/LatticeOps.cpp
	 $(CC) $(CFLAGS) -c  $(SOURCE_CORE)/LatticeOps.cpp -o $@
$(OBJ_CORE)/RLatticeOps.o: $(SOURCE_CORE)/RLatticeOps.cpp
	 $(CC) $(CFLAGS) -c  $(SOURCE_CORE)/RLatticeOps.cpp -o $@
$(OBJ_CORE)/LatticeAlgos.o: $(SOURCE_CORE)/LatticeAlgos.cpp
	 $(CC) $(CFLAGS) -c  $(SOURCE_CORE)/LatticeAlgos.cpp -o $@
$(OBJ_CORE)/QualityMeasures.o: $(SOURCE_CORE)/QualityMeasures.cpp
	 $(CC) $(CFLAGS) -c  $(SOURCE_CORE)/QualityMeasures.cpp -o $@
$(OBJ_CORE)/RSet.o: $(SOURCE_CORE)/RSet.cpp
	 $(CC) $(CFLAGS) -c  $(SOURCE_CORE)/RSet.cpp -o $@
$(OBJ_CORE)/NRCluster.o: $(SOURCE_CORE)/NRCluster.cpp
	 $(CC) $(CFLAGS) -c  $(SOURCE_CORE)/NRCluster.cpp -o $@
$(OBJ_CORE)/RContext.o: $(SOURCE_CORE)/RContext.cpp
	 $(CC) $(CFLAGS) -c  $(SOURCE_CORE)/RContext.cpp -o $@
$(OBJ_CORE)/OpsR.o: $(SOURCE_CORE)/OpsR.cpp
	 $(CC) $(CFLAGS) -c  $(SOURCE_CORE)/OpsR.cpp -o $@
$(OBJ_CORE)/NCluster.o: $(SOURCE_CORE)/NCluster.cpp
	$(CC) $(CFLAGS) -c $(SOURCE_CORE)/NCluster.cpp -o $@
$(OBJ_CORE)/PreProcess.o: $(SOURCE_CORE)/PreProcess.cpp
	$(CC) $(CFLAGS) -c $(SOURCE_CORE)/PreProcess.cpp -o $@
$(OBJ_CORE)/BasicStats.o: $(SOURCE_CORE)/BasicStats.cpp
	$(CC) $(CFLAGS) -c $(SOURCE_CORE)/BasicStats.cpp -o $@
$(OBJ_CORE)/MiscOps.o: $(SOURCE_CORE)/MiscOps.cpp
	$(CC) $(CFLAGS) -c $(SOURCE_CORE)/MiscOps.cpp -o $@

# algorithm helpers
$(OBJ_CORE)/basic.o: $(SOURCE)/$(ALGOS_HELPERS)/basic.cpp
	 $(CC) $(CFLAGS) -c  $(SOURCE)/$(ALGOS_HELPERS)/basic.cpp -o $@
$(OBJ_CORE)/topk.o: $(SOURCE)/$(ALGOS_HELPERS)/topk.cpp
	 $(CC) $(CFLAGS) -c  $(SOURCE)/$(ALGOS_HELPERS)/topk.cpp -o $@




