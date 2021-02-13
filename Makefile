.PHONY: uninstall clean reinstall

SERVER_SRCS := $(wildcard server/src/*.c)
UCHAT_SRCS := $(wildcard client/src/*.c)
LIBS_SRCS := $(wildcard libs/**/*.c)
UNAME := $(shell uname)

ORACLE_FLAGS = -std=c11 -Wall -Wextra -Werror -Wpedantic
GTK_FLAGS = `pkg-config --cflags --libs gtk+-3.0`
JSON_FLAGS = `pkg-config --cflags --libs json-c`

CLIENT_INC = client/inc/

all: clean install

install: uchat_server uchat

uchat_server:
	@if [ "$(UNAME)" = "Linux" ]; then \
		clang ${SERVER_SRCS} ${LIBS_SRCS} -o uchat_server -pthread -ldl -Wno-unknown-pragmas -Wformat; \
	else \
		clang $(ORACLE_FLAGS) ${SERVER_SRCS} ${LIBS_SRCS} -o uchat_server -pthread -ldl -Wno-unknown-pragmas; \
	fi

uchat:
	@if [ "$(UNAME)" = "Linux" ]; then \
		clang ${UCHAT_SRCS} ${LIBS_SRCS} $(GTK_FLAGS) $(JSON_FLAGS) -I $(CLIENT_INC) -pthread -ldl -o uchat; \
	else \
		clang $(ORACLE_FLAGS) ${UCHAT_SRCS} ${LIBS_SRCS} $(GTK_FLAGS) $(JSON_FLAGS) -I $(CLIENT_INC) -pthread -ldl -o uchat; \
	fi

uninstall: clean
	@rm -f uchat_server
	@rm -f uchat

clean:
	@rm -rf server_data
	@rm -f chats.json

reinstall: uninstall all
