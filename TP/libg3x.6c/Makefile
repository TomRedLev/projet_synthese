#==================================================
# version 6c (25 mars 2020)
#==================================================

# compilateur
#CC = gcc
CC = g++

# extension ++ si compil g++
ifeq ($(CC),g++)
  C = ++
else
  C = 
endif

# extension .dbg si DEBUG = yes (compil option -g)
DEBUG = no
ifeq ($(DEBUG),yes)
  CFLAGS  = -O2 -g -Wpointer-arith -Wall -ansi
  D = dbg
else
  CFLAGS = -O2 -ansi
  D = 
endif

EXT = .$(VERSION).$(PROCBIT)$(C)$(D)

lib = -L/usr/lib -lm -lGL -lGLU -lglut
inc = -I/usr/include -I./include
src =  src/

# cible pour les binaires (.a,.so) 
bin = .

# liste des modules
modules = g3x_utils.o g3x_window.o g3x_tools.o g3x_capture.o g3x_transfo.o g3x_pnm.o
#g3x_GLtransfo.o g3x_quaternions.o 

all : g3x clean

# compilation générique d'un module %.o à partir de src/%.c
%.o : $(src)%.c
	@echo "module $@"
	@$(CC) -fpic $(CFLAGS) $(inc) -c $< -o $@
	@echo "------------------------"

# comilation générique d'un exécutable - édition de liens
% : $(modules) %.o
	@echo "assemblage [$^]->$@"
	$(CC) $^ $(lib) -o $@$(EXT)
	rm -f *.o 
	@echo "------------------------"

# compilation de libg3x 
g3x : $(modules)
	@echo "massemblage bibliotheque dynamique $(bin)/lib$@$(EXT).so"
	@$(CC) -fpic -shared $^ $(lib) -o $(bin)/lib$@$(EXT).so
	@echo "massemblage bibliotheque d'archive $(bin)/lib$@$(EXT).a"
	@ar r $(bin)/lib$@$(EXT).a $^
	@echo "------------------------                                "

.PHONY : clean
clean : 
	rm -f *.o 
