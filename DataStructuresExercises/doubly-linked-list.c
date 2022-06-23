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

void dezalocareLista(ListaDubla* lista) {
	Nod* aux = lista->first;

	while (aux->next) {
		Nod* temp = aux;
		aux = aux->next;
		dezalocareNod(temp);
	}

	lista->first = NULL;
	lista->last = NULL;
}

void modificareNumePilot(ListaDubla lista, const char* numeCautat, const char* numeNou) {
	if (lista.first) {
		Nod* aux = lista.first;

		while (aux && strcmp(numeCautat, aux->info.numePilot) != 0) {
			aux = aux->next;
		}

		if (aux) {
			free(aux->info.numePilot);
			aux->info.numePilot = (char*)malloc(sizeof(char) * (strlen(numeNou) + 1));
			strcpy(aux->info.numePilot, numeNou);
		}
	}
}

ListaDubla eliminareDupaNume(ListaDubla* lista, const char* numeCautat) {
	if (lista->first) {
		if (strcmp(lista->first->info.numePilot, numeCautat) == 0) {
			Nod* next = lista->first->next;
			dezalocareNod(lista->first);
			lista->first = next;
			lista->first->prev = NULL;

			return *lista;
		}

		if (strcmp(lista->last->info.numePilot, numeCautat) == 0) {
			Nod* prev = lista->last->prev;
			dezalocareNod(lista->last);
			lista->last = prev;
			lista->last->next = NULL;

			return *lista;
		}

		Nod* aux = lista->first;
		Nod* next = aux->next;

		while (next && strcmp(next->info.numePilot, numeCautat) != 0) {
			aux = aux->next;
			next = aux->next;
		}

		if (aux->next && next->next) {
			aux->next = next->next;

			dezalocareNod(next);

			next = NULL;
		}


		return *lista;
	}

	return *lista;
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

		// lista = inserareInceput(lista, avion);
		lista = inserareSfarsit(lista, avion);

		dezalocareAvion(avion);
	}

	fclose(fisier);

	printf("\n=== Traversare de la inceput ===\n");
	afisareListaInceput(lista);
	printf("\n\n=== Traversare de la sfarsit === \n");
	afisareListaSfarsit(lista);

	printf("\n=== Modificare nume piloti 1, 3, 5 si unul inexistent (Tudor -> Mihai) ===\n");
	modificareNumePilot(lista, "George", "Georgescu");
	modificareNumePilot(lista, "Paul", "Paulescu");
	modificareNumePilot(lista, "Marcian", "Marcianescu");
	modificareNumePilot(lista, "Tudor", "Mihai");
	afisareListaInceput(lista);

	printf("\n=== Eliminare piloti 1, 3, 5 si unul inexistent (Tudor) ===\n");
	lista = eliminareDupaNume(&lista, "Georgescu");
	lista = eliminareDupaNume(&lista, "Paulescu");
	lista = eliminareDupaNume(&lista, "Marcianescu");
	lista = eliminareDupaNume(&lista, "Tudor");
	afisareListaInceput(lista);

	dezalocareLista(&lista);

	return 0;
}