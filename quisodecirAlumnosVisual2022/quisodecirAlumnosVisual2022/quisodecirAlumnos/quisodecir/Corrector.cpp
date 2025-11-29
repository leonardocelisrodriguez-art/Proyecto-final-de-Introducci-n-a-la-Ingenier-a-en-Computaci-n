/*****************************************************************************************************************
	UNIVERSIDAD NACIONAL AUTONOMA DE MEXICO
	FACULTAD DE ESTUDIOS SUPERIORES -ARAGON-

	Computadoras y programacion. 
	(c) Celis Rodríguez Leonardo, 426124294
	
	Quiso decir: Programa principal de la aplicacion de la distancia de Levenstein.
	
******************************************************************************************************************/


#include "stdafx.h"
#include <string.h>
#include <stdio.h>
#include <iostream>
#include "corrector.h"

//Funciones publicas del proyecto
/*****************************************************************************************************************
	DICCIONARIO: Esta funcion crea el diccionario completo
	char *	szNombre				:	Nombre del archivo de donde se sacaran las palabras del diccionario	
	char	szPalabras[][TAMTOKEN]	:	Arreglo con las palabras completas del diccionario
	int		iEstadisticas[]			:	Arreglo con el numero de veces que aparecen las palabras en el diccionario
	int &	iNumElementos			:	Numero de elementos en el diccionario
******************************************************************************************************************/
void	Diccionario			(char *szNombre, char szPalabras[][TAMTOKEN], int iEstadisticas[], int &iNumElementos)
{
	int cantreal = 0;
	char l;
	int let = 0;
	char pTemp[TAMTOKEN];
	FILE* archivo;
	fopen_s(&archivo, szNombre, "r");
	if (archivo != NULL)
	{
		while (fscanf_s(archivo, "%c", &l, 1) == 1)
		{
			if (l == ';' || l == ',' || l == '(' || l == ')' || l == '.' || l == '\t' || l == '\r' || l == '\n' || l == ' ')
			{
				if (let > 0)
				{
					pTemp[let] = '\0';
					let = 0;
					int noHay = 1;
					for (int i = 0; i < cantreal; i++)
					{
						if (strcmp(szPalabras[i], pTemp) == 0)
						{
							noHay = 0;
							iEstadisticas[i]++;
							break;
						}
					}
					if (noHay == 1)
					{
						strcpy_s(szPalabras[cantreal], pTemp);
						iEstadisticas[cantreal] = 1;
						cantreal++;
					}
				}
			}
			else
			{
				pTemp[let] = tolower(l);
				let++;
			}
		}
		fclose(archivo);
	}
	char auxStr[TAMTOKEN];
	int auxE;

	for (int i = 0; i < cantreal - 1; i++)
	{
		for (int j = 0; j < cantreal - i - 1; j++)
		{
			if (strcmp(szPalabras[j], szPalabras[j + 1]) > 0)
			{
				strcpy_s(auxStr, TAMTOKEN, szPalabras[j]);
				strcpy_s(szPalabras[j], TAMTOKEN, szPalabras[j + 1]);
				strcpy_s(szPalabras[j + 1], TAMTOKEN, auxStr);

				auxE = iEstadisticas[j];
				iEstadisticas[j] = iEstadisticas[j + 1];
				iEstadisticas[j + 1] = auxE;
			}
		}
	}

	for (int d = 0; d < cantreal; d++)
	{
		if (szPalabras[d][0] != '\0')
		{
			iNumElementos++;
		}
	}
}

/*****************************************************************************************************************
	ListaCandidatas: Esta funcion recupera desde el diccionario las palabras validas y su peso
	Regresa las palabras ordenadas por su peso
	char	szPalabrasSugeridas[][TAMTOKEN],	//Lista de palabras clonadas
	int		iNumSugeridas,						//Lista de palabras clonadas
	char	szPalabras[][TAMTOKEN],				//Lista de palabras del diccionario
	int		iEstadisticas[],					//Lista de las frecuencias de las palabras
	int		iNumElementos,						//Numero de elementos en el diccionario
	char	szListaFinal[][TAMTOKEN],			//Lista final de palabras a sugerir
	int		iPeso[],							//Peso de las palabras en la lista final
	int &	iNumLista)							//Numero de elementos en la szListaFinal
******************************************************************************************************************/
void	ListaCandidatas		(
	char	szPalabrasSugeridas[][TAMTOKEN],	//Lista de palabras clonadas
	int		iNumSugeridas,						//Lista de palabras clonadas
	char	szPalabras[][TAMTOKEN],				//Lista de palabras del diccionario
	int		iEstadisticas[],					//Lista de las frecuencias de las palabras
	int		iNumElementos,						//Numero de elementos en el diccionario
	char	szListaFinal[][TAMTOKEN],			//Lista final de palabras a sugerir
	int		iPeso[],							//Peso de las palabras en la lista final
	int &	iNumLista)							//Numero de elementos en la szListaFinal
{
	int PalabraExiste = 1;
	for (int i = 0; i < iNumSugeridas; i++)
	{
		int j;
		for (j = 0; j < iNumElementos; j++)
		{
			if (strcmp(szPalabrasSugeridas[i], szPalabras[j]) == 0)
			{
				PalabraExiste = -1;
				break;
			}
		}
		if (PalabraExiste != 1)
		{
			int h = 0;
			strcpy_s(szListaFinal[h], szPalabrasSugeridas[i]);
			iPeso[h] = iEstadisticas[j];
			h++;
			iNumLista++;
			PalabraExiste = 1;
		}
	}

	char auxStr[TAMTOKEN];
	int auxE;
	int bNoOrdenado = true;
	for (int pasada = 0; pasada < iNumLista - 1 && bNoOrdenado; pasada++)
	{
		bNoOrdenado = false;
		for (int i = 0; i < iNumLista - 1; i++)
		{
			if (iPeso[i] < iPeso[i + 1])
			{
				auxE = iPeso[i];
				iPeso[i] = iPeso[i + 1];
				iPeso[i + 1] = auxE;
				bNoOrdenado = true;

				strcpy_s(auxStr, TAMTOKEN, szListaFinal[i]);
				strcpy_s(szListaFinal[i], TAMTOKEN, szListaFinal[i + 1]);
				strcpy_s(szListaFinal[i + 1], TAMTOKEN, auxStr);
			}

		}
	}
}

/*****************************************************************************************************************
	ClonaPalabras: toma una palabra y obtiene todas las combinaciones y permutaciones requeridas por el metodo
	char *	szPalabraLeida,						// Palabra a clonar
	char	szPalabrasSugeridas[][TAMTOKEN], 	//Lista de palabras clonadas
	int &	iNumSugeridas)						//Numero de elementos en la lista
******************************************************************************************************************/
void	ClonaPalabras(
	char* szPalabraLeida,						// Palabra a clonar
	char	szPalabrasSugeridas[][TAMTOKEN], 	//Lista de palabras clonadas
	int& iNumSugeridas)						//Numero de elementos en la lista
{
	int pal = 0;
	int let = 0;
	char Letras[] = { 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','ñ','o','p','q','r','s','t','u','v','w','x','y','z','á','é','í','ó','ú' };
	char PalTemp[TAMTOKEN];
	int LetrasPalabra = strlen(szPalabraLeida);

	if (LetrasPalabra > 1)
	{
		for (int i = 0; i < LetrasPalabra; i++, pal++) //Eliminacion de un caracter
		{
			strcpy_s(PalTemp, szPalabraLeida);
			for (int j = 0; j < LetrasPalabra; j++)
			{
				if (i != j)
				{
					szPalabrasSugeridas[pal][let] = PalTemp[j];
					let++;
				}
			}
			szPalabrasSugeridas[pal][let] = '\0';
			iNumSugeridas++;
			let = 0;
		}
	}

	char aux;
	if (LetrasPalabra > 1)
	{
		for (int i = 0; i < LetrasPalabra - 1; i++, pal++) //Traspocision de caracteres
		{
			strcpy_s(PalTemp, szPalabraLeida);
			aux = PalTemp[i];
			PalTemp[i] = PalTemp[i + 1];
			PalTemp[i + 1] = aux;

			strcpy_s(szPalabrasSugeridas[pal], PalTemp);
			iNumSugeridas++;
		}
	}

	for (int i = 0; i < LetrasPalabra; i++) //Sustituir caracter original por letra del alfabeto
	{
		strcpy_s(PalTemp, szPalabraLeida);
		for (int j = 0; j < 32; j++, pal++)
		{
			PalTemp[i] = Letras[j];
			strcpy_s(szPalabrasSugeridas[pal], PalTemp);
			iNumSugeridas++;
		}
	}

	char PalTempTemp[TAMTOKEN];
	for (int i = 0; i < LetrasPalabra + 1; i++) //Insertar letra del alfabeto entre espacios
	{
		strcpy_s(PalTempTemp, szPalabraLeida);
		strcpy_s(PalTemp, szPalabraLeida);
		int longi = strlen(PalTemp);
		PalTemp[i] = 0;
		for (int h = i; h < LetrasPalabra; h++)
		{
			PalTemp[h + 1] = PalTempTemp[h];
		}
		for (int j = 0; j < 32; j++, pal++)
		{
			PalTemp[i] = Letras[j];
			PalTemp[longi + 1] = '\0';
			strcpy_s(szPalabrasSugeridas[pal], PalTemp);
			iNumSugeridas++;
		}
	}

	strcpy_s(szPalabrasSugeridas[iNumSugeridas], TAMTOKEN, szPalabraLeida);
	iNumSugeridas++;

	for (int i = 0; i < iNumSugeridas - 1; i++) //Ordenar las palabras sugeridas
	{
		char buffer[TAMTOKEN];
		for (int j = i + 1; j < iNumSugeridas; j++)
		{
			if (strcmp(szPalabrasSugeridas[i], szPalabrasSugeridas[j]) > 0)
			{
				strcpy_s(buffer, TAMTOKEN, szPalabrasSugeridas[i]);
				strcpy_s(szPalabrasSugeridas[i], TAMTOKEN, szPalabrasSugeridas[j]);
				strcpy_s(szPalabrasSugeridas[j], TAMTOKEN, buffer);
			}
		}
	}
}