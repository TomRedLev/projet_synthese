#==================================================
# version 6c (25 mars 2020)
#==================================================

# compilateur
CC = gcc
#CC = g++

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
modules = g2x_utils.o g2x_tools.o g2x_capture.o g2x_draw.o g2x_geoalgo.o g2x_transfo.o g2x_polygon.o g2x_pnm.o g2x_window.o 


all : g2x clean

# compilation générique d'un module .o
%.o : $(src)%.c
	@echo "module $@"
	@$(CC) -fpic $(CFLAGS) $(inc) -c $< -o $@
	@echo "------------------------"

# comilation générique d'un exécutable 
% : $(modules) %.o
	@echo "assemblage [$^]->$@"
	$(CC) $^ $(lib) -o $@$(EXT)
	rm -f *.o 
	@echo "------------------------"

# compilation de libg2x 
g2x : $(modules)
	@echo "massemblage bibliotheque dynamique $(bin)/lib$@$(EXT).so"
	@$(CC) -fpic -shared $^ $(lib) -o $(bin)/lib$@$(EXT).so
	@echo "massemblage bibliotheque d'archive $(bin)/lib$@$(EXT).a"
	@ar r $(bin)/lib$@$(EXT).a $^
	@echo "------------------------                                "

.PHONY : clean cleanall
clean : 
	rm -f *.o 

cleanall : 
	rm -f *.o g2x_example* quadtree*
