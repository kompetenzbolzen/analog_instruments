MCU             = atmega8
CPUFREQ         = 160000000
PROGRAMMER      = dragon_isp

CC              = avr-gcc
CFLAGS          = -std=c89 -Wall -mmcu=$(MCU) -DF_CPU=$(CPUFREQ)
LDFLAGS         = -mmcu=$(MCU)
BUILDDIR        = build
SOURCEDIR       = src
OBJECTDIR       = obj

OUTPUT          = analog

SRCS = $(wildcard $(SOURCEDIR)/*.c)
OBJS = $(SRCS:.c=.o)
OBJ  = $(OBJS:$(SOURCEDIR)/%=$(OBJECTDIR)/%)

build: dir $(OBJ)
	@echo LD $(OBJ)
	@$(CC) $(CFLAGS) -o $(BUILDDIR)/$(OUTPUT) $(OBJ) $(LDFLAGS)
	@avr-objcopy -O ihex $(BUILDDIR)/$(OUTPUT) $(BUILDDIR)/$(OUTPUT).hex

debug: -D _DEBUG
debug: build;

dir:
	@mkdir -p $(OBJECTDIR)
	@mkdir -p $(BUILDDIR)

$(OBJECTDIR)/%.o: $(SOURCEDIR)/%.c
	@echo CC $<
	@$(CC) -c $(CFLAGS) $< -o $@

.PHONY: clean
clean:	
	@echo RM $(OBJ)
	@echo RM $(BUILDDIR)/$(OUTPUT)
	@rm -df  $(OBJ)
	@rm -Rdf $(BUILDDIR) $(OBJECTDIR)

all: clean build

flash: build
	@avrdude -p $(MCU) -c $(PROGRAMMER) -U flash:w:$(BUILDDIR)/$(OUTPUT).hex:i

devsetup:
	@echo "--target=avr -isystem /usr/avr/include/ $(CFLAGS)" | tr ' ' '\n' > compile_flags.txt


#--target=avr
#tmarch=atmega8
#-mmcu=atmega8
#-I/usr/avr/include/
