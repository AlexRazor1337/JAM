.PHONY: uninstall clean reinstall

SERVER_SRCS := $(wildcard server/src/*.c)
CLIENT_SRCS := $(wildcard client/src/*.c)
UCHAT_SRCS := $(wildcard uchat_gui/src/*.c)
LIBS_SRCS := $(wildcard libs/**/*.c)
UNAME := $(shell uname)

all: clean install  

install: uchat_server test_client uchat

uchat_server:
	@if [ "$(UNAME)" = "Linux" ]; then \
		clang -std=c11 -Wall -Wextra -Werror -Wpedantic ${SERVER_SRCS} ${LIBS_SRCS} -o uchat_server -pthread -ldl -Wno-unknown-pragmas -Wformat; \
	else \
		clang -std=c11 -Wall -Wextra -Werror -Wpedantic ${SERVER_SRCS} ${LIBS_SRCS} -o uchat_server -pthread -ldl -Wno-unknown-pragmas; \
	fi

test_client:
	@clang -std=c11 -Wall -Wextra -Werror -Wpedantic ${CLIENT_SRCS} ${LIBS_SRCS} -o test_client -pthread -ldl

uchat:
	@clang -std=c11 -Wall -Wextra -Werror -Wpedantic ${UCHAT_SRCS} ${LIBS_SRCS} `pkg-config --cflags --libs gtk+-3.0` `pkg-config --cflags --libs json-c` -I uchat_gui/inc/ -o uchat

uninstall: clean
	@rm -f uchat_server
	@rm -f test_client
	@rm -f uchat

clean:
	@rm -rf server_data

reinstall: uninstall all
