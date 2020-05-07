#include <stdio.h>

typedef struct {
	char* SOPA;		//La Sopa
	int tam;		//Tama�o de Matriz NxN
} SopaDeLetras;

typedef struct nodo{
	char* palabra;
	int cantidadPalabras;
	struct nodo* cabecera;
	struct nodo* sig;
} Lista;

Lista* leerPalabras()
{
	//Variables a usar
	char buffer_palabras[100];										//Buffer de lectura de palabras
	Lista* ListaPalabras = NULL;									//Estructura donde se guardaran las palabras
	FILE* palabrasPtr = fopen("palabras.in", "r");					//Abrimos el archivo palabras.in con un Puntero FILE*

	ListaPalabras = (Lista*)malloc(sizeof(Lista));
	ListaPalabras->cabecera = ListaPalabras;						//El nodo cabecera es el primero que se crea

	if (palabrasPtr == NULL) {										//El archivo se pudo leer?
		ListaPalabras->palabra = NULL;
		ListaPalabras->sig = NULL;
		ListaPalabras->cantidadPalabras = -1;						//No pueden haber palabras -1, por eso es un error
		return ListaPalabras;
	}

	//Obtenemos la cantidad de palabras en el archivo
	fgets(buffer_palabras, 100, palabrasPtr);						//Se lee el numero de palabras a buscar

	ListaPalabras->cantidadPalabras = atoi(buffer_palabras);			//Convierte la primera linea de chars en un entero

	if (ListaPalabras->cantidadPalabras == 0)						//Si el numero de palabras es 0, se retorna vacio
	{
		ListaPalabras->palabra = NULL;
		ListaPalabras->sig = NULL;
		return ListaPalabras;
	}

	//Cargamos las palabras
	while (1) {
		if (fgets(buffer_palabras, 100, palabrasPtr) != NULL) {		//Se lee la linea, si es NULL, no hay m�s palabras
			ListaPalabras->palabra = malloc(strlen(buffer_palabras)*sizeof(char)); //Asignamos espacio
			strcpy(ListaPalabras->palabra, strtok(buffer_palabras, "\n"));				//Guardamos la palabra y eliminamos el caracter \n
			ListaPalabras->sig = (Lista*)malloc(sizeof(Lista));		//Creamos un nuevo nodo y lo anexamos a la siguiente posici�n de la lista
			
			//Guardamos datos constantes de la lista
			ListaPalabras->sig->cabecera = ListaPalabras->cabecera;			
			ListaPalabras->sig->cantidadPalabras = ListaPalabras->cantidadPalabras;
			ListaPalabras->sig->sig = NULL;
			ListaPalabras = ListaPalabras->sig;
		}
		else
			break;									//Hasta que se pueda leer
	}

	fclose(palabrasPtr);							//Cierre de archivo

	return ListaPalabras->cabecera;					//Se retorna el primer elemento de la lista
}

char* generarMatriz(int M, int N)
{
	char* Matriz = malloc(M * N * sizeof(char));					//Reservamos M*N porque es un array de 1 dim

	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < N; j++)
		{
			Matriz[i * N + j] = (char)(i + (j + 1) * (i + 1) + '0');//Se suma '0' para el casting de int to char
																	//Se itera con i*N porque ese sera el movimiento entre las columnas
		}
	}
}

void ImprimirMatriz(char* Matriz, int M, int N, int ImprimirPosiciones)
{
	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (ImprimirPosiciones == 1)
				printf("[%d][%d] = %c\t", i, j, Matriz[i * N + j]);
			else
				printf("%c ", Matriz[i * N + j]);

			if (j + 1 == N)
				printf("\n");
		}
	}
}

void ImprimirPalabras(Lista* palabras)
{
	int i = 1;
	while (palabras->sig != NULL)
	{
		printf("%d) %s\t", i, palabras->palabra);
		if (strlen(palabras->palabra) < 5) printf("\t");	//Para ayudar a mostrar "columnas"
		palabras = palabras->sig;
		i++;
		if (i % 2 == 1)
			printf("\n");									//Para ayudar a mostrar "Columnas"
	}
}

char* transponer(char* MatrizOriginal, int Fila, int Columna)
{
	char* MatrizTranspuesta = malloc(Fila * Columna * sizeof(char));

	for (int i = 0; i < Fila; i++)
	{
		for (int j = 0; j < Columna; j++)
		{
			MatrizTranspuesta[j * Fila + i] = MatrizOriginal[i * Columna + j];
		}
	}

	return MatrizTranspuesta;
}

SopaDeLetras leerSopaDeArchivo()
{
	//Variables a usar
	char	tamanio_char[10],										//Lee la primera linea con el tama�o de matriz
		caracter;												//Iterador de caracter leido
	int		posCaracter = 0;										//Iterador para la posici�n de la matriz a guardar
	SopaDeLetras sopa;												//Estructura donde se guardara la Sopa
	FILE* sopaPtr = fopen("sopa.in", "r");							//Abrimos el archivo sopa.in con un Puntero FILE*

	if (sopaPtr == NULL) {											//El archivo se pudo leer?
		sopa.SOPA = NULL;
		sopa.tam = 0;
		return sopa;
	}

	//Obtenemos el tama�o de la sopa leyendo la primera fila
	fgets(tamanio_char, 10, sopaPtr);								//Se lee el tama�o de matriz primera linea archivo
	sopa.tam = atoi(tamanio_char);									//Convierte la primera linea de chars en un entero

	//Creamos la matriz
	sopa.SOPA = malloc(sopa.tam * sopa.tam * sizeof(char));			//Reservamos el espacio

	while (1) {
		caracter = fgetc(sopaPtr);
		if (caracter == EOF) break;									//Hasta que se pueda leer

		else
		{
			if (caracter == ' ' || caracter == '\n') continue;		//Los saltos de linea y espacios no los debe leer

			sopa.SOPA[posCaracter] = caracter;
			posCaracter++;
		}
	}

	fclose(sopaPtr);												//Cierre de archivo

	return sopa;
}

void main()
{
	/*------------------Se lee la matriz----------------------*/
	printf("Leyendo sopa.in...\t");
	SopaDeLetras sopa = leerSopaDeArchivo();
	if (sopa.SOPA == NULL) {
		printf("Error al leer archivo  sopa.in o no se encontro.");
		return;
	}
	else
		printf("Archivo leido con exito.\n");
	/*------------------Se leen las palabras----------------------*/
	printf("Leyendo palabras.in...\t");
	Lista* palabras = leerPalabras();
	if (palabras->cantidadPalabras == -1) {
		printf("Error al leer archivo palabras.in o no se encontro.");
		return;
	}
	else
		printf("Archivo leido con exito.\n\n");
	/*------------------Se imprimen las palabras------------------*/
	printf("Palabras (%d): \n\n", palabras->cantidadPalabras);
	ImprimirPalabras(palabras);
	/*------------------Se imprime la matriz------------------*/
	printf("\n\nMatriz (%dx%d): \n\n", sopa.tam, sopa.tam);
	ImprimirMatriz(sopa.SOPA, sopa.tam, sopa.tam, 0);
	/*------------------Se imprime la matriz------------------*/
	printf("\n\nMatriz Transpuesta: \n\n");
	ImprimirMatriz(transponer(sopa.SOPA, sopa.tam, sopa.tam), sopa.tam, sopa.tam, 0);					//Se invierte el numero de filas y columnas
}