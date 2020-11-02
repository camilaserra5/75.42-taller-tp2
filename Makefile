# Makefile de ejemplo para C/C++
# Creado: 15/04/2004 - Leandro Lucarella
# Modificado: 01/09/2016 - Pablo Roca
# Copyleft 2004 - Leandro Lucarella, Bajo licencia GPL [http://www.gnu.org/]

# CONFIGURACION
################

# Nombre del ejecutable.
target = tp
target-tsan = tp-tsan

# ExtensiÃ³n de los archivos a compilar (c para C, cpp o cc o cxx para C++).
extension = cpp

# Archivos con el cÃ³digo fuente que componen el ejecutable. Si no se especifica,
# toma todos los archivos con la extensiÃ³n mencionada. Para especificar hay que
# descomentar la lÃ­nea (quitarle el '#' del principio).
# NOTA: No poner cabeceras (.h).
#fuentes = entrada.cpp

# Si usa funciones de math.h, descomentar (quitar el '#' a) la siguiente lÃ­nea.
math = si

# Si usa threads, descomentar (quitar el '#' a) la siguiente lÃ­nea.
threads = si

# Si es un programa GTK+, descomentar (quitar el '#' a) la siguiente lÃ­nea.
#gtk = si

# Si es un programa gtkmm, descomentar (quitar el '#' a) la siguiente lÃ­nea.
#gtkmm = si

# Descomentar si se quiere ver como se invoca al compilador
verbose = si


# CONFIGURACION "AVANZADA"
###########################

# Opciones para el compilador C/C++ para tratamiento de errores y warnings.
CFLAGS += -Wall -Werror -pedantic -pedantic-errors

# Para optimizar el binario resultante lo mejor posible
CFLAGS += -O3

# Para valgrind o debug
CFLAGS += -ggdb -DDEBUG -fno-inline

# Opciones del enlazador.
#LDFLAGS =

# Estandar de C a usar
CSTD = c99

# Estandar de C++ a usar
CXXSTD = c++11

# Si se quiere compilar estaticamente, descomentar la siguiente linea
#static = si


# VARIABLES CALCULADAS A PARTIR DE LA CONFIGURACION
####################################################

# Agrega flags y libs de GTK+ de ser necesario.
ifdef gtk
CFLAGS += $(shell pkg-config --cflags gtk+-3.0) \
	-DG_DISABLE_DEPRECATED 	 	\
	-DGDK_DISABLE_DEPRECATED 	\
	-DGDK_PIXBUF_DISABLE_DEPRECATED \
	-DGTK_DISABLE_DEPRECATED
LDFLAGS += $(shell pkg-config --libs gtk+-3.0)
endif

# Agrega flags y libs de GTK+ de ser necesario.
ifdef gtkmm
CFLAGS += $(shell pkg-config --cflags gtkmm-3.0) \
	-DG_DISABLE_DEPRECATED 	 	\
	-DGDK_DISABLE_DEPRECATED 	\
	-DGDK_PIXBUF_DISABLE_DEPRECATED \
	-DGTK_DISABLE_DEPRECATED	\
	-DGDKMM_DISABLE_DEPRECATED 	\
	-DGTKMM_DISABLE_DEPRECATED
LDFLAGS += $(shell pkg-config --libs gtkmm-3.0)
endif

# Linkea con libm de ser necesario.
ifdef math
LDFLAGS += -lm
endif

# Linkea con threads de ser necesario. Permite el uso de pthread en C y C++. Permite el uso de built-in threads en C++.
ifdef threads
LDFLAGS += -pthread
endif

ifdef static
LDFLAGS += -static
endif

# Se reutilizan los flags de C para C++ tambiÃ©n
CXXFLAGS += $(CFLAGS)

# Se usa enlazador de c++ si es cÃ³digo no C.
ifeq ($(extension), c)
CFLAGS += -std=$(CSTD)
LD = $(CC)
COMPILER = $(CC)
COMPILERFLAGS = $(CFLAGS)
else
CXXFLAGS += -std=$(CXXSTD)
LD = $(CXX)
COMPILER = $(CXX)
COMPILERFLAGS = $(CXXFLAGS)
endif

# Si no especifica archivos, tomo todos.
fuentes ?= $(wildcard *.$(extension))
directorios = $(shell find . -type d -regex '.*\w+')

occ := $(CC)
ocxx := $(CXX)
orm := $(RM)
old := $(LD)
ifdef verbose
RM := $(RM) -v
else
CC =  @echo "  CC  $@"; $(occ)
CXX = @echo "  CXX $@"; $(ocxx)
RM =  @echo "  CLEAN"; $(orm)
LD =  @echo "  LD  $@"; $(old)
endif

LDFLAGS-TSAN = $(LDFLAGS) -fsanitize=thread
COMPILERFLAGS-TSAN = $(COMPILERFLAGS) -fsanitize=thread


# REGLAS
#########

.PHONY: all clean

all: $(target) $(target-tsan)

o_files = $(patsubst %.$(extension),%.o,$(fuentes))
o-tsan_files = $(patsubst %.$(extension),%.o-tsan,$(fuentes))

$(target): $(o_files)
	@if [ -z "$(o_files)" ]; \
	then \
		echo "No hay archivos de entrada en el directorio actual. Recuerde que la extensiÃ³n debe ser '.$(extension)' y que no se aceptan directorios anidados."; \
		if [ -n "$(directorios)" ]; then echo "Directorios encontrados: $(directorios)"; fi; \
		false; \
	fi >&2
	$(LD) $(o_files) -o $(target) $(LDFLAGS)

$(target-tsan): $(o-tsan_files)
	@if [ -z "$(o-tsan_files)" ]; \
	then \
		echo "No hay archivos de entrada en el directorio actual. Recuerde que la extensiÃ³n debe ser '.$(extension)' y que no se aceptan directorios anidados."; \
		if [ -n "$(directorios)" ]; then echo "Directorios encontrados: $(directorios)"; fi; \
		false; \
	fi >&2
	$(LD) $(o-tsan_files) -o $(target-tsan) $(LDFLAGS-TSAN)

%.o-tsan: %.$(extension)
	$(COMPILER) $(COMPILERFLAGS-TSAN) -o $@ -c $<

%.o: %.$(extension)
	$(COMPILER) $(COMPILERFLAGS) -o $@ -c $<

clean-obj:
	$(RM) $(o_files) $(o-tsan_files)

clean: clean-obj
	$(RM) $(target) $(target-tsan)