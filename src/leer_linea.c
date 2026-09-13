#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int agrandar_capacidad(char **linea, size_t *capacidad)
{
	size_t nueva_capacidad = *capacidad < 2 ? 8 : *capacidad * 2;
	char *linea_nueva = realloc(*linea, nueva_capacidad);

	if (linea_nueva == NULL) {
		return -1;
	}

	*linea = linea_nueva;
	*capacidad = nueva_capacidad;

	return 0;
}

int leer_linea_ptr(char **ptr, size_t *tamaño, FILE *archivo)
{
	size_t bloques_usados = 0;

	if (ptr == NULL || tamaño == NULL) {
		return -1;
	}

	if (*ptr == NULL || *tamaño < 2) {
		if (agrandar_capacidad(ptr, tamaño) == -1) {
			return -1;
		}
	}

	(*ptr)[0] = '\0';

	while (fgets(*ptr + bloques_usados,
		     (int)(*tamaño - (size_t)bloques_usados),
		     archivo) != NULL) {
		bloques_usados += strlen(*ptr + bloques_usados);
		if (bloques_usados > 0 && (*ptr)[bloques_usados - 1] == '\n') {
			return (int)bloques_usados;
		}
		if ((size_t)bloques_usados + 1 >= *tamaño) {
			if (agrandar_capacidad(ptr, tamaño) == -1) {
				return -1;
			}
		}
	}

	if (ferror(archivo) || bloques_usados == 0) {
		return -1;
	}

	return (int)bloques_usados;
}

char *leer_linea(FILE *archivo)
{
	size_t capacidad = 8;
	char *linea = malloc(capacidad);

	if (linea == NULL) {
		return NULL;
	}

	if (leer_linea_ptr(&linea, &capacidad, archivo) == -1) {
		free(linea);
		return NULL;
	}

	return linea;
}