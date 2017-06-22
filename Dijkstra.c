// Autor: Matus Olejnik

/*
Popolvár je jednoduchı chlapec z dediny, ktorı celé dni nerobí niè iné ako vynášanie popola zo sporáka v kuchyni a programovanie pre dobroèinné úèely. Dnes, ako tak surfoval po internete, sa k nemu dostala ve¾mi pohoršujúca správa. V neïalekej krajine uniesol drak tri princezné a schoval sa vysoko v horách, kde je ve¾mi akı prístup a chystá sa tam zajtra o polnoci všetky tri princezné zjes.

Samozrejme, e sa tomu nemôete len tak neèinne prizera. Vïaka modernım technológiám máte k dispozícií aj mapu, ktorú rıchlo zverejnil krá¾ – otec troch nešastníc. Vašou úlohou je teda prirodzene èo najskôr najprv zabi draka a potom zachráni všetky tri princezné.

Mapa predstavuje dvojrozmernú mrieku políèok, ktoré môu obsahova:

c	Lesnı chodník
h	Hustı lesnı porast, cez ktorı idete dva-krát pomalšie ako po lesnom chodníku
n	Nepriechodnú prekáku (napr. skalu)
d	Draka
p	Princeznú
0-9	Zastávka lanovky, ktorou sa môete odviez
g	Generátor, ktorım zapnete elektriku pre lanovky
Vašou úlohou je implementova nasledovnú funkciu:

int *zachran_princezne(char **mapa, int n, int m, int t, int *dlzka_cesty);
Táto funkcia má nájs cestu, ktorou postupova, aby popolvár èo najskôr zachránil všetky tri princezné a zabil draka. Mapa je vysoká n políèok a široká m políèok (1 <= n, m, <= 1000). Parameter t (0 <= t <= 106) urèuje, kedy od zaèiatku vášho h¾adania sa drak zobudí a zje prvú princeznú. Keïe drak je ve¾mi rıchly a dokáe sa teleportova, môete sa spo¾ahnú, e ak sa vám ho nepodarí zabi dovtedy ako sa zobudí, princezné u nezachránite. Prechod cez lesnı chodník trvá jednu jednotku èasu a drak sa zobudí v t-tej jednotke èasu, kedy u bude neskoro. A nezabudnite, e najprv musíte zabi draka, a potom môete zachraòova princezné (hoci by ste aj prechádzali predtım cez políèko kde je princezná).

Aby ste mohli poui akúko¾vek lanovku, musíte najprv spusti generátor, inak iadna lanovka nebude fungova. Kadá lanovka je oznaèená nejakou cifrou (od 0 po 9) a lanovkou sa môete premáva iba medzi jej zastávkami, ktoré sú oznaèené rovnakou cifrou. Keï teda nastúpite na zastávku lanovky 0, tak si môete vybra ktorúko¾vek jej inú zastávku oznaèenú na mape rovnako 0. Lanovke trvá (dx+dy+3)/4 èasu, aby sa presunula medzi dvoma zastávkami, kde dx je horizontálny rozdiel pozícií tıchto zastávok a dy je ich vertikálny rozdiel. Delenie je celoèíselné.

Nájdenú cestu vráte ako postupnos súradníc (dvojíc celıch èísel x,y, kde 0 <= x < n a 0 <= y < m) všetkıch navštívenıch políèok. Na zaèiatku sa vdy nachádzate na políèku so súradnicami 0,0 (ktoré bude uvedené ako prvé v ceste) a na poslednom navštívenom políèku musí by jedna z troch princezien. Ak existuje viacero rovnako dlho trvajúcich ciest, vráte tú, ktorá je lexikograficky prvá. Parameter dlzka_cesty je vıstupnı parameter, ktorı nastavte na poèet súradníc, ktoré vrátite. Implementujte vyššie uvedenú funkcionalitu èo mono najefektívnejšie.

Príklad pouitia vašej funkcie:

int i;
int *cesta = zachran_princezne(mapa, n, m, t, &dlzka_cesty);
for (i = 0; i < dlzka_cesty; ++i)
printf("%d %d\n", cesta[i*2], cesta[i*2+1]);
Doleité poznámky: Okrem cestovania lanovkou sa Popolvár presúva len v štyroch smeroch (hore, dole, do¾ava, doprava). Ak nie je zadané inak, prechod cez políèko trvá štandardne jednu jednotku èasu. Teda cez políèka s drakom, princeznou, lanovkou a generátorom trvá prechod tie jednu jednotku èasu. Generátor je na mape najviac jeden. Do vısledného èasu sa zapoèítavajú všetky políèka, cez ktoré Popolvár prejde. Ak teda zaène na políèku (0,0), prejde cez políèko (1,0) a skonèí na políèku (1,1), prièom všetky tri políèka obsahujú lesnı chodník, tak Popolvárovi to trvá 3 jednotky èasu. Zabitie draka, nástup do lanovky, vıstup z lanovky a zapnutie generátora je okamitá akcia, ktorıch trvanie je zanedbate¾né vzh¾adom na èas trvania prechodu cez políèko, ich zarátavame s nulovou dåkou trvania. Keïe Popolvár má celú radu ïalších princezien, ktoré musí ešte zachráni v inıch èastiach sveta (na vstupe je viacero krá¾ovstiev, v ktorıch chce zachráni princezné), musíte èo najskôr zachráni princezné (celkovo vrátane zabitia draka), aby ste mohli èím skôr prejs do ïalšieho krá¾ovstva.

Pred odovzdaním si vlastnú implementáciu dôkladne otestujte. Do testovaèa by ste mali odovzdáva u funkènú verziu. Pri spustení vám testovaè vypíše protokol v rozliènıch scenároch.
*/

#include <stdio.h>
#include <stdlib.h>

#define LEFT(x)  (x*2)
#define RIGHT(x)  ((x*2) + 1)
#define BIGNMB 300000

typedef struct node{
	int x;
	int y;
	int order;
	int distance;
	struct node *next;
} NODE;

int getDistance(int x, int y, char **map);
NODE *newNode(int x, int y, int ord, int dist);
void newEdge(NODE *node, NODE *nNode);
void prepareMap(int x, int y, char **map);
void insertToHeap(NODE *node);
NODE *getMinFromHeap();
int *dijkstra(int from, int *previous[], int x, int y);
int *zachran_princezne(char **mapa, int n, int m, int t, int *dlzka_cesty);

NODE **nodeArray;
  
NODE heap[BIGNMB];
int dragon, heapCount = 0;
int princess[3] = { -1, -1, -1 };
int princessCount = 0;

int getDistance(int x, int y, char **map){
	switch (map[x][y]){
		case 'c': return 1;
		case 'd': return 1;
		case 'g': return 1;
		case 'h': return 2;
		case 'n': return -1;
		case 'p': return 1;
	}
	if ((map[x][y] >= '0') && (map[x][y] <= '9'))
		return 1;

	return 0;
}

NODE *newNode(int x, int y, int ord, int dist){
	NODE *node;

	node = (NODE *)malloc(sizeof(NODE));
	node->x = x;
	node->y = y;
	node->order = ord;
	node->distance = dist;
	node->next = NULL;

	return node;
}

void newEdge(NODE *node, NODE *nNode){
	NODE *akt = NULL;
	akt = node;
  
	while(akt->next != NULL){
    	akt = akt->next;
  	}
	akt->next = newNode(nNode->x, nNode->y, nNode->order, nNode->distance);
}

void prepareMap(int x, int y, char **map){
	int i, j;
	int pos = 0;
	
	nodeArray = (NODE **)malloc(x * y * sizeof(NODE *));
  	
	for (i = 0; i < x; i++){
		for (j = 0; j < y; j++, pos++){

			if (map[i][j] == 'd')
				dragon = pos;

			else if (map[i][j] == 'p'){
				princess[princessCount] = pos;
				princessCount++;
			}
          
			nodeArray[pos] = newNode(i, j, pos, getDistance(i, j, map));

			if (i > 0){
				newEdge(nodeArray[pos - y], nodeArray[pos]);
				newEdge(nodeArray[pos], nodeArray[pos - y]);
			}
			if (j > 0){
				newEdge(nodeArray[pos], nodeArray[pos - 1]);
				newEdge(nodeArray[pos - 1], nodeArray[pos]);
			}
		}
	}

}

void insertToHeap(NODE *node){
	
	heapCount++;
	int j = heapCount;
	
	while (j > 1 && (node->distance < heap[j / 2].distance)){
		heap[j] = heap[j / 2];
		j /= 2;
	}
		
	heap[j].x = node->x;
	heap[j].y = node->y;
	heap[j].order = node->order;
	heap[j].distance = node->distance;
}

NODE *getMinFromHeap(){
	int child, act;
	NODE *min, *last;
	min = (NODE *)malloc(sizeof(NODE));
	last = (NODE *)malloc(sizeof(NODE));
	
	if (heapCount == 0) 
		return NULL;

	min->x = heap[1].x;
	min->y = heap[1].y;
	min->order = heap[1].order;
	min->distance = heap[1].distance;
	min->next = heap[1].next;

	last->x = heap[heapCount].x;
	last->y = heap[heapCount].y;
	last->order = heap[heapCount].order;
	last->distance = heap[heapCount].distance;
	last->next = heap[heapCount].next;
  	heapCount--;
	
	for (act = 1; act * 2 <= heapCount; act = child){
		child = act * 2;
		if (child != heapCount && heap[child + 1].distance < heap[child].distance){
			child++;
		}
		if (last->distance > heap[child].distance){
			heap[act] = heap[child];
		}
		else{
			break;
		}
	}
	heap[act].x = last->x;
	heap[act].y = last->y;
	heap[act].order = last->order;
	heap[act].distance = last->distance;
	heap[act].next = last->next;

	return min;
}

int *dijkstra(int from, int *previous[], int x, int y){
	int *distance;
	NODE *akt, *minHeapNode;

	distance = (int *)malloc(x * y * sizeof(int));
	*previous = (int *)malloc(x * y * sizeof(int));

	int i;
	for (i = 0; i < x* y; i++){
		distance[i] = BIGNMB;
		(*previous)[i] = -1;
	}


	distance[from] = 0;
	(*previous)[from] = 1;
	insertToHeap(nodeArray[from]);
  
	while (heapCount > 0){
		minHeapNode = getMinFromHeap();
		akt = nodeArray[minHeapNode->order];

		while (akt != NULL){
			if (distance[minHeapNode->order] + akt->distance < distance[akt->order]){
				distance[akt->order] = akt->distance + distance[minHeapNode->order];
				insertToHeap(akt);
				(*previous)[akt->order] = minHeapNode->order;
				
			}
			akt = akt->next;
		}
	}
	return distance;
}

int *zachran_princezne(char **mapa, int n, int m, int t, int *dlzka_cesty){
	int *previous;
	int *path, *tmp;
	int i, j, k, l;
	int one, two, three;
	int best, best2, best3;
	int totalCount = 1;

	/* Obnovenie globalnych premennych */
	heapCount = 0;
  	princessCount = 0;
	princess[0] = -1;
	princess[1] = -1;
	princess[2] = -1;
	/* -------- */

	path = (int *)malloc(n * m * 2 * sizeof(int));
	tmp = (int *)malloc(n * m * sizeof(int));

	prepareMap(n, m, mapa);
	path[0] = 0;
	path[1] = 0;

	dijkstra(0, &previous, n, m);

	k = dragon;
	i = 0;
	while (1){
		tmp[i] = k;
		i++;
		k = previous[k];
		totalCount++;

		if (k == 0)
			break;
	}
	l = totalCount - 2;
	for (i = 1; i < totalCount; i++){
		path[i * 2] = nodeArray[tmp[l]]->x;
		path[i * 2 + 1] = nodeArray[tmp[l]]->y;
		l--;
	}

	/* hladanie prvej najlepsej princeznej */
	dijkstra(dragon, &previous, n, m);
	one = princess[0];
	two = princess[1];
	three = princess[2];
	while (1){
		if (one == dragon){
			best = 0;
			break;
		}
		if (two == dragon){
			best = 1;
			break;
		}
		if (three == dragon){
			best = 2;
			break;
		}
		one = previous[one];
		two = previous[two];
		three = previous[three];
	}

	k = princess[best];
	i = 0;
	while (1){
		tmp[i] = k;
		i++;
		k = previous[k];
		totalCount++;

		if (k == dragon)
			break;
	}
	l = i - 1;
	for (j = totalCount - i; j < totalCount; j++){
		path[j * 2] = nodeArray[tmp[l]]->x;
		path[j * 2 + 1] = nodeArray[tmp[l]]->y;
		l--;
	}

	/* Hladanie druhej princeznej */
	dijkstra(princess[best], &previous, n, m);
	one = princess[0];
	two = princess[1];
	three = princess[2];
	while (1){
		if (best == 0){
			if (two == princess[best]){
				best2 = 1;
				break;
			}
			if (three == princess[best]){
				best2 = 2;
				break;
			}
		}
		else if (best == 1){
			if (one == princess[best]){
				best2 = 0;
				break;
			}
			if (three == princess[best]){
				best2 = 2;
				break;
			}
		}
		else if (best == 2){
			if (one == princess[best]){
				best2 = 0;
				break;
			}
			if (two == princess[best]){
				best2 = 1;
				break;
			}
		}
		one = previous[one];
		two = previous[two];
		three = previous[three];
	}

	k = princess[best2];
	i = 0;
	while (1){
		tmp[i] = k;
		i++;
		k = previous[k];
		totalCount++;

		if (k == princess[best])
			break;
	}
	l = i - 1;
	for (j = totalCount - i; j < totalCount; j++){
		path[j * 2] = nodeArray[tmp[l]]->x;
		path[j * 2 + 1] = nodeArray[tmp[l]]->y;
		l--;
	}

	/* Hladanie tretej princeznej */
	if (best == 0){
		if (best2 == 1)
			best3 = 2;
		else if (best2 == 2)
			best3 = 1;
	}
	else if (best == 1){
		if (best2 == 0)
			best3 = 2;
		else if (best2 == 2)
			best3 = 0;
	}
	else if (best == 2){
		if (best2 == 1)
			best3 = 0;
		else if (best2 == 0)
			best3 = 1;
	}
	dijkstra(princess[best2], &previous, n, m);
	one = princess[0];
	two = princess[1];
	three = princess[2];

	k = princess[best3];
	i = 0;
	while (1){
		tmp[i] = k;
		i++;
		k = previous[k];
		totalCount++;

		if (k == princess[best2])
			break;
	}
	l = i - 1;
	for (j = totalCount - i; j < totalCount; j++){
		path[j * 2] = nodeArray[tmp[l]]->x;
		path[j * 2 + 1] = nodeArray[tmp[l]]->y;
		l--;
	}

	*dlzka_cesty = totalCount;

	return path;

}