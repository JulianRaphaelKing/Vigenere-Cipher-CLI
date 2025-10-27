# ---- Project ----
TARGET := vigenere
SRC    := vigenere.c

# Compiler & flags
CC       ?= cc
CFLAGS   ?= -Wall -Wextra -Wpedantic -O2
DBGFLAGS ?= -O0 -g

# Defaults (override at the command line)
PLAIN ?= test_plain.txt
DEC   ?= test_decrypted.txt


# OS tweaks (works on mac/linux; also ok under windows MSYS2/WSL)
ifeq ($(OS),Windows_NT)
  EXE := .exe
  RM  := del /Q
  RUN := .\$(TARGET)$(EXE)
else
  EXE :=
  RM  := rm -f
  RUN := ./$(TARGET)
endif

BIN := $(TARGET)$(EXE)

# build
.PHONY: all
all: $(BIN)

$(BIN): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(BIN)

# run
.PHONY: run
run: $(BIN)
	$(RUN)

# debug build
.PHONY: debug
debug:
	$(CC) $(CFLAGS) $(DBGFLAGS) $(SRC) -o $(BIN)

# clean
.PHONY: clean
clean:
	-$(RM) $(BIN)

# rebuild
.PHONY: rebuild
rebuild: clean all

# sample plaintext file
.PHONY: sample
sample:
	@echo Attack at dawn! >  test_plain.txt
	@echo Keep this between us. >> test_plain.txt
	@echo The package leaves at 10:30 AM. >> test_plain.txt
	@echo Numbers like 123 stay as-is. >> test_plain.txt
	@echo Zig-zag lines and CAPITALS remain untouched. >> test_plain.txt

# compare original vs decrypted (posix cmp)
.PHONY: compare
compare:
	@cmp -s "$(PLAIN)" "$(DEC)" && \
	  echo "OK: '$(DEC)' matches '$(PLAIN)' " || \
	  (echo "Mismatch between '$(PLAIN)' and '$(DEC)' " && exit 1)

