##### User configurable options #####

ARCH        = @ARCH@
COMM        = @COMM@
INSTALL_DIR = @INSTALL_DIR@
CC          = @CC@
F77         = @F77@
CLINKER     = @CLINKER@
FLINKER     = @FLINKER@
OPTFLAGS    = @OPTFLAGS@
#
LIB_PATH    = -L$(INSTALL_DIR)/lib/$(ARCH)/$(COMM)
FLIB_PATH   = @FLIB_PATH_LEADER@$(INSTALL_DIR)/lib/$(ARCH)/$(COMM)
LIB_LIST    = @LIB_LIST@
#
INCLUDE_DIR = @INCLUDE_PATH@ -I$(INSTALL_DIR)/include

### End User configurable options ###

CFLAGS  = @CFLAGS@ $(OPTFLAGS) $(INCLUDE_DIR) -DMPI_$(ARCH)
FFLAGS = @FFLAGS@ $(INCLUDE_DIR) $(OPTFLAGS)
LIBS = $(LIB_PATH) $(LIB_LIST)
FLIBS = $(FLIB_PATH) $(LIB_LIST)
EXECS = hello

default: hello

all: $(EXECS)

hello: hello.o $(INSTALL_DIR)/include/mpi.h
	$(CLINKER) $(OPTFLAGS) -o hello hello.o \
	$(LIB_PATH) $(LIB_LIST) -lm

clean:
	/bin/rm -f *.o *~ PI* $(EXECS)

.c.o:
	$(CC) $(CFLAGS) -c $*.c
.f.o:
	$(F77) $(FFLAGS) -c $*.f
