Patricio Raul   Vicente Hinojosa Godoy patricio.hinojosa@alumnos.uv.cl
Brandon  Elias  Diaz    Videla         brandon.diaz@alumnos.uv.cl 
Giovani  Danilo Lizana  Lagos          giovani.lizana@alumnos.uv.cl 

Instrucciones:
1)Para la compilacion del programa se debe primero realizar el make, para esto se debe ingresar en la carpeta desde la terminal usar el comando make.
2)Luego para la ejecucion se debe tener en cuenta la configuración en la carpeta bin "bin/config.json", con el siguiente formato:
	{
		"ip":	"IP DEL HOST SERVIDOR, PARA INGRESAR AL SERVIDOR",
		"puerto":	PUERTO EN EL CUAL ESTARA A LA ESCUCHA EL SERVIDOR,
		"root_dir":	"RUTA DEL DIRECTORIO DONDE SE ENCUENTRAN LAS PAGINAS HTML",
		"notFoundFile": "RUTA DEL DIRECTORIO DONDE SE ENCUENTRA LA PAGINA ERROR404.HTML"
	}

 un ejemplo seria así:
	{
		"ip":	"127.0.0.1",
		"puerto":	80,
		"root_dir":	"/home/patriciohg/Tarea-2-redes/WWW-data/",
		"notFoundFile": "/home/patriciohg/Tarea-2-redes/WWW-error/"
	}
3)Una vez hecho esto, ya se puede proceder a la ejecucion del programa, para esto desde el directorio principal en este caso "Tarea-2-redes", desde la terminal , ejecutamos el comando "./Server".

4)Ingresar a algun navegador y desde este en la parte de dirección ingresar a la ip puesta en la configuración y solicitar alguna pagina, ejemplo seria:
ingresar desde el navegador a la direccion: "127.0.0.1:80/index.html"
5)para finalizar el servidor presionar "Ctrl+c".


