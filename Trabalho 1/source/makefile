GOOGLE_TEST_LIB = gtest
GOOGLE_TEST_INCLUDE = /usr/local/include

IDIR =../include
CC=gcc
CCPLUS=g++	
CFLAGS=-I $(IDIR) -g
G_FLAGS=-I $(IDIR) -I $(GOOGLE_TEST_INCLUDE) -g -ftest-coverage -fprofile-arcs
LD_FLAGS=-I $(IDIR) -L /usr/local/lib -l $(GOOGLE_TEST_LIB) -l pthread -g -ftest-coverage -fprofile-arcs

ODIR=obj
LDIR =../lib

LIBS=-lm

_DEPS = indice_primario.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o indice_primario.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

_OBJT = tests.o indice_primario.o
OBJT = $(patsubst %,$(ODIR)/%,$(_OBJT))

all: indice_primario testa_ind_prim

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CCPLUS) -c -o $@ $< $(G_FLAGS)

indice_primario: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

testa_ind_prim: $(OBJT)
	$(CCPLUS) -o $@ $^ $(LD_FLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
