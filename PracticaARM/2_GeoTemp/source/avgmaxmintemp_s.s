@;-----------------------------------------------------------------------
@;  "avgmaxmintemp.s": rutines de càlcul de valors mitjans, màxims
@;	i mínims d'una taula de temperatures, expressades en graus
@;	Celsius o Fahrenheit, en format de coma flotant E9M22.
@;	Implementació cridant rutines aritmètiques libE9M22.
@; ----------------------------------------------------------------
@;	santiago.romani@urv.cat
@;	pere.millan@urv.cat
@;	(Febrer-Març 2025, Febrer 2026)
@;-----------------------------------------------------------------------
@;	Programador/a 1: xxx.xxx@estudiants.urv.cat
@;	Programador/a 2: yyy.yyy@estudiants.urv.cat
@;-----------------------------------------------------------------------

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
				@; r0: ...
        push {r1-r2, lr}		@; Salvar registres modificats i adreça retorn

            @; codi "FAKE": CAL SUBSTITUIR-LO!!! per la traducció a assemblador de la rutina en C
			@; -----------  ====================

        ldr r1, =E9M22_sNAN		@; Signaling NaN per indicar rutina pendent d'implementació
        str r1, [r3, #MM_TMINC]		@; mmres->tmin_C = NaN
        str r1, [r3, #MM_TMAXC]		@; mmres->tmax_C = NaN
        str r1, [r3, #MM_TMINF]		@; mmres->tmin_F = NaN
        str r1, [r3, #MM_TMAXF]		@; mmres->tmax_F = NaN
        mov r2, #-1				@; Valor id fictici per indicar pendent implementació
        strh r2, [r3, #MM_IDMIN]	@; mmres->id_min = -1
        strh r2, [r3, #MM_IDMAX]	@; mmres->id_max = -1

        mov r0, r1				@; return (NaN)	→ per indicar pendent implementació

        pop {r1-r2, pc}			@; restaurar registres modificats i retornar



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
				@; r0: ...
        push {r1-r2, lr}		@; Salvar registres modificats i adreça retorn

            @; codi "FAKE": CAL SUBSTITUIR-LO!!! per la traducció a assemblador de la rutina en C
			@; -----------  ====================

        ldr r1, =E9M22_sNAN		@; Signaling NaN per indicar rutina pendent d'implementació
        str r1, [r3, #MM_TMINC]		@; mmres->tmin_C = NaN
        str r1, [r3, #MM_TMAXC]		@; mmres->tmax_C = NaN
        str r1, [r3, #MM_TMINF]		@; mmres->tmin_F = NaN
        str r1, [r3, #MM_TMAXF]		@; mmres->tmax_F = NaN
        mov r2, #-1				@; Valor id fictici per indicar pendent implementació
        strh r2, [r3, #MM_IDMIN]	@; mmres->id_min = -1
        strh r2, [r3, #MM_IDMAX]	@; mmres->id_max = -1

        mov r0, r1				@; return (NaN)	→ per indicar pendent implementació

        pop {r1-r2, pc}			@; restaurar registres modificats i retornar



.end

