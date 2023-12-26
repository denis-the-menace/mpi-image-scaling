CC = mpicc
CFLAGS = -Wall -Wextra -std=c99
IDIR = ./include
SRCDIR = ./src
OBJDIR = ./src/obj

SRC = $(wildcard $(SRCDIR)/*.c)

_OBJ = $(patsubst $(SRCDIR)/%.c, %.o, $(wildcard $(SRCDIR)/*.c))
OBJ = $(addprefix $(OBJDIR)/, $(_OBJ))

EXEC = bilinear_scaling

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) -lm

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -I$(IDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -f $(OBJDIR)/*.o $(EXEC)

