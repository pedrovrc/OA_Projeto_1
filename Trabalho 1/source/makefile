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

all: index

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

index: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
