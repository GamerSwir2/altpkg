CC = gcc
CFLAGS = -Wall -Wextra -O2
LDFLAGS =
SRC = altpkg.c
EXEC = altpkg
INSTALL_DIR = /usr/bin

all: $(EXEC)

$(EXEC): $(SRC)
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: install remove clean

install: $(EXEC)
	install -m 755 $< $(INSTALL_DIR)

uninstall:
	rm -f $(INSTALL_DIR)/$(EXEC)

clean:
	rm -f $(EXEC)

