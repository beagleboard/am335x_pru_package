include $(BUILD_DIR)/makefile.d/base.mk

ROOTDIR = .
TARGET = libpru-driver
CROSS_COMPILE ?= arm-linux-gnueabihf-

CC = $(CROSS_COMPILE)gcc
AR = $(CROSS_COMPILE)ar

INCLUDEDIR = ./pru_sw/app_loader/include

C_FLAGS += -I. -Wall -I$(INCLUDEDIR)

COMPILE.c = $(CC) $(C_FLAGS) $(CPP_FLAGS) -c
AR.c = $(AR) rc
LINK.c = $(CC) -shared

DBGTARGET = $(OUTPUT_DIR)/$(TARGET)d.a
RELTARGET = $(OUTPUT_DIR)/$(TARGET).a
SODBGTARGET = $(OUTPUT_DIR)/$(TARGET)d.so
SORELTARGET = $(OUTPUT_DIR)/$(TARGET).so

DBGCFLAGS = -g -O0 -D__DEBUG
RELCFLAGS = -O3 -mtune=cortex-a8 -march=armv7-a

SOURCES = $(wildcard pru_sw/app_loader/interface/*.c)

PUBLIC_HDRS = $(wildcard $(INCLUDEDIR)/*.h)
PRIVATE_HDRS = $(wildcard *.h)
HEADERS = $(PUBLIC_HDRS) $(PRIVATE_HDRS)

DBGOBJFILES = $(SOURCES:%.c=$(OUTPUT_DIR)/debug/%.o)
RELOBJFILES = $(SOURCES:%.c=$(OUTPUT_DIR)/release/%.o)
PIC_DBGOBJFILES = $(SOURCES:%.c=$(OUTPUT_DIR)/debug/%_PIC.o)
PIC_RELOBJFILES = $(SOURCES:%.c=$(OUTPUT_DIR)/release/%_PIC.o)

.PHONY: clean debug release sodebug sorelease install

all: debug release sodebug sorelease

release: $(RELTARGET)

sorelease: $(SORELTARGET)

sodebug: $(SODBGTARGET)

debug: $(DBGTARGET)

$(OUTPUT_DIR):
	$(VERBOSE)mkdir -p $(OUTPUT_DIR)/release/pru_sw/app_loader/interface
	$(VERBOSE)mkdir -p $(OUTPUT_DIR)/debug/pru_sw/app_loader/interface

$(RELTARGET): $(RELOBJFILES)
	@mkdir -p $(ROOTDIR)/lib
	$(AR.c) $@ $(RELOBJFILES)

$(SORELTARGET): $(PIC_RELOBJFILES)
	@mkdir -p $(ROOTDIR)/lib
	$(LINK.c) -o $@ $(PIC_RELOBJFILES)

$(SODBGTARGET):	$(PIC_DBGOBJFILES)
	@mkdir -p $(ROOTDIR)/lib
	$(LINK.c) -o $@ $(PIC_DBGOBJFILES)

$(DBGTARGET): $(DBGOBJFILES)
	@mkdir -p $(ROOTDIR)/lib
	$(AR.c) $@ $(DBGOBJFILES)

$(RELOBJFILES): $(OUTPUT_DIR)/release/%.o: %.c $(HEADERS) | $(OUTPUT_DIR)
	$(COMPILE.c) $(RELCFLAGS) -o $@ $<

$(PIC_RELOBJFILES): $(OUTPUT_DIR)/release/%_PIC.o: %.c $(HEADERS) | $(OUTPUT_DIR)
	$(COMPILE.c) -fPIC $(RELCFLAGS) -o $@ $<

$(DBGOBJFILES): $(OUTPUT_DIR)/debug/%.o: %.c $(HEADERS) | $(OUTPUT_DIR)
	$(COMPILE.c) $(DBGCFLAGS) -o $@ $<

$(PIC_DBGOBJFILES): $(OUTPUT_DIR)/debug/%_PIC.o: %.c $(HEADERS) | $(OUTPUT_DIR)
	$(COMPILE.c) -fPIC $(DBGCFLAGS) -o $@ $<

clean:
	-rm -rf *~ ./lib/* $(OUTPUT_DIR)
