from socket import timeout
from numpy import character
from ex import Ui_Dialog
from PyQt5 import QtCore, QtGui, QtWidgets
from config import ESPconfig
from findAddresses import findAddresses, handle_data
import pygatt
import time
from rServer import start_server, updateServer
from graph import prep_data
import pyqtgraph as pg
import logging

#pyuic5 ejemplo_tarea2.ui -o ex.py


def serverDict(thread, port, status, protocol):
    return {"thread":thread, "port":port, "status":status, "protocol":protocol, "name":f"({status}, {protocol}) on port {port}"}

class GUIController:
    def __init__(self, parent):
        self.ui = Ui_Dialog()
        self.parent = parent
        self.macs = []
        self.UUIDs = []
        self.servers = []
        self.plot = None
        self.grph = None
        self.macindx = None

        self.adapter = pygatt.GATTToolBackend() ##pygatt
        print()
        

    def setSignals(self):
        self.ui.selec_6.currentIndexChanged.connect(self.leerModoOperacion)
        self.ui.inicio.clicked.connect(self.configSetup)
        self.ui.inicio_7.clicked.connect(self.actualizarMacs)
        self.ui.selec2.clicked.connect(self.conectarMac)
        self.ui.server_start_button.clicked.connect(self.start_server)
        self.ui.servers_active_update.activated.connect(self.server_update_show)
        self.ui.server_update_button.clicked.connect(self.server_update_values)
        self.ui.server_stop.clicked.connect(self.server_stop)
        self.ui.inicio_5.clicked.connect(self.graph)
    
    
    def graph(self):
        if self.plot is None:
            
            grph = pg.ScatterPlotItem()
            axis = pg.DateAxisItem()
            
            self.plot = self.ui.plot1.addPlot()
            self.plot.setAxisItems({'bottom':axis})
            self.plot.addItem(grph)
            self.grph = grph

        types = {
            "Temperatura":"Temp",
            "Humedad":"Hum",
            "Co":"Co",
            "Presion":"Pres",
            "RMS":"Rms",
            "Bateria":"Batt_level",
            "Amplitud X":"Amp_X",
            "Frecuencia X":"Frec_X",
            "Amplitud Y":"Amp_Y",
            "Frecuencia Y":"Frec_Y",
            "Amplitud Z":"Amp_Z",
            "Frecuencia Z":"Frec_Z",
        }
        graph_type = self.ui.selec.currentText()
        data_type = types[graph_type]
        (time, data) = prep_data(data_type)
        self.grph.setData(time, data)
        self.plot.show()
        
        
        
        
        
        
    def server_stop(self):
        sel = self.ui.servers_active_stop.currentText()
        ser = self.get_server(sel)
        thr = ser["thread"]
        thr.stop()
        thr.join()
        self.servers.remove(ser)
        self.update_server_dropdown()
        
    
    def terminate_all_servers(self):
        for server in self.servers:
            thr = server["thread"]
            thr.stop()
            thr.join()
    
    def get_server(self, name:str):
        s = list(filter(lambda server: server["name"] == name, self.servers))[0]
        return s
    
    def update_server_dropdown(self):
        names = [s["name"] for s in self.servers]
        self.ui.servers_active_update.clear()
        self.ui.servers_active_update.addItems(names)
        self.ui.servers_active_stop.clear()
        self.ui.servers_active_stop.addItems(names)
    
    def start_server(self):
        port = self.ui.server_port.value()
        for server in self.servers:
            if server["port"] == port:
                print("Port already in use")
                return
        status = int(self.ui.server_status.currentText())
        protocol = int(self.ui.server_protocol.currentText())
        print(status)
        config = None if status!=20 else self.getConfigParams()

        thr = start_server(status, protocol, port, config)
        server = serverDict(thr, port, status, protocol)
        self.servers.append(server)
        self.update_server_dropdown()

    def server_update_show(self):
        sel = self.ui.servers_active_update.currentText()
        ser = self.get_server(sel)
        self.ui.server_port.setValue(ser["port"])
        status = str(ser["status"])
        protocol = str(ser["protocol"])
        self.ui.server_status.setCurrentIndex(self.ui.server_status.findText(status))
        self.ui.server_protocol.setCurrentIndex(self.ui.server_protocol.findText(protocol))
    
    def server_update_values(self):
        status = int(self.ui.server_status.currentText())
        protocol = int(self.ui.server_protocol.currentText())
        sel = self.ui.servers_active_update.currentText()
        ser = self.get_server(sel)
        thr = ser["thread"]
        port = ser["port"]
        newThr = updateServer(thr, status, protocol)

        self.servers.remove(ser)
        
        newSer = serverDict(newThr, port, status, protocol)
        self.servers.append(newSer)
        self.update_server_dropdown()
        
        
        
        

    def leerConfiguracion(self):
        conf = dict()
        conf['AccSamp'] = self.ui.text_acc_sampling.toPlainText()
        conf['AccSen'] = self.ui.text_acc_sensibity.toPlainText()
        print (conf)
        return conf

    def leerModoOperacion(self):
        index = self.ui.selec_6.currentIndex()
        texto = self.ui.selec_6.itemText(index)
        print(texto)
        return texto

    def actualizarMacs(self):
        adrs = findAddresses()
        self.macs = adrs[1]
        self.UUIDs = adrs[2]
        self.ui.selec_7.clear()
        self.ui.selec_7.addItems(adrs[0])
        #print()

    def conectarMac(self):
        indx = self.ui.selec_7.currentIndex()
        self.macindx = indx
        ##pygatt
        logging.basicConfig()
        logging.getLogger('pygatt').setLevel(logging.DEBUG)
        qty = 0
        while(qty<100):
            try:
                self.adapter.start()
                device = self.adapter.connect(self.macs[indx],timeout=2.0)
                print('Se conecto!')
                characteristics = device.discover_characteristics()
                for i in characteristics.keys():
                    print('Caracteristicas: '+str(i))#list(characteristics.keys())))
                time.sleep(1)
                qty = 100
            except pygatt.exceptions.NotConnectedError:
                qty += 1
                print("Se han fallado: {qty} intentos" )
                print("Not connected")
                time.sleep(1)
            finally:
                self.adapter.stop()
        print("Termino de test de conexión")

    def criticalError(self):
        popup = QtWidgets.QMessageBox(parent= self.parent)
        popup.setWindowTitle('ERROR MASIVO')
        popup.setText('QUE HAS APRETADO, NOS HAS CONDENADO A TODOS')
        popup.setIcon(QtWidgets.QMessageBox.Icon.Critical)
        popup.exec()
        return
        
    def getConfigParams(self):
        def modopToStatus(modop:str):
            d = {
                "Configuracion por Bluethooth":0,
                "Configuracion via TCP en BD":20,
                "Conexion TCP continua":21,
                "Conexion TCP discontinua":22,
                "Conexion UDP":23,
                "BLE continua":30,
                "BLE discontinua":31
            }
            return d[modop]
        configDict={}
        
        index = self.ui.selec_6.currentIndex()
        configDict["Status"]=modopToStatus(self.ui.selec_6.itemText(index))

        index = self.ui.selec_8.currentIndex()
        configDict["Protocolo"] =int( self.ui.selec_8.itemText(index))
        
        configDict["BMI270_Sampling"] = int(self.ui.text_acc_sampling.toPlainText())
        
        configDict["BMI270_Acc_Sensibility"] = int(self.ui.text_acc_sensibity.toPlainText())
        
        configDict["BMI270_Gyro_Sensibility"] = int(self.ui.gxmax_5.toPlainText())
        
        configDict["BME688_Sampling"] = int(self.ui.gxmax_8.toPlainText())
        
        configDict["Discontinuous_Time"] = int(self.ui.gxmax_9.toPlainText())
        
        configDict["port_TCP"] = int(self.ui.gxmax_13.toPlainText())
        
        configDict["port_UDP"] = int(self.ui.gxmax_14.toPlainText())
        
        configDict["host_IP"] = self.ui.gxmax_22.toPlainText()

        
        configDict["ssid"] = self.ui.gxmax_31.toPlainText()
        
        configDict["PASS"] = self.ui.gxmax_32.toPlainText()
        
        ESPconf = ESPconfig(configDict)
        return ESPconf
    
    def configSetup(self):
        ESPconf = self.getConfigParams()
        pack = ESPconf.pack()
        print("El largo del paquete es:" + str(len(pack)))
        qty=0
        while qty<100:
            try:
                self.adapter.start()
                device = self.adapter.connect(self.macs[self.macindx], timeout=2.0)
                device.exchange_mtu(80)
                print('Se conecto!')
                characteristics = device.discover_characteristics().keys()
                device.char_write(list(characteristics)[4], pack)
                print("Se escribio el paquete")
                qty = 100
            except pygatt.exceptions.NotConnectedError:
                qty += 1
                print("Se han fallado: {qty} intentos" )
                print("Not connected")
                time.sleep(1)
            finally:
                self.adapter.stop()

    def stop(self):
        print('Mori')
        self.terminate_all_servers()
        return

if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    Dialog = QtWidgets.QDialog()
    cont = GUIController(Dialog)
    ui = cont.ui
    ui.setupUi(Dialog)
    Dialog.show()
    cont.setSignals()
    # on close, ui should run ui.terminate_all_servers()
    sys.exit(app.exec_())




    # // char* Ssid;
    # memcpy(c.Ssid, &buffer[34], 11);
    # // char* Pass;
    # memcpy(c.Pass, &buffer[45], 11);
    # return c;