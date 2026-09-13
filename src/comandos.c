#include "comandos.h"
#include "tp1.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define RAREZA_INVALIDA '?'

char rareza_a_letra(enum rareza_pokemon rareza);

void imprimir_pokemon(const struct pokemon *pokemon)
{
	if (pokemon == NULL) {
		return;
	}

	printf("%s,%d,%g,%c\n", pokemon->nombre, pokemon->velocidad,
	       pokemon->peso, rareza_a_letra(pokemon->rareza));
}

bool imprimir_todos(struct pokemon *pokemon, void *extra)
{
	(void)extra;
	imprimir_pokemon(pokemon);
	return true;
}

struct filtro_rareza {
	enum rareza_pokemon rareza;
};

bool imprimir_si_coincide_rareza(struct pokemon *pokemon, void *extra)
{
	struct filtro_rareza *filtro = extra;

	if (pokemon == NULL || filtro == NULL) {
		return false;
	}

	if (pokemon->rareza == filtro->rareza) {
		imprimir_pokemon(pokemon);
	}

	return true;
}

int comando_buscar_nombre(tp1_t *tp1, const char *nombre)
{
	if (tp1 == NULL || nombre == NULL) {
		return 1;
	}

	struct pokemon *pokemon = tp1_buscar_pokemon(tp1, nombre);

	if (pokemon == NULL) {
		fprintf(stderr, "No se encontro el pokemon %s\n", nombre);

		return 1;
	}

	imprimir_pokemon(pokemon);
	return 0;
}

int comando_mostrar_uno(tp1_t *tp1)
{
	size_t cantidad = tp1_cantidad(tp1);

	if (cantidad == 0) {
		fprintf(stderr, "La pokedex esta vacia\n");
		return 1;
	}

	srand((unsigned int)time(NULL));

	size_t posicion = (size_t)rand() % cantidad;

	struct pokemon *pokemon = tp1_buscar_orden(tp1, posicion);

	if (pokemon == NULL) {
		return 1;
	}

	imprimir_pokemon(pokemon);
	return 0;
}

int comando_mostrar_todo(tp1_t *tp1)
{
	if (tp1 == NULL) {
		return 1;
	}

	tp1_iterar(tp1, imprimir_todos, NULL);
	return 0;
}

int comando_mostrar_rarezas(tp1_t *tp1)
{
	if (tp1 == NULL) {
		return 1;
	}

	struct filtro_rareza filtro;

	filtro.rareza = RAREZA_COMUN;
	tp1_iterar(tp1, imprimir_si_coincide_rareza, &filtro);

	filtro.rareza = RAREZA_RARO;
	tp1_iterar(tp1, imprimir_si_coincide_rareza, &filtro);

	filtro.rareza = RAREZA_LEGENDARIO;
	tp1_iterar(tp1, imprimir_si_coincide_rareza, &filtro);

	return 0;
}