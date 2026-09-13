#include "comandos.h"
#include "tp1.h"

#include <string.h>

int main(int argc, char *argv[])
{
	if (argc < 3) {
		return 1;
	}

	const char *ruta = argv[1];
	const char *comando = argv[2];

	tp1_t *tp1 = tp1_leer_archivo(ruta);

	if (tp1 == NULL) {
		return 1;
	}

	int resultado = 1;

	if (strcmp(comando, "buscar-nombre") == 0 && argc == 4) {
		resultado = comando_buscar_nombre(tp1, argv[3]);
	} else if (strcmp(comando, "mostrar-uno") == 0 && argc == 3) {
		resultado = comando_mostrar_uno(tp1);
	} else if (strcmp(comando, "mostrar-todo") == 0 && argc == 3) {
		resultado = comando_mostrar_todo(tp1);
	} else if (strcmp(comando, "mostrar-rarezas") == 0 && argc == 3) {
		resultado = comando_mostrar_rarezas(tp1);
	}

	tp1_destruir(tp1);
	return resultado;
}