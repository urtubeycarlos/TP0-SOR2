# TP0 - SOR2

## Introducción
En este trabajo se crea un modulo básico de ejemplo.

## Pasos 
* Ejecutar ```make clean & make``` para compilar el modulo
![](imagenes/1.png)

* Insertar el modulo con ```sudo insmod holaMundo.ko```
![](imagenes/2.png)

* Ver el log de kernel con el comando ```dmesg``` para ver que el modulo se inserto correctamente.
![](imagenes/3.png)

* Eliminar el modulo con ```sudo rmmod holaMundo.ko```
![](imagenes/4.png)

* Ver el log de kernel con el comando ```dmesg``` para ver que el modulo se elimino correctamente.
![](imagenes/5.png)
