#include <stdio.h>
#include <malloc.h>

typedef struct Caine Caine;
typedef struct HashTable HashTable;

struct Caine {
	int* varsta;
	char* nume;
};

struct HashTable {
	Caine** vect;
	int size;
};

Caine* creareCaine(int* varsta, char* nume) {
	Caine* caine = (Caine*)malloc(sizeof(Caine));

	caine->varsta = (int*)malloc(sizeof(int));
	*(caine->varsta) = *varsta;

	caine->nume = (char*)malloc(sizeof(char) * (strlen(nume) + 1));
	strcpy(caine->nume, nume);

	return caine;
}

HashTable creareTabela(int size) {
	HashTable tabela;
	tabela.size = size;
	tabela.vect = (Caine**)malloc(tabela.size * sizeof(Caine));

	for (int i = 0; i < tabela.size; i++) {
		tabela.vect[i] = NULL;
	}

	return tabela;
}

int functieHash(int size, char* nume){
	int sum = 0;

	for (int i = 0; i < strlen(nume); i++) {
		sum += i;
	}

	return sum % size;
}

int inserareTabela(HashTable tabela, Caine* caine) {
	int pozitie = -1;

	if (tabela.vect && tabela.size > 0) {
		pozitie = functieHash(tabela.size, caine->nume);
		Caine* caineNou = creareCaine(caine->varsta, caine->nume);

		if (tabela.vect[pozitie] == NULL) {
			tabela.vect[pozitie] = caineNou;
		}
		else {
			int index = 1;
			int nextPozitie = pozitie + index;

			while (nextPozitie < tabela.size) {
				if (tabela.vect[nextPozitie] == NULL) {
					tabela.vect[nextPozitie] = caineNou;
					pozitie += index;
					break;
				}
				index++;
				nextPozitie += index;
			}
		}
	}

	return pozitie;
}

void afisareCaine(Caine* caine) {
	printf("\nCainele %s are %d ani\n", caine->nume, *(caine->varsta));
}

void afisareTabela(HashTable tabela) {
	if (tabela.vect && tabela.size > 0) {
		for (int i = 0; i < tabela.size; i++) {
			if (tabela.vect[i]) {
				printf("\n--- Pozitie: %d ---", i);
				afisareCaine(tabela.vect[i]);
			}
		}
	}
}

void dezalocareCaine(Caine* caine) {
	free(caine->varsta);
	free(caine->nume);
	free(caine);
}

void dezalocareTabela(HashTable tabela) {
	if (tabela.vect && tabela.size > 0) {
		for (int i = 0; i < tabela.size; i++) {
			if (tabela.vect[i]) {
				dezalocareCaine(tabela.vect[i]);
			}
		}
	}
}

int main() {
	HashTable tabela = creareTabela(101);

	int n = 0;
	char buffer[20];
	FILE* fisier = fopen("hash-table-linear.txt", "r");

	fscanf(fisier, "%d", &n);

	for (int i = 0; i < n; i++) {
		Caine* caine = (Caine*)malloc(sizeof(Caine));

		caine->varsta = (int*)malloc(sizeof(int));
		fscanf(fisier, "%d", caine->varsta);

		fscanf(fisier, "%s", buffer);
		caine->nume = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
		strcpy(caine->nume, buffer);

		inserareTabela(tabela, caine);

		dezalocareCaine(caine);
	}

	fclose(fisier);

	afisareTabela(tabela);

	dezalocareTabela(tabela);

	return 0;
} 