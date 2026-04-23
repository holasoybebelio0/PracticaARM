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
		
        push {r4-r11, lr}		@; Salvar registres modificats i adreça retorn
		
		@; r0  = valor temporal i paràmetre per crides a funcions
		@; r1  = valor temporal i paràmetre per crides a funcions
		@; r2  = id_city (16 bits) i adreça columna dins la fila
		@; r3  = *mmres
		@; r4  = adreça base de la fila de la ciutat seleccionada
		@; r5  = suma acumulada de temperatures
		@; r6  = temperatura màxima trobada
		@; r7  = temperatura mínima
		@; r8  = id mes temperatura màxima
		@; r9  = id mes temperatura mínima
		@; r10 = comptador mes
		@; r11 = *mmres (copia)
		
		mov r2, r2, lsl #16
		mov r2, r2, lsr #16
		mov     r4, #48
		mul     r4, r2, r4
		add     r4, r0, r4
	 
		mov     r11, r3
	 
		ldr     r5, [r4]
		mov     r6, r5
		mov     r7, r5
		mov     r8, #0
		mov     r9, #0
		mov     r10, #1
	 
	.LbucleCiutat:
		cmp     r10, #12
		bge     .LfiBucleCiutat
	 
		mov     r0, #4
		mul     r0, r10, r0
		add     r0, r4, r0
		ldr     r0, [r0]
	 
		push    {r0}
	 
		mov     r1, r0
		mov     r0, r5
		bl      e9m22_add
		mov     r5, r0
	 
		ldr     r0, [sp]
		mov     r1, r6
		bl      e9m22_compare
		cmp     r0, #E9M22_CMP_GREATER
		bne     .LtempMinCiutat
		ldr     r6, [sp]
		mov     r8, r10
	 
	.LtempMinCiutat:
		ldr     r0, [sp]
		mov     r1, r7
		bl      e9m22_compare
		cmp     r0, #E9M22_CMP_LESS
		bne     .LseguentBucleCiutat
		ldr     r7, [sp]
		mov     r9, r10
	 
	.LseguentBucleCiutat:
		pop     {r0}
		add     r10, r10, #1
		b       .LbucleCiutat
	 
	.LfiBucleCiutat:
		mov     r0, #12
		bl      int_to_e9m22
		mov     r1, r0
		mov     r0, r5
		bl      e9m22_div
		mov     r5, r0
	 
		str     r7, [r11, #MM_TMINC]
		str     r6, [r11, #MM_TMAXC]
	 
		mov     r0, r7
		bl      Celsius2Fahrenheit
		str     r0, [r11, #MM_TMINF]
	 
		mov     r0, r6
		bl      Celsius2Fahrenheit
		str     r0, [r11, #MM_TMAXF]
	 
		strh    r9, [r11, #MM_IDMIN]
		strh    r8, [r11, #MM_IDMAX]
	 
		mov     r0, r5
	 
        pop {r4-r11, pc}			@; restaurar registres modificats i retornar



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
		
        push {r4-r11, lr}		@; Salvar registres modificats i adreça retorn
		
		@; r0  = ttemp (punter a la taula) , valor temporal i resultat 
		@; r1  = nrows (nombre de files) / comptador de fila
		@; r2  = id_month (columna) i valor temporal
		@; r3  = id fila temperatura mínima
		@; r4  = adreça temporal ttemp[row][col]
		@; r5  = suma acumulada de temperatures 
		@; r6  = temperatura màxima 
		@; r7  = temperatura mínima 
		@; r8  = id fila temperatura màxima
		@; r9  = nrows 
		@; r10 = id_month 
		@; r11 = *mmres 
		
		mov     r11, r3
	 
		mov r2, r2, lsl #16
		mov r2, r2, lsr #16
		mov r1, r1, lsl #16
		mov r1, r1, lsr #16
	 
		mov     r9, r1
		mov     r10, r2
	 
		mov r4, r2, lsl #2
		add     r4, r0, r4
		ldr     r5, [r4]
	 
		mov     r6, r5
		mov     r7, r5
		mov     r8, #0
		mov     r3, #0
		mov     r1, #1
	 
	.LbucleMes:
		mov r4, r1, lsl #16
		mov r4, r4, lsr #16
		cmp     r4, r9
		bge     .LfiBucleMes
	 
		mov     r4, #48
		mul     r4, r1, r4
		mov r2, r10, lsl #2
		add     r2, r4, r2
		add     r2, r0, r2
		ldr     r2, [r2]
	 
		push    {r0, r1, r2}
	 
		mov     r1, r2
		mov     r0, r5
		bl      e9m22_add
		mov     r5, r0
	 
		ldr     r0, [sp, #8]
		mov     r1, r6
		bl      e9m22_compare
		cmp     r0, #E9M22_CMP_GREATER
		bne     .LtempMinMes
		ldr     r6, [sp, #8]
		ldr     r8, [sp, #4]
	 
	.LtempMinMes:
		ldr     r0, [sp, #8]
		mov     r1, r7
		bl      e9m22_compare
		cmp     r0, #E9M22_CMP_LESS
		bne     .LseguentBucleMes
		ldr     r7, [sp, #8]
		ldr     r3, [sp, #4]
	 
	.LseguentBucleMes:
		pop     {r0, r1, r2}
		add     r1, r1, #1
		b       .LbucleMes
	 
	.LfiBucleMes:
		mov     r0, r9
		bl      int_to_e9m22
		mov     r1, r0
		mov     r0, r5
		bl      e9m22_div
		mov     r5, r0
	 
		str     r7, [r11, #MM_TMINC]
		str     r6, [r11, #MM_TMAXC]
	 
		mov     r0, r7
		bl      Celsius2Fahrenheit
		str     r0, [r11, #MM_TMINF]
	 
		mov     r0, r6
		bl      Celsius2Fahrenheit
		str     r0, [r11, #MM_TMAXF]
	 
		strh    r3, [r11, #MM_IDMIN]
		strh    r8, [r11, #MM_IDMAX]
	 
		mov     r0, r5
		
        pop {r4-r11, pc}			@; restaurar registres modificats i retornar



.end

