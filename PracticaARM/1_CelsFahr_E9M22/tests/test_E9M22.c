/*--------------------------------------------------------------------
|   Testing E9M22 routines;
| --------------------------------------------------------------------
|	pere.millan@urv.cat
|	santiago.romani@urv.cat
|	(Març 2021-2025, Febrer-Març 2026)
| --------------------------------------------------------------------
|	Programador/a 1: xxx.xxx@estudiants.urv.cat
|	Programador/a 2: yyy.yyy@estudiants.urv.cat
| --------------------------------------------------------------------
| © URV — Codi de la pràctica d'ARM de Fonaments de Computadors (FC).
| Ús exclusiu dins l’assignatura; prohibida la seva redistribució.
| Consulteu la llicència: ACADEMIC_LICENSE_URV_FC.txt
|---------------------------------------------------------------------*/

#include "E9M22.h"	/* E9M22: tipus Coma Flotant E9M22, 
					   operacions, valors.
                    */
#include "divmod.h"				/* rutina div_mod() de divisió entera */

#define MAX_ERROR_PERCENTAGE 2	/* 2% maximum allowable error percentage */

#define NSET(real)	( 0x80000000 | (real) )	// Macro per posar en negatiu un valor real

	/* Booleans */
typedef int bool;	// Tipus "bool", equivalent a int
#define cert (0==0)	// valor booleà cert
#define fals (0!=0)	// valor booleà fals


enum e9m22_auxs {nr, rnd, clz, ctz, }; // Auxiliars: to-do

/*****************************************************************************/
/* type definitions of the structured records that hold the test case values */
/*****************************************************************************/

	/* e9m22_classify test cases struct */
typedef struct {
    e9m22_t			num;		/* input parameter of the operation */
    e9m22_class_t 	xresult;	/* Expected result of the operation */
} test_classify_struct;

	/* e9m22_is_xxxx checks (result: 0/non-0) */
typedef struct {
    e9m22_t num;		/* input parameter of the operation */
    bool    xresult;	/* Expected result of the operation */
} test_is_checks_struct;

	/* e9m22_to_float test cases struct */
typedef struct {
    e9m22_t	num;		/* input parameter of the operation */
    float	xresult;	/* Expected result of the operation */
} test_e2f_struct;

	/* float_to_e9m22 test cases struct */
typedef struct {
    float	num;		/* input parameter of the operation */
    e9m22_t	xresult;	/* Expected result of the operation */
} test_f2e_struct;

	/* e9m22_to_int test cases struct */
typedef struct {
    e9m22_t	num;		/* input parameter of the operation */
    int		xresult;	/* Expected result of the operation */
} test_e2i_struct;

	/* int_to_e9m22 test cases struct */
typedef struct {
    int		num;		/* input parameter of the operation */
    e9m22_t	xresult;	/* Expected result of the operation */
} test_i2e_struct;

	/* E9M22 binary operations (2 input parameters/arguments) */
typedef struct {
    e9m22_t num1;		/* 1st input parameter of the operation */
    e9m22_t num2;		/* 2nd input parameter of the operation */
    e9m22_t xresult;	/* Expected result of the operation */
} test_ops2_struct;

	/* E9M22 unary operations (1 input parameter/argument) */
typedef struct {
    e9m22_t num;		/* input parameter of the operation */
    e9m22_t xresult;	/* Expected result of the operation */
} test_ops1_struct;

	/* E9M22 compare operation (result: 0/non-0) */
typedef struct {
    e9m22_t num1;			/* 1st input parameter of the operation */
    e9m22_t num2;			/* 2nd input parameter of the operation */
    e9m22_cmp_t xresult;	/* Expected result of the operation */
} test_comp_struct;

	/* e9m22_decompose test cases struct */
typedef struct {
    e9m22_t			num;		/* input number to decompose */
    e9m22_class_t 	xclass;		/* Expected class of the number */
    unsigned int 	xsign;		/* Expected sign of the number */
    signed short 	xexp;		/* Expected exponent of the number */
    unsigned int 	xmant;		/* Expected mantisa of the number */
} test_decompose_struct;



    /* Paràmetres del tipus C float (IEEE754 binary32) */
#define FLOAT_e		8				/* quantitat de bits d'exponent: 8 */
#define FLOAT_m		(31-FLOAT_e)	/* quantitat de bits de mantissa/significand: 23 */
#define FLOAT_bias	((1<<( FLOAT_e - 1))-1)	/* fracció/bias/excés: 127 */

    /* MÀSCARES per als camps de bits de valors float */
#define FLOAT_MASK_FRAC	((1<<FLOAT_m)-1)	/* bits 22..0:	fracció/mantissa/significand 0x007FFFFF */
#define FLOAT_MASK_EXP	(0x7FFFFFFF ^ FLOAT_MASK_FRAC)	
                                            /* bits 30..23:	exponent (en excés 127) */
#define FLOAT_MASK_SIGN	(1<<31)				/* bit 31:		signe 0x80000000 */

/* Macros per esbrinar el tipus de número "float" */
#define FLOAT_IS_NORMAL(f)		(((f & FLOAT_MASK_EXP) != 0) && ((f & FLOAT_MASK_EXP) != FLOAT_MASK_EXP))
#define FLOAT_IS_DENORMAL(f)	(((f & FLOAT_MASK_EXP) == 0) && ((f & FLOAT_MASK_MANT) != 0))
#define FLOAT_IS_ZERO(f)		 ((f & (FLOAT_MASK_EXP | FLOAT_MASK_FRAC)) == 0)
#define FLOAT_IS_INFINITE(f)	(((f & FLOAT_MASK_EXP) == FLOAT_MASK_EXP) && ((f & FLOAT_MASK_FRAC) == 0))
#define FLOAT_IS_NAN(f)			(((f & FLOAT_MASK_EXP) == FLOAT_MASK_EXP) && ((f & FLOAT_MASK_FRAC) != 0))
#define FLOAT_IS_FINITE(f)		 ((f & FLOAT_MASK_EXP) != FLOAT_MASK_EXP)	/* Ni ±∞, ni NaN */
#define FLOAT_IS_NEGATIVE(f)	  (f & FLOAT_MASK_SIGN)

	/* Alguns VALORS float */
#define FLOAT_ZERO_POS	(+0.0)
#define FLOAT_ZERO_NEG	(-0.0)
#define FLOAT_INF_POS	(+1.0/0.0)
#define FLOAT_INF_NEG	(-1.0/0.0)
#define FLOAT_NAN_POS	(+0.0/0.0)
#define FLOAT_NAN_NEG	(-0.0/0.0)

	/* Límits tipus int */
#define INT_MAX_POS		0x7FFFFFFF
#define INT_MAX_NEG		0x80000000


    /* Alguns valors més en coma flotant E9M22 */
#define E9M22_11_4999	0x409BFFCB	// Valor +11,4999 en format E9M22
#define E9M22_11_5		0x409C0000	// Valor +11,5 en format E9M22
#define E9M22_12_4999	0x40A3FFCB	// Valor +12,4999 en format E9M22
#define E9M22_12_5		0x40A40000	// Valor +12,5 en format E9M22
#define E9M22_12_5001	0x40A40034	// Valor +12,5001 en format E9M22
#define E9M22_456_789	0x41F2327F	// Valor +456,789 en format E9M22
#define E9M22_543_789	0x4211CA45	// Valor +654,321 en format E9M22
#define E9M22_123456	0x43F89000	// Valor +123.456,0 en format E9M22
#define E9M22_0x7FFFFFFF 0x47800000	// Valor MAX_INT 0x7FFFFFFF en format E9M22


/*********************************/
/* the lists of test case values */
/*********************************/

/**** CLASSIFICACIONS: ****/

    /* Tests operació e9m22_classify */
test_classify_struct test_cases_class[] =
							/*   E9M22:	→ Classe E9M22: */
{{	E9M22_ZERO_NEG, 		/* 0: -0.0	→ ZERO */
	E9M22_CLASS_ZERO },			
 {	E9M22_10, 				/* 1: +10.0	→ NORMAL */
    E9M22_CLASS_NORMAL },
 {	E9M22_MAX_DNORM,		/* 2: El Denormal més gran → DENORMAL */
	E9M22_CLASS_DENORMAL },
 {	NSET(E9M22_MIN_NORM),	/* 3: El -Normal més petit → NORMAL */
	E9M22_CLASS_NORMAL },
 {	NSET(E9M22_qNAN),		/* 4: -qNAN	→ NAN */
    E9M22_CLASS_NAN }, 
 {	E9M22_INF_POS,			/* 5: +∞ → INFINITE */
	E9M22_CLASS_INFINITE },
 {	E9M22_sNAN,				/* 6: +sNAN	→ NAN */
    E9M22_CLASS_NAN }, 

	// To-do? Més tests estudiants?

};


    /* Tests operació e9m22_is_normal */
test_is_checks_struct test_cases_is_normal[] =
							/*   E9M22:	→ és_normal?: */
{{	E9M22_ZERO_NEG, 		/* 0: -0.0	→ No */
	fals },			
 {	E9M22_10, 				/* 1: +10.0	→ Sí */
    cert },
 {	E9M22_MAX_DNORM,		/* 2: El Denormal més gran → No */
	fals },
 {	NSET(E9M22_MIN_NORM),	/* 3: El -Normal més petit → Sí */
	cert },
 {	NSET(E9M22_qNAN),		/* 4: -qNAN	→ No */
    fals }, 
 {	E9M22_INF_POS,			/* 5: +∞ → No */
	fals },
 {	E9M22_sNAN,				/* 6: +sNAN	→ No */
    fals }, 

	// To-do? Més tests estudiants?

};


    /* Tests operació e9m22_is_denormal */
test_is_checks_struct test_cases_is_denormal[] =
							/*   E9M22:	→ és_denormal?: */
{{	E9M22_ZERO_NEG, 		/* 0: -0.0	→ No */
	fals },			
 {	E9M22_10, 				/* 1: +10.0	→ No */
    fals },
 {	E9M22_MAX_DNORM,		/* 2: El Denormal més gran → Sí */
	cert },
 {	NSET(E9M22_MIN_NORM),	/* 3: El -Normal més petit → No */
	fals },
 {	NSET(E9M22_qNAN),		/* 4: -qNAN	→ No */
    fals }, 
 {	E9M22_INF_POS,			/* 5: +∞ → No */
	fals },
 {	E9M22_sNAN,				/* 6: +sNAN	→ No */
    fals }, 

	// To-do? Més tests estudiants?

};


    /* Tests operació e9m22_is_zero */
test_is_checks_struct test_cases_is_zero[] =
							/*   E9M22:	→ és_zero?: */
{{	E9M22_ZERO_NEG, 		/* 0: -0.0	→ Sí */
	cert },			
 {	E9M22_10, 				/* 1: +10.0	→ No */
    fals },
 {	E9M22_MAX_DNORM,		/* 2: El Denormal més gran → No */
	fals },
 {	NSET(E9M22_MIN_NORM),	/* 3: El -Normal més petit → No */
	fals },
 {	NSET(E9M22_qNAN),		/* 4: -qNAN	→ No */
    fals }, 
 {	E9M22_INF_POS,			/* 5: +∞ → No */
	fals },
 {	E9M22_sNAN,				/* 6: +sNAN	→ No */
    fals }, 

	// To-do? Més tests estudiants?

};


    /* Tests operació e9m22_is_infinite */
test_is_checks_struct test_cases_is_infinite[] =
							/*   E9M22:	→ és_infinit?: */
{{	E9M22_ZERO_NEG, 		/* 0: -0.0	→ No */
	fals },			
 {	E9M22_10, 				/* 1: +10.0	→ No */
    fals },
 {	E9M22_MAX_DNORM,		/* 2: El Denormal més gran → No */
	fals },
 {	NSET(E9M22_MIN_NORM),	/* 3: El -Normal més petit → No */
	fals },
 {	NSET(E9M22_qNAN),		/* 4: -qNAN	→ No */
    fals }, 
 {	E9M22_INF_POS,			/* 5: +∞ → Sí */
	cert },
 {	E9M22_sNAN,				/* 6: +sNAN	→ No */
    fals }, 

	// To-do? Més tests estudiants?

};


    /* Tests operació e9m22_is_nan */
test_is_checks_struct test_cases_is_nan[] =
							/*   E9M22:	→ és_NaN?: */
{{	E9M22_ZERO_NEG, 		/* 0: -0.0	→ No */
	fals },			
 {	E9M22_10, 				/* 1: +10.0	→ No */
    fals },
 {	E9M22_MAX_DNORM,		/* 2: El Denormal més gran → No */
	fals },
 {	NSET(E9M22_MIN_NORM),	/* 3: El -Normal més petit → No */
	fals },
 {	NSET(E9M22_qNAN),		/* 4: -qNAN	→ Sí */
    cert }, 
 {	E9M22_INF_POS,			/* 5: +∞ → No */
	fals },
 {	E9M22_sNAN,				/* 6: +sNAN	→ Sí */
    cert }, 

	// To-do? Més tests estudiants?

};


    /* Tests operació e9m22_is_finite */
test_is_checks_struct test_cases_is_finite[] =
							/*   E9M22:	→ és_finit?: */
{{	E9M22_ZERO_NEG, 		/* 0: -0.0	→ Sí */
	cert },			
 {	E9M22_10, 				/* 1: +10.0	→ Sí */
    cert },
 {	E9M22_MAX_DNORM,		/* 2: El Denormal més gran → Sí */
	cert },
 {	NSET(E9M22_MIN_NORM),	/* 3: El -Normal més petit → Sí */
	cert },
 {	NSET(E9M22_qNAN),		/* 4: -qNAN	→ No */
    fals }, 
 {	E9M22_INF_POS,			/* 5: +∞ → No */
	fals },
 {	E9M22_sNAN,				/* 6: +sNAN	→ No */
    fals }, 

	// To-do? Més tests estudiants?

};


    /* Tests operació e9m22_is_negative */
test_is_checks_struct test_cases_is_negative[] =
							/*   E9M22:	→ és_negatiu?: */
{{	E9M22_ZERO_NEG, 		/* 0: -0.0	→ Sí */
	cert },			
 {	E9M22_10, 				/* 1: +10.0	→ No */
    fals },
 {	E9M22_MAX_DNORM,		/* 2: El Denormal més gran → No */
	fals },
 {	NSET(E9M22_MIN_NORM),	/* 3: El -Normal més petit → Sí */
	cert },
 {	NSET(E9M22_qNAN),		/* 4: -qNAN	→ Sí */
    cert }, 
 {	E9M22_INF_POS,			/* 5: +∞ → No */
	fals },
 {	E9M22_sNAN,				/* 6: +sNAN	→ No */
    fals }, 

	// To-do? Més tests estudiants?

};



/**** CONVERSIONS: ****/

    /* Tests operació e9m22_to_float */
test_e2f_struct test_cases_e2f[] =
						/*   E9M22:	→ float: */
{{	E9M22_ZERO_NEG, 	/* 0: -0.0	→ -0.0 */
	-0.0 },			
 {	E9M22_10, 			/* 1: +10.0	→ +10.0 */
    +10.0 },
 {	NSET(E9M22_0_0005),	/* 2: -0.0005 → -0.0005 */
	-0.0005 },
 {	NSET(E9M22_MIN_NORM),/* 3: El -Normal més petit → -0.0 */
	-0.0 },
 {	NSET(E9M22_qNAN),	/* 4: -qNAN	→ -NAN */
    FLOAT_NAN_NEG }, 
 {	E9M22_MAX_NORM,		/* 5: +El més gran → +∞ */
	FLOAT_INF_POS },

	// To-do? Més tests estudiants?

};


    /* Tests operació float_to_e9m22 */
test_f2e_struct test_cases_f2e[] =
						/*   float:	→ E9M22: */
{
 {	FLOAT_INF_POS,		/* 3:   +∞ 	→ +∞ */
	E9M22_INF_POS },

{	-0.0, 				/* 0: -0.0	→ -0.0 */
	E9M22_ZERO_NEG },			
 {	-200.0, 			/* 1: -200.0 → -200.0 */
    NSET(E9M22_200) },
 {	0.0002,				/* 2: +0.0002 → +0.0002 */
	E9M22_0_0002 },
 {	FLOAT_INF_POS,		/* 3:   +∞ 	→ +∞ */
	E9M22_INF_POS },
 {	FLOAT_NAN_NEG,		/* 4: -NAN	→ -qNAN */
    NSET(E9M22_qNAN) }, 
 {	-0.1,				/* 5: -0.1	→ -0.1 */
	NSET(E9M22_0_1) },

	// To-do? Més tests estudiants?

};


    /* Tests operació e9m22_to_int */
test_e2i_struct test_cases_e2i[] =
						/*   E9M22:	→ int: */
{{	E9M22_ZERO_NEG, 	/* 0: -0.0	→ +0 */
	0 },			
 {	E9M22_10, 			/* 1: +10.0	→ +10 */
    10 },
 {	NSET(E9M22_0_0005),	/* 2: -0.0005 → 0 */
	0 },
 {	NSET(E9M22_MIN_NORM),/* 3: El -Normal més petit → -0.0 */
	0 },
 {	NSET(E9M22_qNAN),	/* 4: -qNAN	→ -Màxim int */
    INT_MAX_NEG }, 
 {	E9M22_MAX_NORM,		/* 5: +El més gran → +Màxim int */
	INT_MAX_POS },
 {	E9M22_11_4999,		/* 6: +11.4999 → +11 (truncar) */
	11 },
 {	E9M22_11_5,			/* 7: +11.5 → +12 (arrodonir al parell més proper) */
	12 },

	// To-do? Més tests estudiants?

};


    /* Tests operació int_to_e9m22 */
test_i2e_struct test_cases_i2e[] =
						/*   int:	→ E9M22: */
{{	0, 					/* 0: 0		→ +0.0 */
	E9M22_ZERO_POS },			
 {	-10, 				/* 1: -10	→ -10.0 */
    NSET(E9M22_10) },
 {	5000000,			/* 2: +5000000 → +5000000.0 */
	E9M22_5_000_000 },
 {	123456,				/* 3: +123456 → +123456.0 */
	E9M22_123456 },
 {	-123456,			/* 4: -123456 → -123456.0 */
    NSET(E9M22_123456) }, 
 {	INT_MAX_POS,		/* 5: +L'int més gran → +2.147.483.647,0 */
	E9M22_0x7FFFFFFF },

	// To-do? Més tests estudiants?

};


/**** ARITMÈTIQUES: ****/

    /* Tests operació SUMA (add) */
test_ops2_struct test_cases_add[] =
{{	E9M22_ZERO_POS, 		/* 0: 0.0 + 0.0 = 0.0 */
	E9M22_ZERO_POS,
	E9M22_ZERO_POS },
 {	E9M22_1, 				/* 1: 1.0 + 1.0 = 2.0 */
    E9M22_1,
    E9M22_2 },
 {	NSET(E9M22_1),			/* 2: -1.0 + 2.0 = 1.0 */
    E9M22_2,
    E9M22_1 },
 {	E9M22_MAX_DNORM, 		/* 3: DNORMAL + DNORMAL = NORMAL */
	E9M22_MAX_DNORM,
    E9M22_MIN_NORM },
 {	E9M22_INF_NEG, 			/* 4: -∞ + -0 = -∞ */
	E9M22_ZERO_NEG,
	E9M22_INF_NEG },
 {	E9M22_qNAN,				/* 5: NaN + num = NaN */
	E9M22_MAX_DNORM, 
    E9M22_qNAN }, 
 {	E9M22_MAX_NORM, 		/* 6: El més gran + 1 = el més gran */
	E9M22_1,
	E9M22_MAX_NORM },

	// To-do? Més tests estudiants?

};


    /* Tests operació RESTA (sub) */
test_ops2_struct test_cases_sub[] =
{{	E9M22_ZERO_POS, 		/* 0: 0.0 - 0.0 = 0.0 */
	E9M22_ZERO_POS,
	E9M22_ZERO_NEG },			
 {	E9M22_1, 				/* 1: 1.0 - 1.0 = 0.0 */
    E9M22_1,
    E9M22_ZERO_POS },
 {	E9M22_ZERO_POS, 		/* 2: 0 - -∞ = +∞ */
	E9M22_INF_NEG,
	E9M22_INF_POS },
 {	E9M22_1, 				/* 3: 1.0 - -1.0 = 2.0 */
	NSET(E9M22_1),
	E9M22_2 },
 {	E9M22_MAX_DNORM,		/* 4: num - NaN = NaN */
	E9M22_qNAN, 
    E9M22_qNAN }, 
 {	E9M22_MIN_DNORM,		/* 5: El més petit - -1 = +1 */
	NSET(E9M22_1),
	E9M22_1 },

	// To-do? Més tests estudiants?

};


    /* Tests operació MULTIPLICACIÓ (mul) */
test_ops2_struct test_cases_mul[] =
{{	E9M22_ZERO_NEG, 		/* 0: -0.0 * +0.0 = -0.0 */
	E9M22_ZERO_POS,
	E9M22_ZERO_NEG },				
 {	NSET(E9M22_1), 			/* 1: -1.0 * +2.0 = -2.0 */
	E9M22_2,
	NSET(E9M22_2) },
 {	E9M22_5, 				/* 2: 5.0 * 2.0 = 10.0 */
	NSET(E9M22_2),
	NSET(E9M22_10) },
 {	E9M22_0_001, 			/* 3: 0.001 * 100.0 = 0.1 */
	E9M22_100,
	E9M22_0_1 },
 {	E9M22_MAX_NORM,			/* 4: El més gran * -2.0 = -∞ */
	NSET(E9M22_2),
	E9M22_INF_NEG },
 {	E9M22_MIN_DNORM,		/* 5: El més petit * -1 = -El més petit */
	NSET(E9M22_1),
	NSET(E9M22_MIN_DNORM) },

	// To-do? Més tests estudiants?

};


    /* Tests operació DIVISIÓ (div) */
 test_ops2_struct test_cases_div[] =
{{	E9M22_ZERO_NEG, 		/* 0: -0.0 / 0.0 = -NaN */
	E9M22_ZERO_POS,
	NSET(E9M22_qNAN) },			
 {	E9M22_200, 				/* 1: +200.0 / -20.0 = -10.0 */
	NSET(E9M22_20),
	NSET(E9M22_10) },
 {	NSET(E9M22_2), 			/* 2: -2.0 / -10.0 = +0.2 */
	NSET(E9M22_10),
	E9M22_0_2 },
 {	E9M22_MAX_NORM, 		/* 3: molt_gran / -∞ = -0.0 */
	E9M22_INF_NEG,
	E9M22_ZERO_NEG },
 {	E9M22_MAX_DNORM,		/* 4: màxim denormal / -0.5 = -mínim normal */
	NSET(E9M22_0_5),
	NSET(E9M22_MIN_NORM) },
 {	E9M22_MIN_DNORM,		/* 5: mínim denormal / +∞ = +0.0 */
	E9M22_INF_POS,
	E9M22_ZERO_POS },

	// To-do? Més tests estudiants?

};


    /* Tests operació INVERSA (inv) */
test_ops1_struct test_cases_inv[] =
{{	E9M22_2, 				/* 0: 1/2.0 = 0.5 */
	E9M22_0_5 },			
 {	E9M22_0_5, 				/* 1: 1/0.5 = 2.0 */
    E9M22_2 },
 {	E9M22_INF_NEG, 			/* 2: 1/-∞ = -0.0 */
	E9M22_ZERO_NEG },
 {	E9M22_2000,				/* 3: 1/2000.0 = 0.0005 */
	E9M22_0_0005 },
 {	NSET(E9M22_0_0005),		/* 4: 1/-0.0005 = -2000.0 */
    NSET(E9M22_2000) }, 
 {	NSET(E9M22_sNAN),		/* 5: 1/-NaN = -NaN */
	NSET(E9M22_sNAN) },

	// To-do? Més tests estudiants?

};


    /* Tests operació NEGACIÓ (neg) */
test_ops1_struct test_cases_neg[] =
{{	E9M22_ZERO_POS, 		/* 0: -( +0.0 ) = -0.0 */
	E9M22_ZERO_NEG },			
 {	NSET(E9M22_1), 			/* 1: -( -1.0 ) = +1.0 */
    E9M22_1 },
 {	E9M22_0_0005, 			/* 2: -( +0.0005) = -0.0005 */
	NSET(E9M22_0_0005) },
 {	NSET(E9M22_MIN_DNORM),	/* 3: -( -El més petit) = +El més petit */
	E9M22_MIN_DNORM },
 {	NSET(E9M22_qNAN),		/* 4: -( -NaN ) = +NaN */
    E9M22_qNAN }, 
 {	E9M22_MAX_NORM,			/* 5: -( El més gran ) = -El més gran */
	NSET(E9M22_MAX_NORM) },

	// To-do? Més tests estudiants?

};


    /* Tests operació VALOR ABSOLUT (abs) */
test_ops1_struct test_cases_abs[] =
{{	E9M22_ZERO_NEG, 		/* 0: abs( -0.0 ) = +0.0 */
	E9M22_ZERO_POS },			
 {	NSET(E9M22_1), 			/* 1: abs( -1.0 ) = +1.0 */
    E9M22_1 },
 {	E9M22_0_0005, 			/* 2: abs( +0.0005) = +0.0005 */
	E9M22_0_0005 },
 {	NSET(E9M22_MIN_DNORM),	/* 3: abs( -El més petit) = +El més petit */
	E9M22_MIN_DNORM },
 {	NSET(E9M22_qNAN),		/* 4: abs( -NaN ) = +NaN */
    E9M22_qNAN }, 
 {	E9M22_MAX_NORM,			/* 5: abs( +El més gran ) = +El més gran */
	E9M22_MAX_NORM },

	// To-do? Més tests estudiants?

};



/**** COMPARACIÓ: ****/


    /* Tests comparació = eq (e9m22_are_eq) */
test_comp_struct test_cases_compare[] =
{{	E9M22_ZERO_POS, 		/* 0: compare(+0.0, NaN) = No Comparable */
	E9M22_qNAN,
	E9M22_CMP_NC_NAN },
 {	E9M22_1, 				/* 1: compare(+1.0, +1.0) = Iguals */
    E9M22_1,
    E9M22_CMP_EQUAL },
 {	NSET(E9M22_2),			/* 2: compare(-2.0, +2.0) = Menor */
    E9M22_2,
    E9M22_CMP_LESS },
 {	E9M22_MAX_DNORM, 		/* 3: compare(+Màxim denormal, +Màxim denormal) = Iguals */
	E9M22_MAX_DNORM,
    E9M22_CMP_EQUAL },
 {	E9M22_INF_NEG, 			/* 4: compare(-∞, -0) = Menor */
	E9M22_ZERO_NEG,
	E9M22_CMP_LESS },
 {	E9M22_qNAN,				/* 5: compare(NaN, NaN) = No Comparable */
	E9M22_qNAN, 
    E9M22_CMP_NC_NAN }, 
 {	E9M22_INF_POS, 			/* 6: compare(+∞, +El més gran) = Major */
	E9M22_MAX_NORM,
	E9M22_CMP_GREATER },

	// To-do? Més tests estudiants?

};




/**** AUXILIARS: ****/

    /* Tests operació e9m22_decompose */
test_decompose_struct test_cases_decomp[] =
							/*   E9M22:	→ Classe/signe/exponent/mantissa: */
{{	E9M22_ZERO_NEG, 		/* 0: -0.0	→ ZERO/neg/exp:0/mant:0 */
	E9M22_CLASS_ZERO,
	0x80000000, 0, 0 },	 
 {	E9M22_10, 				/* 1: +10.0	→ NORMAL/pos/exp:3/mant:1010+19 0's */
    E9M22_CLASS_NORMAL,
	0, 3, 10<<19},
 {	E9M22_MAX_DNORM,		/* 2: El Denormal més gran → DENORMAL/pos/exp:-254/mant:tot 1's */
	E9M22_CLASS_DENORMAL,
	0, -254, 0x3FFFFF},
 {	NSET(E9M22_MIN_NORM),	/* 3: El -Normal més petit → NORMAL/neg/exp:-254/mant:1 seguit de 22 0's */
	E9M22_CLASS_NORMAL,
	0x80000000, -254, 0x400000},
 {	NSET(E9M22_qNAN),		/* 4: -qNAN	→ NAN/neg/exp:511/mant:22 bits baixos */
    E9M22_CLASS_NAN,
	0x80000000, 511, E9M22_qNAN & E9M22_MASK_FRAC}, 
 {	E9M22_INF_POS,			/* 5: +∞ → INFINITE/pos/exp:511/mant:0 */
	E9M22_CLASS_INFINITE,
	0, 511, 0},
 {	E9M22_sNAN,				/* 6: +sNAN	→ NAN/pos/exp:511/mant:22 bits baixos */
    E9M22_CLASS_NAN,
	0, 511, E9M22_sNAN & E9M22_MASK_FRAC}, 
 {	NSET(E9M22_0_5),		/* 7: -0,5 → NORMAL/neg/exp:-1/mant:1+22 0's */
	E9M22_CLASS_NORMAL,
	0x80000000, -1, 1<<22},

	// To-do? Més tests estudiants?

};




/*************************************/
/* Functions to apply the test cases */
/*************************************/

	/* "Forward" declarations */
unsigned int e9m22_error_percentage(e9m22_t actual_result, e9m22_t expected_result);
unsigned int float_error_percentage(float actual_result, float expected_result);
unsigned int int_error_percentage(int actual_result, int expected_result);
unsigned int bool_error_percentage(int actual_result, int expected_result);


	/* Functions to apply test cases; all return the percentage of OK's */

/*-------------------------------------------*/
/**** JOCS DE PROVES ==> CLASSIFICACIONS: ****/
/*-------------------------------------------*/

	/* Tests of function e9m22_classify */
int apply_tests_classify( 
			test_classify_struct test_cases[],	// Vector de casos a provar
			unsigned int num_cases				// Número de casos al vector
						)		// retorna el percentatge de casos correctes
{
    unsigned int i;						/* loop index */
    e9m22_class_t result;				/* routine result */
    unsigned int num_ok = 0;			/* number of right tests */
    unsigned int num_ko = 0;			/* number of wrong tests */
    unsigned int error_percent, percent_ok, quo, res;
    e9m22_t input;		/* input value for conversion */
    e9m22_class_t xresult;		/* Expected result of the conversion */

    /********* evaluate the list of test case values *********/
    for (i = 0; i < num_cases; i++)
    {
        input = test_cases[i].num; 			// input value
        xresult = test_cases[i].xresult; 	// expected result

			// Call the function to test with the current test case
		result = e9m22_classify(input);

        error_percent = bool_error_percentage(result, xresult);

            /* check if error percentage is under an admisible margin */
        if (error_percent <= MAX_ERROR_PERCENTAGE)
            num_ok++;
        else
			/* BREAKPOINT to detect wrong cases: */
				/* result != xresult */
            num_ko++;
    }

	div_mod(100 * num_ok , num_cases, &quo, &res);
	percent_ok = quo;

/* TESTING POINT: check if number of ok tests is equal to num_cases,
                  or if number of ko tests is 0,
				  or if percent_ok is 100.
    (gdb) p num_ok
    (gdb) p num_ko
    (gdb) p num_cases
    (gdb) p percent_ok
*/

/* BREAKPOINT */
    return percent_ok;
}



	/* Tests of functions with signature: (E9M22) --> int(bool) */
int apply_tests_is( 	int (*func_to_test)(e9m22_t),		// Funció a testejar
						test_is_checks_struct test_cases[],	// Vector de casos a provar
						unsigned int num_cases				// Número de casos al vector
					)							// retorna el percentatge de casos correctes
{
    unsigned int i;				/* loop index */
    int result;					/* routine result */
    unsigned int num_ok = 0;	/* number of right tests */
    unsigned int num_ko = 0;	/* number of wrong tests */
    unsigned int error_percent, percent_ok, quo, res;
    e9m22_t input;				/* input value for function */
    int xresult;				/* Expected result of the function */
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"	// Evitar warning f_input
	float f_input;				// float value (for debug)

    /********* evaluate the list of test case values *********/
    for (i = 0; i < num_cases; i++)
    {
        input = test_cases[i].num; 			// input value
        xresult = test_cases[i].xresult; 	// expected result

			// Call the function to test with the current test case
		result = func_to_test(input);

			// Convert values to float (for debug)
		f_input = e9m22_to_float_c_ (input);

        error_percent = bool_error_percentage(result, xresult);

            /* check if error percentage is under an admisible margin */
        if (error_percent <= MAX_ERROR_PERCENTAGE)
            num_ok++;
        else
			/* BREAKPOINT to detect wrong cases: */
				/* result != xresult */
            num_ko++;
    }

	div_mod(100 * num_ok , num_cases, &quo, &res);
	percent_ok = quo;

/* TESTING POINT: check if number of ok tests is equal to num_cases,
                  or if number of ko tests is 0,
				  or if percent_ok is 100.
    (gdb) p num_ok
    (gdb) p num_ko
    (gdb) p num_cases
    (gdb) p percent_ok
*/

/* BREAKPOINT */
    return percent_ok;
}




/*---------------------------------------*/
/**** JOCS DE PROVES ==> CONVERSIONS: ****/
/*---------------------------------------*/

	/* Tests of function e9m22_to_float */
int apply_tests_e2f( test_e2f_struct test_cases[],	// Vector de casos a provar
					 unsigned int num_cases			// Número de casos al vector
					)							// retorna el percentatge de casos correctes
{
    unsigned int i;						/* loop index */
    float result;						/* routine result */
    unsigned int num_ok = 0;			/* number of right tests */
    unsigned int num_ko = 0;			/* number of wrong tests */
    unsigned int error_percent, percent_ok, quo, res;
    e9m22_t input;		/* input value for conversion */
    float xresult;		/* Expected result of the conversion */

    /********* evaluate the list of test case values *********/
    for (i = 0; i < num_cases; i++)
    {
        input = test_cases[i].num; 			// input value
        xresult = test_cases[i].xresult; 	// expected result

			// Call the function to test with the current test case
		result = e9m22_to_float(input);

        error_percent = float_error_percentage(result, xresult);

            /* check if error percentage is under an admisible margin */
        if (error_percent <= MAX_ERROR_PERCENTAGE)
            num_ok++;
        else
			/* BREAKPOINT to detect wrong cases: */
				/* result != xresult */
            num_ko++;
    }

	div_mod(100 * num_ok , num_cases, &quo, &res);
	percent_ok = quo;

/* TESTING POINT: check if number of ok tests is equal to num_cases,
                  or if number of ko tests is 0,
				  or if percent_ok is 100.
    (gdb) p num_ok
    (gdb) p num_ko
    (gdb) p num_cases
    (gdb) p percent_ok
*/

/* BREAKPOINT */
    return percent_ok;
}



	/* Tests of function float_to_e9m22 */
int apply_tests_f2e( test_f2e_struct test_cases[],	// Vector de casos a provar
					 unsigned int num_cases			// Número de casos al vector
					)							// retorna el percentatge de casos correctes
{
    unsigned int i;						/* loop index */
    e9m22_t result;						/* routine result */
    unsigned int num_ok = 0;			/* number of right tests */
    unsigned int num_ko = 0;			/* number of wrong tests */
    unsigned int error_percent, percent_ok, quo, res;
    float input;		/* input value for conversion */
    e9m22_t xresult;	/* Expected result of the conversion */
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"	// Evitar warning f_xresult
	float f_xresult;

    /********* evaluate the list of test case values *********/
    for (i = 0; i < num_cases; i++)
    {
        input = test_cases[i].num; 			// input value
        xresult = test_cases[i].xresult; 	// expected result
		f_xresult = e9m22_to_float_c_(xresult);	// conversió a float (per a debug)

			// Call the function to test with the current test case
		result = float_to_e9m22(input);

        error_percent = e9m22_error_percentage(result, xresult);

            /* check if error percentage is under an admisible margin */
        if (error_percent <= MAX_ERROR_PERCENTAGE)
            num_ok++;
        else
			/* BREAKPOINT to detect wrong cases: */
				/* result != xresult */
            num_ko++;
    }

	div_mod(100 * num_ok , num_cases, &quo, &res);
	percent_ok = quo;

/* TESTING POINT: check if number of ok tests is equal to num_cases,
                  or if number of ko tests is 0,
				  or if percent_ok is 100.
    (gdb) p num_ok
    (gdb) p num_ko
    (gdb) p num_cases
    (gdb) p percent_ok
*/

/* BREAKPOINT */
    return percent_ok;
}



	/* Tests of function e9m22_to_int */
int apply_tests_e2i( test_e2i_struct test_cases[],	// Vector de casos a provar
					 unsigned int num_cases			// Número de casos al vector
					)							// retorna el percentatge de casos correctes
{
    unsigned int i;						/* loop index */
    int result;						/* routine result */
    unsigned int num_ok = 0;			/* number of right tests */
    unsigned int num_ko = 0;			/* number of wrong tests */
    unsigned int error_percent, percent_ok, quo, res;
    e9m22_t input;		/* input value for conversion */
    int xresult;		/* Expected result of the conversion */

    /********* evaluate the list of test case values *********/
    for (i = 0; i < num_cases; i++)
    {
        input = test_cases[i].num; 			// input value
        xresult = test_cases[i].xresult; 	// expected result

			// Call the function to test with the current test case
		result = e9m22_to_int(input);

        error_percent = int_error_percentage(result, xresult);

            /* check if error percentage is under an admisible margin */
        if (error_percent <= MAX_ERROR_PERCENTAGE)
            num_ok++;
        else
			/* BREAKPOINT to detect wrong cases: */
				/* result != xresult */
            num_ko++;
    }

	div_mod(100 * num_ok , num_cases, &quo, &res);
	percent_ok = quo;

/* TESTING POINT: check if number of ok tests is equal to num_cases,
                  or if number of ko tests is 0,
				  or if percent_ok is 100.
    (gdb) p num_ok
    (gdb) p num_ko
    (gdb) p num_cases
    (gdb) p percent_ok
*/

/* BREAKPOINT */
    return percent_ok;
}


	/* Tests of function int_to_e9m22 */
int apply_tests_i2e( test_i2e_struct test_cases[],	// Vector de casos a provar
					 unsigned int num_cases			// Número de casos al vector
					)							// retorna el percentatge de casos correctes
{
    unsigned int i;						/* loop index */
    e9m22_t result;						/* routine result */
    unsigned int num_ok = 0;			/* number of right tests */
    unsigned int num_ko = 0;			/* number of wrong tests */
    unsigned int error_percent, percent_ok, quo, res;
    int input;			/* input value for conversion */
    e9m22_t xresult;	/* Expected result of the conversion */
	float f_xresult, f_result;

    /********* evaluate the list of test case values *********/
    for (i = 0; i < num_cases; i++)
    {
        input = test_cases[i].num; 			// input value
        xresult = test_cases[i].xresult; 	// expected result
		f_xresult = e9m22_to_float_c_(xresult);	// conversió a float (per a debug)

			// Call the function to test with the current test case
		result = int_to_e9m22(input);
		f_result = e9m22_to_float_c_(result);	// conversió a float (per a debug)

        error_percent = e9m22_error_percentage(result, xresult);

            /* check if error percentage is under an admisible margin */
        if (error_percent <= MAX_ERROR_PERCENTAGE)
            num_ok++;
        else
			/* BREAKPOINT to detect wrong cases: */
				/* result != xresult */
            num_ko++;
    }

	div_mod(100 * num_ok , num_cases, &quo, &res);
	percent_ok = quo;

/* TESTING POINT: check if number of ok tests is equal to num_cases,
                  or if number of ko tests is 0,
				  or if percent_ok is 100.
    (gdb) p num_ok
    (gdb) p num_ko
    (gdb) p num_cases
    (gdb) p percent_ok
*/

/* BREAKPOINT */
    return percent_ok;
}


/*----------------------------------------*/
/**** JOCS DE PROVES ==> ARITMÈTIQUES: ****/
/*----------------------------------------*/

	/* Tests of functions with signature: (E9M22, E9M22) --> E9M22 */
int apply_tests_ops2( 	e9m22_t (*func_to_test)(e9m22_t, e9m22_t),	// Funció a testejar
						test_ops2_struct test_cases[],				// Vector de casos a provar
						unsigned int num_cases						// Número de casos al vector
					)							// retorna el percentatge de casos correctes
{
    unsigned int i;					/* loop index */
    e9m22_t result;					/* routine result */
    unsigned int num_ok = 0;		/* number of right tests */
    unsigned int num_ko = 0;		/* number of wrong tests */
    unsigned int error_percent, percent_ok, quo, res;
    e9m22_t input1, input2;			/* input values for function */
    e9m22_t xresult;				/* Expected result of the function */
	float f_input1, f_input2, f_xresult, f_result;	// float values (for debug)

    /********* evaluate the list of test case values *********/
    for (i = 0; i < num_cases; i++)
    {
        input1 = test_cases[i].num1; 		// input values
        input2 = test_cases[i].num2;
        xresult = test_cases[i].xresult; 	// expected result

			// Call the function to test with the current test case
		result = func_to_test(input1, input2);

			// Convert values to float (for debug)
		f_input1 = e9m22_to_float_c_ (input1);
		f_input2 = e9m22_to_float_c_ (input2);
		f_result = e9m22_to_float_c_ (result);
		f_xresult = e9m22_to_float_c_ (xresult);

        error_percent = e9m22_error_percentage(result, xresult);

            /* check if error percentage is under an admisible margin */
        if (error_percent <= MAX_ERROR_PERCENTAGE)
            num_ok++;
        else
			/* BREAKPOINT to detect wrong cases: */
				/* result != xresult */
            num_ko++;
    }

	div_mod(100 * num_ok , num_cases, &quo, &res);
	percent_ok = quo;

/* TESTING POINT: check if number of ok tests is equal to num_cases,
                  or if number of ko tests is 0,
				  or if percent_ok is 100.
    (gdb) p num_ok
    (gdb) p num_ko
    (gdb) p num_cases
    (gdb) p percent_ok
*/

/* BREAKPOINT */
    return percent_ok;
}


	/* Tests of functions with signature: (E9M22) --> E9M22 */
int apply_tests_ops1( 	e9m22_t (*func_to_test)(e9m22_t),	// Funció a testejar
						test_ops1_struct test_cases[],		// Vector de casos a provar
						unsigned int num_cases				// Número de casos al vector
					)							// retorna el percentatge de casos correctes
{
    unsigned int i;					/* loop index */
    e9m22_t result;					/* routine result */
    unsigned int num_ok = 0;		/* number of right tests */
    unsigned int num_ko = 0;		/* number of wrong tests */
    unsigned int error_percent, percent_ok, quo, res;
    e9m22_t input;					/* input value for function */
    e9m22_t xresult;				/* Expected result of the function */
	float f_input, f_xresult, f_result;	// float values (for debug)

    /********* evaluate the list of test case values *********/
    for (i = 0; i < num_cases; i++)
    {
        input = test_cases[i].num; 			// input value
        xresult = test_cases[i].xresult; 	// expected result

			// Call the function to test with the current test case
		result = func_to_test(input);

			// Convert values to float (for debug)
		f_input = e9m22_to_float_c_ (input);
		f_result = e9m22_to_float_c_ (result);
		f_xresult = e9m22_to_float_c_ (xresult);

        error_percent = e9m22_error_percentage(result, xresult);

            /* check if error percentage is under an admisible margin */
        if (error_percent <= MAX_ERROR_PERCENTAGE)
            num_ok++;
        else
			/* BREAKPOINT to detect wrong cases: */
				/* result != xresult */
            num_ko++;
    }

	div_mod(100 * num_ok , num_cases, &quo, &res);
	percent_ok = quo;

/* TESTING POINT: check if number of ok tests is equal to num_cases,
                  or if number of ko tests is 0,
				  or if percent_ok is 100.
    (gdb) p num_ok
    (gdb) p num_ko
    (gdb) p num_cases
    (gdb) p percent_ok
*/

/* BREAKPOINT */
    return percent_ok;
}



/*--------------------------------------*/
/**** JOCS DE PROVES ==> COMPARACIÓ: ****/
/*--------------------------------------*/


	/* Tests of functions with signature: (E9M22, E9M22) --> int(bool) */
int apply_tests_compare(	test_comp_struct test_cases[],	// Vector de casos a provar
							unsigned int num_cases			// Número de casos al vector
						)							// retorna el percentatge de casos correctes
{
    unsigned int i;				/* loop index */
    int result;					/* routine result */
    unsigned int num_ok = 0;	/* number of right tests */
    unsigned int num_ko = 0;	/* number of wrong tests */
    unsigned int error_percent, percent_ok, quo, res;
    e9m22_t input1, input2;		/* input values for function */
    e9m22_cmp_t xresult;		/* Expected result of the function */
	float f_input1, f_input2;	// float values (for debug)

    /********* evaluate the list of test case values *********/
    for (i = 0; i < num_cases; i++)
    {
        input1 = test_cases[i].num1; 		// input values
        input2 = test_cases[i].num2;
        xresult = test_cases[i].xresult; 	// expected result

			// Call the function to test with the current test case
		result = e9m22_compare(input1, input2);

			// Convert values to float (for debug)
		f_input1 = e9m22_to_float_c_ (input1);
		f_input2 = e9m22_to_float_c_ (input2);

        error_percent = (result == xresult) ? 0 : 100;

            /* check if error percentage is under an admisible margin */
        if (error_percent <= MAX_ERROR_PERCENTAGE)
            num_ok++;
        else
			/* BREAKPOINT to detect wrong cases: */
				/* result != xresult */
            num_ko++;
    }

	div_mod(100 * num_ok , num_cases, &quo, &res);
	percent_ok = quo;

/* TESTING POINT: check if number of ok tests is equal to num_cases,
                  or if number of ko tests is 0,
				  or if percent_ok is 100.
    (gdb) p num_ok
    (gdb) p num_ko
    (gdb) p num_cases
    (gdb) p percent_ok
*/

/* BREAKPOINT */
    return percent_ok;
}




/*-------------------------------------*/
/**** JOCS DE PROVES ==> AUXILIARS: ****/
/*-------------------------------------*/

	/* Tests of function e9m22_classify */
int apply_tests_decompose( 
			test_decompose_struct test_cases[],	// Vector de casos a provar
			unsigned int num_cases				// Número de casos al vector
						)		// retorna el percentatge de casos correctes
{
    unsigned int i;						/* loop index */
    e9m22_class_t res_class;			/* routine results */
	unsigned int res_sign, res_mant;
	signed short res_exp;
    unsigned int num_ok = 0;			/* number of right tests */
    unsigned int num_ko = 0;			/* number of wrong tests */
    unsigned int error_percent, percent_ok, quo, res;
    e9m22_t input;		/* input value to decompose */
    e9m22_class_t xres_class;		/* Expected results of the decomposition */
	unsigned int xres_sign, xres_mant;
	signed short xres_exp;

    /********* evaluate the list of test case values *********/
    for (i = 0; i < num_cases; i++)
    {
        input = test_cases[i].num; 			// input value
        xres_class = test_cases[i].xclass; 	// expected results
		xres_sign  = test_cases[i].xsign;
		xres_exp   = test_cases[i].xexp;
		xres_mant  = test_cases[i].xmant;

			// Call the function to test with the current test case
		res_class = e9m22_decompose(input, &res_sign, &res_exp, &res_mant);

			// Check results
		if ( (res_class != xres_class) || (res_sign != xres_sign)
			|| (res_exp != xres_exp) || (res_mant != xres_mant)
			)
			/* BREAKPOINT(1) to detect wrong values: */
			error_percent = 100;
		else
			error_percent = 0;
	
            /* check if error percentage is under an admisible margin */
        if (error_percent <= MAX_ERROR_PERCENTAGE)
            num_ok++;
        else
			/* BREAKPOINT(2) to detect wrong cases: */
				/* results != xresults */
            num_ko++;
    }

	div_mod(100 * num_ok , num_cases, &quo, &res);
	percent_ok = quo;

/* TESTING POINT: check if number of ok tests is equal to num_cases,
                  or if number of ko tests is 0,
				  or if percent_ok is 100.
    (gdb) p num_ok
    (gdb) p num_ko
    (gdb) p num_cases
    (gdb) p percent_ok
*/

/* BREAKPOINT */
    return percent_ok;
}





#define NUM_ELEMS(VECTOR) (sizeof(VECTOR) / sizeof(VECTOR[0]))

int main(void)
{
    unsigned int perc_ok_class = 0;		/* percentage of correct tests e9m22_classify */
    unsigned int perc_ok_is_norm = 0;	/* percentage of correct tests e9m22_is_normal */
    unsigned int perc_ok_is_dnorm = 0;	/* percentage of correct tests e9m22_is_denormal */
    unsigned int perc_ok_is_zero = 0;	/* percentage of correct tests e9m22_is_zero */
    unsigned int perc_ok_is_inf = 0;	/* percentage of correct tests e9m22_is_infinite */
    unsigned int perc_ok_is_nan = 0;	/* percentage of correct tests e9m22_is_nan */
    unsigned int perc_ok_is_finit = 0;	/* percentage of correct tests e9m22_is_finite */
    unsigned int perc_ok_is_neg = 0;	/* percentage of correct tests e9m22_is_negative */

    unsigned int perc_ok_e2f = 0;	/* percentage of correct tests e9m22_to_float */
    unsigned int perc_ok_f2e = 0;	/* percentage of correct tests float_to_e9m22 */
    unsigned int perc_ok_e2i = 0;	/* percentage of correct tests e9m22_to_int */
    unsigned int perc_ok_i2e = 0;	/* percentage of correct tests int_to_e9m22 */

    unsigned int perc_ok_add = 0;	/* percentage of correct tests e9m22_add */
    unsigned int perc_ok_sub = 0;	/* percentage of correct tests e9m22_sub */
    unsigned int perc_ok_mul = 0;	/* percentage of correct tests e9m22_mul */
    unsigned int perc_ok_div = 0;	/* percentage of correct tests e9m22_div */
    unsigned int perc_ok_inv = 0;	/* percentage of correct tests e9m22_inv */
    unsigned int perc_ok_neg = 0;	/* percentage of correct tests e9m22_neg */
    unsigned int perc_ok_abs = 0;	/* percentage of correct tests e9m22_abs */

    unsigned int perc_ok_cmp = 0;	/* percentage of correct tests e9m22_compare */

    unsigned int perc_ok_decomp=0;	/* percentage of correct tests e9m22_decompose */

		// Jocs de proves e9m22_classify
	perc_ok_class = apply_tests_classify( 
						test_cases_class, NUM_ELEMS(test_cases_class) );	// test cases

		// Jocs de proves e9m22_is_normal
	perc_ok_is_norm = apply_tests_is( e9m22_is_normal, 
						test_cases_is_normal, NUM_ELEMS(test_cases_is_normal) );	// test cases

		// Jocs de proves e9m22_is_denormal
	perc_ok_is_dnorm = apply_tests_is( e9m22_is_denormal, 
						test_cases_is_denormal, NUM_ELEMS(test_cases_is_denormal) );	// test cases

		// Jocs de proves e9m22_is_zero
	perc_ok_is_zero = apply_tests_is( e9m22_is_zero, 
						test_cases_is_zero, NUM_ELEMS(test_cases_is_zero) );	// test cases

		// Jocs de proves e9m22_is_infinite
	perc_ok_is_inf = apply_tests_is( e9m22_is_infinite, 
						test_cases_is_infinite, NUM_ELEMS(test_cases_is_infinite) );	// test cases

		// Jocs de proves e9m22_is_nan
	perc_ok_is_nan = apply_tests_is( e9m22_is_nan, 
						test_cases_is_nan, NUM_ELEMS(test_cases_is_nan) );	// test cases

		// Jocs de proves e9m22_is_finite
	perc_ok_is_finit = apply_tests_is( e9m22_is_finite, 
						test_cases_is_finite, NUM_ELEMS(test_cases_is_finite) );	// test cases

		// Jocs de proves e9m22_is_negative
	perc_ok_is_neg = apply_tests_is( e9m22_is_negative, 
						test_cases_is_negative, NUM_ELEMS(test_cases_is_negative) );	// test cases



		// Jocs de proves e9m22_to_float
	perc_ok_e2f = apply_tests_e2f( test_cases_e2f, NUM_ELEMS(test_cases_e2f) );	// test cases

		// Jocs de proves float_to_e9m22
	perc_ok_f2e = apply_tests_f2e( test_cases_f2e, NUM_ELEMS(test_cases_f2e) );	// test cases

		// Jocs de proves e9m22_to_int
	perc_ok_e2i = apply_tests_e2i( test_cases_e2i, NUM_ELEMS(test_cases_e2i) );	// test cases

		// Jocs de proves int_to_e9m22
	perc_ok_i2e = apply_tests_i2e( test_cases_i2e, NUM_ELEMS(test_cases_i2e) );	// test cases



		// Jocs de proves e9m22_add
	perc_ok_add = apply_tests_ops2( e9m22_add, test_cases_add, NUM_ELEMS(test_cases_add) );	// test cases

		// Jocs de proves e9m22_sub
	perc_ok_sub = apply_tests_ops2( e9m22_sub, test_cases_sub, NUM_ELEMS(test_cases_sub) );	// test cases

		// Jocs de proves e9m22_mul
	perc_ok_mul = apply_tests_ops2( e9m22_mul, test_cases_mul, NUM_ELEMS(test_cases_mul) );	// test cases

		// Jocs de proves e9m22_div
	perc_ok_div = apply_tests_ops2( e9m22_div, test_cases_div, NUM_ELEMS(test_cases_div) );	// test cases

		// Jocs de proves e9m22_inv
	perc_ok_inv = apply_tests_ops1( e9m22_inv, test_cases_inv, NUM_ELEMS(test_cases_inv) );	// test cases

		// Jocs de proves e9m22_neg
	perc_ok_neg = apply_tests_ops1( e9m22_neg, test_cases_neg, NUM_ELEMS(test_cases_neg) );	// test cases

		// Jocs de proves e9m22_abs
	perc_ok_abs = apply_tests_ops1( e9m22_abs, test_cases_abs, NUM_ELEMS(test_cases_abs) );	// test cases



		// Jocs de proves e9m22_compare
	perc_ok_cmp = apply_tests_compare( test_cases_compare, NUM_ELEMS(test_cases_compare) );	// test cases



		// Jocs de proves e9m22_decompose
	perc_ok_decomp = apply_tests_decompose( 
						test_cases_decomp, NUM_ELEMS(test_cases_decomp) );	// test cases




/* TESTING POINT: check if all percentages of OK tests 
                  are greater than 70% required
				  to deliver the FC practice.
    (gdb) p perc_ok_class 
    (gdb) p perc_ok_is_norm 
    (gdb) p perc_ok_is_dnorm 
    (gdb) p perc_ok_is_zero 
    (gdb) p perc_ok_is_inf 
    (gdb) p perc_ok_is_nan 
    (gdb) p perc_ok_is_finit 
    (gdb) p perc_ok_is_neg 
    (gdb) p perc_ok_e2f
    (gdb) p perc_ok_f2e
    (gdb) p perc_ok_e2i
    (gdb) p perc_ok_i2e
    (gdb) p perc_ok_add
    (gdb) p perc_ok_sub
    (gdb) p perc_ok_mul
    (gdb) p perc_ok_div
    (gdb) p perc_ok_inv
    (gdb) p perc_ok_neg
    (gdb) p perc_ok_abs
    (gdb) p perc_ok_cmp
    (gdb) p perc_ok_decomp
*/

/* BREAKPOINT */
    return(0);
}


unsigned int e9m22_error_percentage(e9m22_t actual_result, e9m22_t expected_result)
{
    e9m22_t error_percentage, min_val, diff, div, abs;
    unsigned int int_error;

	if ( e9m22_is_finite(expected_result) )
	{	/* expected_result != NaN o ±∞ */
		min_val = E9M22_0_001;

		if ( (e9m22_compare_c_(e9m22_abs(expected_result), min_val) == E9M22_CMP_LESS) 
			&& (e9m22_compare_c_(e9m22_abs(actual_result), min_val) == E9M22_CMP_LESS ))
		{
			int_error = 0;	// Small values considered correct
		}
		else
		{
				// %error = 100 * Abs((Eexpected - Eactual) / Eexpected);
			diff = e9m22_sub_c_(expected_result, actual_result);
			div = e9m22_div_c_(diff, expected_result);
			abs = e9m22_abs_c_(div);
			error_percentage = e9m22_mul_c_ ( E9M22_100, abs );
			
			int_error = e9m22_to_int_c_(error_percentage);
			
			if ( (int_error==0) && ! e9m22_is_zero(error_percentage) )
				int_error = 1;	/* Notify very small error */
		}
	}
	else
	{
		if ( ( e9m22_is_nan(actual_result) && e9m22_is_nan(expected_result) ) 
				|| ( actual_result == expected_result ) )	// Infinits iguals, inclòs signe
			int_error = 0;		// Resultat OK, 0% error
		else
			int_error = 100;	// Resultat KO, 100% error
	}

    return int_error;
}


unsigned int float_error_percentage(float f_actual_result, float f_expected_result)
{
    e9m22_t actual_result, expected_result;
    unsigned int int_error;

	actual_result = float_to_e9m22_c_(f_actual_result);
	expected_result = float_to_e9m22_c_(f_expected_result);

	int_error = e9m22_error_percentage(actual_result, expected_result);

	return int_error;
}

int abs(int num) { return (num<0) ? -num : num; }

unsigned int int_error_percentage(int actual_result, int expected_result)
{
    unsigned int int_error, quo, mod;

	if ( expected_result != 0 )
	{
			// %error = 100 * Abs((Eexpected - Eactual) / Eexpected);
		div_mod ( 100 * abs(expected_result - actual_result), expected_result, &quo, &mod);
		
		int_error = quo;
		
		if ( (int_error==0) && (expected_result != actual_result) )
			int_error = 1;	/* Notify very small error */
	}
	else
	{	// expected_result == 0
		if ( actual_result == 0 )
			int_error = 0;		// Resultat OK, 0% error
		else
			int_error = 100;	// Resultat KO, 100% error
	}

    return int_error;
}


unsigned int bool_error_percentage(int actual_result, int expected_result)
{
    unsigned int int_error;

		// Mateix valor booleà
	if ( (actual_result != 0) == (expected_result != 0) )
		int_error = 0;		// Resultat OK, 0% error
	else
		int_error = 100;	// Resultat KO, 100% error

    return int_error;
}


