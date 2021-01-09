//  Decides How To OutPut BPM
void serialOutputWhenBeatHappens()
{   
    //For Serial Monitor
    Serial.print("*** Heart-Beat Happened *** ");
    Serial.print("BPM: ");
    Serial.println(BPM);
    Serial.print("Temperature is: ");
    Serial.println(t);
    //For Mobile App
    char a[4];
    char b[4];
    String str1;
    String str2;
    str1=String(BPM);
    str2=String(t);
    str1.toCharArray(b,4);
    str2.toCharArray(a,5);
    Genotronex.print(a);
    Genotronex.print("|");
    Genotronex.print(b);
    Genotronex.println("|");
}

void draw() //For OLED
{
  char a[4];
  char b[4];
  String str1;
  String str2;
  str1=String(BPM);
  str2=String(t);
  str1.toCharArray(b,4);
  str2.toCharArray(a,5); 
  u8g.drawStr( 0, 20, "BPM:");
  u8g.drawStr( 40, 20, b);
  u8g.drawStr( 0, 40, "TEMP:");
  u8g.drawStr( 40, 40, a);
}

void draw2() //For high temperature warning.
{
  u8g.drawStr( 0, 20, "Chill out!");
  u8g.drawStr( 0, 40, "Temp too high");
  char a[4];
  String str2;
  str2=String(t);
  str2.toCharArray(a,5); 
  u8g.drawStr( 0, 60, "TEMP:");
  u8g.drawStr( 40, 60, a);
}

void draw3() //For high BPM warning.
{
  u8g.drawStr( 0, 20, "Peace out");
  u8g.drawStr( 0, 40, "HR too high");
  char b[4];
  String str1;
  str1.toCharArray(b,4);
  u8g.drawStr( 0, 60, "BPM:");
  u8g.drawStr( 40, 60, b);
}


