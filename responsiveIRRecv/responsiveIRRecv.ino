#include <IRremote.h>

const int RECV_PIN = 14;  // Pin connected to the IR receiver
int LED = 27;
ind ID = 1;

void setup() {
  Serial.begin(115200);
  IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK);  // Initialize IR receiver
  Serial.println("IR Receiver Ready");

  pinMode(LED, OUTPUT);

  digitalWrite(LED, HIGH);

}

void loop() {
  if (IrReceiver.decode()) {

    // Collect the command code in a variable
    uint16_t command = IrReceiver.decodedIRData.command;

    // Print the command code
    Serial.print("Command: 0x");
    Serial.println(command);

    // Resume receiving the next signal
    IrReceiver.resume();

    if (command == 22) {
      digitalWrite(LED, LOW);
      delay(500);
      digitalWrite(LED, HIGH);
    }
  }
}
