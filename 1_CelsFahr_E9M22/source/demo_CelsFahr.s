@;-----------------------------------------------------------------------
@;  Description: a program to check the temperature-scale conversion
@;				functions implemented in "CelsiusFahrenheit_c/_s".
@;	IMPORTANT NOTE: there is a much confident testing set 
@;				implemented in "tests/test_CelsiusFahrenheit.c"; 
@;				the aim of "demo_CelsFahr.s" is to show how would it be 
@;				a usual main() code invoking the mentioned functions.
@;-----------------------------------------------------------------------
@;	Author: Santiago Romaní, Pere Millán (DEIM, URV)
@;	Date:   March/2022+2023, February/2024, March/2025, March/2026 
@;-----------------------------------------------------------------------
@;	Programmer 1: evelio.ruiz@estudiants.urv.cat
@;	Programmer 2:  pau.gutierrez@estudiants.urv.cat
@;-----------------------------------------------------------------------
@; © URV — Codi de la pràctica d'ARM de Fonaments de Computadors (FC).
@; Ús exclusiu dins l’assignatura; prohibida la seva redistribució.
@; Consulteu la llicència: ACADEMIC_LICENSE_URV_FC.txt
@;-----------------------------------------------------------------------

.data
        .align 2
    temp1C:	.word 0x41066B85		@; temp1C =  35.21 °C
    temp2F:	.word 0xC0DF0000		@; temp2F = -23.75 °F

.bss
        .align 2
    temp1F:	.space 4				@; expected conversion:  95.3780 °F
    temp2C:	.space 4				@; expected conversion: -30.9722 °C 


.text
        .align 2
        .arm
        .global main
main:
        push {lr}
        
            @; temp1F = Celsius2Fahrenheit(temp1C);
            ldr r0, =temp1C         @; Cargar dirección de temp1C
            ldr r0, [r0]            @; Cargar el valor de temp1C en r0
            bl Celsius2Fahrenheit   @; Llamar a la función
            ldr r1, =temp1F         @; Cargar dirección de destino
            str r0, [r1]            @; Guardar el resultado en temp1F
        
            @; temp2C = Fahrenheit2Celsius(temp2F);
            ldr r0, =temp2F         @; Cargar dirección de temp2F
            ldr r0, [r0]            @; Cargar el valor de temp2F en r0
            bl Fahrenheit2Celsius   @; Llamar a la función
            ldr r1, =temp2C         @; Cargar dirección de destino
            str r0, [r1]            @; Guardar el resultado en temp2C


@; TESTING POINT: check the results
@;	(gdb) p /x temp1F		-> 0x415F60C4 
@;	(gdb) p /x temp2C		-> 0xC0FBE38E 
@; BREAKPOINT
        mov r0, #0					@; return(0)
        
        pop {pc}

.end

