#include <IRremote.h>

const int RECV_PIN = 25;  // Pin connected to the IR receiver
int LED = 33;
int playerID = 1;
int shooterID;

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
    //Serial.print("Command: 0x");
    //Serial.println(command);

    // Resume receiving the next signal
    IrReceiver.resume();

    if (command == 22) {
      Serial.println("Warning Shot!");
    }

    if (command == 12 || command == 24 || command == 94 || command == 8 || command == 28) {
      digitalWrite(LED, LOW);
      delay(100);
      digitalWrite(LED, HIGH);

      Serial.print("You were shot by Player ");
      Serial.println(ShooterID(command));
    }
  }
}


int ShooterID(int code) {
  switch (code) {
    case 12:
    shooterID = 1;
    break;

    case 24:
    shooterID = 2;
    break;

    case 94:
    shooterID = 3;
    break;

    case 8:
    shooterID = 4;
    break;

    case 28:
    shooterID = 5;
    break; 
  }
  return shooterID;
}