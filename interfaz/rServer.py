#imports
import ipaddress
import socket
from DataParse import parseData, response
import traceback
import threading
'''
Aqui están definidos los distintos servidores que se encargan de recibir datos y enviar configuraciones.
Cada cliente es un thread, para poder tener varios a la vez.

Funciones: 
-SERVER_thread: Facilita crear un servidor dados ciertos parámetros
-TCP_frag_recv/UDP_frag_recv: Función para recibir datos fragmentados (en trozos de 1000 bytes)
-updateServer: Función para avisarle a un srevidor que debe empezar a mandar instrucciones de update (cambiar status/protocol en la ESP32)
-start_server: función que se usa en la interfaz para crear un servidor, dados los parametros dados (status, protocol, puerto, etc)



Clases:
-TCP_config_server_thread: Servidor para enviar config por TCP (status 20)
-TCP_server_thread: Servidor para recibir datos por TCP (status 21, 22)
-UDP_server_thread: Servidor para recibir datos por UDP (status 23)

'''

def SERVER_thread(sock_type:str, PORT:int, frag:bool=False, config=None):
    TCP = sock_type == "TCP"
    UDP = sock_type == "UDP"
    TCP_config = sock_type =="TCP_config"
    HOST = "192.168.5.1"
    #str(ipaddress.ip_address(
    #         socket.gethostbyname(socket.gethostname())))
    
    if TCP_config:
        return TCP_config_server_thread(HOST, PORT, config)
    
    if TCP:
        return TCP_server_thread(HOST, PORT, frag)

    if UDP:
        return UDP_server_thread(HOST, PORT, frag)
    

class TCP_config_server_thread(threading.Thread):
    def __init__(self, HOST, PORT, config):
        threading.Thread.__init__(self)
        self.HOST = HOST
        self.PORT = PORT
        self.config = config.pack()
        self.updated = False
        self.resp = response(False, 0, 0)
        self.stopEv =threading.Event()
    
    def update_protocol(self, status:int, protocol:int):
        print("This type of server doesnt need updating")
    
    def stop(self):
        self.stopEv.set()
    
    def stopped(self):
        return self.stopEv.is_set()
    
    def run(self):
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.settimeout(5)
        s.bind((self.HOST, self.PORT))
        s.listen(5)
        print(f"Listening on {self.HOST}:{self.PORT}")
        while not self.stopped():
            try:
                conn, addr = s.accept()
                conn.settimeout(5)
            except TimeoutError:
                print("Waited too much, retrying")
                continue
            except Exception as e:
                print(e)
                continue
            print(f'Conecta3 ({addr[0]}) desde el puerto {addr[1]}')
            while not self.stopped():

                # print(f"Recibido {'|'.join('{:02x}'.format(x) for x in data)}")
                print("Sending configuration")
                try:
                    conn.send(self.config)
                    print("Configuration sent, waiting for OK")
                    try:
                        data = conn.recv(1024)
                        print(data)
                        dec = parseData(data)
                        print(dec)
                        if dec["OK"] == 1:
                            print("OK received")
                            self.stop()
                            break
                    except ConnectionResetError:
                        print("Lost connection: ConnectionResetError")
                        break
                    except TimeoutError:
                        print("Lost connection: TimeoutError")
                        break
                    except Exception as e:
                        print(e)
                        continue
                except Exception:
                    print(traceback.format_exc())
                    break
            


            conn.close()
            print('Desconecta3')
        s.close()
        
        
class TCP_server_thread(threading.Thread):
    def __init__(self, HOST, PORT, frag):
        threading.Thread.__init__(self)
        self.HOST = HOST
        self.PORT = PORT
        self.frag = frag
        self.updated = False
        self.resp = response(False, 0, 0)
        self.stopEv =threading.Event()
    
    def update_protocol(self, status:int, protocol:int):
        self.updated = True
        self.resp = response(True, status, protocol)
    def stop(self):
        # Instrucción de detenre el server. No es inmediata, pero apenas termine un loop se detiene
        self.stopEv.set()
    
    def stopped(self):
        return self.stopEv.is_set()
    

    
    def run(self):
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.settimeout(5)
        s.bind((self.HOST, self.PORT))
        s.listen(5)
        print(f"Listening on {self.HOST}:{self.PORT}")
        while not self.stopped():
            try:# intentamos conectarnos constantemente
                conn, addr = s.accept()
            except Exception as e:
                print(e)
                continue

            except TimeoutError:
                print("Waited too much, retrying")
                continue
            print(f'Conecta3 ({addr[0]}) desde el puerto {addr[1]}')
            while not self.stopped():
                try:#recibimos datos constantemente
                    print("Waiting for data...")
                    conn.settimeout(5)
                    if self.frag:# decidimos entre recibir los datos de manera simple o fragmentada
                        data = TCP_frag_recv(conn)
                    else:
                        data = conn.recv(1024)
                    if data == b'':
                        break
                except ConnectionResetError:
                    print("Lost connection: ConnectionResetError")
                    break
                except TimeoutError:
                    print("Lost connection: TimeoutError")
                    break
                
                # En csao de que no haya error, mandamos el OK de vuelta
                # Si al servidor se le avisó que debe mandar una instrucción de update, el OK tmbien contiene esa instrucción
                print("Sending Confirmation")
                conn.send(self.resp)

                if self.updated:
                    print("sent update message")
                    self.stop()# Una vez mandamos una instrucción de update con éxito, el server se detiene al finalizar el loop pues no necesariamente sirve para recibir la nueva info
                try:
                    info = parseData(data)# guardamos los datos en la BDD
                    print("Info:", info.keys())

                    
                except Exception:
                    print(traceback.format_exc())

            conn.close()
            print('Desconecta3')
        s.close()

class UDP_server_thread(threading.Thread):
    def __init__(self, HOST, PORT, frag):
        threading.Thread.__init__(self)
        self.HOST = HOST
        self.PORT = PORT
        self.frag = frag
        self._stop = threading.Event()
        self.updated = False
        self.resp = response(False, 0, 0)
    
    def update_protocol(self, status:int, protocol:int):
        self.updated = True
        self.resp = response(True, status, protocol)
    
    
    def stop(self):
        self._stop.set()
    
    def stopped(self):
        return self._stop.is_set()
    
    def run(self):
        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        s.settimeout(5)
        s.bind((self.HOST, self.PORT))
        print(f"Listening on {self.HOST}:{self.PORT}")
        while not self.stopped():
            try:#recibimos datos constantemente
                if self.frag:# decidimos entre recibir los datos de manera simple o fragmentada
                    data, addr = UDP_frag_recv(s)
                else:
                    data, addr = s.recvfrom(1024)
                if data == b'':
                    continue
            except TimeoutError:
                print("Lost connection: TimeoutError")
                continue
            except Exception as e:
                print(e)
                continue
            
            
            # enviamos OK y posiblemente una instrucción de update
            s.sendto(self.resp, addr)
            if self.updated:
                self.stop()# una vez enviada la instrucci;ón de update cerramos el server al llegar al fin del loop
            try:
                info = parseData(data)#guardamos los datos
                print("Info:", info.keys())
            except Exception:
                print(traceback.format_exc())
        s.close()
            

def TCP_frag_recv(conn):
    doc = b""
    while True:
        try:
            conn.settimeout(5)
            data = conn.recv(1024)
            if data == b'\0':
                break
            else:
                doc += data
        except TimeoutError:
            conn.send(b'\0')
            raise
        except Exception:
            conn.send(b'\0')
            raise
        conn.send(b'\1')
    return doc

def UDP_frag_recv(s):
    doc = b""
    addr = None
    while True:
        try:

            data, addr = s.recvfrom(1024)
            if data == b'\0':
                break
            else:
                doc += data
        except TimeoutError:
            raise
        except Exception:
            raise
        # s.sendto(b'\1', addr)
    return (doc, addr)

def updateServer(server, status:int, protocol:int):
    port = server.PORT
    server.update_protocol(status, protocol)
    print("Update signal, waiting for join...")
    server.join()
    return start_server(status, protocol, port)

def start_server(status:int, protocol:int, port:int, config=None):
    comp = {
            0: [0],
            20: [0],
            21: [1, 2, 3, 4, 5],
            22: [1, 2, 3, 4, 5],
            23: [1, 2, 3, 4, 5],
            30: [1, 2, 3, 4],
            31: [1, 2, 3, 4],
        }
    if status not in comp.keys():
        raise ValueError(f"Status {status} doesnt exist.")
    elif protocol < 0 or protocol > 5:
        raise ValueError(f"Protocol {protocol} doesnt exist.")
    elif protocol not in comp[status]:
        raise ValueError(
            f"Protocol {protocol} not compatible with status {status}.")
    frag = protocol == 5
    if status == 20:
        client_type = "TCP_config"
    elif status in [21, 22]:
        client_type = "TCP"
    elif status == 23:
        client_type == "UDP"
    thr = SERVER_thread(client_type, port, frag, config)
    thr.start()
    return thr
    
    
if __name__=="__main__":
    import time
    thr = start_server(21, 4, 5000)
    print("asdasdsad")
    try:
        for i in range(3):
            time.sleep(1)
            print("LOOP", i)
    finally:
        thr.stop()
        thr.join()