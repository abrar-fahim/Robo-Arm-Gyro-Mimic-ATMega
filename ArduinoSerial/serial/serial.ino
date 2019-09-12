char  a;
void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:

}

void loop() {
  if (Serial.available()){
    a=Serial.read();
  Serial.print(a);
  }
}