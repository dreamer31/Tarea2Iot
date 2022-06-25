
import pyqtgraph as pg
import numpy as np
import sqlite3 as sql
import json
from datetime import datetime
from pyqtgraph.Qt import QtCore, QtGui

'''
Funciones para rescatar los datos de la BDD y prepararlos para ser graficados

-get_info_filter: Fetchea todas las filas de la BDD, pudiendo opcionalmente filtrar por protocolo, status o MAC
-decodeData: Dado una lista de filas de la BDD, las convierte en diccionario para mas facil acceso a las columnas por nombre
-filter_by_data_type: Dado un tipo de dato que se quiere graficar, entrega todas las filas que tengan ese tipo entre sus datos.
-separe_data: Entrega una lista de timestamps y una lista de valores dado un nombre de tipo de dato y una lista filtrada de filas en forma de diccionario
-prep_data: entrega la tupla de listas (tiempo, valor) necesaria para graficar un tipo de dato dado

La única función que se importa fuera de este archivo es prep_data, que entrega todo listo para meter en un graph.
'''



conn = sql.connect("DB.sqlite")
c = conn.cursor()

inst = '''
SELECT MAC, Status, Protocol, Timestamp, Data 
FROM Info 
WHERE (MAC = :mac OR :mac = '') AND (Status = :status OR :status = '') AND (Protocol = :protocol OR :protocol = '')
ORDER BY Timestamp'''


def get_info_filter(protocol='', status='', mac=''):
    with sql.connect("DB.sqlite") as conn:
        c = conn.cursor()
        return c.execute(inst, {'mac':mac, 'status':status, 'protocol':protocol}).fetchall()

def decodeData(infoList):
    info = []
    for row in infoList:
        mac = row[0]
        status = row[1]
        protocol = row[2]
        timestamp = datetime.strptime(row[3], '%Y-%m-%d %H:%M:%S')
        data = json.loads(row[4])
        info.append({'Mac':mac, 'Status':status, 'Protocol':protocol, 'Timestamp':timestamp, 'Data':data})
    return info

def filter_by_data_type(data_type:str):
    types = {# diccionario con los protocolos que tiene cada tipo de dato
        "Temp":[2, 3, 4, 5],
        "Hum":[2, 3, 4, 5],
        "Pres":[2, 3, 4, 5],
        "Co":[2, 3, 4, 5],
        "Batt_level":[1, 2, 3, 4, 5],
        "Rms":[3, 4],
        "Amp_X":[4],
        "Frec_X":[4],
        "Amp_Y":[4],
        "Frec_Y":[4],
        "Amp_Z":[4],
        "Frec_Z":[4],
    }
    data = []
    if data_type in types.keys():
        for prot in types[data_type]:
            data += get_info_filter(protocol=prot)
    else:
        print("Not a valid data_type")
    return data


def separe_data(data_type:str, data):
    time_prepped = []
    data_prepped = []
    
    for row in data:
        time_prepped.append(row["Timestamp"])
        data_prepped.append(row["Data"][data_type])
    return (time_prepped, data_prepped)

def prep_data(data_type:str):
    data = decodeData(filter_by_data_type(data_type))
    (time, temp) = separe_data(data_type, data)
    return ([x.timestamp() for x in time], temp)




if __name__ == '__main__':
    import sys
    (time, temp) = prep_data("Amp_X")



    app = QtGui.QApplication([])

    axis = pg.DateAxisItem()
    pt = pg.PlotWidget(axisItems = {'bottom': axis})
    pt.plot(time, temp)
    pt.show()
    if (sys.flags.interactive != 1) or not hasattr(QtCore, 'PYQT_VERSION'):
        QtGui.QApplication.instance().exec_()