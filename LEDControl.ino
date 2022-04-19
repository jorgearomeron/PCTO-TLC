/*
Controllo LED
Questo esempio esegue la scansione delle periferiche Bluetooth® Low Energy fino a una con il servizio pubblicizzato
UUID trovato "19b10000-e8f2-537e-4f6c-d104768a1219". Una volta scoperto e connesso,
controllerà a distanza il LED della periferica Bluetooth® Low Energy, quando il pulsante viene premuto o rilasciato.
*/

#include <ArduinoBLE.h> // Includere la libreria per gestire il Bluetooth

// variabili per pulsante
const int buttonPin = 2; // I pulsanti o gli interruttori collegano due punti in un circuito quando vengono premuti
int oldButtonState = LOW;  // stato precedente del pulsante

/*

SETUP()
Usalo per inizializzare variabili, modalità pin, iniziare a usare le librerie, ecc. 
La funzione di configurazione verrà eseguita solo una volta, 
dopo ogni accensione o ripristino della scheda Arduino.

*/
void setup() {
  Serial.begin(9600); // inizializza la comunicazione seriale

  /*
  while:
  Un ciclo while si ripeterà continuamente, e all'infinito, finché l'espressione tra parentesi, () diventa falsa. 
  Qualcosa deve cambiare la variabile testata, altrimenti il ciclo while non uscirà mai. 
  Potrebbe essere nel codice, ad esempio una variabile incrementata, o una condizione esterna, come il test di un sensore. 
  Nel nostro caso la comunicazione seriale

  */

  while (!Serial);

 // configura il pin del pulsante come input
  pinMode(buttonPin, INPUT); //inizializza il pin del pulsante come input

  // inizializzare l'hardware Bluetooth® Low Energy
  BLE.begin();

  Serial.println("Bluetooth® Low Energy Central - LED control"); //stampa messaggio bluetooth su terminale seriale

  // avviare la scansione per le periferiche
  BLE.scanForUuid("19b10000-e8f2-537e-4f6c-d104768a1219");
}

/* 
loop()
controlla se è stata rilevata una periferica 
e invierà serial se viene premuto
*/ 

void loop() {

  BLEDevice peripheral = BLE.available(); //BLEDevice che rappresenta il dispositivo rilevato

  /* 
  L'istruzione if: 
  verifica una condizione ed esegue la seguente istruzione o insieme di istruzioni se la condizione è "vera".
  */

  if (peripheral) {
    //scoperto una periferica, stampare l'indirizzo, il nome locale e il servizio pubblicizzato
    Serial.print("Found ");
    Serial.print(peripheral.address());
    Serial.print(" '");
    Serial.print(peripheral.localName());
    Serial.print("' ");
    Serial.print(peripheral.advertisedServiceUuid());
    Serial.println();

    if (peripheral.localName() != "LED") {
      return;  // L'istruzione return: Terminare una funzione e restituire un valore da una funzione alla funzione chiamante, se lo si desidera.
    }

    // interrompere la scansione
    BLE.stopScan();

    controlLed(peripheral);

    // periferica disconnessa, riavviare la scansione
    BLE.scanForUuid("19b10000-e8f2-537e-4f6c-d104768a1219");
  }
}

/*

Void:
La parola chiave void viene utilizzata solo nelle dichiarazioni di funzione. 
Indica che la funzione non deve restituire alcuna informazione alla funzione da cui è stata chiamata.

*/

void controlLed(BLEDevice peripheral) {
  // connettersi alla periferica
  Serial.println("Connecting ..."); //stampa messaggio bluetooth su terminale seriale

  if (peripheral.connect()) {
    Serial.println("Connected"); //stampa messaggio bluetooth su terminale seriale
  } else { //L'istruzione else: consente un maggiore controllo sul flusso di codice rispetto all'istruzione if di base, consentendo il raggruppamento di più test
    Serial.println("Failed to connect!"); //stampa messaggio bluetooth su terminale seriale
    return;
  }

  // discover peripheral attributes
  Serial.println("Discovering attributes ..."); //stampa messaggio bluetooth su terminale seriale
  if (peripheral.discoverAttributes()) {
    Serial.println("Attributes discovered"); //stampa messaggio bluetooth su terminale seriale
  } else {
    Serial.println("Attribute discovery failed!"); //stampa messaggio bluetooth su terminale seriale
    peripheral.disconnect();
    return;
  }

  // retrieve the LED characteristic
  BLECharacteristic ledCharacteristic = peripheral.characteristic("19b10001-e8f2-537e-4f6c-d104768a1219");

  if (!ledCharacteristic) {
    Serial.println("Peripheral does not have LED characteristic!"); //stampa messaggio bluetooth su terminale seriale
    peripheral.disconnect();
    return;
  } else if (!ledCharacteristic.canWrite()) {
    Serial.println("Peripheral does not have a writable LED characteristic!"); //stampa messaggio bluetooth su terminale seriale
    peripheral.disconnect();
    return;
  }

   /*
  while:
  Un ciclo while si ripeterà continuamente, e all'infinito, finché l'espressione tra parentesi, () diventa falsa. 
  Qualcosa deve cambiare la variabile testata, altrimenti il ciclo while non uscirà mai. 
  Potrebbe essere nel codice, ad esempio una variabile incrementata, o una condizione esterna, come il test di un sensore. 
  Nel nostro caso La periferica Bluetooth

  */
  
  while (peripheral.connected()) {
    // while, la periferica è collegata

    // leggi il pin del pulsante
    int buttonState = digitalRead(buttonPin);

    if (oldButtonState != buttonState) {
      // pulsante cambiato
      oldButtonState = buttonState;

      if (buttonState) {
        Serial.println("button pressed"); //stampa messaggio bluetooth su terminale seriale

        // viene premuto il pulsante, scrivere 0x01 per accendere il LED
        ledCharacteristic.writeValue((byte)0x01);
      } else {
        Serial.println("button released");

        // il pulsante viene rilasciato, scrivere 0x00 per spegnere il LED
        ledCharacteristic.writeValue((byte)0x00);
      }
    }
  }

  Serial.println("Peripheral disconnected"); //stampa messaggio bluetooth su terminale seriale
}
