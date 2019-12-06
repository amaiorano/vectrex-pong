# Game info output to bin header.
GAME_TITLE_1 = '         PONG'
GAME_TITLE_2 = '          BY'
GAME_TITLE_3 = '   ANTONIO MAIORANO'
GAME_YEAR = $(shell date +'%Y')
GAME_MUSIC = 0xfd0d
BIN = pong.bin
VEC = pong.vec

CPP=/usr/local/bin/m6809-unknown-none-g++
CC1PLUS=/usr/local/libexec/gcc/m6809-unknown-none/4.3.[46]/cc1plus
# CC1=/usr/local/libexec/gcc/m6809-unknown-none/4.3.[46]/cc1

CFLAGS =
CFLAGS += -O3
CFLAGS += -fomit-frame-pointer
CFLAGS += -mint8
CFLAGS += -fno-inline 
CFLAGS += -fno-gcse
CFLAGS += -fno-toplevel-reorder
# Don't emit thread-safe guards around local static variables
CFLAGS += -fno-threadsafe-statics
CFLAGS += -msoft-reg-count=0
CFLAGS += -fverbose-asm -fno-time-report -fdiagnostics-show-option
CFLAGS += -W -Wall -Wextra -Wconversion -Werror -Wno-comment -Wno-unused-parameter -Wno-return-type

CC1FLAGS = $(CFLAGS) -quiet

AS=/usr/local/bin/as6809
AFLAGS=-l -og -sy

LN=/usr/local/bin/aslink
LFLAGS= -m -u -ws -b .text=0x0 

SRCS = $(wildcard src/*.cpp)
_OBJS = $(SRCS:.cpp=.o)
OBJS = $(patsubst src/%, %, $(_OBJS))
DEPS = $(OBJS:.o=.d)
MAP = $(BIN:.bin=.map)

.PHONY = all clean print_stats
.PRECIOUS: %.o 

all: $(BIN) $(VEC) print_stats

clean:
	$(RM) $(OBJS) *.o *.map *.hlr *.ram *.rom *.rst *.s *.s19 *.sym *.asm *.lst *.bin *.d *.vec

print_stats: $(MAP) crt0.asm
	@./tools/build/print_stats.py $(MAP) crt0.asm

# Rule to generate a dep file by using the C preprocessor
%.d: src/%.cpp
	@$(CPP) $(CFLAGS) $< -MM -MT $(@:.d=.o) > $@

# Include generated dep files for header deps per source file
-include $(DEPS)

# Produce final .vec file from .bin
%.vec: %.bin
	# Pad to multiple of 8K
	./tools/build/pad_bin.py $< $@
	# Duplicate file to 32K
	./tools/build/dup_bin.py $@ 32

# Produce .bin file from .s19 and _ram.s19
%.bin: %.s19 %_ram.s19
	# Extract ram section into .ram
	srec_cat $*_ram.s19 -offset -0xc880 -o $*.ram -binary || echo -n
	@touch $*.ram
	# Extract rom section into .rom
	srec_cat $*.s19 -o $*.rom -binary
	# Concatenate .rom and .ram into .bin
	cat $*.rom $*.ram > $*.bin

# Link all .o files into single .s19 and _ram.s19
%.s19 %_ram.s19: $(OBJS) crt0.o
	# Link .o files to .s19, _ram.s19, .rst, .map
	$(LN) $(LFLAGS) $*.s19 crt0.o $(OBJS)

# Produce .o from .asm
%.o: %.asm
	# Assemble .asm to .rel
	$(AS) $(AFLAGS) $<
	# Rename .rel to .o
	mv $*.rel $*.o

# Produce crt0.asm from crt0.tpl (template) by replacing placeholders with target base name
crt0.asm:
	cat make/crt0.tpl \
		| sed -e s/GAME_TITLE_1/$(GAME_TITLE_1)/ \
		| sed -e s/GAME_TITLE_2/$(GAME_TITLE_2)/ \
		| sed -e s/GAME_TITLE_3/$(GAME_TITLE_3)/ \
		| sed -e s/GAME_YEAR/$(GAME_YEAR)/ \
		| sed -e s/GAME_MUSIC/$(GAME_MUSIC)/ > crt0.asm

%.o: src/%.cpp
	# Compile .cpp to asm file (.s)
	$(CC1PLUS) $< -dumpbase $* $(CC1FLAGS) -auxbase $* -o $*.s
	# Assemble .s to .rel, .lst, .hlr, .sym
	$(AS) $(AFLAGS) $*.s
	# Rename .rel to .o
	mv $*.rel $*.o
