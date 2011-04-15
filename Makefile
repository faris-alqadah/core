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

# objects

OBJECTS = 	$(OBJ)/IOSet.o \
		$(OBJ)/NameMap.o \
		$(OBJ)/NCluster.o \
		$(OBJ)/PreProcess.o \
		$(OBJ)/Ops.o \
		$(OBJ)/Context.o \
		$(OBJ)/RelationGraph.o \
		$(OBJ)/LatticeOps.o \
		$(OBJ)/LatticeAlgos.o \
		$(OBJ)/basic.o \
		$(OBJ)/topk.o \
		$(OBJ)/QualityMeasures.o \
		$(OBJ)/Berry.o \
		$(OBJ)/RSet.o \
		$(OBJ)/NRCluster.o \
		$(OBJ)/RContext.o \
		$(OBJ)/OpsR.o \
		$(OBJ)/consistency.o \
		$(OBJ)/dispersion.o \
		$(OBJ)/Timing.o 


#driver make programs
NCLUOBJ	=	$(OBJ)/nclu.o
QBBCOBJ =	$(OBJ)/qbbc.o

NCLUTARGET = $(BIN)/nclu
QBBCTARGET = $(BIN)/qbbc

.cpp.o:
	$(CC) -c $(CFLAGS) -o $@ $<


nclu: $(OBJECTS) $(NCLUOBJ)
	$(LINK) $(LFLAGS) -o $(NCLUTARGET) $(OBJECTS) $(NCLUOBJ) $(LIBS)

qbbc: $(OBJECTS) $(QBBCOBJ)
	$(LINK) $(LFLAGS) -o $(QBBCTARGET) $(OBJECTS) $(QBBCOBJ) $(LIBS)
clean:
	-rm -f $(OBJECTS) $(TARGET)






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

# ncluster algorithms
$(OBJ)/Berry.o: $(SOURCE)/nclusters/berry.cpp
	 $(CC) $(CFLAGS) -c  $(SOURCE)/nclusters/berry.cpp -o $@

# algorithm helpers
$(OBJ)/basic.o: $(SOURCE)/algos_helpers/basic.cpp
	 $(CC) $(CFLAGS) -c  $(SOURCE)/algos_helpers/basic.cpp -o $@
$(OBJ)/topk.o: $(SOURCE)/algos_helpers/topk.cpp
	 $(CC) $(CFLAGS) -c  $(SOURCE)/algos_helpers/topk.cpp -o $@

#alpha concepts
$(OBJ)/consistency.o: $(SOURCE)/alpha_concepts/consistency.cpp
	 $(CC) $(CFLAGS) -c  $(SOURCE)/alpha_concepts/consistency.cpp -o $@
$(OBJ)/dispersion.o: $(SOURCE)/alpha_concepts/dispersion.cpp
	 $(CC) $(CFLAGS) -c  $(SOURCE)/alpha_concepts/dispersion.cpp -o $@


# drive programs
$(OBJ)/nclu.o: $(SOURCE)/$(DRIVERS)/nclu.cpp
	$(CC) $(CFLAGS) -c $(SOURCE)/$(DRIVERS)/nclu.cpp -o $@

$(OBJ)/qbbc.o: $(SOURCE)/$(DRIVERS)/qbbc.cpp
	$(CC) $(CFLAGS) -c $(SOURCE)/$(DRIVERS)/qbbc.cpp -o $@




# dependency






	



	  
