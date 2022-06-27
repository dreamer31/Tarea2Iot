# Descripción de Servidor BT + Wifi

### Features que funcionan:

* Status 0, 20, 21, 22, 23
* Generar los prtocolos (con sus datos) 0 a 5
* Poder conectarse mediante BLE y configurar las opciones de base luego de flashear
* Recibir un cambio de status/protocolo mientras está enviando info y cambiar de modo en respuesta


### Iniciar el ESP32:

Para que el ESP32 funcione hay que activar bluetooth y el flasheo con espacio grande en las configuraciones del chip, dado que por las librerias usadas la configuración de espacio default se queda chica, hay que dejarlo con el mismo tipo pero '(large)'.

Al iniciar activa el servidor de BLE esperando un contacto y envio de la configuración empaquetada, puede recibir cualquier clase de datos por la cuarta caracteristica que tiene propiedades de READ, WRITE y NOTIFICATION, se espera que se utilice un WRITE sobre esta misma caracteristica, aunque primeramente se debe ajustar el tamaño máximo de envio para poder enviar una configuración completa (56 bytes). Tras recibir una configuración valida, se reconfigura para entrar en el modo de esta (notese cuales status y protocolos si están implementados en la sección de arriba).

La primera vez que se flashea, el macro RESET_FLAG debe cambiarse amnualmente a 1, y luego debe volver a flashearse con el macro cambiado a 0. Esto es para resettear la flag que indica que ya se hizo la configuración inicial por BLE.

### Archivos:

* **MAIN.c** contiene el main, junto con lo necesario apra resetear la flag de inicio.
* **include/BLE_server.c** contiene el servidor para recibir config por BLE. Es el mismo archivo que se puede encontrar en la sección de BLE_server en la página de ESP-IDF, con cambios para acomodar nuestras necesidades.
* **include/config.h** tiene el struct para maniuplar configuraciones
* **include/configurer.c** tiene funciones para facilitar la manipulación, guardado y desempaque de configuraciones. Tambien tiene funciones para setear la flag de inicio
* **include/connect.c** tiene la función para conectarse a wifi
* **include/nvs.c** tiene funciones para guardar y leer cosas en NVS
* **include/protocols.c** tiene funciones para generar los datos que se enviarán
* **include/sockets.c** tiene los distintos clientes y funciones apra mandar los datos por wifi