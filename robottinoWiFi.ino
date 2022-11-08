#include <SoftwareSerial.h> //include la classe “SoftwareSerial.h”; la classe è un insieme di attrubuti e metodi.
#define rxPin 10 //definisce che il pin 10 della scheda arduino si chiama “rxPn”, che fungerà da ricevitore. 
#define txPin 8  //definisce che il pin 8 della scheda arduino si chiama “txPn”, che fungerà da trasmettitore.
// Set up a new SoftwareSerial object 
SoftwareSerial serialModuloWIFI =  SoftwareSerial(rxPin, txPin);  //crea un nuovo oggetto di nome “mySerial” con la funzione costruttore; l’oggetto ha come parametri i due pin. 

String check4answer(){
    String str = ""; //crea una stringa vuota
    //available è usato per verificare la presenza di dati trasmessi dal moduloWIFI per l'Arduino; 
    while (serialModuloWIFI.available() > 0){ //crea un ciclo while che controllerà i valori del modulo wifi finchè rispettano la condizione;
        char c=serialModuloWIFI.read(); //legge i valori inviati dal modulo e li salva in una funzione;
        Serial.print(c); //stampa i valori del moduloWIFI;
        str += String(c); //aggiunge alla stringa i valori del moduloWIFI; 
    }
    return str; //fa il return della stringa;
}
//invia i comandi al modulo WIFI; riceve i comandi da inviare; aspetta e resrituisce la risposta del modulo WIFI.
String esp01cmd(String cmd){ //crea una funzione che ha come parametri delle stringhe 
  Serial.println("sending: " + cmd); //stampa sul monitor seriale il comando inviato sul monitor serial;
  serialModuloWIFI.println(cmd); //invia il comando al modulo WIFI;
  delay(10); //aspetta 10 millisecondi;
  String risposta = check4answer();} //fa un return della funzione “check4answer()”;
  return risposta; 
void setup()  {
    // Define pin modes for TX and RX
    pinMode(rxPin, INPUT); //definisce il pin come input;
    pinMode(txPin, OUTPUT); //definisce il pin come  output;
    
    // Set the baud rate for the SoftwareSerial object
    //comandi AT configura il modulo WIFI
    serialModuloWIFI.begin(115200); //velocità di trasmissione per l’oggetto da noi creato;
    Serial.begin(9600); //velocità di trasmissione dei dati;
    delay(1000); //aspetta 1 secondo;
    esp01cmd("AT"); //testa la connessione della UART, controlla che sia funzionante
    delay(1000); //aspetta 1 secondo;
    esp01cmd("AT+CWMODE=2"); //setta il modulo WIFI come server wifi, ovevero access point; 
    delay(1000); //aspetta 1 secondo;
    esp01cmd("AT+CWSAP=\"robottino\",\"robottino\",1,4"); //imposta ssid (nome) e la password del wifi
    delay(1000); //aspetta 1 secondo;
    esp01cmd("AT+CIFSR"); //show Access Point IP address
    esp01cmd("AT+CIPMUX=1"); //allow up to 1 connections at the time
    
    
    Serial.println("ESP-01 Configuration Completed"); //dice che la configurazione è completa;
}

void loop() {
    Serial.println("loop...");
    //AT+CWLIF restituisce l'indirizzo IP del client (telefono) connesso al modulo wifi 
    while(esp01cmd("AT+CWLIF").substring(11,18) != "192.168"){//controlla se qualcuno si è collegato
      Serial.println("no connections so far... still waiting"); //dice che nessuno è connesso, sta ancora aspettando;
      delay(1000); //aspetta 1 secondo;
    }// finchè non trova un'indirizzo IP continuerà ad aspettare

    Serial.println("Connection from remote device was Established!!!"); //dice che ha stabilito una connessione;
    //Socket ID: 3
    //accept packets from any IP address/devices
    //Listen to local port 4567
    //outgoing packets could go to any remote host without restrictions...
    esp01cmd("AT+CIPSTART=3,\"UDP\",\"0.0.0.0\",0,4567,2"); //starting UDP Socket Server 
    
//    String str = ;    
//    Serial.println("received: "+esp01cmd("AT+CWLIF").substring(11,18));
    delay(3000); //aspetta 3 secondi;
    while(true) {
      String str = serialModuloWIFI.readString();
      if(str != "") {
        int startOfSTR = str.indexOf(":",10)+1;
        Serial.println("Received: "+str.substring(startOfSTR));
        //Serial.println(startOfSTR);
      }
    }

}


