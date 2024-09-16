#######################################
# Makefile render				      #
#                                     #
#                                     #
#######################################


PROG = render

all : $(PROG)

# Variables for file compilation
CC        =  gcc
CFLAGS    =  -g -Wall
CPPFLAGS  =  -DDEBUG
LDFLAGS   =  -g -lm


render : main.o imageFormationUtils.o
	$(CC) $^ -o $@ $(LDFLAGS)
clean :
	@rm -f *.o

cleanall : clean
	@rm -f $(PROG)