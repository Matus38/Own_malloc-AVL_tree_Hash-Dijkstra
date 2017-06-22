// Autor: Matus Olejnik

/*
V štandardnej kninici jazyka C sú pre alokáciu a uvo¾nenie pamäti k dispozícii funkcie malloc, a free. V tomto zadaní je úlohou implementova vlastnú verziu alokácie pamäti. Vašou úlohou je implementova ŠTYRI funkcie:

void *memory_alloc(unsigned int size);
int memory_free(void *valid_ptr);
int memory_check(void *ptr);
void memory_init(void *ptr, unsigned int size);
Vo vlastnej implementácii môete definova aj iné pomocné funkcie ako vyššie spomenuté, nesmiete však poui existujúce funkcie malloc a free.

Funkcia memory_alloc má poskytova sluby analogické štandardnému malloc. Teda, vstupné parametre sú ve¾kos poadovaného súvislého bloku pamäte a funkcia mu vráti: ukazovate¾ na úspešne alokovanı kus vo¾nej pamäte, ktorı sa vyhradil, alebo NULL, keï nie je moné súvislú pamä poadovanej ve¾kos vyhradi.

Funkcia memory_free slúi na uvo¾nenie vyhradeného bloku pamäti, podobne ako funkcia free. Funkcia vráti 0, ak sa podarilo (funkcia zbehla úspešne) uvo¾ni blok pamäti, inak vráti 1. Môete predpoklada, e parameter bude vdy platnı smerník z predchádzajúcich volaní vrátení funkciou memory_alloc, ktorı ešte nebol uvo¾nenı.

Funkcia memory_check slúi na skontrolovanie, èi parameter (smerník) je platnı smerník, ktorı bol v nejakom z predchádzajúcich volaní vrátení funkciou memory_alloc a zatia¾ nebol uvo¾nenı funkciou memory_free. Funkcia vráti 0, ak sa je smerník neplatnı, inak vráti 1.

Funkcia memory_init slúi na inicializáciu spravovanej vo¾nej pamäte. Predpokladajte, e funkcia sa volá práve raz pred všetkımi volaniami memory_alloc a memory_free. Viï testovanie nišie. Ako vstupnı paramter funkcie príde blok pamäte, ktorú môete poui pre organizovanie a aj pridelenie vo¾nej pamäte.Vaše funkcie nemôu pouíva globálne premenné okrem jednej globálnej premennej na zapamätanie smerníku na pamä, ktorá vstupuje do funkcie memory_init. Smerníky, ktoré pride¾uje vaša funkcia memory_alloc musia by vıhradne z bloku pamäte, ktorá bola pridelená funkcii memory_init.

Program je moné odovzda len raz. Pred odovzdaním si vlastnú implementáciu dôkladne otestujte. Do testovaèa by ste mali odovzdáva u funkènú verziu. Pri spustení vám testovaè vypíše protokol o alokácií v rozliènıch scenároch.
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

void *mmr_ptr;

void createHeader(void *ptr, int size){
	*(unsigned int*)ptr = size - sizeof(int);
}

//ak exstuje viacero volnych blokov vedla seba zluci ich do jedneho
void merge(){

	void *act, *prev;
	int mmr_all = 0;

	act = (char*)mmr_ptr + sizeof(int); 
	prev = act;

	//pripocitanie velkosti bloka pre podmienku v cykle
	//DOLEZITE ESTE PRED ZACATIM CYKLU ABY NASLEDNE KU KONCU POINTER NEUKAZOVAL ZA POLE
	mmr_all += abs(*(int*)act) + sizeof(int);	

	while (mmr_all != (*(int*)mmr_ptr)){
		act = (char*)act + (abs(*(unsigned int*)act)) + sizeof(int); //prejdenie na nasledujucu hlavicku
		mmr_all += abs(*(int*)act) + sizeof(int);	//pripocitanie velkosti bloka pre podmienku v cykle

		//ak je velkost predchadzajuceho a aktualneho bloku kladna, su obaja kladne a mozme ich zlucit
		if (((*(int*)prev) > 0) && ((*(int*)act) > 0)){
			(*(int*)prev) += (*(int*)act) + sizeof(int);	//plus pripocitanie aj hlavicky
		}
		else{
			prev = act;
		}
	}
}

//funkcia priradi ukazatel do globalnej premennej a vytvori hlavnu hlavicku a druhu nasledujucu
void memory_init(void *ptr, unsigned int size){
	mmr_ptr = ptr;  // pointer na zaciatok pamate

	//vytvorenie prvej hlavnej hlavicky
	createHeader(mmr_ptr, size);

	//vytvorenie druhej nasledujucej hlavicky s ulozenou velkostou mensou o velkost dvoch hlaviciek
	createHeader((char*)mmr_ptr + sizeof(int), size - sizeof(int));
}

int memory_free(void *valid_ptr){

	valid_ptr = (unsigned int*)valid_ptr - 1;	//nastavenie na hlavicku
	*(int*)valid_ptr *= -1;						//prevratenie hodnoty velkosti na kladnu, co znaci ze do bloku sa moze zasa zapisovat

	merge();

	return 0;
}

void *memory_alloc(unsigned int size){

	void *act, *new;
	int mmr_all = 0;	//premenna v ktorej bude ulozeny sucet velkosti jednotlivych blokov

	act = (char*)mmr_ptr + sizeof(int);	//ukazem za hlavnu hlavicku v ktorej je uchovana hlavna velkost
	
	while (mmr_all != (*(int*)mmr_ptr)){	//dokial nie je sucet velkosti vsetkych blokov rovny velkosti pamate prehladavam bloky

		//ak je hodnota kladna oznacuje to volny blok
		if ((*(int*)act) > 0){
			//ak sa velkost zostavajucej pamate rovna pozadovanej velkosti tak ju len vratime a nevytvarame hlavicku
			if ((*(unsigned int*)act == size)){
				*(int*)act *= -1;	//velkost bloku nastavime na zapornu hodnotu ktora znaci ze blok je obsadeny
				act = (unsigned int*)act + 1;	//posunieme sa za hlavicku
				return (void*)act;
			}
			//ak je zostavajuca velkost vacsia ako pozadovana + velkost hlavicky
			//vytvori sa nova hlavicka a vrati sa pozicia
			else if ((*(unsigned int*)act > size + sizeof(int))){
				//vytvorenie novej hlavicky na mieste o velkost aktualneho bloku dalej
				new =(char*)act + size + sizeof(int);
				createHeader(new, (abs(*(unsigned int*)act)) - size);	

				//zmenenie aktualnej velkosti bloku a prevratenie na zapornu hodnotu ako znak ze je obsadeny
				*(int*)act = size * -1;

				act = (unsigned int*)act + 1;
				return (void*)act;
			}
			//ak velkost bloku je vacsia alebo rovna ako pozadovana velkost A ZAROVEN
			//ak vyhovuje velkost ale nezmesti sa nova hlavicka
			//alebo vyhovuje velkost a zmesti sa navyse len hlavicka
			//tak sa prideli navyse zostavajuca cast z hlavicky
			else if (((*(unsigned int*)act) <= size + sizeof(int)) && ((*(unsigned int*)act) >= size)){
				*(int*)act *= -1;

				act = (unsigned int*)act + 1;
				return (void*)act;
			}
		}
		mmr_all += abs(*(int*)act) + sizeof(int);	//pripocitanie velkosti bloka pre podmienku v cykle
		act = (char*)act + (abs(*(unsigned int*)act)) + sizeof(int);	//presun na dalsiu hlavicku
	}

	//toto nastane len ak sa v cykle vyssie nereturne ziadny pointer co znaci ze nie je dalsia volna pamat
	printf("Nepodarilo sa pridelit pamat\n");
	return NULL;
}

int memory_check(void *ptr){

	ptr = (unsigned int*)ptr - 1;	//nastavenie na hlavicku

	return ((*(int*)ptr) < 0) ? 1 : 0;	//ak je hodnota zaporna blok je alokovany, ak je hodnota kladna blok nie je alokovany
}

// Tlacidlo 'Spustit' vyskusa vasho spravcu pamati na testovacich scenaroch.
// Vlastna funkcia main() nie je potrebna, a nebude sa spustat.