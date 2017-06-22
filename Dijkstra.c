// Autor: Matus Olejnik

/*
Popolv�r je jednoduch� chlapec z dediny, ktor� cel� dni nerob� ni� in� ako vyn�anie popola zo spor�ka v kuchyni a programovanie pre dobro�inn� ��ely. Dnes, ako tak surfoval po internete, sa k nemu dostala ve�mi pohor�uj�ca spr�va. V ne�alekej krajine uniesol drak tri princezn� a schoval sa vysoko v hor�ch, kde je ve�mi �a�k� pr�stup a chyst� sa tam zajtra o polnoci v�etky tri princezn� zjes�.

Samozrejme, �e sa tomu nem��ete len tak ne�inne prizera�. V�aka modern�m technol�gi�m m�te k dispoz�ci� aj mapu, ktor� r�chlo zverejnil kr� � otec troch ne��astn�c. Va�ou �lohou je teda prirodzene �o najsk�r najprv zabi� draka a potom zachr�ni� v�etky tri princezn�.

Mapa predstavuje dvojrozmern� mrie�ku pol��ok, ktor� m��u obsahova�:

c	Lesn� chodn�k
h	Hust� lesn� porast, cez ktor� idete dva-kr�t pomal�ie ako po lesnom chodn�ku
n	Nepriechodn� prek�ku (napr. skalu)
d	Draka
p	Princezn�
0-9	Zast�vka lanovky, ktorou sa m��ete odviez�
g	Gener�tor, ktor�m zapnete elektriku pre lanovky
Va�ou �lohou je implementova� nasledovn� funkciu:

int *zachran_princezne(char **mapa, int n, int m, int t, int *dlzka_cesty);
T�to funkcia m� n�js� cestu, ktorou postupova�, aby popolv�r �o najsk�r zachr�nil v�etky tri princezn� a zabil draka. Mapa je vysok� n pol��ok a �irok� m pol��ok (1 <= n, m, <= 1000). Parameter t (0 <= t <= 106) ur�uje, kedy od za�iatku v�ho h�adania sa drak zobud� a zje prv� princezn�. Ke�e drak je ve�mi r�chly a dok�e sa teleportova�, m��ete sa spo�ahn��, �e ak sa v�m ho nepodar� zabi� dovtedy ako sa zobud�, princezn� u� nezachr�nite. Prechod cez lesn� chodn�k trv� jednu jednotku �asu a drak sa zobud� v t-tej jednotke �asu, kedy u� bude neskoro. A nezabudnite, �e najprv mus�te zabi� draka, a� potom m��ete zachra�ova� princezn� (hoci by ste aj prech�dzali predt�m cez pol��ko kde je princezn�).

Aby ste mohli pou�i� ak�ko�vek lanovku, mus�te najprv spusti� gener�tor, inak �iadna lanovka nebude fungova�. Ka�d� lanovka je ozna�en� nejakou cifrou (od 0 po 9) a lanovkou sa m��ete prem�va� iba medzi jej zast�vkami, ktor� s� ozna�en� rovnakou cifrou. Ke� teda nast�pite na zast�vku lanovky 0, tak si m��ete vybra� ktor�ko�vek jej in� zast�vku ozna�en� na mape rovnako 0. Lanovke trv� (dx+dy+3)/4 �asu, aby sa presunula medzi dvoma zast�vkami, kde dx je horizont�lny rozdiel poz�ci� t�chto zast�vok a dy je ich vertik�lny rozdiel. Delenie je celo��seln�.

N�jden� cestu vr�te ako postupnos� s�radn�c (dvoj�c cel�ch ��sel x,y, kde 0 <= x < n a 0 <= y < m) v�etk�ch nav�t�ven�ch pol��ok. Na za�iatku sa v�dy nach�dzate na pol��ku so s�radnicami 0,0 (ktor� bude uveden� ako prv� v ceste) a na poslednom nav�t�venom pol��ku mus� by� jedna z troch princezien. Ak existuje viacero rovnako dlho trvaj�cich ciest, vr�te t�, ktor� je lexikograficky prv�. Parameter dlzka_cesty je v�stupn� parameter, ktor� nastavte na po�et s�radn�c, ktor� vr�tite. Implementujte vy��ie uveden� funkcionalitu �o mo�no najefekt�vnej�ie.

Pr�klad pou�itia va�ej funkcie:

int i;
int *cesta = zachran_princezne(mapa, n, m, t, &dlzka_cesty);
for (i = 0; i < dlzka_cesty; ++i)
printf("%d %d\n", cesta[i*2], cesta[i*2+1]);
Dole�it� pozn�mky: Okrem cestovania lanovkou sa Popolv�r pres�va len v �tyroch smeroch (hore, dole, do�ava, doprava). Ak nie je zadan� inak, prechod cez pol��ko trv� �tandardne jednu jednotku �asu. Teda cez pol��ka s drakom, princeznou, lanovkou a gener�torom trv� prechod tie� jednu jednotku �asu. Gener�tor je na mape najviac jeden. Do v�sledn�ho �asu sa zapo��tavaj� v�etky pol��ka, cez ktor� Popolv�r prejde. Ak teda za�ne na pol��ku (0,0), prejde cez pol��ko (1,0) a skon�� na pol��ku (1,1), pri�om v�etky tri pol��ka obsahuj� lesn� chodn�k, tak Popolv�rovi to trv� 3 jednotky �asu. Zabitie draka, n�stup do lanovky, v�stup z lanovky a zapnutie gener�tora je okam�it� akcia, ktor�ch trvanie je zanedbate�n� vzh�adom na �as trvania prechodu cez pol��ko, ich zar�tavame s nulovou d�kou trvania. Ke�e Popolv�r m� cel� radu �al��ch princezien, ktor� mus� e�te zachr�ni� v in�ch �astiach sveta (na vstupe je viacero kr�ovstiev, v ktor�ch chce zachr�ni� princezn�), mus�te �o najsk�r zachr�ni� princezn� (celkovo vr�tane zabitia draka), aby ste mohli ��m sk�r prejs� do �al�ieho kr�ovstva.

Pred odovzdan�m si vlastn� implement�ciu d�kladne otestujte. Do testova�a by ste mali odovzd�va� u� funk�n� verziu. Pri spusten� v�m testova� vyp�e protokol v rozli�n�ch scen�roch.
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