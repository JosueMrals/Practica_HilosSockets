#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

typedef struct {
	char palabra[101];
	int frequencia;
} PalabraArray ;

int comparePalabras(const void *f1, const void *f2){
	PalabraArray *a = (PalabraArray *)f1;
	PalabraArray *b = (PalabraArray *)f2;
	return (b->frequencia - a->frequencia);
}

void contadorFrecuencia(void *arg){
	
	char *nombreArchivo = (char*)malloc(sizeof(char));
	nombreArchivo = (char*)arg;
	PalabraArray *palabras = (PalabraArray*)calloc((1000),sizeof(PalabraArray));
	char *resultado = (char*)malloc(sizeof(resultado)*100);
	int contador = 0;
	int isUnique;
	int i = 0;
	FILE *file;
	char buff[1000];
	
	for (i = 0; i < sizeof(PalabraArray); i++){
		palabras[i].palabra[0] = '\0';
		palabras[i].frequencia = 0;
	}

	file = fopen(nombreArchivo, "r");
	if (file == NULL){
		printf("No se pudo abrir el archivo: ");
	}
			
	else {
		while ( (fscanf(file, "%s", buff)) != EOF)
		{
			isUnique = -1;

			int k;
			for (k = 0; k < contador; k++){
				if (strcmp(palabras[k].palabra, buff) == 0){
					isUnique = k;
				}
			}
			
			if (isUnique == -1){
				strcpy(palabras[contador].palabra, buff);
				palabras[contador].frequencia = 1;
				contador++;
			}

			else {
				palabras[isUnique].frequencia++;
			}

			palabras = realloc(palabras, (sizeof(*palabras) + contador) * sizeof(PalabraArray));	
		}
	}


	qsort(palabras, contador, sizeof(PalabraArray), comparePalabras);

	snprintf(resultado, 10000, "%s %d %s %s %s", nombreArchivo, contador, palabras[0].palabra, palabras[1].palabra, palabras[2].palabra);
	fclose(file);
	printf("%s\n", resultado);

}

int main(int argc, char *argv[]){
	

	pthread_t threads[argc-1];
	pthread_attr_t attr;
	pthread_attr_init(&attr);


	int i;
	for (i = 1; i < argc; i++){
		char* argumentos = (char*)malloc(sizeof(argv[i])+1);
		argumentos = argv[i];

		pthread_create(&threads[i], &attr, (void*) contadorFrecuencia, (void*) argumentos);
	}
	for (i = 1; i < argc; i++){

		pthread_join(threads[i], NULL);
	}

	return 0;
}
