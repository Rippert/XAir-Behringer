
CFLAGS = -Wall -Wextra
LDFLAGS =
LIBS =

OBJDIR = obj
BINDIR = bin

CSRCS = XAir_cparse.c XAir_sprint.c XAir_SceneDef.c XAir_dump.c
OBJS = $(CSRCS:.c=.o)

all: XAir_Interface XAir_SnapBackup

debug: CFLAGS += -g
debug: all

release: CFLAGS += -O
release: all

XAir_Interface: $(addprefix $(OBJDIR)/, XAir_Interface.o XAir_dump.o $(OBJS)) | $(BINDIR)
	$(CC) -o $(BINDIR)/$@ $^ $(LIBS)

XAir_SnapBackup:$(addprefix $(OBJDIR)/, XAir_SnapBackup.o XAir_dump.o $(OBJS)) | $(BINDIR)
	$(CC) -o $(BINDIR)/$@ $^ $(LIBS)
	
$(BINDIR):
	mkdir -p $@

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) -c $(CFLAGS) -o $@ $<
	
$(OBJDIR):
	mkdir -p $@

.PHONEY: clean

clean:
	rm -f *.o $(OBJDIR)/*.o $(BINDIR)/*
