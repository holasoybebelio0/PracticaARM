@;----------------------------------------------------------------
@;  CelsiusFahrenheit.s: rutines de conversió de temperatura 
@;						 en format de Coma Flotant E9M22. 
@;		Implementació cridant rutines aritmètiques libE9M22.
@;----------------------------------------------------------------
@;	santiago.romani@urv.cat
@;	pere.millan@urv.cat
@;	(Març 2021-2023, Febrer 2024, Març 2025, Març 2026)
@;----------------------------------------------------------------
@;	Programador/a 1: xxx.xxx@estudiants.urv.cat
@;	Programador/a 2: yyy.yyy@estudiants.urv.cat
@;----------------------------------------------------------------

.include "E9M22.i"	@; operacions en coma flotant E9M22


.text
		.align 2
		.arm


@; Celsius2Fahrenheit(): converteix una temperatura en graus Celsius 
@;						a la temperatura equivalent en graus Fahrenheit, 
@;						usant valors codificats en Coma Flotant E9M22.
@;	Entrada:
@;		input 	-> R0
@;	Sortida:
@;		R0 		-> output = (input * 9/5) + 32.0;
	.global Celsius2Fahrenheit
Celsius2Fahrenheit:
				@; ús de registres:
				@; r0: paràmetre input / resultat output
				@; rN: ...
		push {lr}
		
		ldr r0, =E9M22_sNAN		@; to-do: NaN per indicar rutina pendent
		
		pop {pc}



@; Fahrenheit2Celsius(): converteix una temperatura en graus Fahrenheit a la
@;						temperatura equivalent en graus Celsius, utilitzant
@;						valors codificats en Coma Flotant E9M22.
@;	Entrada:
@;		input 	-> R0
@;	Sortida:
@;		R0 		-> output = (input - 32.0) * 5/9;
	.global Fahrenheit2Celsius
Fahrenheit2Celsius:
				@; ús de registres:
				@; r0: paràmetre input / resultat output
				@; rN: ...
		push {lr}
		
		ldr r0, =E9M22_sNAN		@; to-do: NaN per indicar rutina pendent
		
		pop {pc}

.end
