PROJECT =  mysh
HEADERS =  $(PROJECT).h
OBJ     =  mysh.o  execute.o globals.o parser.o


C99     =  cc -std=c99
CFLAGS  =  -Wall -pedantic -Werror 


$(PROJECT) : $(OBJ)
	$(C99) $(CFLAGS) -o $(PROJECT) $(OBJ) -lm


%.o : %.c $(HEADERS)
	$(C99) $(CFLAGS) -c $<

clean:
	rm -f $(PROJECT) $(OBJ)

