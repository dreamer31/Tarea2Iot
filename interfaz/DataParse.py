from struct import unpack, pack
import traceback
import json
import sqlite3 as sql

def dataSave(header, data):
    with sql.connect("DB.sqlite") as con:
        cur = con.cursor()
        cur.execute('''insert into Info (MAC, Status, Protocol, Data) values (?, ?, ?, ?)''', (header["MAC"], header["status"], header["protocol"], json.dumps(data)))
    
    
def response(change:bool=False, status:int=255, protocol:int=255):
    OK = 1
    CHANGE = 1 if change else 0
    return pack("<BBBB", OK, CHANGE, status, protocol)

def parseData(packet):
    header = packet[:10]
    data = packet[10:]
    header = headerDict(header)
    dataD = dataDict(header["protocol"], data)
    if dataD is not None:
        dataSave(header, dataD)
        
    return None if dataD is None else {**header, **dataD}

def protUnpack(protocol:int, data):
    protocol_unpack = ["<B", "<Bl", "<BlBfBf", "<BlBfBff", "<BlBfBff6f", "<BlBfBf2000h2000h2000h"]
    return unpack(protocol_unpack[protocol], data)

def headerDict(data):
    M1, M2, M3, M4, M5, M6, protocol, status, leng_msg = unpack("<6B2BH", data)
    MAC = ".".join([hex(x)[2:] for x in [M1, M2, M3, M4, M5, M6]])
    return {"MAC":MAC, "protocol":protocol, "status":status, "length":leng_msg}

def dataDict(protocol:int, data):
    if protocol not in [0, 1, 2, 3, 4, 5]:
        print("Error: protocol doesnt exist")
        return None
    def protFunc(protocol, keys):
        def p(data):
            unp = protUnpack(protocol, data)
            return {key:val for (key,val) in zip(keys, unp)}
        return p
    p0 = ["OK"]
    p1 = ["Batt_level", "Timestamp"]
    p2 = ["Batt_level", "Timestamp", "Temp", "Pres", "Hum", "Co"]
    p3 = ["Batt_level", "Timestamp", "Temp", "Pres", "Hum", "Co", "Rms"]
    p4 = ["Batt_level", "Timestamp", "Temp", "Pres", "Hum", "Co", "Rms", "Amp_X", "Frec_X", "Amp_Y", "Frec_Y", "Amp_Z", "Frec_Z"]
    p5 = ["Batt_level", "Timestamp", "Temp", "Pres", "Hum", "Co"] # , "Acc_X", "Acc_Y", "Acc_Z"
    p = [p0, p1, p2, p3, p4, p5]
    if protocol == 5:
        dt = protUnpack(protocol, data)
        fst = {key:val for (key,val) in zip(p[protocol], dt)}
        fst["Acc_X"] = dt[7:2006]
        fst["Acc_Y"] = dt[2007:4006]
        fst["Acc_Z"] = dt[4007:6006]
        
        return fst
    try:
        return protFunc(protocol, p[protocol])(data)
    except Exception:
        print("Data unpacking Error:", traceback.format_exc())
        return None