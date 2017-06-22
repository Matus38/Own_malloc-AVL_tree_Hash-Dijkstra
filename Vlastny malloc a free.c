// Autor: Matus Olejnik

/*
V �tandardnej kni�nici jazyka C s� pre alok�ciu a uvo�nenie pam�ti k dispoz�cii funkcie malloc, a free. V tomto zadan� je �lohou implementova� vlastn� verziu alok�cie pam�ti. Va�ou �lohou je implementova� �TYRI funkcie:

void *memory_alloc(unsigned int size);
int memory_free(void *valid_ptr);
int memory_check(void *ptr);
void memory_init(void *ptr, unsigned int size);
Vo vlastnej implement�cii m��ete definova� aj in� pomocn� funkcie ako vy��ie spomenut�, nesmiete v�ak pou�i� existuj�ce funkcie malloc a free.

Funkcia memory_alloc m� poskytova� slu�by analogick� �tandardn�mu malloc. Teda, vstupn� parametre s� ve�kos� po�adovan�ho s�visl�ho bloku pam�te a funkcia mu vr�ti: ukazovate� na �spe�ne alokovan� kus vo�nej pam�te, ktor� sa vyhradil, alebo NULL, ke� nie je mo�n� s�visl� pam� po�adovanej ve�kos� vyhradi�.

Funkcia memory_free sl��i na uvo�nenie vyhraden�ho bloku pam�ti, podobne ako funkcia free. Funkcia vr�ti 0, ak sa podarilo (funkcia zbehla �spe�ne) uvo�ni� blok pam�ti, inak vr�ti 1. M��ete predpoklada�, �e parameter bude v�dy platn� smern�k z predch�dzaj�cich volan� vr�ten� funkciou memory_alloc, ktor� e�te nebol uvo�nen�.

Funkcia memory_check sl��i na skontrolovanie, �i parameter (smern�k) je platn� smern�k, ktor� bol v nejakom z predch�dzaj�cich volan� vr�ten� funkciou memory_alloc a zatia� nebol uvo�nen� funkciou memory_free. Funkcia vr�ti 0, ak sa je smern�k neplatn�, inak vr�ti 1.

Funkcia memory_init sl��i na inicializ�ciu spravovanej vo�nej pam�te. Predpokladajte, �e funkcia sa vol� pr�ve raz pred v�etk�mi volaniami memory_alloc a memory_free. Vi� testovanie ni��ie. Ako vstupn� paramter funkcie pr�de blok pam�te, ktor� m��ete pou�i� pre organizovanie a aj pridelenie vo�nej pam�te.Va�e funkcie nem��u pou��va� glob�lne premenn� okrem jednej glob�lnej premennej na zapam�tanie smern�ku na pam�, ktor� vstupuje do funkcie memory_init. Smern�ky, ktor� pride�uje va�a funkcia memory_alloc musia by� v�hradne z bloku pam�te, ktor� bola pridelen� funkcii memory_init.

Program je mo�n� odovzda� len raz. Pred odovzdan�m si vlastn� implement�ciu d�kladne otestujte. Do testova�a by ste mali odovzd�va� u� funk�n� verziu. Pri spusten� v�m testova� vyp�e protokol o alok�ci� v rozli�n�ch scen�roch.
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