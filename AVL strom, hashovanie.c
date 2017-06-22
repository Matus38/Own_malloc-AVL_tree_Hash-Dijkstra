// Autor: Matus Olejnik

/*
Sociálne siete teraz letia. Ľudia vám zadarmo vypĺňajú databázu osobnými údajmi a vy ich len ďalej predávate inzerentom. Chcete, aby vaša sociálna sieť bola lepšia ako ostatné, tak ste sa rozhodli, že na fanúšikovskej stránke si budú návštevníci môcť naživo prehliadať aktuálny zoznam fanúšikov. Napr. keď si tak prehliadate zoznam fanúšikov na vašej obľúbenej stránke, očami ste na 4774. mieste v abecednom poradí, a v tom sa posunie fanúšik o jedno miesto vyššie, pretože niekto v abecede skôr stránku odlajkoval. Návštevník má teda presný prehľad o aktuálnom stave fanúšikov podľa abecedy.

Túto funkcionalitu budete realizovať nasledovnými funkciami:

void init();
void like(char *page, char *user);
void unlike(char *page, char *user);
char *getuser(char *page, int k);

Vo vašom riešení implementujte tieto funkcie. Testovač najskôr zavolá funkciu init(), v ktorej si môžete pripraviť vaše dátové štruktúry v globálnych premenných (napr. inicializácia stromu). Následne bude volať funkcie like(), unlike() a getuser(), s nasledovným významom:

void like(char *page, char *user) - Používateľ s menom user lajkoval stránku s názvom page

void unlike(char *page, char *user) - Používateľ s menom user odlajkoval stránku s názvom page.

char *getuser(char *page, int k) - Zaujíma nás meno k-teho používateľa v abecednom poradí, ktorý lajkuje stránku page. Ak je týchto používateľov menej ako k, tak funkcia vráti NULL.

Vďaka tejto funkcii sa vašej sociálnej sieti začalo veľmi dariť, a prichádzajú na ňu desiatky tisíc požiadaviek za sekundu. Implementujte vyššie uvedenú funkcionalitu čo možno najefektívnejšie.

Pred odovzdaním si vlastnú implementáciu dôkladne otestujte. Do testovača by ste mali odovzdávať už funkčnú verziu. Pri spustení vám testovač vypíše protokol v rozličných scenároch.
*/

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#define RIGHT -1 /* maximalna hodnota kedy je strom vyvazeny na pravej strane */
#define LEFT 1 /* maximalna hodnota kedy je strom vyvazeny na lavej strane */
#define LNAME(x) ((x)->left->name)	/* kluc v lavej vetve */
#define RNAME(x) ((x)->right->name)	/* kluc v pravej vetve */
#define PRIMENMB 10007	/* velkost pola so strankami a pomocna hodnota pri hash funkcii */

typedef struct node{
	char *name;	/* meno uzivatela */
	int height; /* vyska uzla (1 najnizsia) */
	int Lcount; /* pocet uzlov v lavej vetve */
	int Rcount; /* pocet uzlov v pravej vetve */
	struct node *left, *right;
} NODE;

typedef struct pageStruct{
	char *name;	/* nazov stranky */
	int isChanged;	/* znak ci sa ma zavolat funkcia updateLRCount() */
	NODE *ptr;	/* ukazovatel na strom lajkov */
} PAGESTRUCT;

/* Prototypy funkcii */
int getHeight(NODE *n);
void setHeight(NODE *n);
NODE *leftRotate(NODE *n);
NODE *rightRotate(NODE *n);
void updateLRCount(NODE *n);
NODE *insert(NODE  *n, char *name);
int hashPage(char *str);
int *findUser(NODE *node, int order);
NODE *deleteNode(NODE *n, char *name);
NODE *checkBalance(NODE *n, int state);
void init();
void like(char *page, char *user);
void unlike(char *page, char *user);
char *getuser(char *page, int k);

PAGESTRUCT hashTable[PRIMENMB]; /* Hashovacia tabulka s ukazatelmi na lajky a nazvy jednotlivych stranok */
char *resultName;
char *lastPage;
int lastPlace;

int getHeight(NODE *n){
	return (n == NULL) ? 0 : n->height;
}

/* nastavenie informacie o vyske uzla */
void setHeight(NODE *n){
	int l = getHeight(n->left);
	int r = getHeight(n->right);

	/* najdeme uzol ktory ma vacsiu vysku a nastavime ju do aktualneho uzla zvacsenu o 1 */
	n->height = (l > r) ? l + 1 : r + 1;
}

/*  A		 B
     \		/ \
      B	  =>   A   C
     / \	\
    x   C        x
*/
NODE *leftRotate(NODE *n){
	NODE *NR = n->right;			
	NODE *NR_L = NR->left;		
								
	NR->left = n;					
	n->right = NR_L;			
								
	setHeight(n);
	setHeight(NR);
	
	return NR;
}

/*	    C		 B
	   / 		/ \
	  B  	  =>   A   C
	 / \	          /
	A   x            x
*/
NODE *rightRotate(NODE *n){
	NODE *NL = n->left;
	NODE *NL_R = NL->right;

	NL->right = n;
	n->left = NL_R;

	setHeight(n);
	setHeight(NL);

	return NL;
}

/* funkcia aktualizuje pocty uzlov na lavej a pravej strane jednotlivych uzlov*/
void updateLRCount(NODE *n){
	if (n == NULL)
		return;

	if (n->left != NULL){
		updateLRCount(n->left);
		n->Lcount = n->left->Lcount + n->left->Rcount + 1;
	}
	else
		n->Lcount = 0;

	if (n->right != NULL){
		updateLRCount(n->right);
		n->Rcount = n->right->Lcount + n->right->Rcount + 1;
	}
	else
		n->Rcount = 0;
}

/* vlozenie prvku do stromu a jeho vyvazenie */
NODE *insert(NODE *n, char *name){
	int state;

	if (n == NULL){
		NODE *N = (NODE*)malloc(sizeof(NODE));
		N->name = name;
		N->height = 1;
		N->left = NULL;
		N->right = NULL;
		return N;
	}

	/* ak je meno lexikalne mensie ako meno vrchola posunieme sa do lavej vetvy */
	if (strcmp(name, n->name) < 0)
		n->left = insert(n->left, name);

	/* ak je vkladane meno lexikalne vacsie ako meno vrchola posunieme sa do pravej vetvy */
	else if (strcmp(name, n->name) > 0)
		n->right = insert(n->right, name);

	else
		return n;

	setHeight(n); /* nastavenie vahy novemu prvku */

	/* ↓ kontorla vyvazenosti ↓ */
	state = getHeight(n->left) - getHeight(n->right);

	/* lava vetva je nevyvazena zlava, staci ju rotovat doprava */
	if ((state > LEFT) && (strcmp(name, LNAME(n)) < 0)){
		return rightRotate(n);
	}

	/* lava vetva je nevyvazena zprava, najprv sa teda rotuje do lava, cim sa stane nevyvazenou zlava preto sa nasledne rotuje doprava */
	else if ((state > LEFT) && (strcmp(name, LNAME(n))  > 0)){
		n->left = leftRotate(n->left);
		return rightRotate(n);
	}

	/* prava vetva je nevyvazena zprava, staci ju rotovat dolava */
	else if ((state < RIGHT) && (strcmp(name, RNAME(n)) > 0)){
		return leftRotate(n);
	}
	
	/* prava vetva je nevyvazena zlava, najprv sa teda rotuje do prava, cim sa stane nevyvazenou zprava preto sa nasledne rotuje dolava */
	else if ((state < RIGHT) && (strcmp(name, RNAME(n)) < 0)){
		n->right = rightRotate(n->right);
		return leftRotate(n);
	}
	/* ↑ kontorla vyvazenosti ↑ */

	return n;
}

/* zahashovanie stranky do pola a vratenie jej pozicie */
int hashPage(char *str){
	int place = 0, temp = 0;
	int i = 0, a = 1;

	/* ak sa aktualna stranka zhoduje s naposledy pouzitou vratime hned pozadovanie miesto */
	if ((strcmp(str, lastPage)) == 0)
		return lastPlace;

	else{

		for (i = 0; i < (int)strlen(str); i++){
			temp = temp + ((int)str[i] << (i >> 3));
		}
		
		place = (temp << 3) % PRIMENMB;
		while (1){

			if (((hashTable[place].ptr != NULL) && (strcmp(hashTable[place].name, str) == 0)) || (hashTable[place].name == NULL))
				break;
			else{
				a++;
				place = ((place + a) << 3) % PRIMENMB;
			}
		}
		lastPage = str;	/* aktualizacia nazvu poslednej stranky */
		lastPlace = place;	/* aktualizacia posledneho miesta */
	}
	return place;
}

/* najdenie v poradi "order" uzivatela ktory lajkol stranku */
int *findUser(NODE *node, int order){
	if (node == NULL)
		return NULL;

	/* ak je poradie zhodne s poctom uzlov v lavej vetve sme prave v hladanom uzli */
	if (order == node->Lcount){
		resultName = node->name;
		return 0;
	}
	/* ak je hladane poradie mensie ako pocet uzlov v lavej vetve zacneme hladat v nej */
	else if (order < node->Lcount)
		findUser(node->left, order);

	/* inak ak je hladane poradie mensie alebo rovne ako sucet poctov uzlov v lavej a pravej vetve zacneme hladat v pravej vetve
	   s novou hodnotou poradia zmensenu o pocet uzlov v lavej vetve + aktualny uzol */
	else if (order <= node->Rcount + node->Lcount){
		order = order - node->Lcount - 1;
		findUser(node->right, order);
	}
	else
		resultName = node->name;
	/* inorder prehladavanie (neefektivne)
	findUser(node->left, order, temp);

	(*(temp))++;
	if (*temp == order)
	resultName = node->name;

	findUser(node->right, order, temp);
	*/
	return 0;
}

/* vymazanie prvku zo stromu a jeho vyvazenie */
NODE *deleteNode(NODE *n, char *name){
	int state;

	if (n == NULL)
		return n;

	if (strcmp(name, n->name) < 0)
		n->left = deleteNode(n->left, name);

	else if (strcmp(name, n->name) > 0)
		n->right = deleteNode(n->right, name);

	/* ak neplati ziadna podmienka ukazuje sa na pozadovane miesto,
	najprv sa skontroluje ci ma uzol 1 alebo ziadneho potomka */
	else if ((n->left == NULL) || (n->right == NULL)){

		/* ak ma laveho potomka priradi sa do aktualneho */
		if (n->left != NULL){
			n = n->left;
		}
		/* ak ma praveho potomka priradi sa do aktualneho */
		else if (n->right != NULL){
			n = n->right;
		}
		/* inak nema ziadneho potomka tak sa nastavi na NULL */
		else{
			n = NULL;
		}
	}
	/*  ak ma uzol dvoch potomkov */
	else{
		/* v pravej vetve prejdeme na najmensi prvok */
		NODE *temp = n->right;
		while (temp->left != NULL)
			temp = temp->left;

		/* skopirujeme meno najmensieho prvku pravej vetvy na miesto prvka ktory sme vymazali */
		n->name = temp->name;

		/* zacneme rekurziu na vymazanie prvku ktory sme presunuli na miesto vymazaneho */
		n->right = deleteNode(n->right, temp->name);
	}

	if (n == NULL)
		return n;

	setHeight(n);
	state = getHeight(n->left) - getHeight(n->right);
	
	n = checkBalance(n, state);
	return n;
}

/* kontorla vyvazenosti */
NODE *checkBalance(NODE *n, int state){
	
	/* lava vetva je nevyvazena zlava, staci ju rotovat doprava */
	if ((state > LEFT) && (getHeight(n->left->left) - getHeight(n->left->right)) > 0)
		return rightRotate(n);

	/* lava vetva je nevyvazena zprava, najprv sa teda rotuje do lava, cim sa stane nevyvazenou zlava preto sa nasledne rotuje doprava */
	else if ((state > LEFT) && (getHeight(n->left->left) - getHeight(n->left->right)) < 0){
		n->left = leftRotate(n->left);
		return rightRotate(n);
	}

	/* prava vetva je nevyvazena zprava, staci ju rotovat dolava */
	else if ((state < RIGHT) && (getHeight(n->right->left) - getHeight(n->right->right)) < 0)
		return leftRotate(n);

	/* prava vetva je nevyvazena zlava, najprv sa teda rotuje do prava, cim sa stane nevyvazenou zprava preto sa nasledne rotuje dolava */
	else if ((state < RIGHT) && (getHeight(n->right->left) - getHeight(n->right->right)) > 0){
		n->right = rightRotate(n->right);
		return leftRotate(n);
	}

	return n;
}

/* Priprava datovych struktur */
void init(){
	int i;

	for (i = 0; i < PRIMENMB; i++){
		hashTable[i].name = NULL;
		hashTable[i].isChanged = 0;
		hashTable[i].ptr = NULL;
	}
	lastPage = "";
	lastPlace = -1;
}

/* Používateľ s menom user lajkoval stránku s názvom page */
void like(char *page, char *user){
	int place = hashPage(strdup(page));
	hashTable[place].name = strdup(page);
	hashTable[place].ptr = insert(hashTable[place].ptr, strdup(user));
	hashTable[place].isChanged = 1; /* znak ze pri tejto stranke sa ma spustit aj funkcia updateLRCount() */
}

/* Používateľ s menom user odlajkoval stránku s názvom page */
void unlike(char *page, char *user){
	int place = hashPage(strdup(page));
	hashTable[place].ptr = deleteNode(hashTable[place].ptr, strdup(user));
	hashTable[place].isChanged = 1; /* znak ze pri tejto stranke sa ma spustit aj funkcia updateLRCount() */
}

/* Vrati meno k-teho používateľa v abecednom poradí, ktorý lajkuje stránku page. Ak je týchto používateľov menej ako k, tak funkcia vráti NULL */
char *getuser(char *page, int k){

	int place; /* pozicia stranky v hash tabulke */
	place = hashPage(strdup(page));

	/* ak sa menili hodnoty v aktualnej stranke aktualizujeme ich */
	if (hashTable[place].isChanged){
		updateLRCount(hashTable[place].ptr);
		hashTable[place].isChanged = 0;
	}

	if ((hashTable[place].ptr == NULL) || (k < 1) || ((hashTable[place].ptr->Lcount + hashTable[place].ptr->Rcount + 1  < k))){
		//printf("NULL\n");
		return NULL;
	}

	findUser(hashTable[place].ptr, k - 1);
	//printf("%s\n", resultName);
	return resultName;
}


// Tlacidlo 'Spustit' vyskusa vasu socialnu siet na testovacich scenaroch.
// Vlastna funkcia main() nie je potrebna, a nebude sa spustat.
