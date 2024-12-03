#include <IRremote.h>

int RECV_PIN = 14;

IRrecv irrecv(RECV_PIN);

decode_results results;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  irrecv.enableIRIn();
}

void loop() {
  if (irrecv.decode(&results)) {

    Serial.print("Code: ");
    Serial.println(results.value, HEX);
    irrecv.resume();
  }
}
