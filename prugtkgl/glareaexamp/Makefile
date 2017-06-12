
    CC = gcc #-std=c99
CFLAGS = $(shell pkg-config --cflags gio-2.0 gtk+-3.0 epoxy)
  LIBS = $(shell pkg-config --libs gio-2.0 gtk+-3.0 epoxy) -lm -lpangoft2-1.0 -lpango-1.0
   GCR = glib-compile-resources

SRC = glarea-app.c glarea-app-window.c glarea-error.c main.c
GEN = glarea-resources.c

ALL = $(GEN) $(SRC)
OBJS = $(ALL:.c=.o)

all: glarea

glarea-resources.c: glarea.gresource.xml $(shell $(GCR) --sourcedir=. --generate-dependencies glarea.gresource.xml)
	$(GCR) glarea.gresource.xml --target=$@ --sourcedir=. --generate-source

%.o: %.c
	@echo
	$(CC) $(CFLAGS) -c -o $(@F) $<

glarea: $(OBJS)
	@echo 
	$(CC) -o $(@F) $(OBJS) $(LIBS)

clean:
	@rm -f $(GEN) $(OBJS) glarea
