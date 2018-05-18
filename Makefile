all: a.exe

a.exe: database.h noderbtree.h classrbtree.h classrecord.h classcommand.h templateclasslist.h hash.h
	g++ -o server main.cpp database.cpp noderbtree.cpp classrbtree.cpp classrecord.cpp classcommand.cpp hash.cpp
	
