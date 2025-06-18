#include <dummy.h>

#include <SoftwareSerial.h>

// RX = D10 (reçoit du capteur MH-Z16), TX = D11 (non utilisé ici)
SoftwareSerial mhzSerial(10, 11);

void setup() {
  Serial.begin(9600);        // Moniteur série vers PC
  mhzSerial.begin(9600);     // UART MH-Z16
  delay(2000);               // Laisse le capteur démarrer

  Serial.println("Test MH-Z16 démarré...");
}

void loop() {
  // Envoi de la commande de lecture CO2
  uint8_t cmd[9] = { 0xFF, 0x01, 0x86,
                     0x00, 0x00, 0x00,
                     0x00, 0x00, 0x79 };
  mhzSerial.write(cmd, 9);
  delay(50); // Attendre la réponse

  if (mhzSerial.available() >= 9) {
    uint8_t response[9];
    mhzSerial.readBytes(response, 9);

    if (response[0] == 0xFF && response[1] == 0x86) {
      int co2ppm = response[2] * 256 + response[3];
      Serial.print("[MH-Z16] CO2: ");
      Serial.print(co2ppm);
      Serial.println(" ppm");
    } else {
      Serial.println("[MH-Z16] Réponse invalide.");
    }
  } else {
    Serial.println("[MH-Z16] Aucune donnée reçue.");
  }

  delay(3000); // Attendre 3 secondes entre chaque mesure
}
