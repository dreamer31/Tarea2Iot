# Explicación de la interfaz
Aqui habrá una explicación suficiente detallada de la interfaz.


## Modo de uso:
Si es que DB.sqlite no existe, ya sea porque es la priemra vez que se usa la interfaz, o porque se elimnó (los gráficos no se ven muy bien cuando los datos llegan en momentos muy distintos), hayq ue ejecutar sqlInit.py una vez para crear la base de datos.

La interfaz se lanza ejecutando mainWithSetup.py. Solo funciona en sistemas con linuz por tema de dependencias de Pygatt.

## Base de Datos: 

La base de datos usa SQLITE, con una tabla simple con las siguientes columnas:
    * MessageId: Llave primaria
    * MAC: MAC de la ESP32 que mandó un mensaje
    * Status: Status del mensaje recibido (0, 20, 21, 22, 23, 30, 31)
    * Protocol: Protocolo del mensaje recibido (0, 1, 2, 3, 4, 5)
    * Timestamp: Timestampo del momento en que se guardó el mensaje
    * Data: Datos del mensaje, guardados en un string con formato JSON

## Archivos:

* **config.py:** Se encarga de generar y empaquetar mensajes de configuración para configurar la ESP32
* **DataParse.py:** Se encarga de parsear y guardar la info recibida
* **DB.sqlite:** La BDD
* **ejemplo_tarea2.ui:** el arhivo para generar la UI de qtdesigner
* **ex.py:** la UI generada
* **findAddresses.py:** Se encarga de encontrar los dispositivos Bluetooth dipsonibles
* **graph.py:** Se encarga de sacar losd atos de la BDD y prepararlos apra ser graficados
* **mainWithSetup.py:** Junta todo para las funciones de la interfaz
* **rServer.py:** Se encarga de crear los servidores que reciben la info
* **sqlInit.py:** inicializa la BDD
