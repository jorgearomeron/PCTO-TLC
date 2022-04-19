/*
Controllo LED
Questo esempio esegue la scansione delle periferiche Bluetooth® Low Energy fino a una con il servizio pubblicizzato
UUID trovato "19b10000-e8f2-537e-4f6c-d104768a1219". Una volta scoperto e connesso,
controllerà a distanza il LED della periferica Bluetooth® Low Energy, quando il pulsante viene premuto o rilasciato.
*/

#include <ArduinoBLE.h> // Includere la libreria per gestire il Bluetooth

BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214"); // Servizio LED Bluetooth® a basso consumo energetico

// Caratteristica interruttore LED Bluetooth® Low Energy - UUID a 128 bit personalizzato, leggibile e scrivibile dalla centrale
BLEByteCharacteristic switchCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1219", BLERead | BLEWrite);

const int ledPin = LED_BUILTIN; // pin da utilizzare per il LED

/*

SETUP()
Usalo per inizializzare variabili, modalità pin, iniziare a usare le librerie, ecc. 
La funzione di configurazione verrà eseguita solo una volta, 
dopo ogni accensione o ripristino della scheda Arduino.

*/

void setup() {
  Serial.begin(9600); // inizializza la 

   /*
  while:
  Un ciclo while si ripeterà continuamente, e all'infinito, finché l'espressione tra parentesi, () diventa falsa. 
  Qualcosa deve cambiare la variabile testata, altrimenti il ciclo while non uscirà mai. 
  Potrebbe essere nel codice, ad esempio una variabile incrementata, o una condizione esterna, come il test di un sensore. 
  Nel nostro caso la comunicazione seriale

  */

  while (!Serial);

  //inizializza il pin del pulsante come output
  pinMode(ledPin, OUTPUT);

 // inizializzare l'hardware Bluetooth® Low Energy
  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy module failed!"); //stampa messaggio bluetooth su terminale seriale
 
    while (1); //while: Un ciclo while si ripeterà continuamente, e all'infinito, finché l'espressione tra parentesi, () diventa falsa. 
  }

  // imposta il nome locale pubblicizzato e l'UUID del servizio:
  BLE.setLocalName("LED");
  BLE.setAdvertisedService(ledService);

  // aggiungere la caratteristica al servizio
  ledService.addCharacteristic(switchCharacteristic);

  // aggiungere il servizio
  BLE.addService(ledService);

  // impostare il valore iniziale per la caratteristica:
  switchCharacteristic.writeValue(0);

  // inizio annuncio
  BLE.advertise();

  Serial.println("BLE LED Peripheral"); //stampa messaggio bluetooth su terminale seriale
}

/*
  loop():
  Dopo aver creato una funzione setup(), che inizializza e imposta i valori iniziali, 
  la funzione loop() fa esattamente ciò che suggerisce il suo nome e si ripete in loop, 
  consentendo al programma di cambiare e rispondere. 
  Usalo per controllare attivamente la scheda Arduino.
*/

void loop() {
  // ascolta le periferiche Bluetooth® Low Energy per la connessione:
  BLEDevice central = BLE.central();

  /* 
  L'istruzione if: 
  verifica una condizione ed esegue la seguente istruzione o insieme di istruzioni se la condizione è "vera".
  */

  // if una centrale è collegata a una periferica
  if (central) {
    Serial.print("Connected to central: ");
    // stampa l'indirizzo MAC della centrale:
    Serial.println(central.address());

    /*

    while:
    Un ciclo while si ripeterà continuamente, e all'infinito, finché l'espressione tra parentesi, () diventa falsa. 
    Qualcosa deve cambiare la variabile testata, altrimenti il ciclo while non uscirà mai. 
    Potrebbe essere nel codice, ad esempio una variabile incrementata, o una condizione esterna, come il test di un sensore. 
    Nel nostro caso La periferica Bluetooth

   */
       
    while (central.connected()) {  // while la centrale è ancora collegata alla periferica:
      // if il dispositivo remoto ha scritto alla caratteristica,
      // utilizzare il valore per controllare il LED:
      if (switchCharacteristic.written()) {
        if (switchCharacteristic.value()) {   //qualsiasi valore diverso da 0
          Serial.println("LED on");
          digitalWrite(ledPin, HIGH);         // accenderà il LED
        } else {                              // un valore 0
          Serial.println(F("LED off"));
          digitalWrite(ledPin, LOW);          //spegnerà il LED
        }
      }
    }

    // quando la centrale si disconnette, stampalo:
    Serial.print(F("Disconnected from central: ")); //stampa messaggio bluetooth su terminale seriale
    Serial.println(central.address()); //stampa messaggio bluetooth su terminale seriale
  }
}