client: ApplicationClass.o InputClass.o Main.o OpenglClass.o SystemClass.o
	g++ -o client ApplicationClass.o InputClass.o Main.o OpenglClass.o SystemClass.o -l GL -l X11

ApplicationClass.o: ApplicationClass.cpp
	g++ -c ApplicationClass.cpp

InputClass.o: InputClass.cpp
	g++ -c InputClass.cpp

Main.o: Main.cpp
	g++ -c Main.cpp

OpenglClass.o: OpenglClass.cpp
	g++ -c OpenglClass.cpp

SystemClass.o: SystemClass.cpp
	g++ -c SystemClass.cpp