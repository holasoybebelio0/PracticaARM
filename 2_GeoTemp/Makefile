#-------------------------------------------------------------------------------
# Example Makefile to build a library and to test the functions of each module
# Authors: Santiago Romaní, Pere Millán
# Date: April 2021, March 2022-2024, March 2025
#-------------------------------------------------------------------------------
#	Programador/a 1: evelio.ruiz@estudiants.urv.cat
#	Programador/a 2: pau.gutierrez@estudiants.urv.cat
#-------------------------------------------------------------------------------
# © URV — Codi de la pràctica d'ARM de Fonaments de Computadors (FC).
# Ús exclusiu dins l'assignatura; prohibida la seva redistribució.
# Consulteu la llicència: ACADEMIC_LICENSE_URV_FC.txt
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# options for code generation
#-------------------------------------------------------------------------------
ARCH	:= -march=armv5te -mlittle-endian
INCL    := -I./include
ASFLAGS	:= $(ARCH) $(INCL) -g
CCFLAGS	:= -Wall -gdwarf-3 -O0 $(ARCH) $(INCL)
LDFLAGS := -z max-page-size=0x8000

#-------------------------------------------------------------------------------
# make commands
#-------------------------------------------------------------------------------
tota_2a_part_practica_FC : test_geotemp_c.elf test_geotemp_s.elf geotemp_c.elf geotemp_s.elf


# --- Versió C de avgmaxmintemp ---
build/avgmaxmintemp_c.o : source/avgmaxmintemp_c.c \
                          include/E9M22.h include/E9M22_impl.h \
                          include/avgmaxmintemp.h include/CelsiusFahrenheit.h
	arm-none-eabi-gcc $(CCFLAGS) -c source/avgmaxmintemp_c.c -o build/avgmaxmintemp_c.o


# --- Versió S (assemblador) de avgmaxmintemp ---
build/avgmaxmintemp_s.o : source/avgmaxmintemp_s.s \
                          include/E9M22.i include/E9M22_impl.i \
                          include/avgmaxmintemp.i include/avgmaxmintemp.h
	arm-none-eabi-as $(ASFLAGS) source/avgmaxmintemp_s.s -o build/avgmaxmintemp_s.o


# --- data.o ---
build/data.o : source/data.c \
               include/E9M22.h include/data.h \
               include/CelsiusFahrenheit.h include/avgmaxmintemp.h
	arm-none-eabi-gcc $(CCFLAGS) -c source/data.c -o build/data.o


# --- geotemp.o (programa principal) ---
build/geotemp.o : source/geotemp.c \
                  include/E9M22.h include/E9M22_impl.h \
                  include/CelsiusFahrenheit.h include/avgmaxmintemp.h \
                  include/data.h include/geotemp.h
	arm-none-eabi-gcc $(CCFLAGS) -c source/geotemp.c -o build/geotemp.o


# --- Executables geotemp (versió C i S) ---
geotemp_c.elf : build/geotemp.o build/avgmaxmintemp_c.o build/data.o \
                p_lib/CelsiusFahrenheit_s.o p_lib/libE9M22.a
	arm-none-eabi-ld $(LDFLAGS) build/geotemp.o build/avgmaxmintemp_c.o build/data.o \
					p_lib/CelsiusFahrenheit_s.o p_lib/startup.o p_lib/libE9M22.a \
					p_lib/libfoncompus.a -o geotemp_c.elf

geotemp_s.elf : build/geotemp.o build/avgmaxmintemp_s.o build/data.o \
                p_lib/CelsiusFahrenheit_s.o p_lib/libE9M22.a
	arm-none-eabi-ld $(LDFLAGS) build/geotemp.o build/avgmaxmintemp_s.o build/data.o \
					p_lib/CelsiusFahrenheit_s.o p_lib/startup.o p_lib/libE9M22.a \
					p_lib/libfoncompus.a -o geotemp_s.elf


#-------------------------------------------------------------------------------
# test making commands
#-------------------------------------------------------------------------------
build/test_geotemp.o : tests/test_geotemp.c \
                       include/E9M22.h include/E9M22_impl.h \
                       include/avgmaxmintemp.h include/data.h
	arm-none-eabi-gcc $(CCFLAGS) -c tests/test_geotemp.c -o build/test_geotemp.o

test_geotemp_c.elf : build/test_geotemp.o build/avgmaxmintemp_c.o build/data.o \
                     p_lib/CelsiusFahrenheit_s.o p_lib/libE9M22.a
	arm-none-eabi-ld $(LDFLAGS) build/test_geotemp.o build/avgmaxmintemp_c.o build/data.o \
					p_lib/CelsiusFahrenheit_s.o p_lib/startup.o p_lib/libE9M22.a \
					p_lib/libfoncompus.a -o test_geotemp_c.elf

test_geotemp_s.elf : build/test_geotemp.o build/avgmaxmintemp_s.o build/data.o \
                     p_lib/CelsiusFahrenheit_s.o p_lib/libE9M22.a
	arm-none-eabi-ld $(LDFLAGS) build/test_geotemp.o build/avgmaxmintemp_s.o build/data.o \
					p_lib/CelsiusFahrenheit_s.o p_lib/startup.o p_lib/libE9M22.a \
					p_lib/libfoncompus.a -o test_geotemp_s.elf


#-------------------------------------------------------------------------------
# clean commands
#-------------------------------------------------------------------------------
clean :
	@rm -fv build/*.o
	@rm -fv *.elf


#-----------------------------------------------------------------------------
# run commands  (executa geotemp dins d'Insight)
#-----------------------------------------------------------------------------
run : geotemp_s.elf geotemp_c.elf
	arm-eabi-insight geotemp_s.elf &
	# arm-eabi-insight geotemp_c.elf &


#-----------------------------------------------------------------------------
# debug commands  (executa test_geotemp dins d'Insight)
#-----------------------------------------------------------------------------
debug : test_geotemp_s.elf test_geotemp_c.elf
	arm-eabi-insight test_geotemp_s.elf &
	#arm-eabi-insight test_geotemp_c.elf &