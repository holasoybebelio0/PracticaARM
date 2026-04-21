/*-------------------------------------------------------------------
|   "avgmaxmintemp.c": rutines de càlcul de valors mitjans, màxims
|	i mínims d'una taula de temperatures, expressades en graus
|	Celsius o Fahrenheit, en format de coma flotant E9M22.
|	Implementació cridant rutines aritmètiques libE9M22.
| -------------------------------------------------------------------
|	santiago.romani@urv.cat
|	pere.millan@urv.cat
|	(Febrer-Març 2025, Febrer 2026)
| -------------------------------------------------------------------
| © URV — Codi de la pràctica d'ARM de Fonaments de Computadors (FC).
| Ús exclusiu dins l’assignatura; prohibida la seva redistribució.
| Consulteu la llicència: ACADEMIC_LICENSE_URV_FC.txt
| -------------------------------------------------------------------*/

#include "E9M22.h"				/* Tipus Coma Flotant E9M22 i operacions */
#include "CelsiusFahrenheit.h"	/* rutines de conversió C->F i F->C */
#include "avgmaxmintemp.h"		/* mmres: tipus de retorn de màxim i mínim */

#define E9M22_12	0x40A00000	// Valor 12.0 codificat en coma flotant E9M22

/* avgmaxmin_city(): calcula la temperatura mitjana, màxima i mínima 
|				d'una ciutat d'una taula de temperatures, 
|				amb una fila per ciutat i una columna per mes, 
|				expressades en graus Celsius en format E9M22.
|	Paràmetres:
|		ttemp[][12]	→	taula de temperatures, amb 12 columnes i nrows files
|		nrows		→	número de files de la taula
|		id_city		→	índex de la fila (ciutat) a processar
|		*mmres		→	adreça de l'estructura maxmin_t que retornarà 
|						els resultats de temperatures màximes i mínimes
|	Resultat:	temperatura mitjana, expressada en graus Celsius, en format E9M22.
*/		
e9m22_t avgmaxmin_city(e9m22_t ttemp[][12], unsigned short nrows, unsigned short id_city, maxmin_t *mmres)
{
    e9m22_t avg, max, min;
    unsigned short idmin = 0, idmax = 0;	// índexs de temp. mínima i màx.
    unsigned short i;
    e9m22_t tvar;						// variable temporal de temperatura
  
    avg = ttemp[id_city][0];		// inicialitza valors amb primera columna
    max = avg;						// (mes de gener)
    min = avg;
    for (i = 1; i < 12; i++)		// per a la resta de mesos
    {
        tvar = ttemp[id_city][i];			// obtenir temperatura del següent mes
        avg = e9m22_add(avg, tvar);

        if ( e9m22_compare (tvar, max) == E9M22_CMP_GREATER )
        {	
            max = tvar;						// actualitzar valors resultat
            idmax = i;
        }
        if ( e9m22_compare (tvar, min) == E9M22_CMP_LESS )
        {
            min = tvar;
            idmin = i;
        }
    }

        //	avg /= 12;	ajustar valor mitjà
    avg = e9m22_div(avg, E9M22_12);	

    mmres->tmin_C = min;			// transferir mínim-màxim en Celsius
    mmres->tmax_C = max;
                                    // transferir mínim-màxim en Fahrenheit
    mmres->tmin_F = Celsius2Fahrenheit(min);
    mmres->tmax_F = Celsius2Fahrenheit(max);
                                    // transferir índexos mínim i màxim
    mmres->id_min = idmin;
    mmres->id_max = idmax;

    return(avg);
}



/* avgmaxmin_month(): calcula la temperatura mitjana, màxima i mínima d'un mes
|				d'una taula de temperatures, amb una fila per ciutat i una
|				columna per mes, expressades en graus Celsius en format E9M22.
|	Paràmetres:
|		ttemp[][12]	→	taula de temperatures, amb 12 columnes i nrows files
|		nrows		→	número de files de la taula (mínim 1 fila)
|		id_month	→	índex de la columna (mes) a processar
|		*mmres		→	adreça de l'estructura maxmin_t que retornarà 
|						els resultats de temperatures màximes i mínimes
|	Resultat:	temperatura mitjana, expressada en graus Celsius, en format E9M22.
*/		
e9m22_t avgmaxmin_month(e9m22_t ttemp[][12], unsigned short nrows, unsigned short id_month, maxmin_t *mmres)
{
    e9m22_t avg, max, min;
    unsigned short idmin = 0, idmax = 0;	// índexos de temp. mínima i màx.
    unsigned short i;
    e9m22_t tvar;					// variable temporal de temperatura
 
    avg = ttemp[0][id_month];		// inicialitza valors amb primera fila
    max = avg;						// (primera ciutat de la taula)
    min = avg;
    i = 1;							// posicionar índex a la segona fila
    while (i < nrows)				// per a la resta de ciutats
    {
        tvar = ttemp[i][id_month];		// obtenir temperatura següent ciutat
        //avg += tvar;
        avg = e9m22_add(avg, tvar);

        if ( e9m22_compare (tvar, max) == E9M22_CMP_GREATER )
        {	
            max = tvar;						// actualitzar valors resultat
            idmax = i;
        }
        if ( e9m22_compare (tvar, min) == E9M22_CMP_LESS )
        {
            min = tvar;
            idmin = i;
        }

        i++;
    }
        // calcular valor mitjà sobre avg
    avg = e9m22_div(avg, int_to_e9m22(nrows) );	

    mmres->tmin_C = min;			// transferir mínim-màxim en Celsius
    mmres->tmax_C = max;
                                    // transferir mínim-màxim en Fahrenheit
    mmres->tmin_F = Celsius2Fahrenheit(min);
    mmres->tmax_F = Celsius2Fahrenheit(max);
                                    // transferir índexos mínim i màxim
    mmres->id_min = idmin;
    mmres->id_max = idmax;

    return(avg);
}

@;-----------------------------------------------------------------------
@; Fahrenheit2Celsius(): converteix Fahrenheit a Celsius (E9M22)
@;	Entrada: R0 (input Fahrenheit)
@;	Sortida: R0 (resultat Celsius)
@;-----------------------------------------------------------------------
	.global Fahrenheit2Celsius
Fahrenheit2Celsius:
    @; PROLOGUE: Salvem els registres que farem servir i l'adreça de retorn
    push {r4, lr}           @; Salvem r4 i lr a la pila per preservar-los
    mov  r4, r0             @; Guardem l'input (Fahrenheit) a r4

    @; 1a part càlcul: resultat = (input - 32.0)
    @; El valor 32.0 en format E9M22 és 0x41000000
    ldr  r1, =0x41000000    @; Carreguem la constant 32.0 a r1
    bl   e9m22_sub          @; Crida a la resta: r0 = r4 - r1

    @; 2a part càlcul: resultat = (input - 32.0) * 5/9
    @; El valor 5.0 / 9.0 (aprox. 0.555555) en format E9M22 és 0x3F0E38E3
    ldr  r1, =0x3F0E38E3    @; Carreguem la constant 5/9 a r1
    bl   e9m22_mul          @; Crida a la multiplicació: r0 = r0 * r1

    @; EPILOGUE: Alliberem la pila i tornem
    pop  {r4, pc}           @; Restaurem r4 i fem el salt de tornada
