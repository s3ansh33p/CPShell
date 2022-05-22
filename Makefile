APP_NAME:=CPShell

#This Makefile compiles a program for the calculator running hollyhock-2 and for the pc. 
#You need the hollyhock sdk for the calculator version and sdl2 for the pc version.
# github.com/SnailMath/hollyhock-2 
#export SDK_DIR:=/path/to/the/folder/hollyhock-2/sdk/


#you can use 'make all' to compile all and 'make clean' to remove the output files.
#you can compile only the pc version with 'make pc' (you won't need hollyhock)
#or only the calculator version with 'make hhk' (you won't need sdl for this)


#this makefile is based on the makefile in app_tamplate from the original hollyhock project by The6P4C


#ifndef SDK_DIR
#$(error You need to define the SDK_DIR environment variable, and point it to the sdk/ folder)
#endif

#If you use AS sources make sure to write the same functions in C++ because sh4 as does not run on the pc

#The pc compiler
C_PC:=gcc
C_PC_FLAGS:=-W -Wall -DPC -lSDL2

#The sh4 assembler, compiler and linker:
AS:=sh4-elf-as
AS_FLAGS:=

CC:=sh4-elf-gcc 
CC_FLAGS:=-ffreestanding -fshort-wchar -Wall -Wextra -O2 -I $(SDK_DIR)/include/ -I $(SDK_DIR)/newlib/sh-elf/include -j8

CXX:=sh4-elf-g++
CXX_FLAGS:=-ffreestanding -fno-exceptions -fno-rtti -fshort-wchar -Wall -Wextra -O2 -I $(SDK_DIR)/include/ -I $(SDK_DIR)/newlib/sh-elf/include -m4a-nofpu

LD:=sh4-elf-gcc
LD_FLAGS:=-nostartfiles -m4-nofpu -Wno-undef -L$(SDK_DIR)/newlib/sh-elf/lib

READELF:=sh4-elf-readelf
OBJCOPY:=sh4-elf-objcopy

AS_SOURCES:=$(wildcard *.s)
CC_SOURCES:=$(wildcard *.c)
CXX_SOURCES:=$(wildcard *.cpp)
H_INC:=$(wildcard *.h) 
HPP_INC:=$(wildcard *.hpp)
OBJECTS:=$(AS_SOURCES:.s=.o) $(CC_SOURCES:.c=.o) $(CXX_SOURCES:.cpp=.o)

APP_PC:=_$(APP_NAME).elf

APP_ELF:=$(APP_NAME).hhk

APP_BIN:=$(APP_NAME).bin

all: pc hhk

bin: $(APP_BIN) Makefile

pc: $(APP_PC) Makefile

hhk: $(APP_ELF) Makefile

clean:
	rm -f $(OBJECTS) $(APP_ELF) $(APP_PC) $(APP_BIN)

$(APP_PC):  $(CC_SOURCES) $(CXX_SOURCES) $(H_INC) $(HPP_INC)
	$(C_PC) $(CC_SOURCES) $(CXX_SOURCES) -o $(APP_PC) $(C_PC_FLAGS)

$(APP_ELF): $(OBJECTS) $(SDK_DIR)/sdk.o linker.ld
	$(LD) -T linker.ld -o $@ $(LD_FLAGS) $(OBJECTS) $(SDK_DIR)/sdk.o
	$(OBJCOPY) --set-section-flags .hollyhock_name=contents,strings,readonly $(APP_ELF) $(APP_ELF)
	$(OBJCOPY) --set-section-flags .hollyhock_description=contents,strings,readonly $(APP_ELF) $(APP_ELF)
	$(OBJCOPY) --set-section-flags .hollyhock_author=contents,strings,readonly $(APP_ELF) $(APP_ELF)
	$(OBJCOPY) --set-section-flags .hollyhock_version=contents,strings,readonly $(APP_ELF) $(APP_ELF)

$(APP_BIN): $(OBJECTS) $(SDK_DIR)/sdk.o linker.bin.ld
	$(LD) -Wl, --oformat binary -T linker.bin.ld -o $@ $(LD_FLAGS) $(OBJECTS) $(SDK_DIR)/sdk.o

# We're not actually building sdk.o, just telling the user they need to do it
# themselves. Just using the target to trigger an error when the file is
# required but does not exist.
$(SDK_DIR)/sdk.o:
	$(error You need to build the SDK before using it. Run make in the SDK directory, and check the README.md in the SDK directory for more information)

%.o: %.s
	$(AS) $< -o $@ $(AS_FLAGS)

%.o: %.c $(H_INC)
	$(CC) -c $< -o $@ $(CC_FLAGS)

# Break the build if global constructors are present:
# Read the sections from the object file (with readelf -S) and look for any
# called .ctors - if they exist, give the user an error message, delete the
# object file (so that on subsequent runs of make the build will still fail)
# and exit with an error code to halt the build.
%.o: %.cpp $(HPP_INC)
	$(CXX) -c $< -o $@ $(CXX_FLAGS)
	@$(READELF) $@ -S | grep ".ctors" > /dev/null && echo "ERROR: Global constructors aren't supported." && rm $@ && exit 1 || exit 0

#tell make that 'all' 'clean' 'hhk' and 'pc' are not actually files.
.PHONY: all clean hhk pc
