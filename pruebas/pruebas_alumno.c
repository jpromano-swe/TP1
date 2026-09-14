#include "pa2m.h"
#include <stdio.h>
#include "../src/tp1.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define ARCHIVO_VACIO "archivos_prueba/prueba_vacio.csv"
#define ARCHIVO_NORMAL "archivos_prueba/prueba_normal.csv"
#define ARCHIVO_REPETIDOS "archivos_prueba/prueba_repetidos.csv"
#define ARCHIVO_INVALIDOS "archivos_prueba/prueba_linea_invalida.csv"
#define ARCHIVO_PARSEO "archivos_prueba/prueba_parseo.csv"
#define ARCHIVO_A "archivos_prueba/prueba_a.csv"
#define ARCHIVO_B "archivos_prueba/prueba_b.csv"
#define ARCHIVO_SALIDA "archivos_prueba/prueba_salida.csv"
#define ARCHIVO_INEXISTENTE "archivos_prueba/archivo_que_no_existe.csv"

void leer_null_devuelve_null(void)
{
	pa2m_afirmar(tp1_leer_archivo(NULL) == NULL,
		     "Leer un nombre NULL devuelve NULL");
}

void leer_archivo_inexistente_devuelve_null(void)
{
	tp1_t *tp1 = tp1_leer_archivo(ARCHIVO_INEXISTENTE);

	pa2m_afirmar(tp1 == NULL, "Leer un archivo inexistente devuelve NULL");

	tp1_destruir(tp1);
}

void leer_archivo_vacio_devuelve_tp1_vacio(void)
{
	tp1_t *tp1 = tp1_leer_archivo(ARCHIVO_VACIO);

	pa2m_afirmar(tp1 != NULL,
		     "Leer un archivo vacio devuelve un TP1 valido");

	pa2m_afirmar(tp1_cantidad(tp1) == 0, "El TP1 leido tiene cero pokemon");

	tp1_destruir(tp1);
}

void leer_archivo_valido_carga_todos(void)
{
	tp1_t *tp1 = tp1_leer_archivo(ARCHIVO_NORMAL);

	pa2m_afirmar(tp1 != NULL, "Leer un archivo valido devuelve un TP1");

	pa2m_afirmar(tp1_cantidad(tp1) == 5, "Se cargaron los cinco pokemon");

	tp1_destruir(tp1);
}

void leer_ignora_repetidos_sin_distinguir_mayusculas(void)
{
	tp1_t *tp1 = tp1_leer_archivo(ARCHIVO_REPETIDOS);

	pa2m_afirmar(tp1 != NULL, "Se pudo leer el archivo con repetidos");

	pa2m_afirmar(tp1_cantidad(tp1) == 2,
		     "Pikachu y PIKACHU son el mismo pokemon");

	struct pokemon *pikachu = tp1_buscar_pokemon(tp1, "pikachu");

	pa2m_afirmar(pikachu != NULL,
		     "Se encuentra Pikachu ignorando mayusculas");

	if (pikachu != NULL) {
		pa2m_afirmar(pikachu->velocidad == 55,
			     "Se mantiene el primer Pikachu encontrado");

		pa2m_afirmar(pikachu->rareza == RAREZA_COMUN,
			     "Se mantienen los datos del primer Pikachu");
	}

	tp1_destruir(tp1);
}

void leer_ignora_lineas_invalidas(void)
{
	tp1_t *tp1 = tp1_leer_archivo(ARCHIVO_INVALIDOS);

	pa2m_afirmar(tp1 != NULL,
		     "Un archivo con lineas invalidas puede cargarse");

	pa2m_afirmar(tp1_cantidad(tp1) == 1,
		     "Las lineas invalidas se ignoran y se conserva Mew");

	tp1_destruir(tp1);
}

void parsear_conserva_pokemons_validos(void)
{
	tp1_t *tp1 = tp1_leer_archivo(ARCHIVO_PARSEO);

	pa2m_afirmar(tp1 != NULL,
		     "El archivo de parseo devuelve un TP1 valido");

	pa2m_afirmar(tp1_cantidad(tp1) == 3,
		     "Solamente se cargan los tres pokemon validos");

	struct pokemon *magikarp = tp1_buscar_pokemon(tp1, "Magikarp");

	pa2m_afirmar(magikarp != NULL, "Pikachu fue parseado correctamente");

	if (magikarp != NULL) {
		pa2m_afirmar(strcmp(magikarp->nombre, "Magikarp") == 0,
			     "El nombre fue parseado correctamente");

		pa2m_afirmar(magikarp->velocidad == 10,
			     "La velocidad fue parseada correctamente");

		pa2m_afirmar(magikarp->peso > 5.19f && magikarp->peso < 5.21f,
			     "El peso fue parseado correctamente");

		pa2m_afirmar(magikarp->rareza == RAREZA_COMUN,
			     "La rareza fue parseada correctamente");
	}

	tp1_destruir(tp1);
}

void parsear_ignora_nombre_vacio(void)
{
	tp1_t *tp1 = tp1_leer_archivo(ARCHIVO_PARSEO);

	pa2m_afirmar(tp1_buscar_pokemon(tp1, "") == NULL,
		     "Un nombre vacio es invalido");

	tp1_destruir(tp1);
}

void parsear_ignora_velocidades_invalidas(void)
{
	tp1_t *tp1 = tp1_leer_archivo(ARCHIVO_PARSEO);

	pa2m_afirmar(tp1_buscar_pokemon(tp1, "VelocidadCero") == NULL,
		     "Una velocidad igual a cero es invalida");

	pa2m_afirmar(tp1_buscar_pokemon(tp1, "VelocidadAlta") == NULL,
		     "Una velocidad mayor que 99 es invalida");

	pa2m_afirmar(tp1_buscar_pokemon(tp1, "VelocidadTexto") == NULL,
		     "Una velocidad no numérica es invalida");

	tp1_destruir(tp1);
}

void parsear_ignora_pesos_invalidos(void)
{
	tp1_t *tp1 = tp1_leer_archivo(ARCHIVO_PARSEO);

	pa2m_afirmar(tp1_buscar_pokemon(tp1, "PesoCero") == NULL,
		     "Un peso igual a cero es invalido");

	pa2m_afirmar(tp1_buscar_pokemon(tp1, "PesoNegativo") == NULL,
		     "Un peso negativo es invalido");

	pa2m_afirmar(tp1_buscar_pokemon(tp1, "PesoTexto") == NULL,
		     "Un peso no numérico es invalido");

	tp1_destruir(tp1);
}

void parsear_ignora_rarezas_invalidas(void)
{
	tp1_t *tp1 = tp1_leer_archivo(ARCHIVO_PARSEO);

	pa2m_afirmar(tp1_buscar_pokemon(tp1, "RarezaInvalida") == NULL,
		     "Un codigo de rareza desconocido es invalido");

	pa2m_afirmar(tp1_buscar_pokemon(tp1, "RarezaLarga") == NULL,
		     "La rareza debe contener solamente su codigo");

	tp1_destruir(tp1);
}

void parsear_ignora_cantidad_incorrecta_de_campos(void)
{
	tp1_t *tp1 = tp1_leer_archivo(ARCHIVO_PARSEO);

	pa2m_afirmar(tp1_buscar_pokemon(tp1, "FaltanCampos") == NULL,
		     "Una linea con menos de cuatro campos es ignorada");

	pa2m_afirmar(tp1_buscar_pokemon(tp1, "SobranCampos") == NULL,
		     "Una linea con más de cuatro campos es ignorada");

	tp1_destruir(tp1);
}

void cantidad_de_null_es_cero(void)
{
	pa2m_afirmar(tp1_cantidad(NULL) == 0,
		     "La cantidad de TP1 NULL es cero");
}

void cargar_cinco_pokemons_cuenta_cinco(void)
{
	tp1_t *tp1 = tp1_leer_archivo(ARCHIVO_NORMAL);

	pa2m_afirmar(tp1 != NULL, "Se pudo leer el archivo con cinco pokemon");

	size_t cantidad = tp1_cantidad(tp1);

	pa2m_afirmar(cantidad == 5,
		     "%zu pokemons unicos leidos correctamente "
		     "(5 eran los esperados)",
		     cantidad);
	tp1_destruir(tp1);
}

void cantidad_no_cuenta_pokemons_repetidos(void)
{
	tp1_t *tp1 = tp1_leer_archivo(ARCHIVO_REPETIDOS);

	pa2m_afirmar(tp1 != NULL,
		     "Se pudo leer el archivo con pokemons repetidos");

	size_t cantidad = tp1_cantidad(tp1);

	pa2m_afirmar(cantidad == 2,
		     "Se almacenaron %zu pokemons unicos "
		     "(2 eran los esperados)",
		     cantidad);

	tp1_destruir(tp1);
}

void buscar_nombre_existente_lo_encuentra_y_valida(void)
{
	tp1_t *tp1 = tp1_leer_archivo(ARCHIVO_NORMAL);
	struct pokemon *pokemon = tp1_buscar_pokemon(tp1, "lapras");

	pa2m_afirmar(pokemon != NULL, "Se encontro un pokemon existente");

	if (pokemon != NULL) {
		pa2m_afirmar(strcmp(pokemon->nombre, "Lapras") == 0,
			     "El pokemon encontrado tiene el nombre correcto");

		pa2m_afirmar(pokemon->rareza == RAREZA_RARO,
			     "El pokemon encontrado tiene la rareza correcta");
	}

	tp1_destruir(tp1);
}

void buscar_nombre_existente_devuelve_puntero_valido(void)
{
	tp1_t *pokedex = tp1_leer_archivo(ARCHIVO_NORMAL);

	pa2m_afirmar(pokedex != NULL,
		     "Se pudo leer el archivo para realizar la busqueda");

	struct pokemon *pokemon = tp1_buscar_pokemon(pokedex, "lApRaS");

	pa2m_afirmar(pokemon != NULL,
		     "Buscar un pokemon existente devuelve un puntero valido");

	if (pokemon != NULL) {
		pa2m_afirmar(strcmp(pokemon->nombre, "Lapras") == 0,
			     "El pokemon encontrado es Lapras");

		pa2m_afirmar(pokemon->velocidad == 30,
			     "Lapras tiene velocidad 30");

		pa2m_afirmar(pokemon->peso > 50.29f && pokemon->peso < 50.31f,
			     "Lapras tiene peso 50.3");

		pa2m_afirmar(pokemon->rareza == RAREZA_RARO,
			     "Lapras tiene rareza RARO");
	}

	tp1_destruir(pokedex);
}

void buscar_nombre_inexistente_devuelve_null(void)
{
	tp1_t *tp1 = tp1_leer_archivo(ARCHIVO_NORMAL);

	pa2m_afirmar(tp1_buscar_pokemon(tp1, "MissingNo") == NULL,
		     "Buscar un nombre inexistente devuelve NULL");

	tp1_destruir(tp1);
}

void buscar_por_orden_alfabetico(void)
{
	tp1_t *tp1 = tp1_leer_archivo(ARCHIVO_NORMAL);

	struct pokemon *primero = tp1_buscar_orden(tp1, 0);
	struct pokemon *segundo = tp1_buscar_orden(tp1, 1);

	pa2m_afirmar(primero != NULL &&
			     strcmp(primero->nombre, "Azumarill") == 0,
		     "El primer pokemon en orden alfabetico es Azumarill");

	pa2m_afirmar(segundo != NULL && strcmp(segundo->nombre, "Lapras") == 0,
		     "El segundo pokemon en orden alfabetico es Lapras");

	pa2m_afirmar(tp1_buscar_orden(tp1, tp1_cantidad(tp1)) == NULL,
		     "Buscar fuera del rango devuelve NULL");

	tp1_destruir(tp1);
}

void combinar_con_null_devuelve_null(void)
{
	tp1_t *tp1 = tp1_leer_archivo(ARCHIVO_A);

	pa2m_afirmar(tp1_combinar(NULL, NULL) == NULL,
		     "Combinar dos NULL devuelve NULL");

	pa2m_afirmar(tp1_combinar(tp1, NULL) == NULL,
		     "Combinar un TP1 con NULL devuelve NULL");

	tp1_destruir(tp1);
}

void combinar_vacio_con_normal(void)
{
	tp1_t *vacio = tp1_leer_archivo(ARCHIVO_VACIO);
	tp1_t *normal = tp1_leer_archivo(ARCHIVO_NORMAL);

	pa2m_afirmar(vacio != NULL, "Se pudo leer el TP1 vacio");

	pa2m_afirmar(normal != NULL, "Se pudo leer el TP1 normal");

	size_t cantidad_normal = tp1_cantidad(normal);

	tp1_t *combinado = tp1_combinar(vacio, normal);

	pa2m_afirmar(
		combinado != NULL,
		"Combinar un TP1 vacio con uno normal devuelve un TP1 valido");

	pa2m_afirmar(tp1_cantidad(combinado) == cantidad_normal,
		     "El combinado tiene la misma cantidad que el TP1 normal");

	struct pokemon *lapras = tp1_buscar_pokemon(combinado, "Lapras");

	pa2m_afirmar(lapras != NULL,
		     "El combinado conserva los pokemon del TP1 normal");

	tp1_destruir(vacio);
	tp1_destruir(normal);
	tp1_destruir(combinado);
}

void combinar_conserva_el_pokemon_del_primer_tp1(void)
{
	tp1_t *tp1_a = tp1_leer_archivo(ARCHIVO_A);
	tp1_t *tp1_b = tp1_leer_archivo(ARCHIVO_B);
	tp1_t *combinado = tp1_combinar(tp1_a, tp1_b);

	pa2m_afirmar(combinado != NULL,
		     "Combinar dos TP1 validos devuelve un TP1 nuevo");

	pa2m_afirmar(tp1_cantidad(combinado) == 3,
		     "La combinacion contiene tres pokemons unicos");

	struct pokemon *pikachu = tp1_buscar_pokemon(combinado, "pikachu");

	pa2m_afirmar(
		pikachu != NULL && pikachu->velocidad == 55,
		"Si hay un pokemon duplicado, se conserva el pokemon del primer TP");

	tp1_destruir(tp1_a);
	tp1_destruir(tp1_b);

	pikachu = tp1_buscar_pokemon(combinado, "Pikachu");

	pa2m_afirmar(
		pikachu != NULL,
		"El combinado sigue siendo valido despues de destruir los originales");

	tp1_destruir(combinado);
}

bool contar_todos_los_pokemons(struct pokemon *pokemon, void *extra)
{
	if (pokemon == NULL || extra == NULL) {
		return false;
	}

	size_t *iteraciones = extra;
	(*iteraciones)++;

	return true;
}

void iterar_recorre_todos_los_pokemons(void)
{
	tp1_t *tp1 = tp1_leer_archivo(ARCHIVO_NORMAL);
	size_t iteraciones = 0;

	size_t llamadas_funcion =
		tp1_iterar(tp1, contar_todos_los_pokemons, &iteraciones);

	pa2m_afirmar(llamadas_funcion == tp1_cantidad(tp1),
		     "Iterar recorrio todos los pokemons");

	pa2m_afirmar(iteraciones == tp1_cantidad(tp1),
		     "La funcion se invoca una vez por pokemon");

	tp1_destruir(tp1);
}

void destruir_null_devuelve_null(void)
{
	pa2m_afirmar(tp1_destruir(NULL) == NULL,
		     "Destruir TP1 NULL devuelve NULL");
}

int main(void)
{
	pa2m_nuevo_grupo("Pruebas de lectura de archivos");
	leer_null_devuelve_null();
	leer_archivo_inexistente_devuelve_null();
	leer_archivo_vacio_devuelve_tp1_vacio();
	leer_archivo_valido_carga_todos();
	leer_ignora_repetidos_sin_distinguir_mayusculas();
	leer_ignora_lineas_invalidas();

	pa2m_nuevo_grupo("Pruebas de parseo de campos");
	parsear_ignora_nombre_vacio();
	parsear_conserva_pokemons_validos();
	parsear_ignora_velocidades_invalidas();
	parsear_ignora_pesos_invalidos();
	parsear_ignora_rarezas_invalidas();
	parsear_ignora_cantidad_incorrecta_de_campos();

	pa2m_nuevo_grupo("Pruebas de cantidad y busqueda");
	cantidad_de_null_es_cero();
	cargar_cinco_pokemons_cuenta_cinco();
	cantidad_no_cuenta_pokemons_repetidos();
	buscar_nombre_existente_lo_encuentra_y_valida();
	buscar_nombre_inexistente_devuelve_null();
	buscar_por_orden_alfabetico();

	pa2m_nuevo_grupo("Pruebas combinacion");
	combinar_con_null_devuelve_null();
	combinar_conserva_el_pokemon_del_primer_tp1();

	pa2m_nuevo_grupo("Pruebas iteracion");
	iterar_recorre_todos_los_pokemons();

	pa2m_nuevo_grupo("Pruebas destruccion");
	destruir_null_devuelve_null();

	return pa2m_mostrar_reporte();
}