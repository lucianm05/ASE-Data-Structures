#include <stdio.h>
#include <malloc.h>

typedef struct User User;
typedef struct Nod Nod;

struct User {
	int* id;
	char* nume;
};

struct Nod {
	User info;
	struct Nod* left;
	struct Nod* right;
};

User creareUser(int* id, char* nume) {
	User user;

	user.id = (int*)malloc(sizeof(int));
	*(user.id) = *id;

	user.nume = (char*)malloc(sizeof(char) * (strlen(nume) + 1));
	strcpy(user.nume, nume);

	return user;
}

Nod* creareNod(User user, Nod* left, Nod* right) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));

	nou->info = creareUser(user.id, user.nume);
	nou->left = left;
	nou->right = right;

	return nou;
}

Nod* inserareArbore(Nod* rad, User user) {
	if (rad) {
		if (*(rad->info.id) < *(user.id)) {
			rad->left = inserareArbore(rad->left, user);
		}
		else {
			rad->right = inserareArbore(rad->right, user);
		}

		return rad;
	}
	else return creareNod(user, NULL, NULL);
}

void afisareUser(User user) {
	printf("\nUtilizatorul cu id-ul %d se numeste %s", *(user.id), user.nume);
}

void afisareNod(Nod* nod) {
	if (nod) {
		afisareUser(nod->info);
	}
	else {
		printf("\nNodul nu exista");
	}
}

// RSD
void afisarePreordine(Nod* rad) {
	if (rad) {
		afisareNod(rad);
		afisarePreordine(rad->left);
		afisarePreordine(rad->right);
	}
}

// SRD
void afisareInordine(Nod* rad) {
	if (rad) {
		afisareInordine(rad->left);
		afisareNod(rad);
		afisareInordine(rad->right);
	}
}

// DSR
void afisarePostordine(Nod* rad) {
	if (rad) {
		afisarePostordine(rad->right);
		afisarePostordine(rad->left);
		afisareNod(rad);
	}
}

void dezalocareUser(User user) {
	free(user.id);
	free(user.nume);
}

void dezalocareNod(Nod* nod) {
	dezalocareUser(nod->info);
	free(nod);
}

void dezalocareArbore(Nod* rad) {
	Nod* aux = rad;

	if (aux) {
		dezalocareArbore(rad->left);
		dezalocareArbore(rad->right);
		dezalocareNod(aux);
	}
}

int calcInaltime(Nod* rad) {
	if (rad == NULL) return 0;

	int inaltimeLeft = calcInaltime(rad->left);
	int inaltimeRight = calcInaltime(rad->right);

	if (inaltimeLeft > inaltimeRight) {
		return inaltimeLeft + 1;
	}
	else {
		return inaltimeRight + 1;
	}
}

Nod* cautareNod(Nod* rad, int id) {
	if (rad == NULL) return NULL;

	if (*(rad->info.id) == id) return rad;

	if (*(rad->info.id) < id) return cautareNod(rad->left, id);
	else return cautareNod(rad->right, id);
}

void drumRadNod(Nod* rad, int id) {
	if (rad) {
		printf("%d - ", *(rad->info.id));

		if (*(rad->info.id) < id) {
			drumRadNod(rad->left, id);
		}
		else {
			drumRadNod(rad->right, id);
		}
	}
}

Nod* stergereFrunze(Nod* rad) {
	if (rad == NULL) return NULL;

	Nod* aux = rad;

	if (rad->left == NULL && rad->right == NULL) {
		dezalocareNod(aux);
		rad = NULL;
		return rad;
	}

	rad->left = stergereFrunze(rad->left);
	rad->right = stergereFrunze(rad->right);

	return rad;
}

int main() {
	Nod* rad = NULL;
	User user;

	int n = 0;
	char buffer[20];
	FILE* fisier = fopen("binary-search-tree.txt", "r");

	fscanf(fisier, "%d", &n);

	for (int i = 0; i < n; i++) {
		user.id = (int*)malloc(sizeof(int));
		fscanf(fisier, "%d", user.id);

		fscanf(fisier, "%s", buffer);
		user.nume = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
		strcpy(user.nume, buffer);

		rad = inserareArbore(rad, user);

		dezalocareUser(user);
	}

	fclose(fisier);

	printf("\n\nAfisare preordine - RSD");
	afisarePreordine(rad);

	printf("\n\nAfisare inordine - SRD");
	afisareInordine(rad);

	printf("\n\nAfisare postordine - DSR");
	afisarePostordine(rad);

	int inaltime = calcInaltime(rad);
	printf("\n\nInaltime arbore: %d", inaltime);

	printf("\n\nCautare utilizatori 17, 4, 200");
	Nod* nodId17 = cautareNod(rad, 17);
	Nod* nodId4 = cautareNod(rad, 4);
	Nod* nodId200 = cautareNod(rad, 200);
	afisareNod(nodId17);
	afisareNod(nodId4);
	afisareNod(nodId200);
	
	printf("\n\nDrum radacina - nod cu id 35\n");
	drumRadNod(rad, 35);
	printf("\n\nDrum radacina - nod cu id 9\n");
	drumRadNod(rad, 9);
	
	printf("\n\nStergere noduri frunza");
	rad = stergereFrunze(rad);
	afisarePreordine(rad);

	dezalocareArbore(rad);

	return 0;
}