.PHONY: uninstall clean reinstall

SERVER_SRCS := $(wildcard build/server/src/*.c)
CLIENT_SRCS := $(wildcard build/client/src/*.c)
LIBS_SRCS := $(wildcard build/libs/**/*.c)
UNAME := $(shell uname)

server:
	@if [ "$(UNAME)" = "Linux" ]; then \
	clang -std=c11 -Wall -Wextra -Werror -Wpedantic ${SERVER_SRCS} ${LIBS_SRCS} -o server -pthread -ldl -Wno-unknown-pragmas -Wformat; \
	else \
	clang -std=c11 -Wall -Wextra -Werror -Wpedantic ${SERVER_SRCS} ${LIBS_SRCS} -o server -pthread -ldl -Wno-unknown-pragmas; \
	fi

client:
	clang -std=c11 -Wall -Wextra -Werror -Wpedantic ${CLIENT_SRCS} ${LIBS_SRCS} -o client -pthread -ldl

uninstall: clean
	@rm -f server
	@rm -f client

clean:
	@rm -rf server_data

reinstall: uninstall server client

