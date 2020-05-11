#include <stdio.h>

/*------------------Estructuras----------------------*/
typedef struct {
	char* SOPA;		//La Sopa
	int tam;		//Tamaño de Matriz NxN
} SopaDeLetras;

typedef struct nodo {
	char* palabra;
	int cantidadPalabras;
	struct nodo* cabecera;
	struct nodo* sig;
} Lista;

/*------------------I/O Archivos----------------------*/
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
		if (fgets(buffer_palabras, 100, palabrasPtr) != NULL) {		//Se lee la linea, si es NULL, no hay más palabras
			ListaPalabras->palabra = malloc(strlen(buffer_palabras) * sizeof(char)); //Asignamos espacio
			strcpy(ListaPalabras->palabra, strtok(buffer_palabras, "\n"));				//Guardamos la palabra y eliminamos el caracter \n
			ListaPalabras->sig = (Lista*)malloc(sizeof(Lista));		//Creamos un nuevo nodo y lo anexamos a la siguiente posición de la lista

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

SopaDeLetras leerSopaDeArchivo()
{
	//Variables a usar
	char	tamanio_char[10],										//Lee la primera linea con el tamaño de matriz
		caracter;												//Iterador de caracter leido
	int		posCaracter = 0;										//Iterador para la posición de la matriz a guardar
	SopaDeLetras sopa;												//Estructura donde se guardara la Sopa
	FILE* sopaPtr = fopen("sopa.in", "r");							//Abrimos el archivo sopa.in con un Puntero FILE*

	if (sopaPtr == NULL) {											//El archivo se pudo leer?
		sopa.SOPA = NULL;
		sopa.tam = 0;
		return sopa;
	}

	//Obtenemos el tamaño de la sopa leyendo la primera fila
	fgets(tamanio_char, 10, sopaPtr);								//Se lee el tamaño de matriz primera linea archivo
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

/*------------------Funciones Matriz----------------------*/
char* GenerarMatriz(int M, int N)
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

char* TransponerMatriz(char* MatrizOriginal, int Fila, int Columna)
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

char* InvertirMatriz(char* MatrizOriginal, int Fila, int Columna)
{
	char* MatrizInversa = malloc(Fila * Columna * sizeof(char));
	int totalCaracteres = Fila * Columna;

	for (int i = 0; i < Fila; i++)
	{
		for (int j = 0; j < Columna; j++)
		{
			MatrizInversa[i * Fila + j] = MatrizOriginal[((i + 1) * Fila - 1) - j];
		}

	}

	return MatrizInversa;
}

/*------------------Funciones Búsqueda----------------------*/
int BuscarEnLinea(char* Linea, char* Palabra)
{
	char* p1, * p2, * p3;
	int i = 0, j = 0, flag = 0;

	p1 = Linea;
	p2 = Palabra;

	for (i = 0; i < strlen(Linea); i++)
	{
		if (*p1 == *p2)
		{
			p3 = p1;
			for (j = 0; j < strlen(Palabra); j++)
			{
				if (*p3 == *p2)
				{
					p3++; p2++;
				}
				else
					break;
			}
			p2 = Palabra;
			if (j == strlen(Palabra))
			{
				flag = 1;
				return i;
			}
		}
		p1++;
	}
	if (flag == 0)
	{
		return -1;
	}
}

char* ObtenerLinea(char* Matriz, int Columnas, int Fila)
{
	char* Linea = malloc(Columnas * sizeof(char)); //Guarda la linea que se leerá

	for (int i = 0; i < Columnas + 1; i++)//iterador de caracteres en una fila
		*(Linea + i) = *(Matriz + (Columnas * Fila + i)); //Se copian N letras en el string

	*(Linea + Columnas) = '\0'; //Esto permite que los printf no lleguen hasta el proximo null en memoria
	
	return Linea;
}

void BuscarEnMatriz(SopaDeLetras Sopa, Lista* Palabras)
{
	int columna = 0; //Indica en que columna hubo palabra
	Lista* aux = Palabras;

	while (aux->sig != NULL)
	{
		for (int i = 0; i < Sopa.tam; i++)
		{
			char* lineaMatriz = ObtenerLinea(Sopa.SOPA, Sopa.tam, i);
			columna = BuscarEnLinea(lineaMatriz, aux->palabra);

			if (columna != -1)
				printf("%s encontrada en posicion [%i,%i] (->) \n", aux->palabra, i, columna);
		}
		aux = aux->sig;
	}

	aux = Palabras;

	while (aux->sig != NULL)
	{
		for (int i = 0; i < Sopa.tam; i++)
		{
			char* lineaMatriz = ObtenerLinea(InvertirMatriz(Sopa.SOPA, Sopa.tam, Sopa.tam), Sopa.tam, i);
			columna = BuscarEnLinea(lineaMatriz, aux->palabra);

			if (columna != -1)
				printf("%s encontrada en posicion [%i,%i] (<-)\n", aux->palabra, i, Sopa.tam - columna-1);
		}
		aux = aux->sig;
	}

	aux = Palabras;

	while (aux->sig != NULL)
	{
		for (int i = 0; i < Sopa.tam; i++)
		{
			char* lineaMatriz = ObtenerLinea(TransponerMatriz(Sopa.SOPA, Sopa.tam, Sopa.tam), Sopa.tam, i);
			columna = BuscarEnLinea(lineaMatriz, aux->palabra);

			if (columna != -1)
				printf("%s encontrada en posicion [%i,%i] (V)\n", aux->palabra, columna, i);
		}
		aux = aux->sig;
	}

	aux = Palabras;

	while (aux->sig != NULL)
	{
		for (int i = 0; i < Sopa.tam; i++)
		{
			char* lineaMatriz = ObtenerLinea(InvertirMatriz(TransponerMatriz(Sopa.SOPA, Sopa.tam, Sopa.tam), Sopa.tam, Sopa.tam), Sopa.tam, i);

			columna = BuscarEnLinea(lineaMatriz, aux->palabra);

			if (columna != -1)
				printf("%s encontrada en posicion [%i,%i] (A) \n", aux->palabra, Sopa.tam - columna - 1, i);
		}
		aux = aux->sig;
	}
}



void main()
{
	/*------------------Titulo----------------------*/
	printf("    ____ ____ ___  ____    ___  ____\n");
	printf("    [__  |  | |__] |__|    |  \\ |___\n");
	printf("    ___] |__| |    |  |    |__/ |___\n\n");

	printf("    _    ____ ___ ____ ____ ____\n");
	printf("    |    |___  |  |__/ |__| [__\n");
	printf("    |___ |___  |  |  \\ |  | ___]\n\n");


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

	//BuscarEnLinea("jdbwjndjperronsjdndnj", "perro");
	//ObtenerLinea(sopa, 1);
	printf("\n\n");
	BuscarEnMatriz(sopa, palabras);


//
//	/*------------------Se imprime la matriz transpuesta------------------*/
//	printf("\n\nMatriz Transpuesta: \n\n");
//	ImprimirMatriz(TransponerMatriz(sopa.SOPA, sopa.tam, sopa.tam), sopa.tam, sopa.tam, 0);					//Se invierte el numero de filas y columnas
//	/*------------------Se imprime la matriz inversa------------------*/
//	printf("\n\nMatriz Inversa (%dx%d): \n\n", sopa.tam, sopa.tam);
//	ImprimirMatriz(InvertirMatriz(sopa.SOPA, sopa.tam, sopa.tam), sopa.tam, sopa.tam, 0);
//	/*------------------Se imprime la matriz inversa transpuesta------------------*/
//	printf("\n\nMatriz Inversa Transpuesta(%dx%d): \n\n", sopa.tam, sopa.tam);
//	ImprimirMatriz(InvertirMatriz(TransponerMatriz(sopa.SOPA, sopa.tam, sopa.tam), sopa.tam, sopa.tam), sopa.tam, sopa.tam, 0);
}