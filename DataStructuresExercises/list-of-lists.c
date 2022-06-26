#include <stdio.h>
#include <malloc.h>

typedef struct Student Student;
typedef struct NodLs NodLs;
typedef struct NodLp NodLp;

struct Student {
	int* cod;
	char* nume;
	float medie;
};

struct NodLs {
	Student info;
	struct NodLs* next;
};

struct NodLp {
	struct NodLs* info;
	struct NodLp* next;
};

Student creareStudent(int* cod, char* nume, float medie) {
	Student student;

	student.cod = (int*)malloc(sizeof(int));
	*(student.cod) = *(cod);

	student.nume = (char*)malloc(sizeof(char) * (strlen(nume) + 1));
	strcpy(student.nume, nume);

	student.medie = medie;

	return student;
}

NodLs* creareNodLs(Student student, NodLs* next) {
	NodLs* nou = (NodLs*)malloc(sizeof(NodLs));

	nou->info = creareStudent(student.cod, student.nume, student.medie);

	nou->next = next;

	return nou;
}

NodLp* creareNodLp(NodLs* nodLs, NodLp* next) {
	NodLp* nou = (NodLp*)malloc(sizeof(NodLp));

	nou->info = nodLs;

	nou->next = next;

	return nou;
}

NodLs* inserareLs(NodLs* cap, Student student) {
	NodLs* nou = creareNodLs(student, NULL);

	if (cap) {
		NodLs* aux = cap;

		while (aux->next) {
			aux = aux->next;
		}

		aux->next = nou;

		return cap;
	}
	else {
		return nou;
	}
}

NodLp* inserareLp(NodLp* cap, NodLs* nodLs) {
	NodLp* nou = creareNodLp(nodLs, NULL);

	if (cap) {
		NodLp* aux = cap;

		while (aux->next) {
			aux = aux->next;
		}

		aux->next = nou;

		return cap;
	}
	else {
		return nou;
	}
}

void afisareStudent(Student student) {
	printf("\nStudentul cu cod %d se numeste %s si are media %5.2f.\n", *(student.cod), student.nume, student.medie);
}

void afisareLs(NodLs* cap) {
	if (cap) {
		NodLs* aux = cap;

		while (aux) {
			afisareStudent(aux->info);
			aux = aux->next;
		}
	}
}

void afisareLp(NodLp* cap) {
	if (cap) {
		NodLp* aux = cap;

		while (aux) {
			printf("\nLista:\n");
			afisareLs(aux->info);
			aux = aux->next;
		}
	}
}

void dezalocareStudent(Student student) {
	free(student.cod);
	free(student.nume);
}

void dezalocareNodLs(NodLs* nod) {
	dezalocareStudent(nod->info);
	free(nod);
}

void dezalocareLs(NodLs* cap) {
	if (cap) {
		NodLs* aux = cap;

		while (aux) {
			NodLs* temp = aux;
			aux = aux->next;
			dezalocareNodLs(temp);
		}
	}
}

void dezalocareNodLp(NodLp* nod) {
	dezalocareLs(nod->info);
	free(nod);
}

void dezalocareLp(NodLp* cap) {
	if (cap) {
		NodLp* aux = cap;

		while (aux) {
			NodLp* temp = aux;
			aux = aux->next;
			dezalocareNodLp(temp);
		}
	}
}

NodLs* conversieListaSimpla(NodLp* capLp) {
	NodLs* capLs = NULL;

	if (capLp) {
		NodLp* auxLp = capLp;

		while (auxLp) {
			NodLs* auxLs = auxLp->info;

			while (auxLs) {
				capLs = inserareLs(capLs, auxLs->info);
				auxLs = auxLs->next;
			}

			auxLp = auxLp->next;
		}
	}

	return capLs;
}

int main() {
	NodLp* capLp = NULL;
	NodLs* capLsPar = NULL;
	NodLs* capLsImpar = NULL;
	Student student;

	char buffer[20];
	int n = 0;
	FILE* fisier = fopen("list-of-lists.txt", "r");

	fscanf(fisier, "%d", &n);

	for (int i = 0; i < n; i++) {
		student.cod = (int*)malloc(sizeof(int));
		fscanf(fisier, "%d", student.cod);

		fscanf(fisier, "%s", buffer);
		student.nume = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
		strcpy(student.nume, buffer);

		fscanf(fisier, "%f", &student.medie);

		if (*(student.cod) % 2 == 0) {
			capLsPar = inserareLs(capLsPar, student);
		}
		else {
			capLsImpar = inserareLs(capLsImpar, student);
		}

		dezalocareStudent(student);
	}

	fclose(fisier);

	capLp = inserareLp(capLp, capLsPar);
	capLp = inserareLp(capLp, capLsImpar);

	afisareLp(capLp);

	printf("\nConversie lista de liste - lista simpla\n");
	NodLs* capLs = conversieListaSimpla(capLp);
	afisareLs(capLs);
		
	dezalocareLp(capLp);

	return 0;
}