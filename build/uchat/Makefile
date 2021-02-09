NAME		=	client

GTKFLAGS	=	`pkg-config --cflags --libs gtk+-3.0`
JSONFLAGS	=	`pkg-config --cflags --libs json-c`
ORACLE 		= 	-std=c11 -Wall -Wextra -Werror -Wpedantic

CFLAGS		=	$(GTKFLAGS) $(JSONFLAGS) $(ORACLE)

INC 		=	inc/*.h
SRC 		= 	src/*.c

all: install clean

install:
	@cp $(SRC) .
	@cp $(INC) .
	@clang $(CFLAGS) -o $(NAME) *.c

clean:
	@rm -rf *.h
	@rm -rf *.c

uninstall: clean
	@rm -rf $(NAME)
	
reinstall:
	@make uninstall
	@make
