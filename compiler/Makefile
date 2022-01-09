# Makefile based on https://spin.atomicobject.com/2016/08/26/makefile-c-projects

# Build artifacts
TMP 	  = tmp
GENERATED = src/antlr4-generated
GRAMMAR   = ifcc.g4
EXEC_NAME = ifcc

# Default antlr paths
ANTLR4_BINDIR=/usr/bin
ANTLR4_INCDIR=/shares/public/tp/ANTLR4-CPP/antlr4-runtime
ANTLR4_LIBDIR=/shares/public/tp/ANTLR4-CPP/lib

# Compiler options
CC     = clang++
CXXFLAGS = -g -c -I $(ANTLR4_INCDIR) -I $(GENERATED) -ansi -pedantic -std=c++11 -Wall -Wextra -Wno-defaulted-function-deleted -Wno-unknown-warning-option -Wno-unused-parameter -Wno-attributes -Wno-extra-semi
LDFLAGS = -g

# Détection automatique des dépendances dans src/
SRC_DIRS = src

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
# On ajoute les .cpp générés car ils n'existent pas forcément au moment
# de l'exécution du `find`
SRCS := $(SRCS) src/antlr4-generated/ifccBaseVisitor.cpp src/antlr4-generated/ifccLexer.cpp src/antlr4-generated/ifccVisitor.cpp src/antlr4-generated/ifccParser.cpp
OBJS := $(SRCS:%=$(TMP)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS = $(INC_FLAGS) -MMD -MP

all: antlr $(EXEC_NAME)

skip-antlr: $(EXEC_NAME)

# Règle de compilation de la grammaire
antlr: $(GRAMMAR)
	@mkdir -p $(GENERATED)
	$(ANTLR4_BINDIR)/antlr4 -visitor -no-listener -Dlanguage=Cpp -o $(GENERATED) $(GRAMMAR)

# Règle d'édition de liens
$(EXEC_NAME): $(OBJS)
	$(CC) $^ $(ANTLR4_LIBDIR)/libantlr4-runtime.a -o $@ $(LDFLAGS)

# Règles de compilation
# $(TMP)/%.s.o: %.s
# 	mkdir -p $(dir $@)
# 	$(AS) $(ASFLAGS) -c $< -o $@

# $(TMP)/%.c.o: %.c
# 	mkdir -p $(dir $@)
# 	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(TMP)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

-include $(DEPS)

clean:
	@rm -rf $(TMP)
	@rm -rf $(GENERATED)
	@rm -rf $(TMP)

# Permet d'éviter que les fichiers du même nom que des règles
# soient "interprétés" comme des dépendances.
.PHONY: all clean antlr
