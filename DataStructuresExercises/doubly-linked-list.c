#include <stdio.h>
#include <malloc.h>

typedef struct Avion Avion;
typedef struct Nod Nod;
typedef struct ListaDubla ListaDubla;

struct Avion {
	char* numePilot;
	int nrPasageri;
};

struct Nod {
	Avion info;
	struct Nod* prev;
	struct Nod* next;
};

struct ListaDubla {
	struct Nod* first; 
	struct Nod* last;
};

Avion creareAvion(char* numePilot, int nrPasageri) {
	Avion avion;

	avion.numePilot = (char*)malloc(sizeof(char) * (strlen(numePilot) + 1));
	strcpy(avion.numePilot, numePilot);

	avion.nrPasageri = nrPasageri;

	return avion;
}

Nod* creareNod(Avion avion, Nod* prev, Nod* next) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));

	nou->info = creareAvion(avion.numePilot, avion.nrPasageri);
	nou->prev = prev;
	nou->next = next;

	return nou;
}

ListaDubla inserareInceput(ListaDubla lista, Avion avion) {
	Nod* nou = creareNod(avion, NULL, lista.first);

	if (lista.first) {
		lista.first->prev = nou;
		lista.first = nou;
		return lista;
	}
	else {
		lista.first = nou;
		lista.last = nou;
		return lista;
	}
}

ListaDubla inserareSfarsit(ListaDubla lista, Avion avion) {
	Nod* nou = creareNod(avion, lista.last, NULL);

	if (lista.last) {
		lista.last->next = nou;
		lista.last = nou;
		return lista;
	}
	else {
		lista.first = nou;
		lista.last = nou;
		return lista;
	}
}

void afisareAvion(Avion avion) {
	printf("\nAvionul este pilotat de %s si are o capacitate de %d\n", avion.numePilot, avion.nrPasageri);
}

void afisareListaInceput(ListaDubla lista) {
	if (lista.first) {
		Nod* aux = lista.first;

		while (aux) {
			afisareAvion(aux->info);
			aux = aux->next;
		}
	}
}

void afisareListaSfarsit(ListaDubla lista) {
	if (lista.last) {
		Nod* aux = lista.last;

		while (aux) {
			afisareAvion(aux->info);
			aux = aux->prev;
		}
	}
}

void dezalocareAvion(Avion avion) {
	free(avion.numePilot);
}

void dezalocareNod(Nod* nod) {
	dezalocareAvion(nod->info);
	free(nod);
}

void dezalocareListaInceput(ListaDubla *lista) {
	Nod* aux = lista->first;

	while (aux->next) {
		Nod* temp = aux;
		aux = aux->next;
		dezalocareNod(temp);
	}

	lista->first = NULL;
	lista->last = NULL;
}

int main() {
	ListaDubla lista;
	lista.first = NULL;
	lista.last = NULL;

	int n = 0;
	FILE* fisier = fopen("doubly-linked-list.txt", "r");
	char buffer[20];
	Avion avion;

	fscanf(fisier, "%d", &n);

	for (int i = 0; i < n; i++) {
		fscanf(fisier, "%s", buffer);
		avion.numePilot = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
		strcpy(avion.numePilot, buffer);

		fscanf(fisier, "%d", &avion.nrPasageri);

		lista = inserareInceput(lista, avion);

		dezalocareAvion(avion);
	}

	fclose(fisier);

	afisareListaInceput(lista);
	printf("\n");
	afisareListaSfarsit(lista);

	dezalocareListaInceput(&lista);

	return 0;
}