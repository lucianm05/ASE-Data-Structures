#include <stdio.h>
#include <malloc.h>

typedef struct Proiect Proiect;
typedef struct Heap Heap;

struct Proiect {
	int* prioritate;
	char* nume;
};

struct Heap {
	Proiect* vect;
	int dim;
};

Proiect creareProiect(int *prioritate, char* nume) {
	Proiect proiect;

	proiect.prioritate = (int*)malloc(sizeof(int));
	*(proiect.prioritate) = *prioritate;

	proiect.nume = (char*)malloc(sizeof(char) * (strlen(nume) + 1));
	strcpy(proiect.nume, nume);

	return proiect;
}

Heap creareHeap(int dim) {
	Heap heap;

	heap.vect = (Proiect*)malloc(sizeof(Proiect) * dim);
	heap.dim = dim;

	return heap;
}

void filtrareMin(Heap heap, int index) {
	if (heap.dim > 0) {
		int indS = index * 2 + 1;
		int indD = index * 2 + 2;
		int indMax = index;

		if (indS < heap.dim && *(heap.vect[indS].prioritate) < *(heap.vect[indMax].prioritate))
			indMax = indS;

		if (indD < heap.dim && *(heap.vect[indD].prioritate) < *(heap.vect[indMax].prioritate))
			indMax = indD;

		if (indMax != index) {
			Proiect aux = heap.vect[index];
			heap.vect[index] = heap.vect[indMax];
			heap.vect[indMax] = aux;

			if(heap.dim > indMax * 2 + 1)
				filtrareMin(heap, indMax);
		}
	}
}

void filtrareMax(Heap heap, int index) {
	if (heap.dim > 0) {
		int indS = index * 2 + 1;
		int indD = index * 2 + 2;
		int indMax = index;

		if (indS < heap.dim && *(heap.vect[indS].prioritate) > *(heap.vect[indMax].prioritate))
			indMax = indS;

		if (indD < heap.dim && *(heap.vect[indD].prioritate) > *(heap.vect[indMax].prioritate))
			indMax = indD;

		if (indMax != index) {
			Proiect aux = heap.vect[index];
			heap.vect[index] = heap.vect[indMax];
			heap.vect[indMax] = aux;

			if (heap.dim > indMax * 2 + 1)
				filtrareMax(heap, indMax);
		}

	}
}

void inserare(Heap* heap, Proiect proiect) {
	Proiect* vectNou = (Proiect*)malloc(sizeof(Proiect) * (heap->dim + 1));

	for (int i = 0; i < heap->dim; i++)
		vectNou[i] = heap->vect[i];

	heap->dim++;
	vectNou[heap->dim - 1] = proiect;
	free(heap->vect);
	heap->vect = vectNou;

	for (int i = (heap->dim - 1) / 2; i >= 0; i--)
		filtrareMin(*heap, i);

	//for (int i = (heap->dim - 1) / 2; i >= 0; i--)
	//	filtrareMax(*heap, i);
}

Proiect extragere(Heap* heap) {
	Proiect proiect = creareProiect(heap->vect[0].prioritate, heap->vect[0].nume);

	Proiect aux = heap->vect[0];
	heap->vect[0] = heap->vect[heap->dim - 1];
	heap->vect[heap->dim - 1] = aux;

	heap->dim--;

	filtrareMin(*heap, 0);
	//filtrareMax(*heap, 0);

	return proiect;
}

void afisareProiect(Proiect proiect) {
	printf("\nProiectul %s are prioritate %d", proiect.nume, *(proiect.prioritate));
}

void afisareHeap(Heap heap) {
	if (heap.dim > 0 && heap.vect) {
		for (int i = 0; i < heap.dim; i++) {
			afisareProiect(heap.vect[i]);
		}
	}
}

void dezalocareProiect(Proiect proiect) {
	free(proiect.prioritate);
	free(proiect.nume);
}

void dezalocareHeap(Heap heap) {
	if (heap.dim > 0 && heap.vect) {
		for (int i = 0; i < heap.dim; i++) {
			dezalocareProiect(heap.vect[i]);
		}

		free(heap.vect);
	}
}

int main() {
	int n = 0;
	char buffer[20];
	FILE* fisier = fopen("heap.txt", "r");

	fscanf(fisier, "%d", &n);

	Heap heap = creareHeap(n);

	for (int i = 0; i < heap.dim; i++) {
		Proiect proiect;

		proiect.prioritate = (int*)malloc(sizeof(int));
		fscanf(fisier, "%d", proiect.prioritate);

		fscanf(fisier, "%s", buffer);
		proiect.nume = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
		strcpy(proiect.nume, buffer);

		heap.vect[i] = proiect;
	}

	fclose(fisier);

	
	for (int i = (heap.dim - 1) / 2; i >= 0; i--)
		filtrareMin(heap, i);

	//for (int i = (heap.dim - 1) / 2; i >= 0; i--)
	//	filtrareMax(heap, i);

	afisareHeap(heap);

	printf("\n\n=== Inserare proiect Rashet ===");
	int prio = 10;
	Proiect rashet = creareProiect(&prio, "Rashet");
	inserare(&heap, rashet);
	afisareHeap(heap);

	printf("\n\n=== Extragere si dezalocare ===");
	while (heap.dim) {
		Proiect proiect = extragere(&heap);
		afisareProiect(proiect);
		dezalocareProiect(proiect);
	}

	//dezalocareHeap(heap);

	return 0;
} 