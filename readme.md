README
Este repositorio contiene el código fuente del proyecto AfroditaOS. A continuación, se detallan los pasos para configurar y ejecutar el programa correctamente en tu entorno local.

Requisitos
Asegúrate de tener instalados los siguientes requisitos previos:

QEMU
Make (opcional, pero recomendado)

Configuración
Abre una terminal y navega hasta el directorio raíz del proyecto.

Ejecuta el siguiente comando para configurar el entorno:

make setup

Este comando instalará las dependencias necesarias y configurará el entorno para ejecutar el programa.

Ejecución
Una vez que el entorno esté configurado, puedes ejecutar el programa de la siguiente manera:

make exp

Este comando iniciará la ejecución del programa y podrás interactuar con él según corresponda.

Solución de problemas
En caso de que encuentres problemas relacionados con las dependencias, puedes intentar solucionarlos ejecutando los siguientes comandos:

Para limpiar la configuración existente:

make cleansetup

Este comando eliminará las dependencias y configuraciones previas.

Luego, vuelve a ejecutar el comando de configuración:

make setup

Esto reinstalará las dependencias y configurará nuevamente el entorno.

Si aún encuentras problemas después de ejecutar estos comandos, asegúrate de revisar los mensajes de error y verifica que todos los requisitos previos estén instalados correctamente.

¡Listo! Ahora deberías poder ejecutar el programa sin problemas. Si tienes alguna pregunta o necesitas ayuda adicional, no dudes en contactarnos.