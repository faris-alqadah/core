####### Compiler, tools and options

CC	=	g++
CFLAGS	=	-O3 -g -Wno-deprecated
INCPATH	=	
LINK	=	g++
LFLAGS	=	
LIBS	=	

####### Directories

OBJ = objs
SOURCE = source
HEADER = headers
BIN = bin
DRIVERS = drivers
ALGOS_HELPERS = algos_helpers
ALPHA_CONCEPTS = alpha_concepts
NCLUSTERS = nclusters
# objects
#main objects
MAIN_OBJECTS =  $(OBJ)/IOSet.o \
		$(OBJ)/NameMap.o \
		$(OBJ)/NCluster.o \
		$(OBJ)/PreProcess.o \
		$(OBJ)/Ops.o \
		$(OBJ)/Context.o \
		$(OBJ)/RelationGraph.o \
		$(OBJ)/LatticeOps.o \
		$(OBJ)/LatticeAlgos.o \
		$(OBJ)/RSet.o \
		$(OBJ)/NRCluster.o \
		$(OBJ)/RContext.o \
		$(OBJ)/OpsR.o \
		$(OBJ)/QualityMeasures.o \
		$(OBJ)/RLatticeOps.o \
		$(OBJ)/Timing.o
#algos helpers
ALGOS_HELPER_OBJECTS = $(OBJ)/$(ALGOS_HELPERS)/basic.o \
			$(OBJ)/$(ALGOS_HELPERS)/topk.o
#alpha concepts
ALPHA_CONCEPTS_OBJECTS = $(OBJ)/$(ALPHA_CONCEPTS)/consistency.o \
			$(OBJ)/$(ALPHA_CONCEPTS)/dispersion.o \
			$(OBJ)/$(ALPHA_CONCEPTS)/helpers.o
#nclusters
NCLUSTERS_OBJECTS = $(OBJ)/$(NCLUSTERS)/Berry.o

#driver make programs
NCLU_OBJ	=	$(OBJ)/$(NCLUSTERS)/nclu.o
QBBC_OBJ =	$(OBJ)/$(ALPHA_CONCEPTS)/qbbc.o

NCLU_TARGET = $(BIN)/nclu
QBBC_TARGET = $(BIN)/qbbc

#targets
.cpp.o:
	$(CC) -c $(CFLAGS) -o $@ $<

nclu: $(MAIN_OBJECTS) $(ALGOS_HELPER_OBJECTS) $(NCLUSTERS_OBJECTS) $(NCLU_OBJ)
		$(LINK) $(LFLAGS) -o $(NCLU_TARGET) $(MAIN_OBJECTS) $(ALGOS_HELPER_OBJECTS) $(NCLUSTERS_OBJECTS) $(NCLU_OBJ) $(LIBS)

qbbc:  $(MAIN_OBJECTS) $(ALGOS_HELPER_OBJECTS) $(ALPHA_CONCEPTS_OBJECTS) $(QBBC_OBJ)
		$(LINK) $(LFLAGS) -o $(QBBC_TARGET) $(MAIN_OBJECTS) $(ALGOS_HELPER_OBJECTS) $(ALPHA_CONCEPTS_OBJECTS) $(QBBC_OBJ) $(LIBS)


#install and setup scripts
install:
		mkdir $(OBJ)
		mkdir $(BIN)
qbbc_install:
		mkdir $(OBJ)/$(ALPHA_CONCEPTS) 
		mkdir $(OBJ)/$(ALGOS_HELPERS)

nclu_install:
		mkdir $(OBJ)/$(NCLUSTERS)
		mkdir $(OBJ)/$(ALGOS_HELPERS)
clean:
		rm -rf $(OBJ)/
		rm -rf $(BIN)
# main data structures and functionality

$(OBJ)/IOSet.o: $(SOURCE)/IOSet.cpp
	$(CC) $(CFLAGS) -c $(SOURCE)/IOSet.cpp -o $@
$(OBJ)/NameMap.o: $(SOURCE)/NameMap.cpp
	$(CC) $(CFLAGS) -c $(SOURCE)/NameMap.cpp -o $@
$(OBJ)/Ops.o: $(SOURCE)/Ops.cpp
	$(CC) $(CFLAGS) -c $(SOURCE)/Ops.cpp -o $@
$(OBJ)/RelationGraph.o: $(SOURCE)/RelationGraph.cpp
	 $(CC) $(CFLAGS) -c  $(SOURCE)/RelationGraph.cpp -o $@
$(OBJ)/Context.o: $(SOURCE)/Context.cpp
	 $(CC) $(CFLAGS) -c  $(SOURCE)/Context.cpp -o $@
$(OBJ)/Timing.o: $(SOURCE)/Timing.cpp
	 $(CC) $(CFLAGS) -c  $(SOURCE)/Timing.cpp -o $@
$(OBJ)/LatticeOps.o: $(SOURCE)/LatticeOps.cpp
	 $(CC) $(CFLAGS) -c  $(SOURCE)/LatticeOps.cpp -o $@
$(OBJ)/RLatticeOps.o: $(SOURCE)/RLatticeOps.cpp
	 $(CC) $(CFLAGS) -c  $(SOURCE)/RLatticeOps.cpp -o $@
$(OBJ)/LatticeAlgos.o: $(SOURCE)/LatticeAlgos.cpp
	 $(CC) $(CFLAGS) -c  $(SOURCE)/LatticeAlgos.cpp -o $@
$(OBJ)/QualityMeasures.o: $(SOURCE)/QualityMeasures.cpp
	 $(CC) $(CFLAGS) -c  $(SOURCE)/QualityMeasures.cpp -o $@
$(OBJ)/RSet.o: $(SOURCE)/RSet.cpp
	 $(CC) $(CFLAGS) -c  $(SOURCE)/RSet.cpp -o $@
$(OBJ)/NRCluster.o: $(SOURCE)/NRCluster.cpp
	 $(CC) $(CFLAGS) -c  $(SOURCE)/NRCluster.cpp -o $@
$(OBJ)/RContext.o: $(SOURCE)/RContext.cpp
	 $(CC) $(CFLAGS) -c  $(SOURCE)/RContext.cpp -o $@
$(OBJ)/OpsR.o: $(SOURCE)/OpsR.cpp
	 $(CC) $(CFLAGS) -c  $(SOURCE)/OpsR.cpp -o $@
$(OBJ)/NCluster.o: $(SOURCE)/NCluster.cpp
	$(CC) $(CFLAGS) -c $(SOURCE)/NCluster.cpp -o $@

$(OBJ)/PreProcess.o: $(SOURCE)/PreProcess.cpp
	$(CC) $(CFLAGS) -c $(SOURCE)/PreProcess.cpp -o $@

# algorithm helpers
$(OBJ)/$(ALGOS_HELPERS)/basic.o: $(SOURCE)/$(ALGOS_HELPERS)/basic.cpp
	 $(CC) $(CFLAGS) -c  $(SOURCE)/$(ALGOS_HELPERS)/basic.cpp -o $@
$(OBJ)/$(ALGOS_HELPERS)/topk.o: $(SOURCE)/$(ALGOS_HELPERS)/topk.cpp
	 $(CC) $(CFLAGS) -c  $(SOURCE)/$(ALGOS_HELPERS)/topk.cpp -o $@

#ncluster algorithms
$(OBJ)/$(NCLUSTERS)/Berry.o: $(SOURCE)/$(NCLUSTERS)/berry.cpp
		$(CC) $(CFLAGS) -c  $(SOURCE)/$(NCLUSTERS)/berry.cpp -o $@
$(OBJ)/$(NCLUSTERS)/nclu.o: $(SOURCE)/$(DRIVERS)/nclu.cpp
		$(CC) $(CFLAGS) -c $(SOURCE)/$(DRIVERS)/nclu.cpp -o $@


#qbbc algorithms
$(OBJ)/$(ALPHA_CONCEPTS)/consistency.o: $(SOURCE)/$(ALPHA_CONCEPTS)/consistency.cpp
		$(CC) $(CFLAGS) -c  $(SOURCE)/$(ALPHA_CONCEPTS)/consistency.cpp -o $@
$(OBJ)/$(ALPHA_CONCEPTS)/dispersion.o: $(SOURCE)/$(ALPHA_CONCEPTS)/dispersion.cpp
		$(CC) $(CFLAGS) -c  $(SOURCE)/$(ALPHA_CONCEPTS)/dispersion.cpp -o $@
$(OBJ)/$(ALPHA_CONCEPTS)/helpers.o: $(SOURCE)/$(ALPHA_CONCEPTS)/helpers.cpp
		$(CC) $(CFLAGS) -c  $(SOURCE)/$(ALPHA_CONCEPTS)/helpers.cpp -o $@
$(OBJ)/$(ALPHA_CONCEPTS)/qbbc.o: $(SOURCE)/$(DRIVERS)/qbbc.cpp
		$(CC) $(CFLAGS) -c $(SOURCE)/$(DRIVERS)/qbbc.cpp -o $@
	  
