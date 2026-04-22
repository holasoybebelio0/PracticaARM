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
				@; ús de registres:
				@; r0: ttemp (taula de temperatures)
				@; r1: nrows (número de files)
				@; r2: id_city (índex de la ciutat)
				@; r3: mmres (adreça estructura resultats)
				@; r4: avg
				@; r5: max
				@; r6: min
				@; r7: i (índex del mes)
				@; r8: tvar (temperatura temporal)
				@; r9: idmax
				@; r10: idmin
				@; r11: adreça base per calcular posició a la taula
        push {r4-r11, lr}		@; Salvar registres modificats i adreça retorn

        @; Calcular adreça base de la fila de la ciutat: ttemp[id_city]
        @; offset = id_city * 12 * 4 = id_city * 48
        mov r11, r0             @; r11 = base de la taula
        mov r4, r2              @; r4 = id_city
        lsl r4, r4, #4          @; r4 = id_city * 16
        add r4, r4, r2, lsl #5  @; r4 = id_city * 16 + id_city * 32 = id_city * 48
        add r11, r11, r4        @; r11 = adreça base de la fila [id_city]

        @; Inicialitzar valors amb primera columna (mes 0 = gener)
        ldr r4, [r11, #0]       @; avg = ttemp[id_city][0]
        mov r5, r4              @; max = avg
        mov r6, r4              @; min = avg
        mov r9, #0              @; idmax = 0
        mov r10, #0             @; idmin = 0
        mov r7, #1              @; i = 1 (començar pel segon mes)

loop_months:
        cmp r7, #12             @; while (i < 12)
        bge end_loop

        @; tvar = ttemp[id_city][i]
        ldr r8, [r11, r7, lsl #2]  @; r8 = ttemp[id_city][i] (cada element és 4 bytes)

        @; avg = e9m22_add(avg, tvar)
        mov r0, r4              @; primer operand = avg
        mov r1, r8              @; segon operand = tvar
        bl e9m22_add
        mov r4, r1              @; avg = resultat

        @; if (e9m22_compare(tvar, max) == E9M22_CMP_GREATER)
        mov r0, r8
        mov r1, r5
        bl e9m22_compare
        cmp r0, #1              @; E9M22_CMP_GREATER = 1
        bne check_min
        mov r5, r8              @; max = tvar
        mov r9, r7              @; idmax = i

check_min:
        @; if (e9m22_compare(tvar, min) == E9M22_CMP_LESS)
        mov r0, r8
        mov r1, r6
        bl e9m22_compare
        cmp r0, #-1             @; E9M22_CMP_LESS = -1
        bne next_iter
        mov r6, r8              @; min = tvar
        mov r10, r7             @; idmin = i

next_iter:
        add r7, r7, #1          @; i++
        b loop_months

end_loop:
        @; avg = e9m22_div(avg, E9M22_12)
        mov r0, r4
        ldr r1, =E9M22_12       @; constant 12.0
        bl e9m22_div
        mov r4, r1              @; avg = resultat

        @; Guardar resultats a mmres
        str r6, [r3, #MM_TMINC] @; mmres->tmin_C = min
        str r5, [r3, #MM_TMAXC] @; mmres->tmax_C = max

        @; mmres->tmin_F = Celsius2Fahrenheit(min)
        mov r0, r6
        bl Celsius2Fahrenheit
        str r0, [r3, #MM_TMINF]

        @; mmres->tmax_F = Celsius2Fahrenheit(max)
        mov r0, r5
        bl Celsius2Fahrenheit
        str r0, [r3, #MM_TMAXF]

        @; mmres->id_min = idmin
        strh r10, [r3, #MM_IDMIN]
        @; mmres->id_max = idmax
        strh r9, [r3, #MM_IDMAX]

        @; return avg
        mov r0, r4

        pop {r4-r11, pc}		@; restaurar registres modificats i retornar



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
				@; ús de registres:
				@; r0: ttemp (taula de temperatures)
				@; r1: nrows (número de files)
				@; r2: id_month (índex del mes)
				@; r3: mmres (adreça estructura resultats)
				@; r4: avg
				@; r5: max
				@; r6: min
				@; r7: i (índex de la ciutat)
				@; r8: nrows (guardat)
				@; r9: idmax
				@; r10: idmin
				@; r11: offset de la columna (id_month * 4)
				@; r12: tvar (temperatura temporal)
        push {r4-r12, lr}		@; Salvar registres modificats i adreça retorn

        @; Guardar nrows a r8 per usar-lo durant el bucle
        mov r8, r1               @; r8 = nrows

        @; Calcular offset de la columna: id_month * 4
        mov r11, r2              @; r11 = id_month
        lsl r11, r11, #2         @; r11 = id_month * 4 (offset de columna)

        @; Inicialitzar valors amb primera fila (ciutat 0)
        ldr r4, [r0, r11]        @; avg = ttemp[0][id_month]
        mov r5, r4               @; max = avg
        mov r6, r4               @; min = avg
        mov r9, #0               @; idmax = 0
        mov r10, #0              @; idmin = 0
        mov r7, #1               @; i = 1 (començar per la segona ciutat)

loop_cities:
        cmp r7, r8               @; while (i < nrows)
        bge end_loop

        @; tvar = ttemp[i][id_month]
        @; offset = i * 12 * 4 + id_month * 4 = i * 48 + id_month * 4
        mov r1, r7               @; r1 = i
        lsl r1, r1, #4           @; r1 = i * 16
        add r1, r1, r7, lsl #5   @; r1 = i * 16 + i * 32 = i * 48
        add r1, r0, r1           @; r1 = adreça base de la fila i
        ldr r12, [r1, r11]       @; r12 = ttemp[i][id_month] (tvar)

        @; avg = e9m22_add(avg, tvar)
        mov r0, r4               @; primer operand = avg
        mov r1, r12              @; segon operand = tvar
        bl e9m22_add
        mov r4, r1               @; avg = resultat

        @; if (e9m22_compare(tvar, max) == E9M22_CMP_GREATER)
        mov r0, r12
        mov r1, r5
        bl e9m22_compare
        cmp r0, #1               @; E9M22_CMP_GREATER = 1
        bne check_min_month
        mov r5, r12              @; max = tvar
        mov r9, r7               @; idmax = i

check_min_month:
        @; if (e9m22_compare(tvar, min) == E9M22_CMP_LESS)
        mov r0, r12
        mov r1, r6
        bl e9m22_compare
        cmp r0, #-1              @; E9M22_CMP_LESS = -1
        bne next_iter_month
        mov r6, r12              @; min = tvar
        mov r10, r7              @; idmin = i

next_iter_month:
        add r7, r7, #1           @; i++
        b loop_cities

end_loop:
        @; nrows encara està a r8
        mov r1, r8               @; r1 = nrows

        @; avg = e9m22_div(avg, int_to_e9m22(nrows))
        bl int_to_e9m22          @; convertir nrows (r1) a E9M22 → resultat a r0
        mov r1, r0               @; r1 = nrows en E9M22
        mov r0, r4               @; r0 = avg (dividend)
        bl e9m22_div
        mov r4, r1               @; avg = resultat

        @; Guardar resultats a mmres
        str r6, [r3, #MM_TMINC]  @; mmres->tmin_C = min
        str r5, [r3, #MM_TMAXC]  @; mmres->tmax_C = max

        @; mmres->tmin_F = Celsius2Fahrenheit(min)
        mov r0, r6
        bl Celsius2Fahrenheit
        str r0, [r3, #MM_TMINF]

        @; mmres->tmax_F = Celsius2Fahrenheit(max)
        mov r0, r5
        bl Celsius2Fahrenheit
        str r0, [r3, #MM_TMAXF]

        @; mmres->id_min = idmin
        strh r10, [r3, #MM_IDMIN]
        @; mmres->id_max = idmax
        strh r9, [r3, #MM_IDMAX]

        @; return avg
        mov r0, r4

        pop {r4-r12, pc}         @; restaurar registres modificats i retornar



.end

