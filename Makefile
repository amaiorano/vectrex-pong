CPP=/usr/local/bin/m6809-unknown-none-g++
CC1PLUS=/usr/local/libexec/gcc/m6809-unknown-none/4.3.[46]/cc1plus
# CC1=/usr/local/libexec/gcc/m6809-unknown-none/4.3.[46]/cc1

CFLAGS = -O3 -fno-inline -fno-gcse -fno-toplevel-reorder -fverbose-asm -W -Wall -Wextra -Wconversion -Werror -Wno-comment -Wno-unused-parameter -Wno-return-type -fomit-frame-pointer -mint8 -msoft-reg-count=0 -fno-time-report -fdiagnostics-show-option

CC1FLAGS = $(CFLAGS) -quiet

AS=/usr/local/bin/as6809
AFLAGS=-l -og -sy

LN=/usr/local/bin/aslink
LFLAGS= -m -u -ws -b .text=0x0 

SRCS = $(wildcard src/*.cpp)
_OBJS = $(SRCS:.cpp=.o)
OBJS = $(patsubst src/%, %, $(_OBJS))
DEPS = $(OBJS:.o=.d)

.PHONY = all clean
.PRECIOUS: %.o %.s19

all: pong.bin

clean:
	$(RM) $(OBJS) *.o *.map *.hlr *.ram *.rom *.rst *.s *.s19 *.sym *.asm *.lst *.bin *.d

# Rule to generate a dep file by using the C preprocessor
%.d: src/%.cpp
	@$(CPP) $(CFLAGS) $< -MM -MT $(@:.d=.o) > $@

# Include generated dep files for header deps per source file
-include $(DEPS)

# Produce final .bin file from .s19 and _ram.s19
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
	cat make/crt0.tpl | sed -e s/XXX/`echo $* | sed -e "s/crt0//" | tr '[:lower:]' '[:upper:]'`/ > crt0.asm

%.o: src/%.cpp
	# Compile .cpp to asm file (.s)
	$(CC1PLUS) $< -dumpbase $* $(CC1FLAGS) -auxbase $* -o $*.s
	# Assemble .s to .rel, .lst, .hlr, .sym
	$(AS) $(AFLAGS) $*.s
	# Rename .rel to .o
	mv $*.rel $*.o
