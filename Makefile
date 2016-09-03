O_Flags		= -Wall -pedantic
objects 	= test.o BitBoard.o MoveGen.o evaluation.o search.o 
Install_Path 	= /usr/local/Michem_Engine
flags			=	Wshift-cout-overflown

Engine_M :  $(objects)
	g++ -g -o Engine_M $(objects)
search.o : search.cpp search.h typedefs.h
	g++ -g -c search.cpp
test.o	 : BitBoard.h test.cpp MoveGen.h evaluation.h search.h
	g++ -g -c test.cpp
MoveGen.o	: MoveGen.cpp MoveGen.h BitBoard.h typedefs.h 
	g++ -g -c MoveGen.cpp	
BitBoard.o	: BitBoard.cpp typedefs.h BitBoard.h
	g++ -g -c BitBoard.cpp
evaluation.o: evaluation.cpp evaluation.h BitBoard.h
	g++ -g -c evaluation.cpp
clean_objFile :
	rm -f *.o
clean : 
	rm Engine_M
help : 
	echo "I havent written this part yet :<"
