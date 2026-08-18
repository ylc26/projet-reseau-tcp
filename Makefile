CC ?= gcc
CFLAGS ?= -std=c11 -Wall -Wextra -pedantic
BIN_DIR := bin
SRC_DIR := src

TARGETS := \
	$(BIN_DIR)/client_tcp \
	$(BIN_DIR)/serveur_tcp \
	$(BIN_DIR)/client_udp \
	$(BIN_DIR)/serveur_udp

.PHONY: all clean

all: $(TARGETS)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BIN_DIR)/client_tcp: $(SRC_DIR)/client_tcp.c | $(BIN_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BIN_DIR)/serveur_tcp: $(SRC_DIR)/serveur_tcp.c | $(BIN_DIR)
	$(CC) $(CFLAGS) $< -o $@ -pthread

$(BIN_DIR)/client_udp: $(SRC_DIR)/client_udp.c | $(BIN_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BIN_DIR)/serveur_udp: $(SRC_DIR)/serveur_udp.c | $(BIN_DIR)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(BIN_DIR)
