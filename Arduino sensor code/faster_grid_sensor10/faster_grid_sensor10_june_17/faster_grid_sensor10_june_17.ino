//This code is for a fabric grid sensor of given dimensions. 'ground' and 'power' must be specified as
//the grid rows and columns, and the appropriate number and location of pins must be specified in
//'groundPinIDs' and 'powerPinIDs'

const int ground=10; //define grid size (ground is digital-only side, power is analog-digital split side)
const int power=10;
const int g=22; //define ground pin (g) and power pin (p) starting placements on processor
const int p=44;
const int buf=(power*ground*2)+9; //buffer will hold 2 bytes for each point on the grid (upper and lower bound)
//plus 9 for header (4), timestamp (4) and checksum (1)

int groundPinIDs[] = {g,g+1,g+2,g+3,g+4,g+5,g+6,g+7,g+8,g+9};
int powerPinIDs[] = {p,p+1,p+2,p+3,p+4,p+5,p+6,p+7,p+8,p+9};

byte pressureBuffer[buf] = {};
byte presUB; //Upper and lower bounds of pressure reading for each grid point
byte presLB;

unsigned long time;



void setup() {

  Serial.begin(512000);
  
    for(int groundPinIndex = 0; groundPinIndex < ground; groundPinIndex++){
    
    pinMode(groundPinIDs[groundPinIndex],OUTPUT);
    digitalWrite(groundPinIDs[groundPinIndex],LOW);
  }
  
  for(int powerPinIndex = 0; powerPinIndex < power; powerPinIndex++){
    
    pinMode(powerPinIDs[powerPinIndex],OUTPUT);
    digitalWrite(powerPinIDs[powerPinIndex],LOW);
  }

  // Add or delete analog pins here as grid size changes
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  pinMode(A2,INPUT);
  pinMode(A3,INPUT);
  pinMode(A4,INPUT);
  pinMode(A5,INPUT);
  pinMode(A6,INPUT);
  pinMode(A7,INPUT);
  pinMode(A8,INPUT);
  pinMode(A9,INPUT);
  /*pinMode(A10,INPUT);
  pinMode(A11,INPUT);
  pinMode(A12,INPUT);
  pinMode(A13,INPUT);
  pinMode(A14,INPUT);*/
 // pinMode(A15,INPUT);
  pressureBuffer[0] = 0xff;
  pressureBuffer[1] = 0xff;
  pressureBuffer[2] = 0xff;
  pressureBuffer[3] = 0xff;

  
}


// sets digital pins one at a time to replace more expensive pinMode function
// uses bitwise operations to set and clear the registers. (& to clear, | to set.)
// valMode is boolean, with high being true, pin is integer
// PORTX is the value/state register for Arduino port x, DDRX is the direction register for Arduino port x. 
void setPin(boolean valMode, int pin) {
  switch (pin) {
    
  case 44:
  if (valMode) {
    DDRL = DDRL | B00100000;
    PORTL = PORTL | B00100000;
  }
  else {
    DDRL = DDRL & B11011111;
    PORTL = PORTL & B11011111;
  }
  break;
  
  case 45:
  if (valMode) {
    DDRL = DDRL | B00010000;
    PORTL = PORTL | B00010000;
  }
  else {
    DDRL = DDRL & B11101111;
    PORTL = PORTL & B11101111;
  }
  break;
  
  case 46:
  if (valMode) {
    DDRL = DDRL | B00001000;
    PORTL = PORTL | B00001000;
  }
  else {
    DDRL = DDRL & B11110111;
    PORTL = PORTL & B11110111;
  }
  break;
    
  case 47:
  if (valMode) {
    DDRL = DDRL | B00000100;
    PORTL = PORTL | B00000100;
  }
  else {
    DDRL = DDRL & B11111011;
    PORTL = PORTL & B11111011;
  }
  break;
    
  case 48:
  if (valMode) {
    DDRL = DDRL | B00000010;
    PORTL = PORTL | B00000010;
  }
  else {
    DDRL = DDRL & B11111101;
    PORTL = PORTL & B11111101;
  }
  break;
  
  case 49:
  if (valMode) {
    DDRL = DDRL | B00000001;
    PORTL = PORTL | B00000001;
  }
  else {
    DDRL = DDRL & B11111110;
    PORTL = PORTL & B11111110;
  }
  break;
  
  case 50:
  if (valMode) {
    DDRB = DDRB | B00001000;
    PORTB = PORTB | B00001000;
  }
  else {
    DDRB = DDRB & B11110111;
    PORTB = PORTB & B11110111;
  }
  break;
    
  case 51:
  if (valMode) {
    DDRB = DDRB | B00000100;
    PORTB = PORTB | B00000100;
  }
  else {
    DDRB = DDRB & B11111011;
    PORTB = PORTB & B11111011;
  }
  break;
    
  case 52:
  if (valMode) {
    DDRB = DDRB | B00000010;
    PORTB = PORTB | B00000010;
  }
  else {
    DDRB = DDRB & B11111101;
    PORTB = PORTB & B11111101;
  }
  break;
    
  case 53:
  if (valMode) {
    DDRB = DDRB | B00000001;
    PORTB = PORTB | B00000001;
  }
  else {
    DDRB = DDRB & B11111110;
    PORTB = PORTB & B11111110;
  }
  break;
    
  default: 
//    // do I need a default?
  break;
}
}



void loop() {
   
     int values[power][ground] = {};
  
  unsigned long checksum = 0;
  
  for(int groundPinIndex = 0; groundPinIndex < ground; groundPinIndex++){
    
    int groundPin = groundPinIDs[groundPinIndex];
    
    pinMode(groundPin, OUTPUT);
    digitalWrite(groundPin, LOW);
    
    for(int powerPinIndex = 0; powerPinIndex < power; powerPinIndex++){
      int powerPin = powerPinIDs[powerPinIndex];
      setPin(true, powerPin);   // added june 16
      //pinMode(powerPin, OUTPUT);
      //digitalWrite(powerPin, HIGH);  
      //delayMicroseconds(10); //delay between sampling grid points to prevent artifacts
      //It's been arranged that the analog pin indices are the same as the power pin indices (0 to 4). 
      //It's convenient in code, but if you change the wiring, you'll need to be more specific here.
      int sample = analogRead(powerPinIndex); 
      values[powerPinIndex][groundPinIndex] = sample;
      checksum += sample;
      //digitalWrite(powerPin, LOW);
      //pinMode(powerPin, INPUT);
    }
    pinMode(groundPin, INPUT);
    
  }
  
  time=millis();
  pressureBuffer[4]=(byte)(time);
  pressureBuffer[5]=(byte)(time >> 8);
  pressureBuffer[6]=(byte)(time >> 16);
  pressureBuffer[7]=(byte)(time >> 24);
  checksum += time;
  
  for(int powerPinIndex = 0; powerPinIndex < power; powerPinIndex++){
    for(int groundPinIndex = 0; groundPinIndex < ground; groundPinIndex++){
      presUB = (byte)(values[powerPinIndex][groundPinIndex] >> 8);    // higher eight bits
      presLB = (byte)(values[powerPinIndex][groundPinIndex]);       // lower eight bits
      pressureBuffer[8+(powerPinIndex*2*ground)+(groundPinIndex*2)] = presLB;
      pressureBuffer[8+(powerPinIndex*2*ground)+(groundPinIndex*2)+1] = presUB;
    }
  }

  pressureBuffer[buf-1]=(byte)(checksum);
  Serial.flush();
  Serial.write(pressureBuffer, buf);

 }
