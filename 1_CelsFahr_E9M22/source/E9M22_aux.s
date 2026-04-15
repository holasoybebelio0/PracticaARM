@;-----------------------------------------------------------------------
@;  Fitxer:		E9M22_aux.s
@;  Descripció: rutines auxiliars per a rutines en Coma Flotant E9M22.
@;-----------------------------------------------------------------------
@;	Autor: Pere Millán (DEIM, URV)
@;	Data:  Març/2025, Febrer-Març/2026 
@;-----------------------------------------------------------------------
@; © URV — Codi de la pràctica d'ARM de Fonaments de Computadors (FC).
@; Ús exclusiu dins l’assignatura; prohibida la seva redistribució.
@; Consulteu la llicència: ACADEMIC_LICENSE_URV_FC.txt
@;-----------------------------------------------------------------------


.text
        .align 2
        .arm


@;******************************************************/
@;*  Rutines per desar regs L/M modificats per codi C  */
@;******************************************************/

		.global e9m22_classify_c
e9m22_classify_c:
		push {r1-r3, r12, lr}		@; salvar regs modificables per rutina en C
		bl   e9m22_classify_c_		@; cridar rutina en C
		pop  {r1-r3, r12, pc}		@; restaurar regs modificables per rutina en C

		.global e9m22_is_normal_c
e9m22_is_normal_c:
		push {r1-r3, r12, lr}		@; salvar regs modificables per rutina en C
		bl   e9m22_is_normal_c_		@; cridar rutina en C
		pop  {r1-r3, r12, pc}		@; restaurar regs modificables per rutina en C

		.global e9m22_is_denormal_c
e9m22_is_denormal_c:
		push {r1-r3, r12, lr}		@; salvar regs modificables per rutina en C
		bl   e9m22_is_denormal_c_		@; cridar rutina en C
		pop  {r1-r3, r12, pc}		@; restaurar regs modificables per rutina en C

		.global e9m22_is_zero_c
e9m22_is_zero_c:
		push {r1-r3, r12, lr}		@; salvar regs modificables per rutina en C
		bl   e9m22_is_zero_c_		@; cridar rutina en C
		pop  {r1-r3, r12, pc}		@; restaurar regs modificables per rutina en C

		.global e9m22_is_infinite_c
e9m22_is_infinite_c:
		push {r1-r3, r12, lr}		@; salvar regs modificables per rutina en C
		bl   e9m22_is_infinite_c_		@; cridar rutina en C
		pop  {r1-r3, r12, pc}		@; restaurar regs modificables per rutina en C

		.global e9m22_is_nan_c
e9m22_is_nan_c:
		push {r1-r3, r12, lr}		@; salvar regs modificables per rutina en C
		bl   e9m22_is_nan_c_		@; cridar rutina en C
		pop  {r1-r3, r12, pc}		@; restaurar regs modificables per rutina en C

		.global e9m22_is_finite_c
e9m22_is_finite_c:
		push {r1-r3, r12, lr}		@; salvar regs modificables per rutina en C
		bl   e9m22_is_finite_c_		@; cridar rutina en C
		pop  {r1-r3, r12, pc}		@; restaurar regs modificables per rutina en C

		.global e9m22_is_negative_c
e9m22_is_negative_c:
		push {r1-r3, r12, lr}		@; salvar regs modificables per rutina en C
		bl   e9m22_is_negative_c_	@; cridar rutina en C
		pop  {r1-r3, r12, pc}		@; restaurar regs modificables per rutina en C




		.global e9m22_to_float_c
e9m22_to_float_c:
		push {r1-r3, r12, lr}		@; salvar regs modificables per rutina en C
		bl   e9m22_to_float_c_		@; cridar rutina en C
		pop  {r1-r3, r12, pc}		@; restaurar regs modificables per rutina en C

		.global float_to_e9m22_c
float_to_e9m22_c:
		push {r1-r3, r12, lr}		@; salvar regs modificables per rutina en C
		bl   float_to_e9m22_c_		@; cridar rutina en C
		pop  {r1-r3, r12, pc}		@; restaurar regs modificables per rutina en C

		.global e9m22_to_int_c
e9m22_to_int_c:
		push {r1-r3, r12, lr}		@; salvar regs modificables per rutina en C
		bl   e9m22_to_int_c_		@; cridar rutina en C
		pop  {r1-r3, r12, pc}		@; restaurar regs modificables per rutina en C


		.global int_to_e9m22_c
int_to_e9m22_c:
		push {r1-r3, r12, lr}		@; salvar regs modificables per rutina en C
		bl   int_to_e9m22_c_		@; cridar rutina en C
		pop  {r1-r3, r12, pc}		@; restaurar regs modificables per rutina en C





		.global e9m22_add_c
e9m22_add_c:
		push {r1-r3, r12, lr}		@; salvar regs modificables per rutina en C
		bl   e9m22_add_c_			@; cridar rutina en C
		pop  {r1-r3, r12, pc}		@; restaurar regs modificables per rutina en C


		.global e9m22_sub_c
e9m22_sub_c:
		push {r1-r3, r12, lr}		@; salvar regs modificables per rutina en C
		bl   e9m22_sub_c_			@; cridar rutina en C
		pop  {r1-r3, r12, pc}		@; restaurar regs modificables per rutina en C


		.global e9m22_mul_c
e9m22_mul_c:
		push {r1-r3, r12, lr}		@; salvar regs modificables per rutina en C
		bl   e9m22_mul_c_			@; cridar rutina en C
		pop  {r1-r3, r12, pc}		@; restaurar regs modificables per rutina en C


		.global e9m22_div_c
e9m22_div_c:
		push {r1-r3, r12, lr}		@; salvar regs modificables per rutina en C
		bl   e9m22_div_c_			@; cridar rutina en C
		pop  {r1-r3, r12, pc}		@; restaurar regs modificables per rutina en C


		.global e9m22_inv_c
e9m22_inv_c:
		push {r1-r3, r12, lr}		@; salvar regs modificables per rutina en C
		bl   e9m22_inv_c_			@; cridar rutina en C
		pop  {r1-r3, r12, pc}		@; restaurar regs modificables per rutina en C


		.global e9m22_neg_c
e9m22_neg_c:
		push {r1-r3, r12, lr}		@; salvar regs modificables per rutina en C
		bl   e9m22_neg_c_			@; cridar rutina en C
		pop  {r1-r3, r12, pc}		@; restaurar regs modificables per rutina en C


		.global e9m22_abs_c
e9m22_abs_c:
		push {r1-r3, r12, lr}		@; salvar regs modificables per rutina en C
		bl   e9m22_abs_c_			@; cridar rutina en C
		pop  {r1-r3, r12, pc}		@; restaurar regs modificables per rutina en C




		.global e9m22_compare_c
e9m22_compare_c:
		push {r1-r3, r12, lr}		@; salvar regs modificables per rutina en C
		bl   e9m22_compare_c_		@; cridar rutina en C
		pop  {r1-r3, r12, pc}		@; restaurar regs modificables per rutina en C




		.global e9m22_decompose_c
e9m22_decompose_c:
		push {r1-r3, r12, lr}		@; salvar regs modificables per rutina en C
		bl   e9m22_decompose_c_		@; cridar rutina en C
		pop  {r1-r3, r12, pc}		@; restaurar regs modificables per rutina en C


		.global e9m22_normalize_and_round_c
e9m22_normalize_and_round_c:
		push {r1-r3, r12, lr}		@; salvar regs modificables per rutina en C
		bl   e9m22_normalize_and_round_c_	@; cridar rutina en C
		pop  {r1-r3, r12, pc}		@; restaurar regs modificables per rutina en C


		.global e9m22_round_c
e9m22_round_c:
		push {r1-r3, r12, lr}		@; salvar regs modificables per rutina en C
		bl   e9m22_round_c_			@; cridar rutina en C
		pop  {r1-r3, r12, pc}		@; restaurar regs modificables per rutina en C




		.global count_leading_zeros_c
count_leading_zeros_c:
		push {r1-r3, r12, lr}		@; salvar regs modificables per rutina en C
		bl   count_leading_zeros_c_		@; cridar rutina en C
		pop  {r1-r3, r12, pc}		@; restaurar regs modificables per rutina en C


		.global count_trailing_zeros_c
count_trailing_zeros_c:
		push {r1-r3, r12, lr}		@; salvar regs modificables per rutina en C
		bl   count_trailing_zeros_c_	@; cridar rutina en C
		pop  {r1-r3, r12, pc}		@; restaurar regs modificables per rutina en C




@; umul32x32_64():	multiplica 2 operands naturals de 32 bits
@;					i retorna el resultat de 64 bits.
@; declaració en C:
@; 		unsigned long long umul32x32_64(unsigned int a, unsigned int b);

        .global umul32x32_64
umul32x32_64:
				@; ús de registres:
				@; r0: paràmetre a / resultat (32 bits baixos)
				@; r1: paràmetre b / resultat (32 bits alts)
				@; r2: còpia del paràmetre a
        push {r2, lr}
        
        mov r2, r0
        umull r0, r1, r2, r1	@; r0(low):r1(hi) = a*b (64 bits)
        
        pop {r2, pc}


@; umul32x32_2x32(): multiplica 2 operands naturals de 32 bits
@;					 i retorna el resultat de 64 bits.
@; declaració en C:
@; 		void umul32x32_2x32(unsigned int a, unsigned int b,
@;							unsigned int *mulLow, unsigned int *mulHigh );
        .global umul32x32_2x32
umul32x32_2x32:
				@; ús de registres:
				@; r0: paràmetre a
				@; r1: paràmetre b
				@; r2: @resultat (32 bits baixos)
				@; r3: @resultat (32 bits alts)
				@; r4: resultat a*b (32 bits baixos)
				@; r5: resultat a*b (32 bits alts)
        push {r4, r5, lr}
        
        umull r4, r5, r0, r1	@; r4(low):r5(hi) = a*b (64 bits)
        str r4, [r2]			@; desar 32 bits baixos d'a*b
        str r5, [r3]			@; desar 32 bits alts d'a*b
        
        pop {r4, r5, pc}

.end

