//By Jackson Borneman
//Serial Remote

#include <IRremote.h>

#define RECV_PIN 7
#define STAT_PIN 13

IRrecv irrecv(RECV_PIN);
IRsend irsend;

int Selection = 0;
int Menu = 0;
decode_results results;


void setup() { //Sets up the Script
  Serial.begin(9600);
  Serial.println("Hello World");
  pinMode(STAT_PIN, OUTPUT);
  runGUI();
}
void clear() {//Clears the Serial window for better Viewing
  for (int X = 0; X < 125; X++) {
    Serial.println("");
  }
}
void drawMain() { //Draws Main Menu
  Serial.println("Please Make A Selection:");
  Serial.println("	1) Projector");
  Serial.println("	2) Macros");
  Serial.println("	3) Identify");
}
void drawProj() { //Draws Projector Submenu
  Serial.println("Projector submenu");
  Serial.println("Please Make A Selection:");
  Serial.println("	1) Power");
  Serial.println("	2) No Show");
  Serial.println("	3) Freeze");
  Serial.println("	4) Mute");
  Serial.println("	5) Volume Up");
  Serial.println("	6) Volume Down");
  Serial.println("	7) Exit");
}
void drawMacro() { //Draws Macro Submenu
  Serial.println("Macro submenu");
  Serial.println("Please Make A Selection");
  Serial.println("  1) Power Off");
  Serial.println("	1) Exit");
}
void runGUI() { //Runs The GUI
  clear();
  switch (Menu) {
	case 0:{ //Menu
		drawMain();
		Menu = Selection;
		break;
	}
    case 1:{ //Projector
      drawProj();
      if (Selection > 0) {
        switch (Selection) {
          case 1:{ //Power
            digitalWrite(STAT_PIN, HIGH);
            irsend.sendNEC(0xCCFF00FF, 32);
            Serial.println("Sent Power");
            for (int X = 0; X <= 5; X++) {
				irsend.sendNEC(REPEAT, 32);
				Serial.println("Sent NEC repeat");
            }
            digitalWrite(STAT_PIN, LOW);
            break;
		  }
          case 2:{ //No Show
            digitalWrite(STAT_PIN, HIGH);
            irsend.sendNEC(0xCCFFD12E, 32);
            Serial.println("Sent No Show");
            digitalWrite(STAT_PIN, LOW);
            break;
		  }
          case 3:{ //Freeze
            digitalWrite(STAT_PIN, HIGH);
            irsend.sendNEC(0xCCFFC23D, 32);
            Serial.println("Sent Freeze");
            digitalWrite(STAT_PIN, LOW);
            break;
		  }
          case 4:{ // Mute
            digitalWrite(STAT_PIN, HIGH);
            Serial.println("Sent Mute");
            irsend.sendNEC(0xCCFFD02F, 32);
            digitalWrite(STAT_PIN, LOW);
            break;
		  }
          case 5:{ // Volume up
            digitalWrite(STAT_PIN, HIGH);
            irsend.sendNEC(0xCCFFB847, 32);
            Serial.println("Sent Volume UP");
            digitalWrite(STAT_PIN, LOW);
            break;
		  }
          case 6:{ // Volume Down
            digitalWrite(STAT_PIN, HIGH);
            irsend.sendNEC(0xCCFF7887, 32);
            Serial.println("Sent Volume Down");
            digitalWrite(STAT_PIN, LOW);
            break;
			}
          case 7:{ // Back to main menu
            Menu = 0;
            runGUI();
            break;
		  }
          default:{ // Invalid Selection
            Serial.println("Invalid Selection");
            break;
		  }
        }
        Selection = 0;
      }
      break;
	}
    case 2:{ //Macro
      drawMacro();
      if (Selection > 0) {
        switch (Selection) {
          case 1:
            for (int A = 0; A != 2; A++) {
				digitalWrite(STAT_PIN, HIGH);
				Serial.println("Sent Power");
				irsend.sendNEC(0xCCFF00FF, 32);
				digitalWrite(STAT_PIN,LOW);
				for (int X = 0; X <= 5; X++) { //Send Power
				digitalWrite(STAT_PIN, HIGH);
                irsend.sendNEC(REPEAT, 32);
                Serial.println("Sent NEC repeat");
				digitalWrite(STAT_PIN,HIGH);
                delay(500);
              }
            }
          case 2:
            Menu = 0;
            break;
          default:
            Serial.println("Invalid Selection");
            break;
        }
      }
      Selection = 0;
      break;
	}
    case 3:{ //Record
      runRec(&results);
      Selection = 0;
      runGUI();
      break;
	}
    default:{
      Serial.println("Invalid Selection");
      Selection = 0;
      Menu = 0;
      runGUI();
      break;
	}
  }
  
}

void runRec(decode_results *results) { //Records Ir info //Credit to the IRremote libary maker
  Serial.println("Please Press any button on the remote!");
  { // Code to Grab ir and display it
    int codeType = -1; // The type of code
    unsigned long codeValue; // The code value if not raw
    unsigned int rawCodes[RAWBUF]; // The durations if raw
    int codeLen; // The length of the code
    int toggle = 0; // The RC5/6 toggle state
    codeType = results->decode_type;
    int count = results->rawlen;
    bool codeFound = false;
    while (!codeFound) {
      if (codeType != NULL) {
        if (codeType == UNKNOWN) {
          Serial.println("Received unknown code, saving as raw");
          codeLen = results->rawlen - 1;
          // To store raw codes:
          // Drop first value (gap)
          // Convert from ticks to microseconds
          // Tweak marks shorter, and spaces longer to cancel out IR receiver distortion
          for (int i = 1; i <= codeLen; i++) {
            if (i % 2) {
              // Mark
              rawCodes[i - 1] = results->rawbuf[i] * USECPERTICK - MARK_EXCESS;
              Serial.print(" m");
            }
            else {
              // Space
              rawCodes[i - 1] = results->rawbuf[i] * USECPERTICK + MARK_EXCESS;
              Serial.print(" s");
            }
            Serial.print(rawCodes[i - 1], DEC);
          }
          Serial.println("");
        }
        else {
          if (codeType == NEC) {
            Serial.print("Received NEC: ");
            if (results->value == REPEAT) {
              // Don't record a NEC repeat value as that's useless.
              Serial.println("repeat; ignoring.");
            }
          }
          else if (codeType == SONY) {
            Serial.print("Received SONY: ");
          }
          else if (codeType == PANASONIC) {
            Serial.print("Received PANASONIC: ");
          }
          else if (codeType == JVC) {
            Serial.print("Received JVC: ");
          }
          else if (codeType == RC5) {
            Serial.print("Received RC5: ");
          }
          else if (codeType == RC6) {
            Serial.print("Received RC6: ");
          }
          else {
            Serial.print("Unexpected codeType ");
            Serial.print(codeType, DEC);
            Serial.println("");
          }
        }
        Serial.println(results->value, HEX);
        codeValue = results->value;
        codeLen = results->bits;
      }
      else {
        irrecv.resume();
      }
    }
  }
  Serial.println("send anything to contunue");

}
void loop() {} //Base Loop
void serialEvent() {//Decodes Serial info when sent
  Selection = Serial.parseInt();
    if(Menu == 0){//Cleans up menu Switch
	  	Menu = Selection;
		Selection = 0;
  }
  Serial.print("You sent: ");
  Serial.println(Selection);
  runGUI(); 
}