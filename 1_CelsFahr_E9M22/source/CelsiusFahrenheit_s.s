@;--------------------------------------------------------------------
@;  CelsiusFahrenheit.s: rutines de conversió de temperatura 
@;						 en format de Coma Flotant E9M22. 
@;		Implementació cridant rutines aritmètiques libE9M22.
@;--------------------------------------------------------------------
@;	santiago.romani@urv.cat
@;	pere.millan@urv.cat
@;	(Març 2021-2023, Febrer 2024, Març 2025, Març 2026)
@;--------------------------------------------------------------------
@;	Programador/a 1: evelio.ruiz@estudiants.urv.cat
@;	Programador/a 2: pau.gutierrez@estudiants.urv.cat
@;--------------------------------------------------------------------
@; © URV — Codi de la pràctica d'ARM de Fonaments de Computadors (FC).
@; Ús exclusiu dins l’assignatura; prohibida la seva redistribució.
@; Consulteu la llicència: ACADEMIC_LICENSE_URV_FC.txt
@;--------------------------------------------------------------------

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
		push {r4, lr}           
		ldr  r1, =0x3FF33333    @; Constant 1.8
		bl   e9m22_mul_c        @; <--- AÑADE _c
		ldr  r1, =0x41000000    @; Constant 32.0
		bl   e9m22_add_c        @; <--- AÑADE _c
		pop {r4, pc}



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
		push {r4, lr}
		
		ldr r1, =0x41000000
		bl e9m22_sub_s
		
		
		ldr r1, =0x3F871C71
		bl e9m22_mul_c
		
		
		pop {r4, pc}






.end

