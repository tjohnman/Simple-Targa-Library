SRC = src/simpleTGA.cpp
OBJ = $(SRC:.cpp=.o)
OUT = libsimpletarga.a

INCLUDES = -I ./include/
CCFLAGs = -O2
CCC = g++
LIBS = 
LDFLAGS = 
.SUFFIXES= .cpp
default: dep $(OUT)

.cpp.o:
	$(CCC) $(INCLUDES) $(CCFLAGS) -c $< -o $@

$(OUT): $(OBJ)
	ar rcs $(OUT) $(OBJ)

depend: dep

dep:
	makedepend -- $(CFLAGS) -- $(INCLUDES) $(SRC)

clean:
	rm -f $(OBJ) $(OUT) Makefile.bak

# DO NOT DELETE

src/simpleTGA.o: ./include/simpleTGA.h
