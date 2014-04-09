Potrebnoj je prevesti program s naredbom:
	make

Pokretanje ide s:
	./LAB2 <ulazna datoteka>

Parametri se podešavaju unutar funkcije main, polje gaParams:
	0:sigma1 - disprezija za operator mutacija1  
	1:sigma2 - disperzija za operator mutacija2
	2:p1 - vjerojatnost mutacije u opetoru1
	3:p2 - vjerojatnost mutacije u operatoru2
	4:v - vjerojatnost uputrebe operatora1
	5:iteracija - broj iteracija
	6:eps - vrijednost pogreske 
	7:NC - broj jedinki
	8:min  
	9:max  - [min, max] interval u kojem su generirani pocetni parametri
	10:k - k-turnira

Ukoliko se mijenjaju parametri
	make cLAB2
	make 

i onda ponovno pokretanje 
	./LAB2 <ulazna datoteka>