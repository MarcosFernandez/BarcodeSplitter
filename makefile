CXX=g++
RM=rm -f
CPPFLAGS=-O3 -Wall -c -fmessage-length=0

SRCS=main.cpp support.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

all: BarcodeSplitter

BarcodeSplitter: $(OBJS)
	$(CXX) -o BarcodeSplitter $(OBJS)  

main.o: main.cpp support.h

support.o: support.h support.cpp

clean:
	$(RM) $(OBJS) BarcodeSplitter

dist-clean: clean
	$(RM) BarcodeSplitter


