#include <Keypad.h>
#include <Adafruit_NeoPixel.h>
//Ta bort kommentar nedenfor dersom bruk av display
// #include <MD_MAX72xx.h>
// #include <MD_Parola.h>

// #define MAX_DEVICES 4
// #define DATA_PIN 11
// #define CLK_PIN 3
// #define CS_PIN 12
// #define HARDWARE_TYPE MD_MAX72XX::FC16_HW
// MD_Parola display = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

#define PIN_NEO_PIXEL 13   // Pin som er koblet til led listen
#define NUM_PIXELS 60      // antall led lys på listen

#define PIN_PIR_sensor 2  // pin som pir sensoren er koblet til

//KeyPad
const byte ROWS = 4;
const byte COLS = 3;
// Define the keypad
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte rowPins[ROWS] = {10, 9, 8, 7};
byte colPins[COLS] = {6, 5, 4};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
Adafruit_NeoPixel NeoPixel(NUM_PIXELS, PIN_NEO_PIXEL, NEO_GRB + NEO_KHZ800);

// Setter variabler
int teller = 0;
int maksTeller = 20;
bool settMaks = false;

void setup() {
  Serial.begin(9600);

  NeoPixel.begin();
  pinMode(PIN_PIR_sensor, INPUT);
}

void loop() {
  NeoPixel.clear();
  if (digitalRead(PIN_PIR_sensor) == HIGH) {   // sjekker om pir sensoren har oppdaget bevegelse
    counter++;        // øker teller
    Serial.print("Counter: ");    //printer teller i serial monitor
    Serial.println(counter);
    Serial.print("Max Counter: ");
    Serial.println(maxCounter);
    // displayCount();

    endreFarge();  //endrer farge på led lyset basert på telleren
    delay(2000);
  }

  char key = customKeypad.getKey();   //sjekker om keypaden er trykket på
  if (key) {
    handleKeypadInput(key);
  }
}

void endreFarge() {
  int ledTeller = teller * 100 / maksTeller; // beregner prosenten av telleren ut i fra makstelleren

  for (int pixel = 0; pixel < NUM_PIXELS; pixel++) {
    if (ledCounter <= 35) {   // setter fargen til grønn
      // Green color
      NeoPixel.setPixelColor(pixel, NeoPixel.Color(0, 255, 0));
    } else if (ledCounter <= 75) {  // setter fargen til gul
      // Yellow color
      NeoPixel.setPixelColor(pixel, NeoPixel.Color(255, 255, 0));
    } else {    // setter fargen til rød
      // Red color
      NeoPixel.setPixelColor(pixel, NeoPixel.Color(255, 0, 0));
    }
  }

  NeoPixel.show();  // Oppdaterer Led stripen
}


// void displayCount() {  //dersom man har display vil denne metoden printe telleren 
//   display.displayClear();
//   display.print(counter);
// }

void handleKeypadInput(char nokkel) {
  switch (nokkel) {
    case '#':     // dersom man trykker på # bekrefter man maksantallet man har tastet inn
      settMaks = false;
      Serial.print("Maks teller er satt til: ");
      Serial.println(maksTeller);
      // display.print("Maks teller er satt til: " + String(maksteller)); // dersom man har display vil denne koden printe til displayet
      break;
    case '*':     //dersom man vil endre maksantallet trykker man på *
      Serial.println("Skriv inn maksantallet: ");
      // display.displayClear();
      // display.println("Enter counter value:");
      settMaks = true;
      maksTeller = 0;
      break;
    default:
      if (settMaks) {     // dersom settMaks er true (man har trykket på *) sa oppdaterer man makstelleren
        Serial.println(nokkel);
        // display.print(maxCounter);
        maksTeller = maksTeller * 10 + (nokkel - '0'); // gjor om makstelleren (fra char)til en int
      }
      break;
  }
}
