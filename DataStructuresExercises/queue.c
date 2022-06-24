#include <stdio.h>
#include <malloc.h>

typedef struct Pasager Pasager;
typedef struct Nod Nod;
typedef struct Coada Coada;

struct Pasager {
	char* nume;
	int* varsta;
};

struct Nod {
	Pasager info;
	struct Nod* next;
};

struct Coada {
	struct Nod* first;
	struct Nod* last;
};

Pasager crearePasager(char* nume, int* varsta) {
	Pasager pasager;

	pasager.nume = (char*)malloc(sizeof(char) * (strlen(nume) + 1));
	strcpy(pasager.nume, nume);

	pasager.varsta = (int*)malloc(sizeof(int));
	*(pasager.varsta) = *(varsta);

	return pasager;
}

Nod* creareNod(Pasager pasager, Nod* next) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));

	nou->info = crearePasager(pasager.nume, pasager.varsta);

	nou->next = next;

	return nou;
}

void afisarePasager(Pasager pasager) {
	printf("\nPasagerul se numeste %s si are %d ani\n", pasager.nume, *(pasager.varsta));
}

void afisareCoada(Coada coada) {
	if (coada.first) {
		Nod* aux = coada.first;

		while (aux) {
			afisarePasager(aux->info);
			aux = aux->next;
		}
	}
}

void dezalocarePasager(Pasager pasager) {
	free(pasager.nume);
	free(pasager.varsta);
}

void dezalocareNod(Nod* nod) {
	dezalocarePasager(nod->info);
	free(nod);
}

void put(Coada* coada, Pasager pasager) {
	Nod* nou = creareNod(pasager, NULL);

	if (coada->first == NULL && coada->last == NULL) {
		coada->first = nou;
		coada->last = nou;
	}
	else {
		coada->last->next = nou;
		coada->last = nou;
	}
}

int get(Coada* coada, Pasager* pasager) {
	if (coada->first != NULL && coada->last != NULL) {
		*pasager = crearePasager(coada->first->info.nume, coada->first->info.varsta);

		Nod* aux = coada->first;
		coada->first = coada->first->next;

		dezalocareNod(aux);

		return 0;
	}
	else if (coada->first == NULL) {
		coada->last = NULL;
		return -1;
	}
}



int main() {
	Coada coada;
	coada.first = NULL;
	coada.last = NULL;
	Pasager pasager;

	FILE* fisier = fopen("queue.txt", "r");
	int n = 0;
	char buffer[20];

	fscanf(fisier, "%d", &n);

	for (int i = 0; i < n; i++) {
		fscanf(fisier, "%s", buffer);
		pasager.nume = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
		strcpy(pasager.nume, buffer);

		pasager.varsta = (int*)malloc(sizeof(int));
		fscanf(fisier, "%d", pasager.varsta);

		put(&coada, pasager);

		dezalocarePasager(pasager);
	}

	fclose(fisier);

	afisareCoada(coada);

	//dezalocare
	while (get(&coada, &pasager) == 0) {
		dezalocarePasager(pasager);
	}

	return 0;
}