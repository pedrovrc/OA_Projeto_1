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

_DEPS = manutencao.h indice.h ordenacao.h pesquisa.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o indice.o ordenacao.o manutencao.o pesquisa.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

_OBJT = tests.o indice.o manutencao.o
OBJT = $(patsubst %,$(ODIR)/%,$(_OBJT))

all: index testa_index

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CCPLUS) -c -o $@ $< $(G_FLAGS)

index: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

testa_index: $(OBJT)
	$(CCPLUS) -o $@ $^ $(LD_FLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
