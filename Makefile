CXX = c++
CPPFLAGS = -Wall
CXXFLAGS = -g -O0
#LDFLAGS = -ltermios

gainer-led: gainer-led.o gainer.o
	$(CXX) -o $@ $^ $(LDFLAGS)

clean:
	rm -f *.o
