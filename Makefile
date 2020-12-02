.PHONY: uninstall clean reinstall

SERVER_SRCS := $(wildcard build/server/src/*.c)
CLIENT_SRCS := $(wildcard build/client/src/*.c)
LIBS_SRCS := $(wildcard build/libs/**/*.c)

server:
	@clang -std=c11 -Wall -Wextra -Werror -Wpedantic ${SERVER_SRCS} ${LIBS_SRCS} -o server

client:
	@clang -std=c11 -Wall -Wextra -Werror -Wpedantic ${CLIENT_SRCS} ${LIBS_SRCS} -o client

uninstall: clean
	@rm -f server
	@rm -f client

clean:
	@rm -rf server_data

reinstall: uninstall server client

