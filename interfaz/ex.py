# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'ejemplo_tarea2.ui'
#
# Created by: PyQt5 UI code generator 5.15.7
#
# WARNING: Any manual changes made to this file will be lost when pyuic5 is
# run again.  Do not edit this file unless you know what you are doing.


from PyQt5 import QtCore, QtGui, QtWidgets


class Ui_Dialog(object):
    def setupUi(self, Dialog):
        Dialog.setObjectName("Dialog")
        Dialog.resize(816, 733)
        self.tabWidget = QtWidgets.QTabWidget(Dialog)
        self.tabWidget.setGeometry(QtCore.QRect(20, 10, 781, 701))
        self.tabWidget.setObjectName("tabWidget")
        self.tab = QtWidgets.QWidget()
        self.tab.setObjectName("tab")
        self.label_27 = QtWidgets.QLabel(self.tab)
        self.label_27.setGeometry(QtCore.QRect(20, 30, 191, 21))
        self.label_27.setStyleSheet("color: rgb(0, 0, 0);")
        self.label_27.setObjectName("label_27")
        self.inicio_7 = QtWidgets.QPushButton(self.tab)
        self.inicio_7.setGeometry(QtCore.QRect(200, 20, 101, 41))
        self.inicio_7.setMinimumSize(QtCore.QSize(101, 41))
        self.inicio_7.setStyleSheet("color: rgb(213, 213, 213);\n"
"background-color: rgb(11, 0, 172);\n"
"")
        self.inicio_7.setObjectName("inicio_7")
        self.selec_7 = QtWidgets.QComboBox(self.tab)
        self.selec_7.setGeometry(QtCore.QRect(10, 70, 291, 31))
        self.selec_7.setStyleSheet("background-color: rgb(255, 255, 255);")
        self.selec_7.setObjectName("selec_7")
        self.selec_7.addItem("")
        self.selec_7.setItemText(0, "")
        self.selec2 = QtWidgets.QPushButton(self.tab)
        self.selec2.setGeometry(QtCore.QRect(310, 20, 101, 41))
        self.selec2.setMinimumSize(QtCore.QSize(101, 41))
        self.selec2.setMaximumSize(QtCore.QSize(16777215, 41))
        self.selec2.setStyleSheet("background-color: rgb(168, 168, 168);")
        self.selec2.setObjectName("selec2")
        self.label_29 = QtWidgets.QLabel(self.tab)
        self.label_29.setGeometry(QtCore.QRect(370, 120, 81, 16))
        self.label_29.setStyleSheet("color: rgb(0, 0, 0);")
        self.label_29.setObjectName("label_29")
        self.text_acc_sampling = QtWidgets.QTextEdit(self.tab)
        self.text_acc_sampling.setGeometry(QtCore.QRect(100, 150, 71, 31))
        self.text_acc_sampling.setStyleSheet("background-color: rgb(255, 255, 255);")
        self.text_acc_sampling.setObjectName("text_acc_sampling")
        self.label_30 = QtWidgets.QLabel(self.tab)
        self.label_30.setGeometry(QtCore.QRect(10, 160, 91, 16))
        self.label_30.setStyleSheet("color: rgb(0, 0, 0);")
        self.label_30.setObjectName("label_30")
        self.label_31 = QtWidgets.QLabel(self.tab)
        self.label_31.setGeometry(QtCore.QRect(10, 210, 91, 16))
        self.label_31.setStyleSheet("color: rgb(0, 0, 0);")
        self.label_31.setObjectName("label_31")
        self.text_acc_sensibity = QtWidgets.QTextEdit(self.tab)
        self.text_acc_sensibity.setGeometry(QtCore.QRect(100, 200, 71, 31))
        self.text_acc_sensibity.setStyleSheet("background-color: rgb(255, 255, 255);")
        self.text_acc_sensibity.setObjectName("text_acc_sensibity")
        self.gxmax_5 = QtWidgets.QTextEdit(self.tab)
        self.gxmax_5.setGeometry(QtCore.QRect(100, 250, 71, 31))
        self.gxmax_5.setStyleSheet("background-color: rgb(255, 255, 255);")
        self.gxmax_5.setObjectName("gxmax_5")
        self.label_39 = QtWidgets.QLabel(self.tab)
        self.label_39.setGeometry(QtCore.QRect(10, 260, 91, 16))
        self.label_39.setStyleSheet("color: rgb(0, 0, 0);")
        self.label_39.setObjectName("label_39")
        self.label_41 = QtWidgets.QLabel(self.tab)
        self.label_41.setGeometry(QtCore.QRect(200, 160, 111, 16))
        self.label_41.setStyleSheet("color: rgb(0, 0, 0);")
        self.label_41.setObjectName("label_41")
        self.gxmax_8 = QtWidgets.QTextEdit(self.tab)
        self.gxmax_8.setGeometry(QtCore.QRect(310, 150, 71, 31))
        self.gxmax_8.setStyleSheet("background-color: rgb(255, 255, 255);")
        self.gxmax_8.setObjectName("gxmax_8")
        self.label_42 = QtWidgets.QLabel(self.tab)
        self.label_42.setGeometry(QtCore.QRect(200, 210, 111, 16))
        self.label_42.setStyleSheet("color: rgb(0, 0, 0);")
        self.label_42.setObjectName("label_42")
        self.gxmax_9 = QtWidgets.QTextEdit(self.tab)
        self.gxmax_9.setGeometry(QtCore.QRect(310, 200, 71, 31))
        self.gxmax_9.setStyleSheet("background-color: rgb(255, 255, 255);")
        self.gxmax_9.setObjectName("gxmax_9")
        self.gxmax_13 = QtWidgets.QTextEdit(self.tab)
        self.gxmax_13.setGeometry(QtCore.QRect(310, 250, 71, 31))
        self.gxmax_13.setStyleSheet("background-color: rgb(255, 255, 255);")
        self.gxmax_13.setObjectName("gxmax_13")
        self.label_54 = QtWidgets.QLabel(self.tab)
        self.label_54.setGeometry(QtCore.QRect(240, 260, 61, 16))
        self.label_54.setStyleSheet("color: rgb(0, 0, 0);")
        self.label_54.setObjectName("label_54")
        self.label_55 = QtWidgets.QLabel(self.tab)
        self.label_55.setGeometry(QtCore.QRect(420, 160, 61, 20))
        self.label_55.setStyleSheet("color: rgb(0, 0, 0);")
        self.label_55.setObjectName("label_55")
        self.gxmax_14 = QtWidgets.QTextEdit(self.tab)
        self.gxmax_14.setGeometry(QtCore.QRect(500, 150, 71, 31))
        self.gxmax_14.setStyleSheet("background-color: rgb(255, 255, 255);")
        self.gxmax_14.setObjectName("gxmax_14")
        self.gxmax_22 = QtWidgets.QTextEdit(self.tab)
        self.gxmax_22.setGeometry(QtCore.QRect(480, 250, 281, 31))
        self.gxmax_22.setStyleSheet("background-color: rgb(255, 255, 255);")
        self.gxmax_22.setObjectName("gxmax_22")
        self.label_70 = QtWidgets.QLabel(self.tab)
        self.label_70.setGeometry(QtCore.QRect(400, 260, 81, 16))
        self.label_70.setStyleSheet("color: rgb(0, 0, 0);")
        self.label_70.setObjectName("label_70")
        self.gxmax_31 = QtWidgets.QTextEdit(self.tab)
        self.gxmax_31.setGeometry(QtCore.QRect(630, 150, 131, 31))
        self.gxmax_31.setStyleSheet("background-color: rgb(255, 255, 255);")
        self.gxmax_31.setObjectName("gxmax_31")
        self.label_86 = QtWidgets.QLabel(self.tab)
        self.label_86.setGeometry(QtCore.QRect(590, 160, 31, 16))
        self.label_86.setStyleSheet("color: rgb(0, 0, 0);")
        self.label_86.setObjectName("label_86")
        self.label_87 = QtWidgets.QLabel(self.tab)
        self.label_87.setGeometry(QtCore.QRect(590, 210, 31, 16))
        self.label_87.setStyleSheet("color: rgb(0, 0, 0);")
        self.label_87.setObjectName("label_87")
        self.gxmax_32 = QtWidgets.QTextEdit(self.tab)
        self.gxmax_32.setGeometry(QtCore.QRect(630, 200, 131, 31))
        self.gxmax_32.setStyleSheet("background-color: rgb(255, 255, 255);")
        self.gxmax_32.setObjectName("gxmax_32")
        self.inicio = QtWidgets.QPushButton(self.tab)
        self.inicio.setGeometry(QtCore.QRect(290, 410, 161, 41))
        self.inicio.setMinimumSize(QtCore.QSize(101, 41))
        self.inicio.setStyleSheet("color: rgb(213, 213, 213);\n"
"background-color: rgb(0, 115, 0);\n"
"")
        self.inicio.setObjectName("inicio")
        self.label_28 = QtWidgets.QLabel(self.tab)
        self.label_28.setGeometry(QtCore.QRect(440, 310, 121, 16))
        self.label_28.setStyleSheet("color: rgb(0, 0, 0);")
        self.label_28.setObjectName("label_28")
        self.selec_8 = QtWidgets.QComboBox(self.tab)
        self.selec_8.setGeometry(QtCore.QRect(370, 340, 261, 31))
        self.selec_8.setStyleSheet("background-color: rgb(255, 255, 255);")
        self.selec_8.setObjectName("selec_8")
        self.selec_8.addItem("")
        self.selec_8.addItem("")
        self.selec_8.addItem("")
        self.selec_8.addItem("")
        self.selec_8.addItem("")
        self.selec_8.addItem("")
        self.server_start_button = QtWidgets.QPushButton(self.tab)
        self.server_start_button.setGeometry(QtCore.QRect(260, 530, 101, 41))
        self.server_start_button.setMinimumSize(QtCore.QSize(101, 41))
        self.server_start_button.setStyleSheet("color: rgb(213, 213, 213);\n"
"background-color: rgb(0, 115, 0);\n"
"")
        self.server_start_button.setObjectName("server_start_button")
        self.server_stop = QtWidgets.QPushButton(self.tab)
        self.server_stop.setGeometry(QtCore.QRect(600, 530, 151, 41))
        self.server_stop.setMinimumSize(QtCore.QSize(101, 41))
        self.server_stop.setStyleSheet("color: rgb(213, 213, 213);\n"
"background-color: rgb(108, 5, 5);\n"
"")
        self.server_stop.setObjectName("server_stop")
        self.selec_6 = QtWidgets.QComboBox(self.tab)
        self.selec_6.setGeometry(QtCore.QRect(80, 340, 261, 31))
        self.selec_6.setStyleSheet("background-color: rgb(255, 255, 255);")
        self.selec_6.setObjectName("selec_6")
        self.selec_6.addItem("")
        self.selec_6.addItem("")
        self.selec_6.addItem("")
        self.selec_6.addItem("")
        self.selec_6.addItem("")
        self.selec_6.addItem("")
        self.selec_6.addItem("")
        self.label_26 = QtWidgets.QLabel(self.tab)
        self.label_26.setGeometry(QtCore.QRect(150, 310, 171, 16))
        self.label_26.setStyleSheet("color: rgb(0, 0, 0);")
        self.label_26.setObjectName("label_26")
        self.servers_active_stop = QtWidgets.QComboBox(self.tab)
        self.servers_active_stop.setGeometry(QtCore.QRect(380, 540, 201, 22))
        self.servers_active_stop.setObjectName("servers_active_stop")
        self.label_2 = QtWidgets.QLabel(self.tab)
        self.label_2.setGeometry(QtCore.QRect(40, 520, 47, 13))
        self.label_2.setObjectName("label_2")
        self.label_3 = QtWidgets.QLabel(self.tab)
        self.label_3.setGeometry(QtCore.QRect(180, 520, 47, 13))
        self.label_3.setObjectName("label_3")
        self.server_port = QtWidgets.QSpinBox(self.tab)
        self.server_port.setGeometry(QtCore.QRect(30, 540, 61, 22))
        self.server_port.setMaximum(100000)
        self.server_port.setProperty("value", 5000)
        self.server_port.setObjectName("server_port")
        self.server_status = QtWidgets.QComboBox(self.tab)
        self.server_status.setGeometry(QtCore.QRect(110, 540, 41, 22))
        self.server_status.setObjectName("server_status")
        self.server_status.addItem("")
        self.server_status.addItem("")
        self.server_status.addItem("")
        self.server_status.addItem("")
        self.server_status.addItem("")
        self.server_status.addItem("")
        self.server_status.addItem("")
        self.servers_active_update = QtWidgets.QComboBox(self.tab)
        self.servers_active_update.setGeometry(QtCore.QRect(30, 590, 211, 22))
        self.servers_active_update.setObjectName("servers_active_update")
        self.server_update_button = QtWidgets.QPushButton(self.tab)
        self.server_update_button.setGeometry(QtCore.QRect(260, 580, 101, 41))
        self.server_update_button.setMinimumSize(QtCore.QSize(101, 41))
        self.server_update_button.setStyleSheet("color: rgb(213, 213, 213);\n"
"background-color: rgb(0, 115, 0);\n"
"")
        self.server_update_button.setObjectName("server_update_button")
        self.server_protocol = QtWidgets.QComboBox(self.tab)
        self.server_protocol.setGeometry(QtCore.QRect(170, 540, 61, 22))
        self.server_protocol.setObjectName("server_protocol")
        self.server_protocol.addItem("")
        self.server_protocol.addItem("")
        self.server_protocol.addItem("")
        self.server_protocol.addItem("")
        self.server_protocol.addItem("")
        self.server_protocol.addItem("")
        self.label_7 = QtWidgets.QLabel(self.tab)
        self.label_7.setGeometry(QtCore.QRect(110, 520, 47, 13))
        self.label_7.setObjectName("label_7")
        self.tabWidget.addTab(self.tab, "")
        self.tab_2 = QtWidgets.QWidget()
        self.tab_2.setObjectName("tab_2")
        self.plot1 = GraphicsLayoutWidget(self.tab_2)
        self.plot1.setGeometry(QtCore.QRect(-10, 30, 501, 461))
        self.plot1.setStyleSheet("background-color: rgb(0, 0, 0);")
        self.plot1.setObjectName("plot1")
        self.inicio_5 = QtWidgets.QPushButton(self.tab_2)
        self.inicio_5.setGeometry(QtCore.QRect(570, 90, 101, 41))
        self.inicio_5.setMinimumSize(QtCore.QSize(101, 41))
        self.inicio_5.setStyleSheet("color: rgb(213, 213, 213);\n"
"background-color: rgb(0, 115, 0);\n"
"")
        self.inicio_5.setObjectName("inicio_5")
        self.label_23 = QtWidgets.QLabel(self.tab_2)
        self.label_23.setGeometry(QtCore.QRect(500, 60, 71, 20))
        self.label_23.setStyleSheet("color: rgb(0, 0, 0);")
        self.label_23.setObjectName("label_23")
        self.selec = QtWidgets.QComboBox(self.tab_2)
        self.selec.setGeometry(QtCore.QRect(560, 60, 131, 21))
        self.selec.setStyleSheet("background-color: rgb(255, 255, 255);")
        self.selec.setObjectName("selec")
        self.selec.addItem("")
        self.selec.addItem("")
        self.selec.addItem("")
        self.selec.addItem("")
        self.selec.addItem("")
        self.selec.addItem("")
        self.selec.addItem("")
        self.selec.addItem("")
        self.selec.addItem("")
        self.selec.addItem("")
        self.selec.addItem("")
        self.selec.addItem("")
        self.tabWidget.addTab(self.tab_2, "")

        self.retranslateUi(Dialog)
        self.tabWidget.setCurrentIndex(0)
        QtCore.QMetaObject.connectSlotsByName(Dialog)

    def retranslateUi(self, Dialog):
        _translate = QtCore.QCoreApplication.translate
        Dialog.setWindowTitle(_translate("Dialog", "Dialog"))
        self.label_27.setText(_translate("Dialog", "Buscar dispositivos Bluetooth"))
        self.inicio_7.setText(_translate("Dialog", "Buscar BLE"))
        self.selec2.setText(_translate("Dialog", "Selecionar"))
        self.label_29.setText(_translate("Dialog", "Parametros "))
        self.label_30.setText(_translate("Dialog", "Acc Sampling"))
        self.label_31.setText(_translate("Dialog", "Acc Sensibility"))
        self.label_39.setText(_translate("Dialog", "Gyro Sensibility"))
        self.label_41.setText(_translate("Dialog", "BME688 Sampling"))
        self.label_42.setText(_translate("Dialog", "Discontinuos time"))
        self.label_54.setText(_translate("Dialog", "Port TCP"))
        self.label_55.setText(_translate("Dialog", "Port UDP"))
        self.label_70.setText(_translate("Dialog", "Host_ip_addr"))
        self.label_86.setText(_translate("Dialog", "Ssid"))
        self.label_87.setText(_translate("Dialog", "Pass"))
        self.inicio.setText(_translate("Dialog", "Save Conf"))
        self.label_28.setText(_translate("Dialog", "Protocolo"))
        self.selec_8.setItemText(0, _translate("Dialog", "0"))
        self.selec_8.setItemText(1, _translate("Dialog", "1"))
        self.selec_8.setItemText(2, _translate("Dialog", "2"))
        self.selec_8.setItemText(3, _translate("Dialog", "3"))
        self.selec_8.setItemText(4, _translate("Dialog", "4"))
        self.selec_8.setItemText(5, _translate("Dialog", "5"))
        self.server_start_button.setText(_translate("Dialog", "Iniciar Server"))
        self.server_stop.setText(_translate("Dialog", "Detener Monitoreo"))
        self.selec_6.setItemText(0, _translate("Dialog", "Configuracion por Bluethooth"))
        self.selec_6.setItemText(1, _translate("Dialog", "Configuracion via TCP en BD"))
        self.selec_6.setItemText(2, _translate("Dialog", "Conexion TCP continua"))
        self.selec_6.setItemText(3, _translate("Dialog", "Conexion TCP discontinua"))
        self.selec_6.setItemText(4, _translate("Dialog", "Conexion UDP"))
        self.selec_6.setItemText(5, _translate("Dialog", "BLE continua"))
        self.selec_6.setItemText(6, _translate("Dialog", "BLE discontinua"))
        self.label_26.setText(_translate("Dialog", "Modo de operacion (status)"))
        self.label_2.setText(_translate("Dialog", "Port"))
        self.label_3.setText(_translate("Dialog", "Protocol"))
        self.server_status.setItemText(0, _translate("Dialog", "0"))
        self.server_status.setItemText(1, _translate("Dialog", "20"))
        self.server_status.setItemText(2, _translate("Dialog", "21"))
        self.server_status.setItemText(3, _translate("Dialog", "22"))
        self.server_status.setItemText(4, _translate("Dialog", "23"))
        self.server_status.setItemText(5, _translate("Dialog", "30"))
        self.server_status.setItemText(6, _translate("Dialog", "31"))
        self.server_update_button.setText(_translate("Dialog", "Updatear Server"))
        self.server_protocol.setItemText(0, _translate("Dialog", "0"))
        self.server_protocol.setItemText(1, _translate("Dialog", "1"))
        self.server_protocol.setItemText(2, _translate("Dialog", "2"))
        self.server_protocol.setItemText(3, _translate("Dialog", "3"))
        self.server_protocol.setItemText(4, _translate("Dialog", "4"))
        self.server_protocol.setItemText(5, _translate("Dialog", "5"))
        self.label_7.setText(_translate("Dialog", "Status"))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.tab), _translate("Dialog", "Configuracion"))
        self.inicio_5.setText(_translate("Dialog", "Graficar"))
        self.label_23.setText(_translate("Dialog", "Variable"))
        self.selec.setItemText(0, _translate("Dialog", "Temperatura"))
        self.selec.setItemText(1, _translate("Dialog", "Humedad"))
        self.selec.setItemText(2, _translate("Dialog", "Co"))
        self.selec.setItemText(3, _translate("Dialog", "Presion"))
        self.selec.setItemText(4, _translate("Dialog", "RMS"))
        self.selec.setItemText(5, _translate("Dialog", "Bateria"))
        self.selec.setItemText(6, _translate("Dialog", "Frecuencia X"))
        self.selec.setItemText(7, _translate("Dialog", "Frecuencia Y"))
        self.selec.setItemText(8, _translate("Dialog", "Frecuencia Z"))
        self.selec.setItemText(9, _translate("Dialog", "Amplitud X"))
        self.selec.setItemText(10, _translate("Dialog", "Amplitud Y"))
        self.selec.setItemText(11, _translate("Dialog", "Amplitud Z"))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.tab_2), _translate("Dialog", "Visualizacion"))
from pyqtgraph import GraphicsLayoutWidget
