#include <stdio.h>
#include <malloc.h>

typedef struct Pisica Pisica;
typedef struct Nod Nod;
typedef struct HashTable HashTable;

struct Pisica {
	int* varsta;
	char* nume;
};

struct Nod {
	Pisica info;
	struct Nod* next;
};

struct HashTable {
	Nod** vect;
	int size;
};

Pisica crearePisica(int* varsta, char* nume) {
	Pisica pisica;

	pisica.varsta = (int*)malloc(sizeof(int));
	*(pisica.varsta) = *(varsta);

	pisica.nume = (char*)malloc(sizeof(char) * (strlen(nume) + 1));
	strcpy(pisica.nume, nume);

	return pisica;
}

Nod* creareNod(Pisica pisica, Nod* next) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));

	nou->info = crearePisica(pisica.varsta, pisica.nume);

	nou->next = next;

	return nou;
}

HashTable creareTabela(int size) {
	HashTable tabela;
	tabela.size = size;
	tabela.vect = (Nod**)malloc(sizeof(Nod) * tabela.size);

	for (int i = 0; i < tabela.size; i++) {
		tabela.vect[i] = NULL;
	}

	return tabela;
}

int functieHash(HashTable tabela, Pisica pisica) {
	return strlen(pisica.nume) % tabela.size;
}

Nod* inserareLista(Nod* cap, Pisica pisica) {
	Nod* nou = creareNod(pisica, NULL);

	if (cap == NULL) return nou;

	Nod* aux = cap;

	while (aux->next) {
		aux = aux->next;
	}

	aux->next = nou;

	return cap;
}

int inserareTabela(HashTable tabela, Pisica pisica) {
	if (tabela.vect && tabela.size > 0) {
		int pozitie = functieHash(tabela, pisica);

		if (pozitie <= tabela.size) {
			tabela.vect[pozitie] = inserareLista(tabela.vect[pozitie], pisica);
		}
	}
}

void afisarePisica(Pisica pisica) {
	printf("\nPisica %s are %d ani\n", pisica.nume, *(pisica.varsta));
}

void afisareLista(Nod* cap) {
	if (cap) {
		Nod* aux = cap;

		while (aux) {
			afisarePisica(aux->info);
			aux = aux->next;
		}
	}
}

void afisareTabela(HashTable tabela) {
	if (tabela.vect && tabela.size > 0) {
		for (int i = 0; i < tabela.size; i++) {
			if (tabela.vect[i]) {
				printf("\nPozitie: %d", i);
				afisareLista(tabela.vect[i]);
			}
		}
	}
}

void dezalocarePisica(Pisica pisica) {
	free(pisica.varsta);
	free(pisica.nume);
}

void dezalocareNod(Nod* nod) {
	dezalocarePisica(nod->info);
	free(nod);
}

void dezalocareLista(Nod* cap) {
	if (cap) {
		Nod* aux = cap;

		while (aux) {
			Nod* temp = aux;
			aux = aux->next;
			dezalocareNod(temp);
		}
	}
}

void dezalocareTabela(HashTable tabela) {
	if (tabela.vect && tabela.size > 0) {
		for (int i = 0; i < tabela.vect[i]; i++) {
			if (tabela.vect[i]) {
				dezalocareLista(tabela.vect[i]);
			}
		}
	}
}

int main() {
	HashTable tabela = creareTabela(101);
	Pisica pisica;

	int n = 0;
	char buffer[20];
	FILE* fisier = fopen("hash-table-chaining.txt", "r");

	fscanf(fisier, "%d", &n);

	for (int i = 0; i < n; i++) {
		pisica.varsta = (int*)malloc(sizeof(int));
		fscanf(fisier, "%d", pisica.varsta);

		fscanf(fisier, "%s", buffer);
		pisica.nume = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
		strcpy(pisica.nume, buffer);

		inserareTabela(tabela, pisica);

		dezalocarePisica(pisica);
	}

	fclose(fisier);

	afisareTabela(tabela);

	dezalocareTabela(tabela);

	return 0;
}