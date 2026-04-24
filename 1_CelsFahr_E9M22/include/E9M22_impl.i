@;--------------------------------------------------------------------
@;  Selecció implementació (C/ARM) rutines de coma flotant E9M22.
@;	HA DE COINCIDIR amb el seleccionat a E9M22_impl.h.
@;--------------------------------------------------------------------
@;	pere.millan@urv.cat
@;	(Març 2025, Febrer-Març 2026)
@;--------------------------------------------------------------------
@;	Programador/a 1: evelio.ruiz@estudiants.urv.cat
@;	Programador/a 2: yyy.yyy@estudiants.urv.cat
@;--------------------------------------------------------------------
@; © URV — Codi de la pràctica d'ARM de Fonaments de Computadors (FC).
@; Ús exclusiu dins l’assignatura; prohibida la seva redistribució.
@; Consulteu la llicència: ACADEMIC_LICENSE_URV_FC.txt
@;--------------------------------------------------------------------


@;#########################################################
@; Cal deixar DESCOMENTADA la versió de rutina a utilitzar
@;#########################################################


@;************************************************************
@;* Rutines de CLASSIFICACIÓ de valors en coma flotant E9M22 *
@;************************************************************

@;e9m22_classify	=	e9m22_classify_c	@; Versió en C
e9m22_classify	=	e9m22_classify_s	@; Versió assemblador

@;e9m22_is_normal	=	e9m22_is_normal_c	@; Versió en C
e9m22_is_normal	=	e9m22_is_normal_s	@; Versió assemblador

@;e9m22_is_denormal	=	e9m22_is_denormal_c	@; Versió en C
e9m22_is_denormal	=	e9m22_is_denormal_s	@; Versió assemblador

@;e9m22_is_zero	=	e9m22_is_zero_c	@; Versió en C
e9m22_is_zero	=	e9m22_is_zero_s	@; Versió assemblador

@;e9m22_is_infinite	=	e9m22_is_infinite_c	@; Versió en C
e9m22_is_infinite	=	e9m22_is_infinite_s	@; Versió assemblador

@;e9m22_is_nan	=	e9m22_is_nan_c	@; Versió en C
e9m22_is_nan	=	e9m22_is_nan_s	@; Versió assemblador

@;e9m22_is_finite	=	e9m22_is_finite_c	@; Versió en C
e9m22_is_finite	=	e9m22_is_finite_s	@; Versió assemblador

@;e9m22_is_negative	=	e9m22_is_negative_c	@; Versió en C
e9m22_is_negative	=	e9m22_is_negative_s	@; Versió assemblador


@;******************************************************
@;* Rutines de CONVERSIÓ de valors E9M22 <-> float/int *
@;******************************************************

e9m22_to_float	=	e9m22_to_float_c	@; Versió en C
@;e9m22_to_float	=	e9m22_to_float_s	@; Versió assemblador

float_to_e9m22	=	float_to_e9m22_c	@; Versió en C
@;float_to_e9m22	=	float_to_e9m22_s	@; Versió assemblador

e9m22_to_int	=	e9m22_to_int_c	@; Versió en C
@;e9m22_to_int	=	e9m22_to_int_s	@; Versió assemblador

int_to_e9m22	=	int_to_e9m22_c	@; Versió en C
@;int_to_e9m22	=	int_to_e9m22_s	@; Versió assemblador


@;*************************************************
@;* Operacions ARITMÈTIQUES en Coma Flotant E9M22 *
@;*************************************************

e9m22_add	=	e9m22_add_c	@; Versió en C
@;e9m22_add	=	e9m22_add_s	@; Versió assemblador

@;e9m22_sub	=	e9m22_sub_c	@; Versió en C
e9m22_sub	=	e9m22_sub_s	@; Versió assemblador

e9m22_mul	=	e9m22_mul_c	@; Versió en C
@;e9m22_mul	=	e9m22_mul_s	@; Versió assemblador

@;e9m22_div	=	e9m22_div_c	@; Versió en C
e9m22_div	=	e9m22_div_s	@; Versió assemblador

@;e9m22_inv	=	e9m22_inv_c	@; Versió en C
e9m22_inv	=	e9m22_inv_s	@; Versió assemblador

@;e9m22_neg	=	e9m22_neg_c	@; Versió en C
e9m22_neg	=	e9m22_neg_s	@; Versió assemblador

@;e9m22_abs	=	e9m22_abs_c	@; Versió en C
e9m22_abs	=	e9m22_abs_s	@; Versió assemblador


@;***********************************************************
@;* Operació de COMPARACIÓ de números en Coma Flotant E9M22 *
@;***********************************************************

@;e9m22_compare	=	e9m22_compare_c	@; Versió en C
e9m22_compare	=	e9m22_compare_s	@; Versió assemblador


@;**********************************************************
@;* Funcions auxiliars: NORMALITZACIÓ i ARRODONIMENT E9M22 *
@;**********************************************************

@;e9m22_decompose	=	e9m22_decompose_c	@; Versió en C
e9m22_decompose	=	e9m22_decompose_s	@; Versió assemblador

e9m22_normalize_and_round	=		e9m22_normalize_and_round_c	@; Versió en C
@;e9m22_normalize_and_round	=	e9m22_normalize_and_round_s	@; Versió assemblador

e9m22_round	=	e9m22_round_c	@; Versió en C
@;e9m22_round	=	e9m22_round_s	@; Versió assemblador


@;****************************************************************
@;* Funcions AUXILIARS per treballar amb els bits de codificació *
@;****************************************************************

count_leading_zeros	=	count_leading_zeros_c	@; Versió en C
@;count_leading_zeros	=	count_leading_zeros_s	@; Versió assemblador

count_trailing_zeros	=	count_trailing_zeros_c	@; Versió en C
@;count_trailing_zeros	=	count_trailing_zeros_s	@; Versió assemblador



