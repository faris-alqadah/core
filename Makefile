####### Compiler, tools and options

CC	=	g++
CFLAGS	=	-O3 -g -Wno-deprecated
INCPATH	=	
LINK	=	g++
LFLAGS	=	
LIBS	=	

####### Directories

OBJ = Objs
SOURCE = Source
HEADER = Headers


OBJECTS = 	$(OBJ)/IOSet.o \
		$(OBJ)/NameMap.o \
		$(OBJ)/NCluster.o \
		$(OBJ)/PreProcess.o \
		$(OBJ)/Globals.o \
		$(OBJ)/Ops.o \
		$(OBJ)/Numerical.o \
		$(OBJ)/HashTable.o \
		$(OBJ)/Context.o \
		$(OBJ)/RelationNode.o \
		$(OBJ)/RelationGraph.o \
		$(OBJ)/Timing.o \
		$(OBJ)/SimMeasures.o \
		$(OBJ)/PostProcess.o \
		$(OBJ)/TreeCluAlg.o \
		$(OBJ)/LatticeOps.o \


MAINOBJS =	$(OBJ)/Main.o
CLUSTERCONCEPTSOBJ = $(OBJ)/ClusterConcepts.o
STARCLUOBJ = $(OBJ)/StarClu.o


TARGET	=	TreeClu
TARGET1 = ClusterConcepts
TARGET2 = StarClu

.cpp.o:
	$(CC) -c $(CFLAGS) -o $@ $<


$(TARGET): $(OBJECTS) $(MAINOBJS)
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(MAINOBJS) $(LIBS)

ClusterConcepts: $(OBJECTS) $(CLUSTERCONCEPTSOBJ)
	$(LINK) $(LFLAGS) -o $(TARGET1) $(OBJECTS) $(CLUSTERCONCEPTSOBJ) $(LIBS)


StarClu: $(OBJECTS) $(STARCLUOBJ)
	$(LINK) $(LFLAGS) -o $(TARGET2) $(OBJECTS) $(STARCLUOBJ) $(LIBS)

clean:
	-rm -f $(OBJECTS) $(TARGET)
	-rm -f $STARCLUOBJ
	-rm -f $CLUSTERCONCEPTSOBJ


$(OBJ)/NCluster.o: $(SOURCE)/NCluster.cpp
	$(CC) $(CFLAGS) -c $(SOURCE)/NCluster.cpp -o $@

$(OBJ)/PreProcess.o: $(SOURCE)/PreProcess.cpp
	$(CC) $(CFLAGS) -c $(SOURCE)/PreProcess.cpp -o $@

$(OBJ)/Main.o: $(SOURCE)/Main.cpp
	 $(CC) $(CFLAGS) -c  $(SOURCE)/Main.cpp -o $@

$(OBJ)/ClusterConcepts.o: $(SOURCE)/ClusterConcepts.cpp
	 $(CC) $(CFLAGS) -c  $(SOURCE)/ClusterConcepts.cpp -o $@

$(OBJ)/StarClu.o: $(SOURCE)/StarClu.cpp
	$(CC) $(CFLAGS) -c $(SOURCE)/StarClu.cpp -o $@

$(OBJ)/IOSet.o: $(SOURCE)/IOSet.cpp
	$(CC) $(CFLAGS) -c $(SOURCE)/IOSet.cpp -o $@

$(OBJ)/NameMap.o: $(SOURCE)/NameMap.cpp
	$(CC) $(CFLAGS) -c $(SOURCE)/NameMap.cpp -o $@

$(OBJ)/Globals.o: $(SOURCE)/Globals.cpp
	$(CC) $(CFLAGS) -c $(SOURCE)/Globals.cpp -o $@

$(OBJ)/Ops.o: $(SOURCE)/Ops.cpp
	$(CC) $(CFLAGS) -c $(SOURCE)/Ops.cpp -o $@

$(OBJ)/Numerical.o: $(SOURCE)/Numerical.cpp
	$(CC) $(CFLAGS) -c $(SOURCE)/Numerical.cpp -o $@

$(OBJ)/HashTable.o: $(SOURCE)/HashTable.cpp
	 $(CC) $(CFLAGS) -c  $(SOURCE)/HashTable.cpp -o $@

$(OBJ)/RelationNode.o: $(SOURCE)/RelationNode.cpp
	 $(CC) $(CFLAGS) -c  $(SOURCE)/RelationNode.cpp -o $@

$(OBJ)/RelationGraph.o: $(SOURCE)/RelationGraph.cpp
	 $(CC) $(CFLAGS) -c  $(SOURCE)/RelationGraph.cpp -o $@

$(OBJ)/Context.o: $(SOURCE)/Context.cpp
	 $(CC) $(CFLAGS) -c  $(SOURCE)/Context.cpp -o $@

$(OBJ)/LatticeOps.o: $(SOURCE)/LatticeOps.cpp
	 $(CC) $(CFLAGS) -c  $(SOURCE)/LatticeOps.cpp -o $@
$(OBJ)/Timing.o: $(SOURCE)/Timing.cpp
	 $(CC) $(CFLAGS) -c  $(SOURCE)/Timing.cpp -o $@

$(OBJ)/SimMeasures.o: $(SOURCE)/SimMeasures.cpp
	 $(CC) $(CFLAGS) -c  $(SOURCE)/SimMeasures.cpp -o $@

$(OBJ)/PostProcess.o: $(SOURCE)/PostProcess.cpp
	 $(CC) $(CFLAGS) -c  $(SOURCE)/PostProcess.cpp -o $@

$(OBJ)/TreeCluAlg.o: $(SOURCE)/TreeCluAlgs.cpp
	 $(CC) $(CFLAGS) -c  $(SOURCE)/TreeCluAlgs.cpp -o $@

$(HEADER)/NCluster.h: $(HEADER)/IOSet.h
$(HEADER)/PreProcess.h: $(HEADER)/Globals.h
$(HEADER)/Ops.h:  $(HEADER)/IOSet.h
$(HEADER)/Globals.h: $(HEADER)/HashTable.h
$(HEADER)/Globals.h: $(HEADER)/RelationGraph.h
$(HEADER)/RelationGraph.h: $(HEADER)/RelationNode.h
$(HEADER)/RelationNode.h: $(HEADER)/Context.h
$(HEADER)/Context.h: $(HEADER)/IOSet.h
$(HEADER)/HashTable.h: $(HEADER)/HashNode.h
$(HEADER)/HashNode.h: $(HEADER)/NCluster.h





	



	  
