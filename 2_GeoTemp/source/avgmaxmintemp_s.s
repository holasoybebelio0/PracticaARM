@;--------------------------------------------------------------------
@;  "avgmaxmintemp.s": rutines de càlcul de valors mitjans, màxims
@;	i mínims d'una taula de temperatures, expressades en graus
@;	Celsius o Fahrenheit, en format de coma flotant E9M22.
@;	Implementació cridant rutines aritmètiques libE9M22.
@; -------------------------------------------------------------------
@;	santiago.romani@urv.cat
@;	pere.millan@urv.cat
@;	(Febrer-Març 2025, Febrer 2026)
@;--------------------------------------------------------------------
@;	Programador/a 1: evelio.ruiz@estudiants.urv.cat
@;	Programador/a 2: pau.gutierrez@estudiants.urv.cat
@;--------------------------------------------------------------------
@; © URV — Codi de la pràctica d'ARM de Fonaments de Computadors (FC).
@; Ús exclusiu dins l’assignatura; prohibida la seva redistribució.
@; Consulteu la llicència: ACADEMIC_LICENSE_URV_FC.txt
@;--------------------------------------------------------------------

.include "E9M22.i"			@; operacions en coma flotant E9M22

.include "avgmaxmintemp.i"	@; offsets per accedir a structs 'maxmin_t'


.text
        .align 2
        .arm


@;-----------------------------------------------------------------------
@; avgmaxmin_city(): calcula la temperatura mitjana, màxima i mínima 
@;				d'una ciutat d'una taula de temperatures, 
@;				amb una fila per ciutat i una columna per mes, 
@;				expressades en graus Celsius en format E9M22.
@;	Entrada:
@;		ttemp[][12]	→	R0: taula de temperatures, amb 12 columnes i nrows files
@;		nrows		→	R1: número de files de la taula
@;		id_city		→	R2: índex de la fila (ciutat) a processar
@;		*mmres		→	R3: adreça de l'estructura maxmin_t que retornarà els
@;						resultats de temperatures màximes i mínimes
@;	Sortida:
@;		R0			→	temperatura mitjana, expressada en graus Celsius, en format E9M22.		
@;-----------------------------------------------------------------------
        .global avgmaxmin_city
avgmaxmin_city:
        push {r3, r4-r11, lr}   @; Desa registres i alinea pila

        mov     r4, r0          @; r4 = ttemp
        
        lsl     r6, r2, #16
        lsr     r6, r6, #16     @; r6 = id_city (zero-extended)

        mov     r1, #48         @; Mida fila (12x4 bytes)
        mul     r0, r6, r1      @; Offset ciutat
        add     r4, r4, r0      @; r4 = Base ciutat (mes 0)

        ldr     r7, [r4]        @; r7 = Suma
        mov     r8, r7          @; r8 = Max
        mov     r9, r7          @; r9 = Min
        mov     r10, #0         @; r10 = id_max
        mov     r11, #0         @; r11 = id_min
        mov     r6, #1          @; r6 = i (mes 1)

    .LbucleCiutat:
        cmp     r6, #12         
        bge     .LfiBucleCiutat @; Fi si i >= 12
     
        add     r4, r4, #4      @; Següent mes

        mov     r0, r7          
        ldr     r1, [r4]        
        bl      e9m22_add
        mov     r7, r0          @; Actualitza suma
     
        ldr     r0, [r4]        
        mov     r1, r8          
        bl      e9m22_compare
        cmp     r0, #E9M22_CMP_GREATER
        bne     .LtempMinCiutat
        ldr     r8, [r4]        @; Nou Max
        mov     r10, r6         @; Nou id_max
     
    .LtempMinCiutat:
        ldr     r0, [r4]        
        mov     r1, r9          
        bl      e9m22_compare
        cmp     r0, #E9M22_CMP_LESS
        bne     .LseguentBucleCiutat
        ldr     r9, [r4]        @; Nou Min
        mov     r11, r6         @; Nou id_min
     
    .LseguentBucleCiutat:
        add     r6, r6, #1      @; i++
        b       .LbucleCiutat
     
    .LfiBucleCiutat:
        push    {r7}            @; Save sum to protect from clobber
        mov     r0, #12
        bl      int_to_e9m22    
        mov     r1, r0          @; Divisor = 12.0
        pop     {r0}            @; Restore sum to r0 directly
        bl      e9m22_div
        mov     r7, r0          @; r7 = Mitjana
     
        ldr     r3, [sp, #0]    @; Recupera *mmres
     
        str     r9, [r3, #MM_TMINC]  @; Desa Min (C)
        str     r8, [r3, #MM_TMAXC]  @; Desa Max (C)
        strh    r11, [r3, #MM_IDMIN] @; Desa id_min
        strh    r10, [r3, #MM_IDMAX] @; Desa id_max
     
        ldr     r0, [r3, #MM_TMINC]
        bl      Celsius2Fahrenheit
        ldr     r3, [sp, #0]
        str     r0, [r3, #MM_TMINF]  @; Desa Min (F)
     
        ldr     r0, [r3, #MM_TMAXC]
        bl      Celsius2Fahrenheit
        ldr     r3, [sp, #0]
        str     r0, [r3, #MM_TMAXF]  @; Desa Max (F)
     
        mov     r0, r7          @; Retorna Mitjana
        pop     {r3, r4-r11, pc} @; Restaura i surt			@; restaurar registres modificats i retornar



@;-----------------------------------------------------------------------
@; avgmaxmin_month(): calcula la temperatura mitjana, màxima i mínima 
@;				d'un mes d'una taula de temperatures, 
@;				amb una fila per ciutat i una columna per mes, 
@;				expressades en graus Celsius en format E9M22.
@;	Entrada:
@;		ttemp[][12]	→	R0: taula de temperatures, amb 12 columnes i nrows files
@;		nrows		→	R1: número de files de la taula
@;		id_month	→	R2: índex de la columna (mes) a processar
@;		*mmres		→	R3: adreça de l'estructura maxmin_t que retornarà els
@;						resultats de temperatures màximes i mínimes
@;	Sortida:
@;		R0			→	temperatura mitjana, expressada en graus Celsius, en format E9M22.		
@;-----------------------------------------------------------------------
        .global avgmaxmin_month
avgmaxmin_month:
        push {r3, r4-r11, lr}   @; Desa registres i alinea pila

        mov     r4, r0          @; r4 = ttemp
        
        lsl     r1, r1, #16
        lsr     r5, r1, #16     @; r5 = nrows (zero-extended)
        
        lsl     r2, r2, #16
        lsr     r6, r2, #16     @; r6 = id_month (zero-extended)

        mov     r1, #4          @; Mida dada (4 bytes)
        mul     r0, r6, r1      @; Offset mes
        add     r4, r4, r0      @; r4 = Base mes (ciutat 0)

        ldr     r7, [r4]        @; r7 = Suma
        mov     r8, r7          @; r8 = Max
        mov     r9, r7          @; r9 = Min
        mov     r10, #0         @; r10 = id_max
        mov     r11, #0         @; r11 = id_min
        mov     r6, #1          @; r6 = i (ciutat 1)

    .LbucleMes:
        cmp     r6, r5          
        bge     .LfiBucleMes    @; Fi si i >= nrows
     
        add     r4, r4, #48     @; Següent ciutat

        mov     r0, r7          
        ldr     r1, [r4]        
        bl      e9m22_add
        mov     r7, r0          @; Actualitza suma
     
        ldr     r0, [r4]        
        mov     r1, r8          
        bl      e9m22_compare
        cmp     r0, #E9M22_CMP_GREATER
        bne     .LtempMinMes
        ldr     r8, [r4]        @; Nou Max
        mov     r10, r6         @; Nou id_max
     
    .LtempMinMes:
        ldr     r0, [r4]        
        mov     r1, r9          
        bl      e9m22_compare
        cmp     r0, #E9M22_CMP_LESS
        bne     .LseguentBucleMes
        ldr     r9, [r4]        @; Nou Min
        mov     r11, r6         @; Nou id_min
     
    .LseguentBucleMes:
        add     r6, r6, #1      @; i++
        b       .LbucleMes
     
    .LfiBucleMes:
        push    {r7}            @; Save sum to protect from clobber
        mov     r0, r5          
        bl      int_to_e9m22    
        mov     r1, r0          @; Divisor = nrows (float)
        pop     {r0}            @; Restore sum to r0 directly
        bl      e9m22_div
        mov     r7, r0          @; r7 = Mitjana
     
        ldr     r3, [sp, #0]    @; Recupera *mmres
     
        str     r9, [r3, #MM_TMINC]  @; Desa Min (C)
        str     r8, [r3, #MM_TMAXC]  @; Desa Max (C)
        strh    r11, [r3, #MM_IDMIN] @; Desa id_min
        strh    r10, [r3, #MM_IDMAX] @; Desa id_max
     
        ldr     r0, [r3, #MM_TMINC]
        bl      Celsius2Fahrenheit
        ldr     r3, [sp, #0]
        str     r0, [r3, #MM_TMINF]  @; Desa Min (F)
     
        ldr     r0, [r3, #MM_TMAXC]
        bl      Celsius2Fahrenheit
        ldr     r3, [sp, #0]
        str     r0, [r3, #MM_TMAXF]  @; Desa Max (F)
     
        mov     r0, r7          @; Retorna Mitjana
        pop     {r3, r4-r11, pc} @; Restaura i surt

.end