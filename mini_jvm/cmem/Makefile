CFLAGS+=-ansi -DWITH_DBG_STRDUP
OBJ=memleak.o example.o
BIN=example

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	$(RM) $(OBJ) $(BIN)
