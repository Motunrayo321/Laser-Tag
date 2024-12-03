#include <IRremote.h>

const int RECV_PIN = 14;  // Pin connected to the IR receiver
int LED = 27;

void setup() {
  Serial.begin(115200);
  IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK);  // Initialize IR receiver
  Serial.println("IR Receiver Ready");

  pinMode(LED, OUTPUT);

  digitalWrite(LED, HIGH);

}

void loop() {
  if (IrReceiver.decode()) {

    // if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
    //         Serial.println(F("Received noise or an unknown (or not yet enabled) protocol"));
    //         // We have an unknown protocol here, print extended info
    //         IrReceiver.printIRResultRawFormatted(&Serial, true);
    //         IrReceiver.resume(); // Do it here, to preserve raw data for printing with printIRResultRawFormatted()
    //     } else {
    //         IrReceiver.resume(); // Early enable receiving of the next IR frame
    //         IrReceiver.printIRResultShort(&Serial);
    //         IrReceiver.printIRSendUsage(&Serial);
    //     }
    //   }     Serial.println();
    // }

    // Collect the command code in a variable
    uint16_t command = IrReceiver.decodedIRData.command;

    // Print the command code
    Serial.print("Command: 0x");
    Serial.println(command); //, HEX);

    // Resume receiving the next signal
    IrReceiver.resume();

    if (command == 22) {
      digitalWrite(LED, LOW);
      delay(500);
      digitalWrite(LED, HIGH);
    }
  }
}
