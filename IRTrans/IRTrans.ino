#include <IRremote.h>

// Specify the pin connected to the IR transmitter
const int IR_TRANSMITTER_PIN = 3; // Change this to match your actual transmitter pin

IRsend irsend(IR_TRANSMITTER_PIN);

void setup() {
  Serial.begin(9600);
  Serial.println("IR Transmitter Ready");
}

void loop() {
  Serial.println("Sending IR signal for 2 seconds...");
  
  unsigned long startTime = millis();
  while (millis() - startTime < 2000) {  // Send signal continuously for 2 seconds
    irsend.sendNEC(0xF7C03F, 32);  // Example NEC signal, repeat sending
    delay(50);  // Small delay to avoid overwhelming the receiver
  }
  
  Serial.println("Waiting for 5 seconds...");
  delay(500);  // Wait for 5 seconds before repeating
}



