/******************************************************************************
  Project  : Arduino 3G module UC20(ETEE058)
  Compiler : Arduino 1.6.11
  Board    : Arduino UNO
  Device   : 3G Module (ETEE058)
  Author   : Chiang Mai Maker Club

  UC20      --->    Arduino
  PWRKEY    --->    D4
  GND       --->    GND PWR
  VIN       --->    5 - 12 Vin
  REF       --->    IOREF
  RxD       --->    D9
  TxD       --->    D8
  GND       --->    GND
*******************************************************************************/
#include "TEE_UC20.h"
#include "SoftwareSerial.h"
#include <AltSoftSerial.h>
#include "call.h"
#include "sms.h"
#include "internet.h"
#include "File.h"
#include "http.h"

INTERNET net;
UC_FILE file;
HTTP http;

//SIM AIS  internet
#define APN "internet"
#define USER ""
#define PASS ""

//SoftwareSerial mySerial(8, 9); // RX, TX
AltSoftSerial mySerial;

void debug(String data) {
  Serial.println(data);
}
void data_out(char data)  {
  Serial.write(data);
}
void setup()  {
  Serial.begin(9600);
  gsm.begin(&mySerial, 9600);
  gsm.Event_debug = debug;
  Serial.println(F("UC20"));
  gsm.PowerOn();
  while (gsm.WaitReady()) {}

  Serial.print(F("GetOperator --> "));
  Serial.println(gsm.GetOperator());
  Serial.print(F("SignalQuality --> "));
  Serial.println(gsm.SignalQuality());

  Serial.println(F("Disconnect net"));
  net.DisConnect();
  Serial.println(F("Set APN and Password"));
  net.Configure(APN, USER, PASS);
  Serial.println(F("Connect net"));
  net.Connect();
  Serial.println(F("Show My IP"));
  Serial.println(net.GetIP());
  Serial.println(F("Start HTTP"));
  http.begin(1);
  Serial.println(F("Send HTTP GET"));
//  http.url("http://api.thingspeak.com/update?api_key=5T4WXGZFE1PZPS2K&field1=55");
  http.url("https://api.netpie.io/topic/SmartTrash/trash?auth=XQ6k3d23JGK15WG:KLOQM5CXr5xj1NPYzvkXnn0uU&data=1");
  Serial.println(http.get());
  Serial.println(F("Clear data in RAM"));
  file.Delete(RAM, "*");
  Serial.println(F("Save HTTP Response To RAM"));
  http.SaveResponseToMemory(RAM, "web.hml");
  Serial.println(F("Read data in RAM"));
  read_file(RAM, "web.hml");
  Serial.println(F("Disconnect net"));
  net.DisConnect();
  Serial.println("PWR OFF");
  gsm.PowerOff();
}

void read_file(String pattern, String file_name)  {
  file.DataOutput =  data_out;
  file.ReadFile(pattern, file_name);
}
void loop() {

  if (gsm.available())
  {
    Serial.write(gsm.read());
  }
  if (Serial.available())
  {
    char c = Serial.read();
    gsm.write(c);
  }
}
