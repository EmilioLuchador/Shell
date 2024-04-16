CC = gcc
CFLAGS = -g -Wall -std=gnu17 -Iinclude
LFLAGS = -lpthread
SRCDIR = src
OBJDIR = obj
BUILDDIR = build

SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))
EXECUTABLE = $(BUILDDIR)/major2.exe

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) | $(BUILDDIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

clean:
	rm -rf $(OBJDIR) $(BUILDDIR)

run: $(EXECUTABLE)
	./$(EXECUTABLE)
