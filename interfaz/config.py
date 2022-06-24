import ipaddress
import socket
import struct

__configList = [0, 0, 1, 2, 3, 4, 5, 5000, 6000, str(ipaddress.ip_address(
            socket.gethostbyname(socket.gethostname()))), "iotWifi", "iotPass"]

WIFI_LEN = 10

class ESPconfig:

    def __init__(self, confDict):

        
        self.BMI270_Sampling = confDict["BMI270_Sampling"]
        self.BMI270_Acc_Sensibility = confDict["BMI270_Acc_Sensibility"]
        self.BMI270_Gyro_Sensibility = confDict["BMI270_Gyro_Sensibility"]
        self.BME688_Sampling = confDict["BME688_Sampling"]
        self.Discontinuous_Time = confDict["Discontinuous_Time"]
        
        self.protocol(confDict["Status"], confDict["Protocolo"])       
        self.ip(confDict["host_IP"], confDict["port_TCP"], confDict["port_UDP"])        
        self.wifi(confDict["ssid"], confDict["PASS"])


    def wifi(self, ssid: str, pswd: str)->None:
        if len(ssid) > WIFI_LEN:
            raise ValueError(f"SSID is too long, max {WIFI_LEN} characters.")
        if len(pswd) > WIFI_LEN:
            raise ValueError(f"pswd is too long, max {WIFI_LEN} characters.")
        self.Ssid = ssid
        self.Pass = pswd

    def ip(self, ip: str, tcp: int, udp: int)->None:
        self.Host_Ip_Addr = int(ipaddress.ip_address(ip))
        self.Port_TCP = tcp
        self.Port_UDP = udp

    def protocol(self, status: int, protocol: int)->None:
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
        self.status = status
        self.ID_Protocol = protocol

    def pack(self)->bytes:
        int8_t = "b"
        int32_t = "i"
        string_t = f"{WIFI_LEN+1}s"
        formatStr = "<"+int8_t*2+int32_t*7+"I"+string_t*2
        bits = struct.pack(formatStr,
                           int(self.status),
                           int(self.ID_Protocol),
                           int(self.BMI270_Sampling),
                           int(self.BMI270_Acc_Sensibility),
                           int(self.BMI270_Gyro_Sensibility),
                           int(self.BME688_Sampling),
                           int(self.Discontinuous_Time),
                           int(self.Port_TCP),
                           int(self.Port_UDP),
                           int(self.Host_Ip_Addr),
                           self.Ssid.encode("ASCII"),
                           self.Pass.encode("ASCII"))
        print(f"Packed Configuration in {len(bits)} bytes")
        return bits

def testConfig():
    return ESPconfig(__configList)


# if __name__=="__main__":

#     a = ESPconfig(__configList)
#     a.protocol(21, 4)
#     for i, bit in enumerate(a.pack()):
#         print(f"{i}:{bit}, ", end="")
