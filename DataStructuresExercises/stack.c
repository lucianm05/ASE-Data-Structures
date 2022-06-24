#include <stdio.h>
#include <malloc.h>

typedef struct Cutie Cutie;
typedef struct Nod Nod;

struct Cutie {
	char* material;
	int* volum;
};

struct Nod {
	Cutie info;
	struct Nod* next;
};

Cutie creareCutie(char* material, int* volum) {
	Cutie cutie;

	cutie.material = (char*)malloc(sizeof(char) * (strlen(material) + 1));
	strcpy(cutie.material, material);

	cutie.volum = (int*)malloc(sizeof(int));
	*(cutie.volum) = *(volum);

	return cutie;
}

Nod* creareNod(Cutie cutie, Nod* next) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));

	nou->info = creareCutie(cutie.material, cutie.volum);

	nou->next = next;

	return nou;
}

void dezalocareCutie(Cutie cutie) {
	free(cutie.material);
	free(cutie.volum);
}

void dezalocareNod(Nod* nod) {
	dezalocareCutie(nod->info);
	free(nod);
}

void push(Nod** varf, Cutie cutie) {
	Nod* nou = creareNod(cutie, NULL);

	if (*varf == NULL) {
		*varf = nou;
	}
	else {
		nou->next = *varf;
		*varf = nou;
	}
}

int pop(Nod** varf, Cutie* cutie) {
	if (*varf == NULL) return -1;

	*cutie = creareCutie((*varf)->info.material, (*varf)->info.volum);

	Nod* aux = *varf;
	*varf = (*varf)->next;

	dezalocareNod(aux);

	return 0;
}

void afisareCutie(Cutie cutie) {
	printf("\nCutia este facuta din %s si are un volum de %d\n", cutie.material, *(cutie.volum));
}

void afisareStiva(Nod* varf) {
	if (varf) {
		Nod* aux = varf;

		while (aux) {
			afisareCutie(aux->info);
			aux = aux->next;
		}
	}
}

void conversieStivaVector(Nod** varf, Cutie* vector, int* nr) {
	while (pop(varf, &vector[*nr]) == 0) {
		(*nr)++;
	}
}

int main() {
	Nod* varf = NULL;
	Cutie cutie;

	FILE* fisier = fopen("stack.txt", "r");
	int n = 0;
	char buffer[20];

	fscanf(fisier, "%d", &n);

	for (int i = 0; i < n; i++) {
		fscanf(fisier, "%s", buffer);
		cutie.material = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
		strcpy(cutie.material, buffer);

		cutie.volum = (int*)malloc(sizeof(int));
		fscanf(fisier, "%d", cutie.volum);

		push(&varf, cutie);

		dezalocareCutie(cutie);
	}

	fclose(fisier);

	afisareStiva(varf);

	// conversie stiva - vector
	printf("\n=== Conversie stiva - vector ===\n");
	Cutie* vector = (Cutie*)malloc(sizeof(Cutie) * n);
	int nr = 0;
	conversieStivaVector(&varf, vector, &nr);
	for (int i = 0; i < nr; i++) {
		afisareCutie(vector[i]);
	}
	for (int i = 0; i < nr; i++) {
		dezalocareCutie(vector[i]);
	}

	// dezalocare
	//while (pop(&varf, &cutie) == 0) {
	//	dezalocareCutie(cutie);
	//}

	return 0;
}