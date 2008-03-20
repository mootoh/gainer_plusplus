CXX = c++
CPPFLAGS = -Wall -DDEBUG
CXXFLAGS = -g -O0

LIB = gainer
TARGET_LIB = lib$(LIB).dylib

LIBS = -L. -l$(LIB)

$(TARGET_LIB): gainer.o
	$(CXX) -dynamiclib -o $@ $^

gainer-led: gainer-led.o $(TARGET_LIB)
	$(CXX) -o $@ $^ $(LIBS)

gainer-button: gainer-button.o $(TARGET_LIB)
	$(CXX) -o $@ $^ $(LIBS)

gainer-cdio: gainer-cdio.o $(TARGET_LIB)
	$(CXX) -o $@ $^ $(LIBS)


clean:
	rm -f *.o $(TARGET_LIB)
