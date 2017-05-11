//By Jackson Borneman
//The Menu
int Selection = 0;
int Menu = 0;
void setup(){
	Serial.begin(9600);
  	Serial.println("Hello World");
	runGUI();
}
void clear(){
  for(int X = 0; X < 200; X++){
  	Serial.println("");
  }
}
void drawMain(){
	Serial.println("Please Make A Selection:");
  	Serial.println("	1) Projector");
    Serial.println("	2) Macros");
}
void drawProj(){
	Serial.println("Projector submenu");
  	Serial.println("Please Make A Selection:");
  	Serial.println("	1) Power");
  	Serial.println("	2) Freeze");
  	Serial.println("	3) Exit");
}
void drawMacro(){
  Serial.println("Macro submenu");
  Serial.println("Please Make A Selection");
  Serial.println("	1) Exit");
}
void loop(){}
void runGUI(){
  clear();
  switch (Menu){
    case 0:
    	drawMain();
          Menu = Selection;
          Selection = 0;
  		break;
    case 1:
    	drawProj();
    	if(Selection>0){
          switch (Selection){
            case 1:
          		//send power
            	Serial.println("Yes");
          		break;
          	case 2:
          		//send Freeze
            	Serial.println("ABC");
          		break;
          	case 3:
          		Menu = 0;
          		break;
          }
          Selection = 0;
        }
    	break;
    case 2:
    	drawMacro();
    if(Selection>0){
          switch (Selection){
          	case 1:
          		Menu = 0;
          		break;
          }
    }
      Selection = 0;
    	break;
  
  }
}
void serialEvent(){
	Selection = Serial.parseInt();
	if(Menu == 1){
		Menu = Selection;
	}
  	Serial.print("You sent: ");
  	Serial.println(Selection);
  	runGUI();
}
