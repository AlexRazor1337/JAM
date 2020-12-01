.PHONY: uninstall clean reinstall

SERVER_SRCS := $(wildcard build/server/src/*.c)
LIBS_SRCS := $(wildcard build/libs/**/*.c)

server:
	@clang -std=c11 -Wall -Wextra -Werror -Wpedantic ${SERVER_SRCS} ${LIBS_SRCS} -o server

uninstall: clean
	@rm -f server

clean:
	@rm -rf server_data

reinstall: uninstall server

