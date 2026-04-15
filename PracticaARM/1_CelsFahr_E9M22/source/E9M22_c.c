/*------------------------------------------------------------------
|   E9M22_c.c: operacions amb números en format Coma Flotant E9M22. 
| ------------------------------------------------------------------
|	pere.millan@urv.cat
|	(Febrer-Març 2025, Febrer-Març 2026)
| ------------------------------------------------------------------
| © URV — Codi de la pràctica d'ARM de Fonaments de Computadors (FC).
| Ús exclusiu dins l’assignatura; prohibida la seva redistribució.
| Consulteu la llicència: ACADEMIC_LICENSE_URV_FC.txt
| ------------------------------------------------------------------*/

#include "E9M22.h"	/* e9m22_t: tipus Coma Flotant E9M22
                       MAKE_E9M22(real): codifica un valor real en format E9M22
                       E9M22_MASK_SIGN: màscara per obtenir el bit de signe
                    */
#include "divmod.h"		/* rutina div_mod() de divisió entera */


/************************************************************/
/* Rutines de CLASSIFICACIÓ de valors en coma flotant E9M22 */
/************************************************************/


/*------------------------------------------------------------------------------
	e9m22_classify(): indica la classe de número E9M22.
------------------------------------------------------------------------------*/

/* 🤖🤖🤖🤖🤖🤖🤖🤖🤖🤖 IAgen_01 INICI 🤖🤖🤖🤖🤖🤖🤖🤖🤖🤖 */

e9m22_class_t e9m22_classify_c_(e9m22_t num)
{
	unsigned int exponent, mantissa;
	e9m22_class_t resultat;

	exponent = num & E9M22_MASK_EXP;

		// Primer detectem Normals (potser els més habituals?)
	if ( (exponent != 0) && (exponent != E9M22_MASK_EXP) )
	{
		resultat = E9M22_CLASS_NORMAL;
	}
	else
	{
		mantissa = num & E9M22_MASK_FRAC;

		if (exponent == 0)
		{	// Pot ser denormal o zero
			if (mantissa != 0)
				resultat = E9M22_CLASS_DENORMAL;
			else
				resultat = E9M22_CLASS_ZERO;
		}
		else	// Pot ser infinit o NaN
		{
			if (mantissa == 0)
				resultat = E9M22_CLASS_INFINITE;
			else
				resultat = E9M22_CLASS_NAN;
		}
	}

	return resultat;
}
/* 🤖🤖🤖🤖🤖🤖🤖🤖🤖🤖 IAgen_01 FINAL 🤖🤖🤖🤖🤖🤖🤖🤖🤖🤖 */



/*------------------------------------------------------------------------------
	e9m22_is_normal(): retorna un valor ≠0 si num és de classe NORMAL E9M22
						(bits d'exponent ni tot 0's ni tot 1's).
------------------------------------------------------------------------------*/
int e9m22_is_normal_c_(e9m22_t num)
{
	unsigned int exponent;
	int resultat;

	exponent = num & E9M22_MASK_EXP;

		// Els Normals no tenen exponent tot 0's ni tot 1's
	if ( (exponent != 0) && (exponent != E9M22_MASK_EXP) )
		resultat = 0xFC2026;	// Sí és normal (true: valor ≠0)
	else
		resultat = 0;	// No és normal (false: valor =0)

	return resultat;
}


/*------------------------------------------------------------------------------
	e9m22_is_denormal(): retorna un valor ≠0 si num és de classe DENORMAL E9M22
						(bits exponent tots 0's i algun bit de fracció ≠0).
------------------------------------------------------------------------------*/
int e9m22_is_denormal_c_(e9m22_t num)
{
	unsigned int exponent, mantissa;
	int resultat;

	exponent = num & E9M22_MASK_EXP;
	mantissa = num & E9M22_MASK_FRAC;

		// Els denormals tenen exponent tot 0's i mantissa ≠0
	if ( (exponent == 0) && (mantissa != 0) )
		resultat = 0xFC2026;	// Sí és denormal (true: valor ≠0)
	else
		resultat = 0;	// No és denormal (false: valor =0)

	return resultat;
}


/*------------------------------------------------------------------------------
	e9m22_is_zero(): retorna un valor ≠0 si num codifica un valor ±0,0 E9M22
					(bits d'exponent i de fracció tots 0's).
------------------------------------------------------------------------------*/
int e9m22_is_zero_c_(e9m22_t num)
{
	unsigned int exponent, mantissa;
	int resultat;

	exponent = num & E9M22_MASK_EXP;
	mantissa = num & E9M22_MASK_FRAC;

		// Els valors zero tenen exponent i mantissa tots dos a 0's
	if ( (exponent == 0) && (mantissa == 0) )
		resultat = 0xFC2026;	// Sí és zero (true: valor ≠0)
	else
		resultat = 0;	// No és zero (false: valor =0)

	return resultat;
}


/*------------------------------------------------------------------------------
	e9m22_is_infinite(): retorna un valor ≠0 si num codifica un valor ±∞ E9M22
						(bits d'exponent tots 1's i bits de fracció tots 0's).
------------------------------------------------------------------------------*/
int e9m22_is_infinite_c_(e9m22_t num)
{
	unsigned int exponent, mantissa;
	int resultat;

	exponent = num & E9M22_MASK_EXP;
	mantissa = num & E9M22_MASK_FRAC;

		// Els infinits tenen exponent tot 1's i mantissa =0
	if ( (exponent == E9M22_MASK_EXP) && (mantissa == 0) )
		resultat = 0xFC2026;	// Sí és infinit (true: valor ≠0)
	else
		resultat = 0;	// No és infinit (false: valor =0)

	return resultat;
}


/*------------------------------------------------------------------------------
	e9m22_is_nan(): retorna un valor ≠0 si num codifica un valor NaN E9M22
					(bits d'exponent tots 1's i algun bit de fracció ≠0).
------------------------------------------------------------------------------*/
int e9m22_is_nan_c_(e9m22_t num)
{
	unsigned int exponent, mantissa;
	int resultat;

	exponent = num & E9M22_MASK_EXP;
	mantissa = num & E9M22_MASK_FRAC;

		// Els NaN's tenen exponent tot 1's i mantissa ≠0
	if ( (exponent == E9M22_MASK_EXP) && (mantissa != 0) )
		resultat = 0xFC2026;	// Sí és NaN (true: valor ≠0)
	else
		resultat = 0;	// No és Nan (false: valor =0)

	return resultat;
}


/*------------------------------------------------------------------------------
	e9m22_is_finite(): retorna un valor ≠0 si num és un valor ni ±∞, ni NaN
						(algun bit d'exponent ≠1: normal, denormal o zero).
------------------------------------------------------------------------------*/
int e9m22_is_finite_c_(e9m22_t num)
{
	unsigned int exponent;
	int resultat;

	exponent = num & E9M22_MASK_EXP;

		// Els Finits no tenen l'exponent tot a 1's
	if ( exponent != E9M22_MASK_EXP )
		resultat = 0xFC2026;	// Sí és finit (true: valor ≠0)
	else
		resultat = 0;	// No és finit (false: valor =0)

	return resultat;
}


/*------------------------------------------------------------------------------
	e9m22_is_negative(): retorna un valor ≠0 si num és negatiu
						(bit de signe =1).
------------------------------------------------------------------------------*/
int e9m22_is_negative_c_(e9m22_t num)
{
	unsigned int signe;
	int resultat;

	signe = num & E9M22_MASK_SIGN;

		// Els negatius tenen signe 1
	if ( signe == E9M22_MASK_SIGN )
		resultat = 0xFC2026;	// Sí és negatiu (true: valor ≠0)
	else
		resultat = 0;	// No és negatiu (false: valor =0)

	return resultat;

}



/******************************************************/
/* Rutines de CONVERSIÓ de valors E9M22 <-> float/int */
/******************************************************/

    /* Paràmetres del tipus C float (IEEE754 binary32) */
#define FLOAT_e		8				/* quantitat de bits d'exponent: 8 */
#define FLOAT_m		(31-FLOAT_e)	/* quantitat de bits de mantissa/significand: 23 */
#define FLOAT_bias	((1<<( FLOAT_e - 1))-1)	/* fracció/bias/excés: 127 */

    /* MÀSCARES per als camps de bits de valors float */
#define FLOAT_MASK_FRAC	((1<<FLOAT_m)-1)	/* bits 22..0:	fracció/mantissa/significand 0x007FFFFF */
#define FLOAT_MASK_EXP	(0x7FFFFFFF ^ FLOAT_MASK_FRAC)	
                                            /* bits 30..23:	exponent (en excés 127) */
#define FLOAT_MASK_SIGN	(1<<31)				/* bit 31:		signe 0x80000000 */
#define FLOAT_1_IMPLICIT_NORMAL (1<<FLOAT_m)	/* 1 implícit als valors normals */

    /* Exponents màxim i mínim del tipus C float (IEEE754 binary32) */
#define FLOAT_EXP_MIN	(1-FLOAT_bias)
#define FLOAT_EXP_MAX	FLOAT_bias



/*------------------------------------------------------------------------------
	e9m22_to_float(): converteix un valor E9M22 a float.
------------------------------------------------------------------------------*/
float e9m22_to_float_c_(e9m22_t num)
{
    union { float f; uint32_t u; } resultat;
    unsigned int signe;
    int exponent, despl;

    signe = num & E9M22_MASK_SIGN;

    if ( e9m22_is_nan(num) )
    {	// generar NaN mantenint payload i bits q/s
        resultat.u = signe | FLOAT_MASK_EXP 
                    | ((num & 0x00300000) << 1) | (num & 0x0000FFFF);
    }
    else
    {
        exponent = ((num & E9M22_MASK_EXP) >> E9M22_m) - E9M22_bias;
        if ( e9m22_is_infinite(num) || (exponent > FLOAT_EXP_MAX) )
        {	// Infinit o exponent massa gran (overflow)
            resultat.u = signe | FLOAT_MASK_EXP;	// Generar codificació ±∞
        }
        else
        {
            if ( e9m22_is_zero(num) || (exponent < FLOAT_EXP_MIN) )
            {	// Zero o exponent massa petit (denormal/underflow)
                if ( exponent >= (FLOAT_EXP_MIN - FLOAT_m) )
                {
                    despl = -exponent + FLOAT_EXP_MIN -1;
                    resultat.u = signe | 
                                ( (E9M22_1_IMPLICIT_NORMAL | (num & E9M22_MASK_FRAC) ) >> despl );
                }
                else
                {	// Underflow --> zero
                    resultat.u = signe;
                }
            }
            else
            {	// Normalitzat
                resultat.u = signe | ((exponent + FLOAT_bias) << FLOAT_m) 
                            | ((num <<1) & FLOAT_MASK_FRAC) ;
            }
        }
    }

    return resultat.f;
}



/*------------------------------------------------------------------------------
	float_to_e9m22(): converteix un valor float a E9M22.
------------------------------------------------------------------------------*/
e9m22_t float_to_e9m22_c_(float num)
{
    e9m22_t resultat;
    unsigned int signe, bits_float;
    int exponent, leading_zeros;

    bits_float = FLOAT_TO_BITS(num);
    signe = bits_float & FLOAT_MASK_SIGN;
    
    if ( (bits_float & FLOAT_MASK_EXP) == FLOAT_MASK_EXP )	/* Infinit o NaN */
    {
        if ( (bits_float & FLOAT_MASK_FRAC) != 0 )	/* NaN, mantenint bits q/s i payload */
        {
            resultat = signe | E9M22_MASK_EXP 
                        | ( (bits_float & 0x00600000) >> 1)	// reposicionar bits qNaN/sNaN
                        | (bits_float & 0x000FFFFF) ;		// possible payload NaN
        }
        else	/* Infinit (amb signe) */
        {
            resultat = signe | E9M22_MASK_EXP; // Bits fracció/mantissa/significand a 0
        }
    }
    else	/* Normal, Zero o Denormal */
    {
        if ( (bits_float & FLOAT_MASK_EXP) != 0 )	/* Normalitzat */
        {
                /* Calcular nou exponent codificat */
            exponent = ((bits_float & FLOAT_MASK_EXP) >> FLOAT_m ) - FLOAT_bias + E9M22_bias;
            resultat = signe | (exponent << E9M22_m ) | ((bits_float >> 1) & E9M22_MASK_FRAC) ;
        }
        else	/* Denormal o Zero */
        {
            if ( (bits_float & FLOAT_MASK_FRAC) == 0 )	/* ±Zero */
            {
                resultat = bits_float;	/* El ±Zero es codifica igual en els 2 formats */
            }
            else	/* Denormal. Codificable com a Normal en E9M22 */
            {
                exponent = FLOAT_EXP_MIN;	// -126
                resultat = bits_float & FLOAT_MASK_FRAC;
                leading_zeros = count_leading_zeros_c_(resultat);	/* On es troba el 1r bit a 1? */
                resultat <<= (leading_zeros - 8);	/* Moure 1r bit a 1 a posició de bit implícit normalitzat */
                exponent -= (leading_zeros - 8);	/* Ajustar valor de l'exponent */
                resultat = signe 		/* Combinar signe + exponent + fracció */
                            | ((exponent + E9M22_bias) << E9M22_m ) 
                            | (resultat & E9M22_MASK_FRAC);
            }
        }
    }

    return resultat;
}



/*------------------------------------------------------------------------------
	e9m22_to_int(): converteix un valor E9M22 a int.
------------------------------------------------------------------------------*/
int e9m22_to_int_c_(e9m22_t num)
{
    int exponent, despl, resultat;

    exponent = ((num & E9M22_MASK_EXP) >> E9M22_m) - E9M22_bias ;

    if ( e9m22_is_finite(num) && (exponent <= 30) )
    {
        if ( e9m22_is_normal(num) && (exponent >= 0) )
        {
            resultat = E9M22_1_IMPLICIT_NORMAL | (num & E9M22_MASK_FRAC);

            if (exponent != E9M22_m)
            {
                if (exponent < E9M22_m)
                {
                    despl = (E9M22_m - exponent);
                    resultat = e9m22_round_c_(resultat, despl);
                    resultat >>= despl;
                }
                else
                    resultat <<= (exponent - E9M22_m);
            }
        }
        else /* Massa petit, Denormal o Zero --> 0 */
            resultat = 0;
    }
    else
    {	// num és NaN o ±∞ o massa gran: retorna valor màxim
        resultat = 0x7FFFFFFF;
    }

    if ( e9m22_is_negative(num) ) resultat = -resultat;

    return resultat;
}



/*------------------------------------------------------------------------------
	int_to_e9m22(): converteix un valor int a E9M22.
------------------------------------------------------------------------------*/
e9m22_t int_to_e9m22_c_(int num)
{
    e9m22_t resultat;
    int exponent;
    unsigned int signe = 0, leading_zeros, lz2, despl;

    if ( num < 0)
    {
        num = -num;
        signe = E9M22_MASK_SIGN;
    }

    leading_zeros = count_leading_zeros_c_(num);

    if (leading_zeros < 32)
    {
        exponent = 31 - leading_zeros;
        if (leading_zeros < E9M22_e)
        {	// Cal desplaçar a la dreta
            despl = E9M22_e - leading_zeros;
            resultat = e9m22_round_c_(num, despl);
            lz2 = count_leading_zeros_c_(resultat);
            if (lz2 < leading_zeros)
            {
                despl++;
                exponent++;
            }
            resultat >>= despl;
        }
        else if (leading_zeros > E9M22_e)
        {	// Cal desplaçar a l'esquerra
            despl = leading_zeros - E9M22_e;
            resultat = num << despl;
        }
        else
        {	// No cal desplaçar
            // exponent = E9M22_m;
			resultat = num;
        }
            // Combinar components E9M22:
        resultat = signe | ((exponent + E9M22_bias) << E9M22_m) | (resultat & E9M22_MASK_FRAC);
    }
    else
    {	// És el zero (positiu)
        resultat = 0;
    }

    return resultat;
}



/*************************************************/
/* Operacions ARITMÈTIQUES en Coma Flotant E9M22 */
/*************************************************/

/*------------------------------------------------------------------------------
  e9m22_add():	calcula i retorna la suma dels 2 operands,
                (num1 + num2) codificats en coma flotant E9M22.

  # Casos "especials" amb la suma:
    • Si algun operand és NaN, la suma és NaN
    • +∞ + +∞ = +∞ / -∞ + -∞ = -∞ / +∞ + -∞ = NaN
    • ±∞ + finit = ±∞
------------------------------------------------------------------------------*/
e9m22_t e9m22_add_c_(e9m22_t num1, e9m22_t num2)
{
    e9m22_t suma;
    unsigned int signe1, signe2, signe_suma, despl, dif_exp;
    int exp1, exp2, exp_suma, mant1, mant2, mant_suma;
	e9m22_class_t class1, class2;
	signed short exp_sh1, exp_sh2;
	unsigned int mant_nat1, mant_nat2;

		/* Obtenir components i classe dels 2 operands */
	class1 = e9m22_decompose(num1, &signe1, &exp_sh1, &mant_nat1);
	class2 = e9m22_decompose(num2, &signe2, &exp_sh2, &mant_nat2);

    if ( e9m22_is_finite(num1) && e9m22_is_finite(num2) )
    {
        if ( (class1 != E9M22_CLASS_ZERO) && (class2 != E9M22_CLASS_ZERO) )
        {	/* num1 i num2 són normals o denormals */

			exp1 = exp_sh1; mant1 = mant_nat1;
			exp2 = exp_sh2; mant2 = mant_nat2;

            /* Exponents diferents? Cal ALINEAR mantisses? */
            if ( exp1 != exp2 )
            {
                if (exp1 < exp2)
                {
                    dif_exp = exp2 - exp1;
                    if (dif_exp < E9M22_e)
                    {	/* Alinear despl. esq. mant2 */
                        mant2 <<= dif_exp;
                        exp2 -= dif_exp;
                    }
                    else
                    {	/* Alinear despl. esq. mant2 ... */
                        despl = E9M22_e - 1;
                        mant2 <<= despl;
                        exp2 -= despl;

                        /* ... i despl. drt. mant1 */
                        dif_exp = exp2 - exp1;
                        mant1 >>= dif_exp;
                        exp1 += dif_exp;
                    }
                }
                else /* exp1 > exp2 */
                {
                    dif_exp = exp1 - exp2;
                    if (dif_exp < E9M22_e)
                    {	/* Alinear despl. esq. mant1 */
                        mant1 <<= dif_exp;
                        exp1 -= dif_exp;
                    }
                    else
                    {	/* Alinear despl. esq. mant1 ... */
                        despl = E9M22_e - 1;
                        mant1 <<= despl;
                        exp1 -= despl;

                        /* ... i despl. drt. mant2 */
                        dif_exp = exp1 - exp2;
                        mant2 >>= dif_exp;
                        exp2 += dif_exp;
                    }
                }
            }

            if (mant1 == 0)
                suma = num2;
            else if (mant2 == 0)
                suma = num1;
            else
            {	/* Sumar mantisses no 0, amb signe (Ca2) si tenen signe diferent */
                if (signe1 != signe2)
                {	/* Fer Ca2/negar el que sigui negatiu */
                    if ( e9m22_is_negative(num1) )
                        mant1 = -mant1;
                    else
                        mant2 = -mant2;
                }

                mant_suma = mant1 + mant2;	// Sumar mantisses (amb signe, Ca2)
                if (mant_suma < 0)
                {
                    mant_suma = -mant_suma;			// mantissa natural (no negativa)
                }

                    // Calcular signe resultat
                if ( (signe1 == signe2) || (e9m22_abs_c_(num1) >= e9m22_abs_c_(num2)) )
                    signe_suma = signe1;
                else
                    signe_suma = signe2;

                exp_suma = exp1; // serveix també exp2, estan alineats

                suma = e9m22_normalize_and_round(signe_suma, exp_suma, mant_suma);
            }
        }
        else
        {	/* num1 o num2 són zero */
            if ( e9m22_is_zero(num1) )
                suma = num2;
            else
                suma = num1;
        }
    }
    else /* num1 o num2 són NaN o ±∞ */
    {
        if ( class1 == E9M22_CLASS_NAN )
            suma = num1;	// NaN
        else if ( class2 == E9M22_CLASS_NAN )
            suma = num2;	// NaN
        else if ( class1 == E9M22_CLASS_INFINITE )
             {
                if ( class2 == E9M22_CLASS_INFINITE )
                {
                    if (signe1 == signe2)
                        suma = num1;		// ±∞ + ±∞ = ±∞
                    else
                        suma = E9M22_qNAN;	// +∞ + -∞ = NaN
                }
                else
                {
                    suma = num1;		// ±∞ + finit = ±∞
                }
             }
        else 
            suma = num2;		// finit + ±∞ = ±∞
    }

    return suma;
}


/*------------------------------------------------------------------------------
  e9m22_sub():	calcula i retorna la diferència dels 2 operands,
                (num1 - num2) codificats en coma flotant E9M22.
                Aplica la suma amb el 2n operand negat.
------------------------------------------------------------------------------*/
e9m22_t e9m22_sub_c_(e9m22_t num1, e9m22_t num2)
{
    e9m22_t resta, num2negat;

    num2negat = e9m22_neg_c_(num2);	/* ⚠️ Aplicant màscares seria més òptim */ 

        /* Restem sumant num2 negat a num1 */
    resta = e9m22_add_c_ (num1, num2negat);

    return resta;
}


/*------------------------------------------------------------------------------
  e9m22_mul():	calcula i retorna el producte dels 2 operands,
                (num1 × num2) codificats en coma flotant E9M22.

  # Casos "especials" amb el producte:
    • Si algun operand és NaN, el producte és NaN
    • ±∞ × ±∞ = ±∞ / ±∞ × ±0 = ±NaN
    • ±∞ × finit(≠0) = ±∞
------------------------------------------------------------------------------*/
e9m22_t e9m22_mul_c_(e9m22_t num1, e9m22_t num2)
{
    e9m22_t producte;
    unsigned int prod64lo, prod64hi;		// Resultat de la multiplicació (64 bits)
    unsigned int signe1, signe2, signe_prod, despl;
    unsigned int num_trailing_zeros, num_leading_zeros, sticky, mask_sticky;
    int exp1, exp2, exp_prod, mant1, mant2, mant_prod;
	e9m22_class_t class1, class2;
	signed short exp_sh1, exp_sh2;
	unsigned int mant_nat1, mant_nat2;

		/* Obtenir components i classe dels 2 operands */
	class1 = e9m22_decompose(num1, &signe1, &exp_sh1, &mant_nat1);
	class2 = e9m22_decompose(num2, &signe2, &exp_sh2, &mant_nat2);

    if (signe1 == signe2)		/* ⚠️ Hi ha implementacions més eficients 🤔 */
        signe_prod = 0;	// Positiu
    else
        signe_prod = E9M22_MASK_SIGN;	// Negatiu


    if ( e9m22_is_finite(num1) && e9m22_is_finite(num2) )
    {
        if ( (class1 != E9M22_CLASS_ZERO) && (class2 != E9M22_CLASS_ZERO) )
        {	/* num1 i num2 són normals o denormals */

			exp1 = exp_sh1; mant1 = mant_nat1;
			exp2 = exp_sh2; mant2 = mant_nat2;


            /* Ajustar mantisses per minimitzar bits resultat producte */
            num_trailing_zeros = count_trailing_zeros_c_(mant1);
            if (num_trailing_zeros > 0)
            {
                mant1 >>= num_trailing_zeros;	// Eliminar zeros per la dreta
                exp1 += num_trailing_zeros;		// Ajustar exponent 1
            }

            num_trailing_zeros = count_trailing_zeros_c_(mant2);
            if (num_trailing_zeros > 0)
            {
                mant2 >>= num_trailing_zeros;	// Eliminar zeros per la dreta
                exp2 += num_trailing_zeros;		// Ajustar exponent 2
            }

                // multiplicació de 64 bits, podem tenir fins a 2*23 = 46 bits
            umul32x32_2x32(mant1, mant2, &prod64lo, &prod64hi);
            exp_prod = exp1 + exp2 - E9M22_m;	// Exponent (ajustat) producte

            // Verificar si el resultat supera els 32 bits (32 bits alts != 0):
            if ( prod64hi !=0 )
            {
                num_leading_zeros = count_leading_zeros_c_(prod64hi);
                despl = 32 - num_leading_zeros;
                    /* Detectar si els bits que "es perdran" amb el desplaçament són 0 o no-0 */
                mask_sticky = (1<<despl) - 1;	// Màscara per accedir als bits que es "perdran"
                sticky = ((prod64lo & mask_sticky) != 0) ? 1 : 0;
                    /* Convertir resultat a 32 bits i ajustar exponent: */
                mant_prod = (prod64hi << num_leading_zeros) | (prod64lo >> despl);
                exp_prod += despl;
                    /* Afegir informació dels bits "perduts": */
                mant_prod |= sticky;
                
            }
            else	// El resultat de la multiplicació ja es troba als 32 bits baixos de prod64
            {
                mant_prod = prod64lo;	// Agafar els 32 bits baixos de prod64 (els 32 alts són 0)
            }

            producte = e9m22_normalize_and_round(signe_prod, exp_prod, mant_prod);
        }
        else
        {	/* num1 o num2 són zero */
            producte = signe_prod;	// ±0
        }
    }
    else /* num1 o num2 són NaN o ±∞ */
    {
        if ( class1 == E9M22_CLASS_NAN )
            producte = num1;	// NaN
        else if ( class2 == E9M22_CLASS_NAN )
            producte = num2;	// NaN
        else if ( class1 == E9M22_CLASS_INFINITE )
             {
                if ( class2 == E9M22_CLASS_ZERO )
                {		// ±∞ × ±0 = ±NaN
                    producte = signe_prod | E9M22_qNAN;
                }
                else
                {		// ±∞ × (≠0) = ±∞
                    producte = signe_prod | E9M22_INF_POS;
                }
             }
        else // num2 = ±∞
             {
                if ( class1 == E9M22_CLASS_ZERO )
                {		// ±∞ × ±0 = ±NaN
                    producte = signe_prod | E9M22_qNAN;
                }
                else
                {		// (≠0) × ±∞ = ±∞
                    producte = signe_prod | E9M22_INF_POS;
                }
             }
    }

    return producte;
}


/*------------------------------------------------------------------------------
  e9m22_div():	calcula i retorna la divisió dels 2 operands,
                (num1 ÷ num2) codificats en coma flotant E9M22.
				[ Implementació "indirecta": div(a, b) = mul(a, inv(b)) ]

  # Casos "especials" amb la divisió:
    • Si algun operand és NaN, la divisió és NaN
    • ±∞ ÷ ±∞ / ±0 ÷ ±0 = ±NaN
    • num ÷ ±∞ = ±0
    • ±∞ ÷ num = ±∞
    • finit(≠0) ÷ ±0 = ±∞
------------------------------------------------------------------------------*/
e9m22_t e9m22_div_c_(e9m22_t num1, e9m22_t num2)
{
    e9m22_t invers2, divisio;

		/* Calcular 1/num2 */
	invers2 = e9m22_inv(num2);

		/* a/b = a*(1/b) */
	divisio = e9m22_mul(num1, invers2); 

    return divisio;
}


/*------------------------------------------------------------------------------
  e9m22_inv():	calcula i retorna l'invers de l'operand,
                (1,0 ÷ num) codificat en coma flotant E9M22.

  # Casos "especials" amb la inversa:
    • 1,0 ÷ NaN = NaN
    • 1,0 ÷ ±∞  = ±0
    • 1,0 ÷ ±0  = ±∞
------------------------------------------------------------------------------*/
e9m22_t e9m22_inv_c_(e9m22_t num)
{
    e9m22_t inversa;
    unsigned int quo, res;	// Quocient i residu de div_mod()
    unsigned int signe_div;
    unsigned int num_trailing_zeros, num_leading_zeros;
    int exp, exp_div;
	unsigned int mant, mant_div;
	e9m22_class_t class;
	signed short exp_sh;
	unsigned int mant_nat;

		/* Obtenir components i classe de l'operand */
	class = e9m22_decompose(num, &signe_div, &exp_sh, &mant_nat);


    if ( e9m22_is_finite(num) )
    {
        if ( class != E9M22_CLASS_ZERO )
        {	/* num és normal o denormal */

			exp = exp_sh;
			mant = mant_nat;


            /* Ajustar mantissa per maximitzar bits resultat divisió:
                denominador mínim (lsr) */
            num_trailing_zeros = count_trailing_zeros_c_(mant);
            if (num_trailing_zeros > 0)
            {
                mant >>= num_trailing_zeros;	// Eliminar zeros per la dreta
                exp += num_trailing_zeros;		// Ajustar exponent
            }

            div_mod(0x80000000, mant, &quo, &res);	// Calcular 1,0 / num
            mant_div = quo;

            exp_div = -exp + E9M22_m - E9M22_e;	// Exponent (ajustat) divisio


			if (res > 0) // divisió no exacta, preparar informació arrodoniment
			{
				num_leading_zeros = count_leading_zeros(mant_div);
				if (num_leading_zeros >=2)	// Hi lloc per a bits Round i Sticky?
				{
					mant_div = mant_div << 2;	// Fer espai per als 2 bits
					exp_div = exp_div - 2;		// Ajustar exponent
				}
				
				// Usar residu per calcular bits Round & Sticky per a arrodoniment:
				if (mant <= res*2)	// Round bit = 1?
					mant_div = mant_div | 2;	// Activar Round bit

				if (mant < res*2)	// Sticky bit = 1?
					mant_div = mant_div | 1;	// Activar Sticky bit
			}

            inversa = e9m22_normalize_and_round(signe_div, exp_div, mant_div);
        }
        else
        {	/* num és zero: 1,0 ÷ ±0,0 = ±∞ */
			inversa = signe_div | E9M22_INF_POS;
        }
    }
    else  /* num és NaN o ±∞ */
    {
        if ( class == E9M22_CLASS_NAN )
            inversa = num;	// 1÷NaN = NaN
        else	/* num és ±∞: 1,0 ÷ ±∞ = ±0 */
            inversa = signe_div;	// ±0
    }

    return inversa;
}


/*------------------------------------------------------------------------------
  e9m22_neg():	canvia el signe (nega) de l'operand num
                codificat en coma flotant E9M22.
------------------------------------------------------------------------------*/
e9m22_t e9m22_neg_c_(e9m22_t num)
{
    e9m22_t resultat;

        /* ⚠️ Funciona, però hi ha formes més òptimes de fer-ho */ 
    if ( e9m22_is_negative(num) )
            /* Posar a 0 el bit de signe (negatiu → positiu) */
        resultat = num & ~E9M22_MASK_SIGN;
    else
            /* Posar a 1 el bit de signe (positiu → negatiu) */
        resultat = num | E9M22_MASK_SIGN;
    
    return resultat;
}


/*------------------------------------------------------------------------------
  e9m22_abs():	calcula i retorna el valor absolut
                de l'operand num codificat en coma flotant E9M22.
------------------------------------------------------------------------------*/
e9m22_t e9m22_abs_c_(e9m22_t num)
{
    e9m22_t resultat;

        /* ⚠️ Funciona, però hi ha formes més òptimes de fer-ho */ 
    if ( e9m22_is_negative(num) )
            /* Posar a 0 el bit de signe (negatiu → positiu) */
        resultat = num & ~E9M22_MASK_SIGN;
    else
            /* num ja és positiu */
        resultat = num;

    return resultat;
}


/***********************************************************/
/* Operació de COMPARACIÓ de números en Coma Flotant E9M22 */
/***********************************************************/

/* Regles generals d'ordenació de números en coma flotant:
    • NaN no és comparable amb res (ni amb un altre NaN: NaN ≠ NaN).
    • +0.0 = -0.0
    • -∞ < -Normals < -Denormals < -0.0 = +0.0 < +Denormals < +Normals < +∞
*/


/* e9m22_compare(): compara 2 valors E9M22.
                   Retorna un valor e9m22_cmp_t (=, >, <, no comparable/NaN).
*/
e9m22_cmp_t e9m22_compare_c_(e9m22_t num1, e9m22_t num2)
{
    e9m22_cmp_t resultat;
	e9m22_class_t class1, class2;
	unsigned int sign1, sign2;

	class1 = e9m22_classify(num1);
	class2 = e9m22_classify(num2);


    if ( (class1 != E9M22_CLASS_NAN) && (class2 != E9M22_CLASS_NAN) )
    {
		sign1 = num1 & E9M22_MASK_SIGN;
		sign2 = num2 & E9M22_MASK_SIGN;

		if (sign1 != sign2)
		{	// Signes diferents
			if ( (class1 == E9M22_CLASS_ZERO) && (class2 == E9M22_CLASS_ZERO) )
			{	// +0 = -0
				resultat = E9M22_CMP_EQUAL;
			}
			else
			{	// Negatiu < Positiu
				if (sign1 == 0)
				{	// +num1 > -num2
					resultat = E9M22_CMP_GREATER;
				}
				else
				{	// -num1 < +num2
					resultat = E9M22_CMP_LESS;
				}
			}
		}
		else	// Mateix signe
		{
			if (sign1 == 0)
			{	// Tots 2 nums POSITIUS
				if (num1 > num2)
					resultat = E9M22_CMP_GREATER;
				else if (num1 == num2)
					resultat = E9M22_CMP_EQUAL;
				else
					resultat = E9M22_CMP_LESS;
			}
			else
			{	// Tots 2 nums NEGATIUS (s'ordenen al revés)
				if (num1 > num2)
					resultat = E9M22_CMP_LESS;
				else if (num1 == num2)
					resultat = E9M22_CMP_EQUAL;
				else
					resultat = E9M22_CMP_GREATER;
			}
		}
    }
    else 
    {	// Algun operand NaN: no comparable
		resultat = E9M22_CMP_NC_NAN;
    }

    return resultat;
}




/**********************************************************/
/* Funcions auxiliars: DESCOMPOSICIÓ(split),              */
/*                     NORMALITZACIÓ i ARRODONIMENT E9M22 */
/**********************************************************/

/*------------------------------------------------------------------------------
  e9m22_decompose():
	Funció per obtenir els 3 componentes E9M22 (signe, exponent, mantissa) 
	i la classe de número E9M22 (e9m22_class_t).
------------------------------------------------------------------------------*/
e9m22_class_t e9m22_decompose_c_( e9m22_t num,	// Número E9M22 a descomposar
					unsigned int *sign_out,	// adreça on desar el signe (al MSB)
					signed short *exp_out,	// adreça on desar l'exponent real (sense excés)
					unsigned int *mant_out	// adreça on desar la mantissa (amb 1, implícit)
						// (si alguna adreça és 0/NULL, no s'escriu aquell component)
		)
			/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			Components retornats segons classe de número:
			• Normal: mantissa (amb 1, implícit); exponent que correspongui (sense excés/bias) 
			• Denormal: mantissa que correspongui; exponent E9M22_Emin (-254)
			• Zero: mantissa 0; exponent 0
			• Infinit: mantissa 0; exponent 511
			• NaN: mantissa≠0 (payload); exponent 511
			▸▸▸ El bit de signe no depèn de la classe de número E9M22.
			++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
{
	unsigned int signe, mantissa;
	signed short exponent;
	e9m22_class_t resultat;

	if (sign_out != 0)
	{
		// Obtenir i desar signe
		signe = num & E9M22_MASK_SIGN;
		*sign_out = signe;
	}

	exponent = (num & E9M22_MASK_EXP) >> E9M22_m;
	mantissa = num & E9M22_MASK_FRAC;

		// Processar exponent
	if (exponent == 0)
	{	// Zero o denormal
		if (mantissa != 0)
		{	// Denormal
			resultat = E9M22_CLASS_DENORMAL;
			exponent = E9M22_Emin;	// -254
			// La mantissa ja és correcta
		}
		else
		{	// Zero
			// L'exponent i matissa ja són correctes
			resultat = E9M22_CLASS_ZERO;
		}
	}
	else 
#define EXP_TOT_1s ((1<<E9M22_e)-1)
	if (exponent != EXP_TOT_1s )	// Normal
	{
			// Afegir 1 implícit a la mantissa:
		mantissa = mantissa | E9M22_1_IMPLICIT_NORMAL;
			// Obtenir exponent real restant el bias:
		exponent = exponent - E9M22_bias;
		resultat = E9M22_CLASS_NORMAL;
	}
	else	// Infinit o NaN
	{
		// L'exponent i matissa ja són correctes
		if (mantissa == 0)
			resultat = E9M22_CLASS_INFINITE;
		else
			resultat = E9M22_CLASS_NAN;
	}

	if (exp_out != 0)
	{
		// Desar exponent
		*exp_out = exponent;
	}

	if (mant_out != 0)
	{
		// Desar mantissa
		*mant_out = mantissa;
	}

	return resultat;
}


/*------------------------------------------------------------------------------
  e9m22_normalize_and_round():
	Funció per normalitzar i arrodonir al més proper.
------------------------------------------------------------------------------*/
e9m22_t e9m22_normalize_and_round_c_(
            unsigned int signe, int exponent, unsigned int mantissa)
{
    e9m22_t resultat;
    unsigned leading_zeros, lz2, despl;

    leading_zeros = count_leading_zeros_c_(mantissa); 

    if (leading_zeros < 32)
    {
        exponent += E9M22_e - leading_zeros;
        if (leading_zeros < E9M22_e)
        {	// Cal desplaçar a la dreta
            despl = E9M22_e - leading_zeros;
            resultat = e9m22_round_c_(mantissa, despl);
            lz2 = count_leading_zeros_c_(resultat);
            if (lz2 < leading_zeros)
            {
                despl++;
                exponent++;
            }
            resultat >>= despl;
        }
        else if (leading_zeros > E9M22_e)
        {	// Cal desplaçar a l'esquerra
            despl = leading_zeros - E9M22_e;
            resultat = mantissa << despl;
        }
        else
        {	// No cal desplaçar
            // exponent = E9M22_m;
            resultat = mantissa;
        }
        
        // Exponent fora de rang?
        if (exponent > E9M22_Emax)
        {	// Overflow: ±∞
            resultat = signe | E9M22_MASK_EXP;
        }
        else if ( exponent < E9M22_Emin )
        {	// Underflow: Denormal o ±zero
            despl = E9M22_Emin - exponent;
            resultat = signe | (mantissa >> despl);
        }
        else
        {
            // Normalitzat, combinar components E9M22:
            resultat = signe | ((exponent + E9M22_bias) << E9M22_m) | (resultat & E9M22_MASK_FRAC);
        }
    }
    else
    {	// És el ±zero 
        resultat = signe;
    }

    return resultat;
}



/*------------------------------------------------------------------------------
  e9m22_round():
  Arrodoniment al més proper. Si es troba al mig, al més proper parell. 
  Retorna la mantissa arrodonida (+1) o l'original (trunc) sense desplaçar.
------------------------------------------------------------------------------*/
unsigned int e9m22_round_c_(unsigned int mantissa, unsigned int nbits_to_shift_right)
{
    unsigned int resultat, mask_guard, mask_round, mask_sticky;
    unsigned char guard, round, sticky; // LSB mantissa i els 2 següents bits a la seva dreta

    resultat = mantissa;

    if (nbits_to_shift_right >= 2)
    {
        mask_guard = 1 << nbits_to_shift_right;	// El bit de menys pes del valor normalitzat
        guard = ((mantissa & mask_guard) != 0) ? 1 : 0;
        mask_round = mask_guard >> 1;	// El bit a la dreta del de menys pes del valor normalitzat
        round = ((mantissa & mask_round) != 0) ? 1 : 0;
        mask_sticky = mask_round - 1;	// La resta de bits a la dreta del "round bit"
        sticky = ((mantissa & mask_sticky) != 0) ? 1 : 0;
        if ( round && ( guard || sticky) )	// cal arrodonir amunt?
        {
            resultat += mask_guard;
        }
    }
    else
        if (nbits_to_shift_right == 1)	// No hi ha sticky bit (es considera 0)
        {
            if ( (mantissa&3) == 3)	// cal arrodonir amunt?
            {
                resultat += 2;
            }
        }
        else // no cal desplaçar (ni arrodonir)
        {
        }

    return resultat;	
}


/****************************************************************/
/* Funcions AUXILIARS per treballar amb els bits de codificació */
/****************************************************************/

/*------------------------------------------------------------------------------
  count_leading_zeros(): compta quants bits a 0 hi ha 
                          des del bit de més pes (esquerra).
------------------------------------------------------------------------------*/
unsigned int count_leading_zeros_c_(unsigned int num)
{
    unsigned int resultat;

            /* ⚠️ En assemblador es pot fer moltíssim més eficient */ 
    if ( num == 0)
        resultat = 32;
    else
    {
        resultat = 0;
        while ( (num & 0x80000000) == 0)
        {
            resultat++;
            num <<= 1;
        }
    }

    return resultat;
}

/*------------------------------------------------------------------------------
  count_trailing_zeros(): compta quants bits a 0 hi ha 
                           des del bit de menys pes (dreta).
------------------------------------------------------------------------------*/
unsigned int count_trailing_zeros_c_(unsigned int num)
{
    unsigned int resultat;

    if ( num == 0)
        resultat = 32;
    else
    {
        resultat = 0;
        while ( (num & 1) == 0)
        {
            resultat++;
            num >>= 1;
        }
    }

    return resultat;
}


