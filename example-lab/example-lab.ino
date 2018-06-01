int ledPin = D5;                    // Use GPIO pin 5 for LED control

void setup() {                        // Perform this part once when first powered on
  Serial.begin(115200);               // Open a serial connection (for debugging)
  delay(10);                          // Wait 10 milliseconds (1/100th of a second)
  Serial.println("Test the Serial Monitor"); // DEBUG CODE

  pinMode(ledPin, OUTPUT);            // Set the LED pin to Output
  digitalWrite(ledPin, LOW);          // Set the pin to LOW (Off)

}

void loop() {
  digitalWrite(ledPin, HIGH);         // Set the pin High (On)
  Serial.println("On");               // DEBUG CODE
  delay(1000);                        // Wait one second
  digitalWrite(ledPin, LOW);          // Turn off the LED
  Serial.println("Off");              // DEBUG CODE
  delay(1000);
}
