#include "tp1.h"
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdbool.h>
#include <float.h>

#define MAX_CAMPOS 4
#define CAP_INICIAL 1

int agrandar_vector_pokemons(tp1_t *pokedex);
char *leer_linea(FILE *archivo);
struct pokemon *parsear_pokemon(char *linea);

static int parsear_rareza(char letra, enum rareza_pokemon *rareza);

static size_t separar_campos_por_comas(char *linea, char **campos,
				       size_t campos_totales);

static void ordenar_pokemons_por_nombre(struct pokemon *pokemons,
					size_t cantidad);

struct tp1 {
	struct pokemon *pokemones;
	unsigned int cantidad_pokemons;
	size_t capacidad_pokemons;
};

tp1_t *crear_tp1_vacio()
{
	tp1_t *tp1 = malloc(sizeof(tp1_t));
	if (tp1 == NULL) {
		return NULL;
	}
	tp1->cantidad_pokemons = 0;
	tp1->capacidad_pokemons = CAP_INICIAL;
	tp1->pokemones =
		malloc(tp1->capacidad_pokemons * sizeof(*tp1->pokemones));

	if (tp1->pokemones == NULL) {
		free(tp1);
		return NULL;
	}

	return tp1;
}

int agregar_pokemon_a_pokedex(tp1_t *pokedex, struct pokemon *nuevo_pokemon)
{
	if (tp1_buscar_pokemon(pokedex, nuevo_pokemon->nombre) != NULL) {
		free(nuevo_pokemon->nombre);
		free(nuevo_pokemon);
		return 0;
	}

	if (pokedex->cantidad_pokemons == pokedex->capacidad_pokemons) {
		if (agrandar_vector_pokemons(pokedex) == -1) {
			free(nuevo_pokemon->nombre);
			free(nuevo_pokemon);
			return -1;
		}
	}

	pokedex->pokemones[pokedex->cantidad_pokemons] = *nuevo_pokemon;
	pokedex->cantidad_pokemons++;

	free(nuevo_pokemon);

	return 1;
}

tp1_t *tp1_leer_archivo(const char *nombre)
{
	if (nombre == NULL) {
		return NULL;
	}

	FILE *archivo = fopen(nombre, "r");
	if (archivo == NULL) {
		return NULL;
	}

	tp1_t *pokedex = crear_tp1_vacio();
	if (pokedex == NULL) {
		fclose(archivo);
		return NULL;
	}

	char *linea;
	while ((linea = leer_linea(archivo)) != NULL) {
		struct pokemon *nuevo_pokemon = parsear_pokemon(linea);
		free(linea);
		if (nuevo_pokemon == NULL) {
			continue;
		}
		if (agregar_pokemon_a_pokedex(pokedex, nuevo_pokemon) == -1) {
			fclose(archivo);
			tp1_destruir(pokedex);
			return NULL;
		}
	}
	ordenar_pokemons_por_nombre(pokedex->pokemones,
				    pokedex->cantidad_pokemons);
	fclose(archivo);
	return pokedex;
}

bool parsear_velocidad(char *campo, int *resultado)
{
	if (campo == NULL || resultado == NULL) {
		return false;
	}

	char *fin;
	long valor = strtol(campo, &fin, 10);
	if (fin == campo || *fin != '\0' || valor < 1 || valor > 99) {
		return false;
	}
	*resultado = (int)valor;
	return true;
}

bool parsear_peso(char *campo, float *resultado)
{
	if (campo == NULL || resultado == NULL) {
		return false;
	}

	char *fin;
	long valor = strtof(campo, &fin);

	if (fin == campo || *fin != '\0' || valor < 0 || valor > FLT_MAX) {
		return false;
	}

	*resultado = (int)valor;
	return true;
}

int parsear_rareza(char letra, enum rareza_pokemon *rareza)
{
	switch (letra) {
	case 'C':
		*rareza = RAREZA_COMUN;
		return 1;

	case 'R':
		*rareza = RAREZA_RARO;
		return 1;

	case 'L':
		*rareza = RAREZA_LEGENDARIO;
		return 1;

	default:
		return 0;
	}
}

struct pokemon *parsear_pokemon(char *linea)
{
	char *campos[MAX_CAMPOS];
	size_t cantidad_campos =
		separar_campos_por_comas(linea, campos, MAX_CAMPOS);

	if (cantidad_campos != MAX_CAMPOS) {
		return NULL;
	}

	if (campos[0] == NULL || campos[0][0] == '\0') {
		return NULL;
	}

	int velocidad;
	float peso;
	enum rareza_pokemon rareza;

	if (!parsear_velocidad(campos[1], &velocidad) ||
	    !parsear_peso(campos[2], &peso) || campos[3][0] == '\0' ||
	    campos[3][1] != '\0' || !parsear_rareza(campos[3][0], &rareza)) {
		return NULL;
	}

	struct pokemon *pokemon = malloc(sizeof(struct pokemon));

	if (pokemon == NULL) {
		return NULL;
	}

	pokemon->nombre = malloc(strlen(campos[0]) + 1);

	if (pokemon->nombre == NULL) {
		free(pokemon);
		return NULL;
	}

	strcpy(pokemon->nombre, campos[0]);
	pokemon->velocidad = velocidad;
	pokemon->peso = peso;
	pokemon->rareza = rareza;
	return pokemon;
}

size_t convertir_campos_en_strings(char *linea, char **campos,
				   size_t cant_campos, size_t max_campos)
{
	for (size_t i = 0; linea[i] != '\0'; i++) {
		if (linea[i] == ',') {
			linea[i] = '\0';

			if (cant_campos < max_campos) {
				campos[cant_campos] = &linea[i + 1];
			}
			cant_campos++;
		}
	}
	return cant_campos;
}

size_t separar_campos_por_comas(char *linea, char **campos,
				size_t campos_totales)
{
	{
		if (linea == NULL || campos == NULL || campos_totales == 0)
			return 0;

		size_t cant_campos = 1;
		campos[0] = linea;

		cant_campos = convertir_campos_en_strings(
			linea, campos, cant_campos, campos_totales);

		return cant_campos;
	}
}

int agrandar_vector_pokemons(tp1_t *pokedex)
{
	size_t nueva_capacidad = pokedex->capacidad_pokemons * 2;

	struct pokemon *nuevo_vector = realloc(
		pokedex->pokemones, nueva_capacidad * sizeof(*nuevo_vector));

	if (nuevo_vector == NULL) {
		return -1;
	}
	pokedex->pokemones = nuevo_vector;
	pokedex->capacidad_pokemons = nueva_capacidad;
	return 0;
}

size_t tp1_cantidad(tp1_t *tp1)
{
	if (tp1 == NULL) {
		return 0;
	}
	return tp1->cantidad_pokemons;
}

void *tp1_destruir(tp1_t *tp1)
{
	if (tp1 == NULL) {
		return NULL;
	}

	for (size_t i = 0; i < tp1->cantidad_pokemons; i++) {
		free(tp1->pokemones[i].nombre);
	}
	free(tp1->pokemones);
	free(tp1);

	return NULL;
}

struct pokemon *tp1_buscar_pokemon(tp1_t *pokedex, const char *nombre_pokemon)
{
	if (!pokedex || !nombre_pokemon) {
		return NULL;
	}
	for (size_t i = 0; i < pokedex->cantidad_pokemons; i++) {
		if (strcasecmp(pokedex->pokemones[i].nombre, nombre_pokemon) ==
		    0) {
			return &pokedex->pokemones[i];
		}
	}
	return NULL;
}

void ordenar_pokemons_por_nombre(struct pokemon *pokemons, size_t cantidad)
{
	if (!pokemons || cantidad < 2) {
		return;
	}

	for (size_t i = 0; i + 1 < cantidad; i++) {
		size_t indice_menor = i;

		for (size_t j = i + 1; j < cantidad; j++) {
			if (strcasecmp(pokemons[j].nombre,
				       pokemons[indice_menor].nombre) < 0) {
				indice_menor = j;
			}
		}

		if (indice_menor != i) {
			struct pokemon auxiliar = pokemons[i];
			pokemons[i] = pokemons[indice_menor];
			pokemons[indice_menor] = auxiliar;
		}
	}
}

void ordenar_pokemons_por_rareza(struct pokemon *pokemons, size_t cantidad)
{
	if (!pokemons || cantidad < 2) {
		return;
	}

	for (size_t i = 0; i + 1 < cantidad; i++) {
		size_t indice_menor = i;

		for (size_t j = i + 1; j < cantidad; j++) {
			if (pokemons[j].rareza <
			    pokemons[indice_menor].rareza) {
				indice_menor = j;
			}
		}

		if (indice_menor != i) {
			struct pokemon auxiliar = pokemons[i];
			pokemons[i] = pokemons[indice_menor];
			pokemons[indice_menor] = auxiliar;
		}
	}
}

int agregar_copia_pokemon(tp1_t *destino, struct pokemon *pokemon)
{
	if (tp1_buscar_pokemon(destino, pokemon->nombre) != NULL) {
		return 0;
	}
	if (destino->cantidad_pokemons == destino->capacidad_pokemons) {
		if (agrandar_vector_pokemons(destino) == -1) {
			return -1;
		}
	}

	struct pokemon copia_pokemon = *pokemon;

	copia_pokemon.nombre = malloc(strlen(pokemon->nombre) + 1);
	if (copia_pokemon.nombre == NULL) {
		return -1;
	}

	strcpy(copia_pokemon.nombre, pokemon->nombre);

	destino->pokemones[destino->cantidad_pokemons] = copia_pokemon;
	destino->cantidad_pokemons++;

	return 0;
}

struct pokemon *tp1_buscar_orden(tp1_t *tp1, size_t n)
{
	if (tp1 == NULL || n >= tp1->cantidad_pokemons) {
		return NULL;
	}

	return &tp1->pokemones[n];
}

tp1_t *tp1_combinar(tp1_t *tp1_a, tp1_t *tp1_b)
{
	if (tp1_a == NULL || tp1_b == NULL) {
		return NULL;
	}

	tp1_t *resultado = crear_tp1_vacio();

	if (resultado == NULL) {
		return NULL;
	}

	if (resultado->pokemones == NULL) {
		free(resultado);
		return NULL;
	}

	for (size_t i = 0; i < tp1_a->cantidad_pokemons; i++) {
		if (agregar_copia_pokemon(resultado, &tp1_a->pokemones[i]) ==
		    -1) {
			tp1_destruir(resultado);
			return NULL;
		}
	}

	for (size_t i = 0; i < tp1_b->cantidad_pokemons; i++) {
		if (agregar_copia_pokemon(resultado, &tp1_b->pokemones[i]) ==
		    -1) {
			tp1_destruir(resultado);
			return NULL;
		}
	}

	ordenar_pokemons_por_nombre(resultado->pokemones,
				    resultado->cantidad_pokemons);

	return resultado;
}

size_t tp1_iterar(tp1_t *tp1, bool (*f)(struct pokemon *, void *), void *extra)
{
	if (tp1 == NULL || f == NULL) {
		return 0;
	}

	size_t iteraciones = 0;

	for (size_t i = 0; i < tp1->cantidad_pokemons; i++) {
		iteraciones++;

		if (!f(&tp1->pokemones[i], extra)) {
			break;
		}
	}

	return iteraciones;
}

char rareza_a_letra(enum rareza_pokemon rareza)
{
	switch (rareza) {
	case RAREZA_COMUN:
		return 'C';

	case RAREZA_RARO:
		return 'R';

	case RAREZA_LEGENDARIO:
		return 'L';
	}

	return '\0';
}

tp1_t *tp1_escribir_archivo(tp1_t *tp1, const char *nombre)
{
	if (tp1 == NULL || nombre == NULL) {
		return NULL;
	}

	FILE *archivo = fopen(nombre, "w");
	if (archivo == NULL) {
		return NULL;
	}

	for (size_t i = 0; i < tp1->cantidad_pokemons; i++) {
		struct pokemon *pokemon = &tp1->pokemones[i];
		char rareza = rareza_a_letra(pokemon->rareza);

		if (rareza == '\0' ||
		    fprintf(archivo, "%s,%d,%g,%c\n", pokemon->nombre,
			    pokemon->velocidad, pokemon->peso, rareza) < 0) {
			fclose(archivo);
			return NULL;
		}
	}

	if (fclose(archivo) != 0) {
		return NULL;
	}

	return tp1;
}