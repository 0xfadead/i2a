OBJS += src/main.o src/arghandle.o src/imgopen.o src/imgprocessing.o
OUT ?= build/main

CC ?= cc
CCLD ?= cc
WFLAGS += -Wall -Wextra -Wpedantic -Werror
CFLAGS += -O2 -Iextern/loglib/ -g3
LFLAGS += -lm

COMPILE_FLAG_OUT ?= compile_flags.txt

.SUFFIXES: .c .o

.c.o:
	@echo "Compiling $< -> $@"
	@$(CC) -c $< -o $@ $(WFLAGS) $(CFLAGS)

$(OUT): $(OBJS)
	mkdir -p build
	@echo "Linking $(OBJS) -> $(OUT)"
	@$(CCLD) $(OBJS) -o $(OUT) $(LFLAGS) -s

debug: $(OBJS)
	mkdir -p build
	@echo "Linking $(OBJS) -> $(OUT) (with debug info)"
	@$(CCLD) $(OBJS) -o $(OUT) $(LFLAGS) -g3

.PHONY: clean gen_compile_flags

clean:
	rm -f $(OBJS) $(OUT)

gen_compile_flags:
	echo $(CFLAGS) $(WFLAGS) $(LFLAGS) | sed -z 's/ /\n/g' > $(COMPILE_FLAG_OUT)
