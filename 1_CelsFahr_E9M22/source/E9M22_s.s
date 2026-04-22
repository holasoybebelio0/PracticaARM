@;-----------------------------------------------------------------------
@;  E9M22_s.s: operacions amb números en format Coma Flotant E9M22.
@;-----------------------------------------------------------------------
@;	santiago.romani@urv.cat
@;	pere.millan@urv.cat
@;	(Març 2021-2023, Febrer 2024, Març 2025, Març 2026)
@;-----------------------------------------------------------------------
@;	Programador/a 1: evelio.ruiz@estudiants.urv.cat
@;	Programador/a 2: pau.gutierrez@estudiants.urv.cat
@;-----------------------------------------------------------------------
@; © URV — Codi de la pràctica d'ARM de Fonaments de Computadors (FC).
@; Ús exclusiu dins l’assignatura; prohibida la seva redistribució.
@; Consulteu la llicència: ACADEMIC_LICENSE_URV_FC.txt
@;-----------------------------------------------------------------------

.include "E9M22.i"

FLOAT_sNAN	=	0x7FA00000	@; Un possible NaN (signaling) en binary32 

.text
        .align 2
        .arm


@;************************************************************
@;* Rutines de CLASSIFICACIÓ de valors en coma flotant E9M22 *
@;************************************************************

@;-----------------------------------------------------------------------
@; e9m22_classify_s(): indica la classe de número E9M22.
@;	Entrada:
@;		num 	-> R0: valor E9M22 a classificar
@;	Sortida:
@;		R0 		-> valor E9M22_CLASS_ZERO|DENORMAL|NORMAL|INFINITE|NAN
@;-----------------------------------------------------------------------
	.global e9m22_classify_s
e9m22_classify_s:
				@; ús de registres:
				@; r0: valor a classificar
				@; r1: màscara E9M22_MASK_EXP (i posteriorment a E9M22_MASK_FRAC)
				@; r2: exponent
				@; r3: mantissa
	push {r1-r3, lr}

	ldr r1, =E9M22_MASK_EXP
	
	and r2, r0, r1 @; exponent
		
	cmp r2, #0
	beq .L_else_classify @; if ( (exponent != 0) && (exponent != E9M22_MASK_EXP) )
	cmp r2, r1 
	beq .L_else_classify

		@; ÉS NORMAL: 
	mov r0, #E9M22_CLASS_NORMAL 
	b .L_fin_classify

.L_else_classify:
		ldr r1, =E9M22_MASK_FRAC @; ara utilitzarem l'altre màscara (no cal l'altre)
		and r3, r0, r1 @; mantissa

		cmp r2, #0 @; if (exponent == 0)
		bne .L_classify_inf_nan @; Pot ser infinit o NaN

		cmp r3, #0 @; if (mantissa != 0)
		beq .L_classify_class_zero @; És de la classe ZERO

	.L_classify_class_denormal: 
		@; si no és de la classe ZERO seguirà per aqui el codi: 
		@; ÉS DENORMAL
		mov r0, #E9M22_CLASS_DENORMAL
		b .L_fin_classify
	.L_classify_inf_nan:
		cmp r3, #0
		bne .L_classify_class_nan @; si (mantissa != 0) és NaN

		@; ÉS INFINIT
		mov r0, #E9M22_CLASS_INFINITE
		b .L_fin_classify

	.L_classify_class_nan:
		mov r0, #E9M22_CLASS_NAN @; ÉS NaN
		b .L_fin_classify
	.L_classify_class_zero: 
		mov r0, #E9M22_CLASS_ZERO @; ÉS ZERO
		b .L_fin_classify
.L_fin_classify:
	pop {r1-r3, pc}


@;-----------------------------------------------------------------------
@; e9m22_is_normal_s(): 
@;				retorna un valor ≠0 si num és de classe NORMAL E9M22
@;						(bits d'exponent ni tot 0's ni tot 1's).
@;	Entrada:
@;		num 	-> R0: valor E9M22 a detectar si és de classe NORMAL.
@;	Sortida:
@;		R0 		-> ≠0 si num és de classe NORMAL; =0 altrament.
@;-----------------------------------------------------------------------
	.global e9m22_is_normal_s
e9m22_is_normal_s:
				@; ús de registres:
				@; r0: valor a detectar si és normal
				@; r1: valor de la màscara EM922_MASK_EXP
				@; r2: valor de exponent

		push {r1-r2, lr}
		ldr r1, =E9M22_MASK_EXP @; carreguem la màscara a R1 

		and r2, r0, r1 @; exponent = num & EM922_MASK_EXP assignat a R2

		cmp r2, #0 		@ Si exponente == 0 -> no es normal
		beq .L_fals_if_is_normal
		
		cmp r2, r1 		@ Si exponente == todos 1s -> no es normal
		beq .L_fals_if_is_normal

		ldr r0, =0xFC2026	@ Sí es normal (Retornar True / Valor != 0)
		b .L_fin_is_normal
		

	.L_fals_if_is_normal: 
		mov r0, #0 @; assignem valor 0 a resultat
	
	.L_fin_is_normal:
		pop {r1-r2, pc}



@;-----------------------------------------------------------------------
@; e9m22_is_denormal_s(): 
@;				retorna un valor ≠0 si num és de classe DENORMAL E9M22
@;					(bits d'exponent tots 0's i algun bit de fracció ≠0).
@;	Entrada:
@;		num 	-> R0: valor E9M22 a detectar si és de classe DENORMAL.
@;	Sortida:
@;		R0 		-> ≠0 si num és de classe DENORMAL; =0 altrament.
@;-----------------------------------------------------------------------
	.global e9m22_is_denormal_s
e9m22_is_denormal_s:
				@; ús de registres:
				@; r0: valor a detectar si és DENORMAL
				@; r1: valor amb la màscara E9M22_MASK_EXP
				@; r2: exponent i posteriorment mantissa

		push {r1-r2, lr}

		ldr r1, =E9M22_MASK_EXP @; carreguem la màscara a r1
		and r2, r0, r1 @; operació exponent assignada a r2
		

		cmp r2, #0 
		bne .L_fals_if_is_denormal @; si exponent != 0, fals
		
		ldr r1, =E9M22_MASK_FRAC @;Carreguem Mascara mantissa
		and r2, r0, r1			 @;r2 = Mantissa 
		
		cmp r2, #0
		beq .L_fals_if_is_denormal @; si mantissa == 0, fals

		ldr r0, =0xFC2026 @; el valor sí es DENORMAL
		b .L_fin_is_denormal

	.L_fals_if_is_denormal:
		mov r0, #0 @; assignem 0 a resultat
	
	.L_fin_is_denormal:
		pop {r1-r2, pc}



@;-----------------------------------------------------------------------
@; e9m22_is_zero_s(): 
@;				retorna un valor ≠0 si num codifica un valor ±0,0 E9M22
@;					(bits d'exponent i de fracció tots 0's).
@;	Entrada:
@;		num 	-> R0: valor E9M22 a detectar si és de classe ZERO.
@;	Sortida:
@;		R0 		-> ≠0 si num és de classe ZERO (±0,0); =0 altrament.
@;-----------------------------------------------------------------------
	.global e9m22_is_zero_s
e9m22_is_zero_s:
				@; ús de registres:
				@; r0: valor a detectar si és de classe ZERO
				@; r1: valor amb la màscara E9M22_MASK_EXP 
					@; (i posteriorment màscara E9M22_MASK_FRAC)
				@; r2: exponent i posteriorment mantissa

		push {r1-r2, lr}

		ldr r1, =E9M22_MASK_EXP
		
		and r2, r0, r1 @; exponent = num & E9M22_MASK_EXP

		cmp r2, #0 @; si exponent != 0, fals
		bne .L_fals_if_is_zero 

		ldr r1, =E9M22_MASK_FRAC

		and r2, r0, r1 @; mantissa = num & E9M22_MASK_FRAC

		cmp r2, #0 @; si mantissa != 0, fals
		bne .L_fals_if_is_zero

		ldr r0, =0xFC2026 @; carreguem el valor a resultat si és classe ZERO
		b .L_fin_is_zero

	.L_fals_if_is_zero:
		mov r0, #0 @; valor de resultat si no és classe ZERO
	
	.L_fin_is_zero:
		pop {r1-r2, pc}



@;-----------------------------------------------------------------------
@; e9m22_is_infinite_s(): 
@;				retorna un valor ≠0 si num codifica un valor ±∞ E9M22
@;					(bits d'exponent tots 1's i bits de fracció tots 0's).
@;	Entrada:
@;		num 	-> R0: valor E9M22 a detectar si és de classe INFINIT.
@;	Sortida:
@;		R0 		-> ≠0 si num és de classe INFINIT; =0 altrament.
@;-----------------------------------------------------------------------
	.global e9m22_is_infinite_s
e9m22_is_infinite_s:
				@; ús de registres:
				@; r0: valor E9M22 a detectar si es INFINIT
				@; r1: màscara E9M22_MASK_EXP 
					@; (i posteriorment màscara E9M22_MASK_FRAC)
				@; r2: exponent i posteriorment mantissa

		push {r1-r2, lr}

		ldr r1, =E9M22_MASK_EXP 
		and r2, r0, r1 @; exponent 
		

		cmp r2, r1 @; si exponent != E9M22_MASK_EXP, fals
		bne .L_fals_if_is_infinite

		ldr r1, =E9M22_MASK_FRAC
		and r2, r0, r1 @; mantissa

		cmp r2, #0 @; o si mantissa != 0, fals
		bne .L_fals_if_is_infinite

		ldr r0, =0xFC2026 @; es infinit
		b .L_fin_is_infinite

	.L_fals_if_is_infinite:
		mov r0, #0 @; no es infinit, valor 0 a resultat
	
	.L_fin_is_infinite:
		pop {r1-r2, pc}



@;-----------------------------------------------------------------------
@; e9m22_is_nan_s(): 
@;				retorna un valor ≠0 si num codifica un valor NaN E9M22
@;					(bits d'exponent tots 1's i algun bit de fracció ≠0).
@;	Entrada:
@;		num 	-> R0: valor E9M22 a detectar si és de classe NAN.
@;	Sortida:
@;		R0 		-> ≠0 si num és de classe NAN; =0 altrament.
@;-----------------------------------------------------------------------
	.global e9m22_is_nan_s
e9m22_is_nan_s:
				@; ús de registres:
				@; r0: valor E9M22 a detectar si és classe NAN
				@; r1: màscara E9M22_MASK_EXP
				@; r2: exponent i posteriorment mantissa

		push {r1-r2, lr}
		ldr r1, =E9M22_MASK_EXP


		and r2, r0, r1 @; exponent
		

		cmp r2, r1	@; si exponent != E9M22_MASK_EXP, fals
		bne .L_fals_if_is_nan

		ldr r1, =E9M22_MASK_FRAC
		and r2, r0, r1 @; mantissa
		
		cmp r2, #0 @; si mantissa == 0, fals
		beq .L_fals_if_is_nan

		ldr r0, =0xFC2026 @; si es NaN
		b .L_fin_is_nan
	
	.L_fals_if_is_nan:
		mov r0, #0 @; no es NaN, valor de resultat 0

	.L_fin_is_nan:	
		pop {r1-r2, pc}



@;-----------------------------------------------------------------------
@; e9m22_is_finite_s(): 
@;				retorna un valor ≠0 si num és un valor ni ±∞, ni NaN
@;					(algun bit d'exponent ≠1: normal, denormal o zero).
@;	Entrada:
@;		num 	-> R0: valor E9M22 a detectar si és d'alguna classe FINITA.
@;	Sortida:
@;		R0 		-> ≠0 si num és d'alguna classe FINITA; =0 altrament.
@;-----------------------------------------------------------------------
	.global e9m22_is_finite_s
e9m22_is_finite_s:
				@; ús de registres:
				@; r0: valor E9M22 a detectar si és d'alguna classe FINITA
				@; r1: màscara E9M22_MASK_EXP
				@; r2: exponent
			
		push {r1-r2, lr}

		ldr r1, =E9M22_MASK_EXP @; carreguem la màscara E9M22_MASK_EXP
		
		and r2, r0, r1 @; exponent 
		cmp r2, r1 @; si exponent == E9M22_MASK_EXP, fals 
		beq .L_fals_if_is_finite

		ldr r0, =0xFC2026
		b .L_fin_is_finite 
	
	.L_fals_if_is_finite: 
		mov r0, #0 @; no es finito, valor de resultat 0
	
	.L_fin_is_finite:
		pop {r1-r2, pc}



@;-----------------------------------------------------------------------
@; e9m22_is_negative_s(): 
@;				retorna un valor ≠0 si num és negatiu (bit de signe =1).
@;	Entrada:
@;		num 	-> R0: valor E9M22 a detectar si és NEGATIU.
@;	Sortida:
@;		R0 		-> ≠0 si num és NEGATIU; =0 si és POSITIU.
@;-----------------------------------------------------------------------
	.global e9m22_is_negative_s
e9m22_is_negative_s:
				@; ús de registres:
				@; r0: valor E9M22 a detectar si és negatiu
				@; r1: màscara E9M22_MASK_SIGN
				@; r2: signe

		push {r1-r2, lr}

		ldr r1, =E9M22_MASK_SIGN @; carreguem la màscara a r1

		and r2, r0, r1 @; signe
		
		cmp r2, r1 @; si no és negatiu, fals 
		bne .L_fals_if_is_negative

		ldr r0, =0xFC2026 @; és negatiu
		b .L_fin_is_negative

	.L_fals_if_is_negative:
		mov r0, #0 @; no és negatiu
	
	.L_fin_is_negative:
		pop {r1-r2, pc}




@;******************************************************
@;* Rutines de CONVERSIÓ de valors E9M22 <-> float/int *
@;******************************************************

@;-----------------------------------------------------------------------
@; e9m22_to_float_s(): converteix un valor E9M22 a float.
@;	Entrada:
@;		input 	-> R0: valor E9M22 a convertir
@;	Sortida:
@;		R0 		-> valor E9M22 convertit a float.
@;-----------------------------------------------------------------------
	.global e9m22_to_float_s
e9m22_to_float_s:
				@; ús de registres:
				@; r0: ...
		push {lr}

		ldr r0, =FLOAT_sNAN		@; to-do: NaN per indicar rutina pendent
		
		pop {pc}



@;-----------------------------------------------------------------------
@; float_to_e9m22_s(): converteix un valor float a E9M22.
@;	Entrada:
@;		input 	-> R0: valor float a convertir
@;	Sortida:
@;		R0 		-> valor float convertit a E9M22.
@;-----------------------------------------------------------------------
	.global float_to_e9m22_s
float_to_e9m22_s:
				@; ús de registres:
				@; r0: ...
		push {lr}

		ldr r0, =E9M22_sNAN		@; to-do: NaN per indicar rutina pendent
		
		pop {pc}



@;-----------------------------------------------------------------------
@; e9m22_to_int_s(): converteix un valor E9M22 a enter (arrodonit).
@;	Entrada:
@;		input 	-> R0: valor E9M22 a convertir
@;	Sortida:
@;		R0 		-> valor E9M22 convertit a enter.
@;-----------------------------------------------------------------------
	.global e9m22_to_int_s
e9m22_to_int_s:
				@; ús de registres:
				@; r0: ...
		push {lr}

		mov r0, #-1		@; to-do: -1 per indicar rutina pendent
		
		pop {pc}



@;-----------------------------------------------------------------------
@; int_to_e9m22_s(): converteix un valor enter a E9M22.
@;	Entrada:
@;		input 	-> R0: valor enter a convertir
@;	Sortida:
@;		R0 		-> valor float convertit a E9M22.
@;-----------------------------------------------------------------------
	.global int_to_e9m22_s
int_to_e9m22_s:
    push {lr}
    pop {pc}


@;*************************************************
@;* Operacions ARITMÈTIQUES en Coma Flotant E9M22 *
@;*************************************************

@;-----------------------------------------------------------------------
@; e9m22_add_s(): calcula i retorna la suma dels 2 operands,
@;                (num1 + num2) codificats en coma flotant E9M22.
@;	Entrada:
@;		num1 	-> R0: primer operand
@;		num2 	-> R1: segon operand
@;	Sortida:
@;		R0 		-> valor E9M22 de (num1 + num2).
@;-----------------------------------------------------------------------
	.global e9m22_add_s
e9m22_add_s:
    push {lr}
    pop {pc}


@;-----------------------------------------------------------------------
@; e9m22_sub_s(): calcula i retorna la diferència dels 2 operands,
@;                (num1 - num2) codificats en coma flotant E9M22.
@;	Entrada:
@;		num1 	-> R0: primer operand
@;		num2 	-> R1: segon operand
@;	Sortida:
@;		R0 		-> valor E9M22 de (num1 - num2).
@;-----------------------------------------------------------------------
	.global e9m22_sub_s
e9m22_sub_s:
    push {r1-r2, lr}          

    
    mov r2, r0              @; num1 = r4
    mov r0, r1              @; num2 = r0
    bl e9m22_neg_s      @; R0 = e9m22_neg_s(num2)  resultat=num2(R0) negat

    
    mov r1, r0              @; R1 = num2 negat (R0)
    mov r0, r2              @; R0 = num1
    bl e9m22_add_c_        @; R0 = e9m22_add_s(num1, num2negat)

    
    pop {r1-r2, pc}        




@;-----------------------------------------------------------------------
@; e9m22_mul_s(): calcula i retorna el producte dels 2 operands,
@;                (num1 × num2) codificats en coma flotant E9M22.
@;	Entrada:
@;		num1 	-> R0: primer operand
@;		num2 	-> R1: segon operand
@;	Sortida:
@;		R0 		-> valor E9M22 de (num1 × num2).
@;-----------------------------------------------------------------------
	.global e9m22_mul_s
e9m22_mul_s:
    push    {lr}
    
    pop     {pc}



@;-----------------------------------------------------------------------
@; e9m22_div_s(): calcula i retorna la divisió dels 2 operands,
@;                (num1 ÷ num2) codificats en coma flotant E9M22.
@;					Implementació "indirecta": a ÷ b = a × (1 ÷ b)
@;	Entrada:
@;		num1 	-> R0: primer operand
@;		num2 	-> R1: segon operand
@;	Sortida:
@;		R0 		-> valor E9M22 de (num1 ÷ num2).
@;-----------------------------------------------------------------------
	.global e9m22_div_s
e9m22_div_s:
    @; Paso 1: Salvar R4 (para preservar num1) y LR (para volver)
    push {r4, lr}           

    @; Paso 2: Calcular el inverso del segundo operando: 1 / num2
    mov r4, r0              @; Guardamos num1 en R4 para que no se pierda
    mov r0, r1              @; Ponemos num2 en R0 para la función inv
    bl e9m22_inv_c_         @; R0 = e9m22_inv_s(num2) -> resultado: invers2

    @; Paso 3: Realizar la multiplicación: num1 * invers2
    mov r1, r0              @; Movemos el inverso (resultado de inv) a R1
    mov r0, r4              @; Recuperamos num1 de R4 y lo ponemos en R0
    bl e9m22_mul_c_          @; R0 = e9m22_mul_s(num1, invers2)

    @; Paso 4: Restaurar registros y retornar
    pop {r4, pc}



@;-----------------------------------------------------------------------
@; e9m22_inv_s(): calcula i retorna l'invers de l'operand,
@;                (1,0 ÷ num) codificat en coma flotant E9M22.
@;	Entrada:
@;		num 	-> R0: valor a invertir
@;	Sortida:
@;		R0 		-> valor E9M22 invertit de num.
@;-----------------------------------------------------------------------
	.global e9m22_inv_s
e9m22_inv_s:
    push    {r1-r9, lr}
	
	mov r4, r0 @; Guardem el valor
	
	mov  r0, r4                @; R0 = num
    add  r1, sp, #0            @; R1 = &signe_div (adreça a la pila)
    add  r2, sp, #4            @; R2 = &exp_sh    (adreça a la pila)
    add  r3, sp, #8            @; R3 = &mant_nat  (adreça a la pila)
    bl   e9m22_decompose       @; Crida a la rutina; el resultat 'class' queda a R0
    mov  r5, r0                @; Guardem 'class' a r5

    @; if ( e9m22_is_finite(num) )
    mov  r0, r4                @; R0 = num
    bl   e9m22_is_finite       
    cmp  r0, #0                @; Comprovem si el resultat és fals (0)
    beq  .L_No_Es_Finit     @; Si no és finit

 

.L_No_Es_Finit:

    cmp  r0, #0                @; if ( e9m22_is_finite(num) )
    beq  .L_inv_nan_inf         @; Si no és finit, saltem al final (NaN o Infinit)

    
    cmp  r5, #E9M22_CLASS_ZERO @; if ( class != E9M22_CLASS_ZERO )
    beq  .L_inv_es_zero         @; Si és zero, saltem a la part d'inversa = infinit

    @; --- CAS NORMAL O DENORMAL ---
    @; Carreguem valors desats a la pila per e9m22_decompose
    ldrsh r6, [sp, #4]         @; r6 = exp (exp_sh)
    ldr   r8, [sp, #8]         @; r8 = mant (mant_nat)

    @; num_trailing_zeros = count_trailing_zeros_c_(mant);
    mov   r0, r8
    bl    count_trailing_zeros
    cmp   r0, #0
    ble   .L_inv_skip_tz        @; if (num_trailing_zeros > 0)
    
    @; mant >>= num_trailing_zeros; exp += num_trailing_zeros;
    lsr   r8, r8, r0           
    add   r6, r6, r0           

.L_inv_skip_tz:
    @; div_mod(0x80000000, mant, &quo, &res);
    ldr   r0, =0x80000000      @; dividend
    mov   r1, r8               @; divisor (mant ajustat)
    add   r2, sp, #4           @; &quo (reutilitzem espai a la pila)
    add   r3, sp, #8           @; &res (reutilitzem espai a la pila)
    bl    div_mod

    ldr   r7, [sp, #4]         @; r7 = mant_div (quo)
    ldr   r9, [sp, #8]         @; r9 = res

    @; exp_div = -exp + E9M22_m - E9M22_e; (E9M22_m - E9M22_e = 22 - 9 = 13)
    rsb   r6, r6, #13          @; r6 = 13 - exp (exp_div)

    @; if (res > 0)
    cmp   r9, #0
    beq   .L_inv_normalize

    @; num_leading_zeros = count_leading_zeros(mant_div);
    mov   r0, r7
    bl    count_leading_zeros
    cmp   r0, #2               @; if (num_leading_zeros >= 2)
    blt   .L_inv_skip_shift
    
    @; mant_div <<= 2; exp_div -= 2;
    lsl   r7, r7, #2
    sub   r6, r6, #2

.L_inv_skip_shift:
    
    @; if (mant <= res*2) Round bit = 1 (mant_div |= 2)
    mov   r0, r9, lsl #1       @; r0 = res * 2
    cmp   r8, r0               @; mant vs res*2
    orrle r7, r7, #2           

    @; if (mant < res*2) Sticky bit = 1 (mant_div |= 1)
    cmp   r8, r0
    orrlt r7, r7, #1           

.L_inv_normalize:
    @; inversa = e9m22_normalize_and_round(signe_div, exp_div, mant_div);
    ldr   r0, [sp, #0]         @; Carreguem signe_div des de la pila
    mov   r1, r6               @; r1 = exp_div
    mov   r2, r7               @; r2 = mant_div
    bl    e9m22_normalize_and_round
    b     .L_inv_fi       @;r0 = resultat

.L_inv_es_zero:
    @; inversa = signe_div | E9M22_INF_POS; (1/0 = infinit amb el mateix signe)
    ldr   r0, [sp, #0]         @; signe_div
    ldr   r1, =E9M22_INF_POS
    orr   r0, r0, r1
    b     .L_inv_fi

.L_inv_nan_inf:
    cmp   r5, #E9M22_CLASS_NAN
    moveq r0, r4               @; if (class == NAN) inversa = num (r4)
    ldrne r0, [sp, #0]         @; else (INF) inversa = signe_div (bit 31, resta és 0)

.L_inv_fi:
     
    pop  {r1-r9, pc}
	



@;-----------------------------------------------------------------------
@; e9m22_neg_s(): canvia el signe (nega) de l'operand num
@;                codificat en coma flotant E9M22.
@;	Entrada:
@;		num 	-> R0: valor a negar
@;	Sortida:
@;		R0 		-> valor E9M22 negat de num.
@;-----------------------------------------------------------------------
	.global e9m22_neg_s
e9m22_neg_s:
    push {r1, lr}
    ldr r1, =E9M22_MASK_SIGN
    eor r0, r0, r1      @; xor per invertir el bit
    pop {r1, pc}


@;-----------------------------------------------------------------------
@; e9m22_abs_s(): calcula i retorna el valor absolut
@;                de l'operand num codificat en coma flotant E9M22.
@;	Entrada:
@;		num 	-> R0: valor a calcular el valor absolut
@;	Sortida:
@;		R0 		-> valor absolut E9M22 de num.
@;-----------------------------------------------------------------------
	.global e9m22_abs_s
e9m22_abs_s:
    push {r1, lr}
    ldr r1, =E9M22_MASK_SIGN
    bic r0, r0, r1      @; netejem el bit
    pop {r1, pc}


@;***********************************************************
@;* Operació de COMPARACIÓ de números en Coma Flotant E9M22 *
@;***********************************************************

@;-----------------------------------------------------------------------
@; e9m22_compare_s(): compara 2 valors E9M22.
@;	Entrada:
@;		num1 	-> R0: primer operand
@;		num2 	-> R1: segon operand
@;	Sortida:
@;		R0 		-> Retorna un valor e9m22_cmp_t (=, >, <, no comparable/NaN).
@;-----------------------------------------------------------------------
	.global e9m22_compare_s
e9m22_compare_s:
				
    push {r4-r9, lr}

    mov r4, r0              @; r4 = num1
    mov r5, r1              @; r5 = num2

    @; class1 = e9m22_classify(num1)
    mov r0, r4
    bl  e9m22_classify
    mov r6, r0              @; r6 = class1

    @; class2 = e9m22_classify(num2)
    mov r0, r5
    bl  e9m22_classify
    mov r7, r0              @; r7 = class2

    @; if ( (class1 != E9M22_CLASS_NAN) && (class2 != E9M22_CLASS_NAN) )
    cmp r6, #E9M22_CLASS_NAN
    beq ._compare_es_nan    @; Salta si num1 és NaN
    cmp r7, #E9M22_CLASS_NAN
    beq ._compare_es_nan    @; Salta si num2 és NaN

    @; Obtenir signes: sign1 = num1 & MASK; sign2 = num2 & MASK;
    ldr r2, =E9M22_MASK_SIGN
    and r8, r4, r2          @; r8 = sign1
    and r9, r5, r2          @; r9 = sign2

    @; if (sign1 != sign2)
    cmp r8, r9
    bne ._compare_signes_diferents

    @; MAteix Signe
    cmp r8, #0					
    bne ._compare_tots_negatius

._compare_tots_positius:
    @; Si tots dos son positius, l'ordre és el natural dels bits
    cmp r4, r5
    moveq r0, #E9M22_CMP_EQUAL
    movhi r0, #E9M22_CMP_GREATER
    movlo r0, #E9M22_CMP_LESS
    b ._compare_final

._compare_tots_negatius:
    @; Si tots dos son negatius, l'ordre s'inverteix respecte al patró de bits
    cmp r4, r5
    moveq r0, #E9M22_CMP_EQUAL
    movhi r0, #E9M22_CMP_LESS      @; Un valor de bits més alt en negatiu és "més petit"
    movlo r0, #E9M22_CMP_GREATER
    b ._compare_final

._compare_signes_diferents:
    @; if ( (class1 == E9M22_CLASS_ZERO) && (class2 == E9M22_CLASS_ZERO) )
    cmp r6, #E9M22_CLASS_ZERO
    bne ._compare_no_zeros
    cmp r7, #E9M22_CLASS_ZERO
    moveq r0, #E9M22_CMP_EQUAL     @; +0.0 és igual a -0.0
    beq ._compare_final

._compare_no_zeros:
    @; Si un es negatiu i l'altre positiu, el positiu sempre es mes gran
    cmp r8, #0
    moveq r0, #E9M22_CMP_GREATER    @; num1 (positiu) > num2 (negatiu)
    movne r0, #E9M22_CMP_LESS       @; num1 (negatiu) < num2 (positiu)
    b ._compare_final

._compare_es_nan:
    @; Si algun operand és NaN, no son comparables
    mov r0, #E9M22_CMP_NC_NAN

._compare_final:
   
    pop {r4-r9, pc}




@;**********************************************************
@;* Funcions auxiliars:	DESCOMPOSICIÓ(split),			   *
@;*						NORMALITZACIÓ i ARRODONIMENT E9M22 *
@;**********************************************************


@;--------------------------------------------------------------------------
@; e9m22_decompose_s(): 
@;    Funció per obtenir els 3 componentes E9M22 (signe, exponent, mantissa) 
@;    i la classe de número E9M22 (e9m22_class_t).
@;  Entrada:
@;		num 	-> R0: número E9M22 a descomposar
@;		@signe	-> R1: adreça (a 32 bits naturals) on desar el signe (al MSB)
@;		@exp	-> R2: adreça (a 16 bits enters) on desar l'exponent real (sense excés)
@;		@mant	-> R3: adreça (a 32 bits naturals) on desar la mantissa (amb 1, implícit)
@;		NOTA:	(si alguna adreça és 0/NULL, no s'escriu aquell component)
@;  Sortida:
@;		R0 		-> retorna la classe de num: E9M22_CLASS_ZERO|DENORMAL|NORMAL|INFINITE|NAN
@;
@;			+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@;			Components retornats segons classe de número:
@;			• Normal: mantissa (amb 1, implícit); exponent que correspongui (sense excés/bias) 
@;			• Denormal: mantissa que correspongui; exponent E9M22_Emin (-254)
@;			• Zero: mantissa 0; exponent 0
@;			• Infinit: mantissa 0; exponent 511
@;			• NaN: mantissa≠0 (payload); exponent 511
@;			▸▸▸ El bit de Signe no depèn de la classe de número E9M22.
@;			+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@;
@;--------------------------------------------------------------------------
	.global e9m22_decompose_s
e9m22_decompose_s:
				@; ús de registres:
				@; r0: ...
		push {lr}

		ldr r0, =E9M22_sNAN		@; to-do: NaN per indicar rutina pendent
		
		pop {pc}



@;-----------------------------------------------------------------------
@; e9m22_normalize_and_round_s(): normalitza 
@;                	i arrodoneix al més proper.
@;	Entrada:
@;		signe 	 -> R0: signe del valor a normalitzar
@;		exponent -> R1: exponent del valor a normalitzar
@;		mantissa -> R2: mantissa del valor a normalitzar
@;	Sortida:
@;		R0 		-> retorna un valor E9M22 normalitzat i arrodonit.
@;-----------------------------------------------------------------------
	.global e9m22_normalize_and_round_s
e9m22_normalize_and_round_s:
				@; ús de registres:
				@; r0: ...
		push {lr}

		ldr r0, =E9M22_sNAN		@; to-do: NaN per indicar rutina pendent
		
		pop {pc}



@;-----------------------------------------------------------------------
@; e9m22_round_s(): Arrodoniment al més proper. 
@;                	Si es troba al mig, al més proper parell.
@;	Entrada:
@;		mantissa  -> R0: mantissa del valor a arrodonir
@;		nbits_shr -> R1: quantitat de bits a desplaçar a la dreta
@;	Sortida:
@;		R0 		 -> Retorna la mantissa arrodonida (+1) 
@;					o l'original (trunc) sense desplaçar.
@;-----------------------------------------------------------------------
	.global e9m22_round_s
e9m22_round_s:
				@; ús de registres:
				@; r0: ...
		push {lr}

		mov r0, #-1		@; to-do: -1 per indicar rutina pendent
		
		pop {pc}



@;****************************************************************
@;* Funcions AUXILIARS per treballar amb els bits de codificació *
@;**************************************************************** 


@;-----------------------------------------------------------------------
@; count_leading_zeros_s(): compta quants bits a 0 hi ha 
@;                			des del bit de més pes (esquerra).
@;	Entrada:
@;		num  -> R0: número de 32 bits a analitzar.
@;	Sortida:
@;		R0 		 -> número de bits a zero (0-32) des del MSB.
@;-----------------------------------------------------------------------
	.global count_leading_zeros_s
count_leading_zeros_s:
				@; ús de registres:
				@; r0: ...
		push {lr}

		mov r0, #0xFC		@; to-do: 252 per indicar rutina pendent
		
		pop {pc}



@;-----------------------------------------------------------------------
@; count_trailing_zeros_s(): compta quants bits a 0 hi ha 
@;                 			 des del bit de menys pes (dreta).
@;	Entrada:
@;		num  -> R0: número de 32 bits a analitzar.
@;	Sortida:
@;		R0 		 -> número de bits a zero (0-32) des del LSB.
@;-----------------------------------------------------------------------
	.global count_trailing_zeros_s
count_trailing_zeros_s:
				@; ús de registres:
				@; r0: ...
		push {lr}

		mov r0, #0xFC		@; to-do: 252 per indicar rutina pendent
		
		pop {pc}






.end

