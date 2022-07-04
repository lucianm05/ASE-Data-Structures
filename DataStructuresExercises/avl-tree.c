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

int calcInaltime(Nod* rad) {
	if (rad == NULL) return 0;

	int inaltimeStg = calcInaltime(rad->left);
	int inaltimeDr = calcInaltime(rad->right);

	if (inaltimeStg > inaltimeDr) return inaltimeStg + 1;
	else return inaltimeDr + 1;
}

Nod* rotireLaDreapta(Nod* rad) {
	if (rad) {
		Nod* aux = rad->left;
		rad->left = aux->right;
		aux->right = rad;
		return aux;
	} 
	return rad;
}

Nod* rotireLaStanga(Nod* rad) {
	if (rad) {
		Nod* aux = rad->right;
		rad->right = aux->left;
		aux->left = rad;
		return aux;
	}
	return rad;
}

int calcGradEchilibru(Nod* rad) {
	int inaltimeSt = calcInaltime(rad->left);
	int inaltimeDr = calcInaltime(rad->right);
	return inaltimeSt - inaltimeDr;
}

Nod* inserareArbore(Nod* rad, User user) {
	if (rad == NULL) return creareNod(user, NULL, NULL);

	if (*(rad->info.id) < *(user.id)) {
		rad->left = inserareArbore(rad->left, user);
	}
	else {
		rad->right = inserareArbore(rad->right, user);
	}

	if (calcGradEchilibru(rad) == 2) {
		if (calcGradEchilibru(rad->left) != 1) {
			rad->left = rotireLaStanga(rad->left);
		}
		rad = rotireLaDreapta(rad);
	}

	if (calcGradEchilibru(rad) == -2) {
		if (calcGradEchilibru(rad->right) != -1) {
			rad->right = rotireLaDreapta(rad->right);
		}
		rad = rotireLaStanga(rad);
	}

	return rad;
}

void afisareUser(User user) {
	printf("\nUserul cu id %d se numeste %s", *(user.id), user.nume);
}

// RSD
void afisarePreordine(Nod* rad) {
	if (rad) {
		afisareUser(rad->info);
		afisarePreordine(rad->left);
		afisarePreordine(rad->right);
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
	if (rad) {
		dezalocareArbore(rad->left);
		dezalocareArbore(rad->right);
		dezalocareNod(rad);
	}
}

int main() {
	Nod* rad = NULL;
	User user;

	int n = 0;
	char buffer[20];
	FILE* fisier = fopen("avl-tree.txt", "r");

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

	afisarePreordine(rad);

	int inaltime = calcInaltime(rad);
	printf("\nInaltime: %d", inaltime);



	dezalocareArbore(rad);

	return 0;
}