MCU             = atmega8
PROGRAMMER      = dragon_isp

CPUFREQ         = 8000000 #8MHz
BAUD            = 9600

# https://www.engbedded.com/fusecalc/
# Int. RC Osc. 8MHz
# No WD
# No BOD
LFUSE           = 0xc4
HFUSE           = 0xd9

CC              = avr-gcc
CFLAGS          = -std=c89 -Wall -mmcu=$(MCU) -DF_CPU=$(CPUFREQ) -DBAUD=$(BAUD)
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
	@avr-size --mcu=$(MCU) --format=avr $(BUILDDIR)/$(OUTPUT)

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
	@sudo avrdude -p $(MCU) -c $(PROGRAMMER) -U flash:w:$(BUILDDIR)/$(OUTPUT).hex:i

fuse:
	@sudo avrdude -p $(MCU) -c $(PROGRAMMER) -U lfuse:w:$(LFUSE):m -U hfuse:w:$(HFUSE):m

devsetup:
	@echo "--target=avr -isystem /usr/avr/include/ $(CFLAGS)" | tr ' ' '\n' > compile_flags.txt


#--target=avr
#tmarch=atmega8
#-mmcu=atmega8
#-I/usr/avr/include/
