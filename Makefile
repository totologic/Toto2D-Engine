main : main.o
	g++ -L/opt/vc/lib -lGLESv2 -lEGL -lopenmaxil -lbcm_host -lvcos -lvchiq_arm -lpthread -lrt -L../libs/ilclient -o main main.o

main.o : main.cpp
	g++ -c -I/opt/vc/include -I./ -I/opt/vc/include/interface/vcos/pthreads -I/opt/vc/include/interface/vmcs_host/linux -I../libs/ilclient -I../Toto2DEngine -o main.o main.cpp

main.o : FORCE
FORCE: