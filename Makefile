IDIR = include
SDIR = src
CC=gcc
CFLAGS=-I$(IDIR)

DIRS=build
$(info $(shell mkdir -p $(DIRS)))

all: $(DIRS)/server

ODIR=src/obj

_SRC = http.c doublylinkedlist.c htable.c cache.c server.c 
SRC = $(patsubst %,$(SDIR)/%,$(_DEPS))

_DEPS = http.h doublylinkedlist.h htable.h cache.h server.h 
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = http.o doublylinkedlist.o htable.o cache.o server.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: $(SRC) $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(DIRS)/server: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) 

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
