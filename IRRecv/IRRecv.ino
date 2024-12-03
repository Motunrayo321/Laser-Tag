#include <IRremote.h>

int RECV_PIN = 14;  // Connect IR receiver's signal pin to GPIO 14

IRrecv irrecv(RECV_PIN);
decode_results results;

void setup() {
  Serial.begin(115200);
  irrecv.enableIRIn();  // Start the receiver
  Serial.println("IR Receiver Ready");
}

void loop() {
  if (irrecv.decode(&results)) {
    Serial.println("ON");  // Signal detected
    irrecv.resume();       // Ready for next signal
    delay(100);            // Small delay to avoid repeated "ON" messages for the same signal
  } else {
    static unsigned long lastPrint = 0;
    if (millis() - lastPrint > 1000) {  // Print "OFF" once per second if no signal
      Serial.println("OFF");
      lastPrint = millis();
    }
  }
}
