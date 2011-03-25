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
		$(OBJ)/Timing.o 


MAINOBJS =	$(OBJ)/Main.o
TARGET	=	nclu


.cpp.o:
	$(CC) -c $(CFLAGS) -o $@ $<


$(TARGET): $(OBJECTS) $(MAINOBJS)
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(MAINOBJS) $(LIBS)



clean:
	-rm -f $(OBJECTS) $(TARGET)


$(OBJ)/NCluster.o: $(SOURCE)/NCluster.cpp
	$(CC) $(CFLAGS) -c $(SOURCE)/NCluster.cpp -o $@

$(OBJ)/PreProcess.o: $(SOURCE)/PreProcess.cpp
	$(CC) $(CFLAGS) -c $(SOURCE)/PreProcess.cpp -o $@

$(OBJ)/Main.o: $(SOURCE)/Main.cpp
	$(CC) $(CFLAGS) -c $(SOURCE)/Main.cpp -o $@

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
$(OBJ)/Berry.o: $(SOURCE)/nclusters/berry.cpp
	 $(CC) $(CFLAGS) -c  $(SOURCE)/nclusters/berry.cpp -o $@

$(OBJ)/basic.o: $(SOURCE)/algos_helpers/basic.cpp
	 $(CC) $(CFLAGS) -c  $(SOURCE)/algos_helpers/basic.cpp -o $@
$(OBJ)/topk.o: $(SOURCE)/algos_helpers/topk.cpp
	 $(CC) $(CFLAGS) -c  $(SOURCE)/algos_helpers/topk.cpp -o $@
$(OBJ)/QualityMeasures.o: $(SOURCE)/QualityMeasures.cpp
	 $(CC) $(CFLAGS) -c  $(SOURCE)/QualityMeasures.cpp -o $@

$(HEADER)/NCluster.h: $(HEADER)/IOSet.h
$(HEADER)/Ops.h:  $(HEADER)/IOSet.h
$(HEADER)/Context.h: $(HEADER)/IOSet.h
$(HEADER)/LatticeOps.h: $(HEADER)/RelationGraph.h





	



	  
