#include "client.h"
#include <readline/readline.h>

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	logger = iniciar_logger();

	log_info(logger, "hola soy una consola");	//AL LOGGER CREADO PREVIAMENTE LE MANDO EL MENSAJE	

	config = iniciar_config();		//FUNCION CONFIG LA CREO E INICIO

	// CON CONFIG CREADO, LEEMOS PUERTO E IP
	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");

	// LOGGEAMOS EL VALOR DE CONFIG
	log_info(logger, "LEI LA IP %s Y PUERTO %s\n", ip, puerto);		//COPADO PRESTAR ATENCION COMO MUESTRO VALORES IP Y PUERTO

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/
	// Creamos una conexión hacia el servidor
	
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje("Hola servidor soy el cliente!", conexion);

	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger = log_create( "tp0.log", "logger.tp0", 1 , LOG_LEVEL_INFO);		//CREO EL LOGGER, el uno es porque la consola esta activa.
	return nuevo_logger;	//devuelvo el nuevo logger
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config = config_create("./cliente.config"); //VER VALOR A INGRESAR, EN ESTE CASO USE PATHCONG

	return nuevo_config;
}

void leer_consola(t_log* logger)		
{
	char* leido;
	leido = readline("> ");
	while (1){
		if (string_is_empty(leido)){
			break;
		}
		log_info(logger, leido);
		leido = readline("> ");
	}
	free(leido);
	
	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	// ¡No te olvides de liberar las lineas antes de regresar!

}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete = crear_paquete();

	// Leemos y esta vez agregamos las lineas al paquete
	while (1) {
			// Leer una línea
			leido = readline("> ");

			agregar_a_paquete(paquete, leido, strlen(leido) + 1); // Agregar la línea al paquete

			if (string_is_empty(leido)){	// Verificar si la línea es vacía
				free(leido);
				break;
			// Liberar la memoria asignada a la línea vacía y salir del bucle
			}
		}

	// Enviar el paquete
    enviar_paquete(paquete, conexion);

	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	eliminar_paquete(paquete);

}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	log_destroy(logger);		//LIBERO LOGGER
	config_destroy(config);		//LIBERO CONFIG
}