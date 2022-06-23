#include <stdio.h>
#include <malloc.h>

typedef struct Muzeu Muzeu;
typedef struct Nod Nod;

struct Muzeu {
	char* nume;
	float pretBilet;
	int vizitatoriZi;
};

struct Nod {
	Muzeu info;
	Nod* next;
};

Nod* creareNod(Muzeu muzeu, Nod* next) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));

	nou->info = muzeu;
	nou->info.nume = (char*)malloc(sizeof(char) * (strlen(muzeu.nume) + 1));
	strcpy(nou->info.nume, muzeu.nume);

	nou->next = next;

	return nou;
}

Nod* inserareInceput(Nod* cap, Muzeu muzeu) {
	return creareNod(muzeu, cap);
}

Nod* inserareSfarsit(Nod* cap, Muzeu muzeu) {
	if (cap) {
		Nod* aux = cap;

		while (aux->next) {
			aux = aux->next;
		}

		aux->next = creareNod(muzeu, NULL);

		return cap;
	}
	else {
		return creareNod(muzeu, NULL);
	}
}

void afisareMuzeu(Muzeu muzeu) {
	printf("Muzeul %s are %d vizitatori pe zi, iar biletul costa %5.2f.\n", muzeu.nume, muzeu.vizitatoriZi, muzeu.pretBilet);
}

void afisareLista(Nod* cap) {
	if (cap) {
		Nod* aux = cap;

		while (aux) {
			afisareMuzeu(aux->info);
			aux = aux->next;
		}
	}
}

void dezalocareLista(Nod* cap) {
	if (cap) {
		Nod* aux = cap;

		while (aux->next) {
			Nod* temp = aux;
			aux = aux->next;
			free(temp->info.nume);
			free(temp);
		}
	}
}

int nrMuzeeBiletMin(Nod* cap, int pretMinBilet) {
	int nr = 0;

	if (cap) {
		Nod* aux = cap;

		while (aux) {
			if (aux->info.pretBilet > pretMinBilet) {
				nr++;
			}

			aux = aux->next;
		}
	}

	return nr;
}

void modificareNumeMuzeu(Nod* cap, const char* numeCautat, const char* numeNou) {
	if (cap) {
		Nod* aux = cap;

		while (aux && strcmp(numeCautat, aux->info.nume) != 0) {
			aux = aux->next;
		}

		if (aux) {
			free(aux->info.nume);
			aux->info.nume = (char*)malloc(sizeof(char) * (strlen(numeNou) + 1));
			strcpy(aux->info.nume, numeNou);
		}
	}
}

Nod* eliminareNodDupaNume(Nod* cap, const char* numeCautat) {
	if (cap) {
		if (strcmp(cap->info.nume, numeCautat) == 0) {
			Nod* next = cap->next;

			free(cap->info.nume);
			free(cap);
			cap = NULL;

			return next;
		}
		else {
			Nod* aux = cap;
			Nod* next = aux->next;

			while (next && strcmp(next->info.nume, numeCautat) != 0) {
				aux = aux->next;
				next = aux->next;
			}

			if (aux->next && next->next) {
				aux->next = next->next;

				free(next->info.nume);
				free(next);
				next = NULL;
			}

			return cap;
		}
	}

	return;
}

int main() {
	Nod* cap = NULL;
	Muzeu muzeu;
	FILE* fisier = fopen("linked-list.txt", "r");
	int n = 0;
	char buffer[20];

	fscanf(fisier, "%d", &n);

	for (int i = 0; i < n; i++) {
		fscanf(fisier, "%s", buffer);
		muzeu.nume = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
		strcpy(muzeu.nume, buffer);

		fscanf(fisier, "%f", &muzeu.pretBilet);

		fscanf(fisier, "%d", &muzeu.vizitatoriZi);

		// inserare la inceput
		//cap = inserareInceput(cap, muzeu);

		// inserare la sfarsit
		cap = inserareSfarsit(cap, muzeu);

		free(muzeu.nume);
	}

	afisareLista(cap);

	int nrMuzee = nrMuzeeBiletMin(cap, 9);
	printf("Nr. muzeelor cu pretul biletului cel putin 9 lei: %d\n", nrMuzee);

	printf("\n=== Modificare nume muzeu Istorie in Eminescu ===\n");
	modificareNumeMuzeu(cap, "Istorie", "Eminescu");
	afisareLista(cap);

	printf("\n=== Eliminare muzeu cu numele Antipa ===\n");
	cap = eliminareNodDupaNume(cap, "Antipa");
	afisareLista(cap);

	printf("\n=== Eliminare muzeu cu numele Arta ===\n");
	cap = eliminareNodDupaNume(cap, "Arta");
	afisareLista(cap);

	dezalocareLista(cap);

	return 0;
}