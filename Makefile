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

OBJ_CORE = $(OBJ)/core
SOURCE_CORE = $(SOURCE)/core
HEADER_CORE = $(HEADER)/core
BIN = bin
DRIVERS = drivers

ALGOS_HELPERS = algos_helpers
ALPHA_CONCEPTS = alpha_concepts
NCLUSTERS = nclusters
GHIN = ghin
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
		$(OBJ_CORE)/Timing.o
#algos helpers
ALGOS_HELPER_OBJECTS = $(OBJ)/$(ALGOS_HELPERS)/basic.o \
			$(OBJ)/$(ALGOS_HELPERS)/topk.o
#alpha concepts
ALPHA_CONCEPTS_OBJECTS = $(OBJ)/$(ALPHA_CONCEPTS)/consistency.o \
			$(OBJ)/$(ALPHA_CONCEPTS)/dispersion.o \
			$(OBJ)/$(ALPHA_CONCEPTS)/basic_prefix_alpha.o \
			$(OBJ)/$(ALPHA_CONCEPTS)/alpha_concepts_algos.o \
			$(OBJ_CORE)/RLatticeOps.o \
			$(OBJ)/$(ALPHA_CONCEPTS)/helpers.o
#nclusters
NCLUSTERS_OBJECTS = $(OBJ)/$(NCLUSTERS)/Berry.o
#ghin
GHIN_OBJECTS = $(OBJ)/$(GHIN)/Framework.o
#driver make programs
NCLU_OBJ	=	$(OBJ)/$(NCLUSTERS)/nclu.o
QBBC_OBJ =	$(OBJ)/$(ALPHA_CONCEPTS)/qbbc.o
STAR_CHARM_OBJ = $(OBJ)/$(ALPHA_CONCEPTS)/starcharm.o
GHIN_OBJ = $(OBJ)/$(GHIN)/ghin.o

NCLU_TARGET = $(BIN)/nclu
QBBC_TARGET = $(BIN)/qbbc
STAR_CHARM_TARGET = $(BIN)/starcharm
GHIN_TARGET = $(BIN)/ghin
#targets
.cpp.o:
	$(CC) -c $(CFLAGS) -o $@ $<

nclu: $(MAIN_OBJECTS) $(ALGOS_HELPER_OBJECTS) $(NCLUSTERS_OBJECTS) $(NCLU_OBJ)
		$(LINK) $(LFLAGS) -o $(NCLU_TARGET) $(MAIN_OBJECTS) $(ALGOS_HELPER_OBJECTS) $(NCLUSTERS_OBJECTS) $(NCLU_OBJ) $(LIBS)

qbbc:  $(MAIN_OBJECTS) $(ALGOS_HELPER_OBJECTS) $(ALPHA_CONCEPTS_OBJECTS) $(QBBC_OBJ)
		$(LINK) $(LFLAGS) -o $(QBBC_TARGET) $(MAIN_OBJECTS) $(ALGOS_HELPER_OBJECTS) $(ALPHA_CONCEPTS_OBJECTS) $(QBBC_OBJ) $(LIBS)

starcharm:  $(MAIN_OBJECTS) $(ALGOS_HELPER_OBJECTS) $(ALPHA_CONCEPTS_OBJECTS) $(STAR_CHARM_OBJ)
		$(LINK) $(LFLAGS) -o $(STAR_CHARM_TARGET) $(MAIN_OBJECTS) $(ALGOS_HELPER_OBJECTS) $(ALPHA_CONCEPTS_OBJECTS) $(STAR_CHARM_OBJ) $(LIBS)
ghin:  $(MAIN_OBJECTS) $(ALGOS_HELPER_OBJECTS) $(GHIN_OBJECTS) $(GHIN_OBJ)
		$(LINK) $(LFLAGS) -o $(GHIN_TARGET) $(MAIN_OBJECTS) $(ALGOS_HELPER_OBJECTS) $(GHIN_OBJECTS) $(GHIN_OBJ) $(LIBS)

#install and setup scripts
install:
		mkdir -p $(OBJ) 
		mkdir -p $(OBJ_CORE)
		mkdir -p $(SOURCE_CORE)
		mkdir -p $(HEADER_CORE)
		mkdir -p $(BIN)
qbbc_install:
		mkdir -p $(OBJ)/$(ALPHA_CONCEPTS) 
		mkdir -p $(OBJ)/$(ALGOS_HELPERS)

starcharm_install:
		mkdir -p $(OBJ)/$(ALPHA_CONCEPTS)
		mkdir -p $(OBJ)/$(ALGOS_HELPERS)

nclu_install:
		mkdir -p $(OBJ)/$(NCLUSTERS)
		mkdir -p $(OBJ)/$(ALGOS_HELPERS)
ghin_install:
		mkdir -p $(OBJ)/$(GHIN)
		mkdir -p $(OBJ)/$(ALGOS_HELPERS)
clean:
		rm -rf $(OBJ)
		rm -rf $(BIN)
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
$(OBJ)/$(ALPHA_CONCEPTS)/basic_prefix_alpha.o: $(SOURCE)/$(ALPHA_CONCEPTS)/basic_prefix_alpha.cpp
		$(CC) $(CFLAGS) -c  $(SOURCE)/$(ALPHA_CONCEPTS)/basic_prefix_alpha.cpp -o $@
$(OBJ)/$(ALPHA_CONCEPTS)/alpha_concepts_algos.o: $(SOURCE)/$(ALPHA_CONCEPTS)/alpha_concepts_algos.cpp
		$(CC) $(CFLAGS) -c  $(SOURCE)/$(ALPHA_CONCEPTS)/alpha_concepts_algos.cpp -o $@
$(OBJ)/$(ALPHA_CONCEPTS)/qbbc.o: $(SOURCE)/$(DRIVERS)/qbbc.cpp
		$(CC) $(CFLAGS) -c $(SOURCE)/$(DRIVERS)/qbbc.cpp -o $@	  
#starcharm algorithm
$(OBJ)/$(ALPHA_CONCEPTS)/starcharm.o: $(SOURCE)/$(DRIVERS)/star_charm.cpp
		$(CC) $(CFLAGS) -c $(SOURCE)/$(DRIVERS)/star_charm.cpp -o $@
#ghin algorithm
$(OBJ)/$(GHIN)/Framework.o: $(SOURCE)/$(GHIN)/Framework.cpp
		$(CC) $(CFLAGS) -c  $(SOURCE)/$(GHIN)/Framework.cpp -o $@
$(OBJ)/$(GHIN)/ghin.o: $(SOURCE)/$(DRIVERS)/ghin.cpp
		$(CC) $(CFLAGS) -c $(SOURCE)/$(DRIVERS)/ghin.cpp -o $@