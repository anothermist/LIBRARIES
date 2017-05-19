/**
 * SetArduinoClock. 
 *
 * portIndex must be set to the port connected to the Arduino
 * 
 * Clicking the window sends the current time to the arduino
 * as the string value of the number of seconds since Jan 1 1970
 *  
 * The clock has two second hands, the white one is the Arduino time
 * the red one is the local time, if the time is exactly the same
 * you will only see the red hand, although due to round trip serial delays
 * the Arduino clock may be one second behind.
 */

import processing.serial.*;

Serial myPort;     // Create object from Serial class
public static final short LF = 10;     // ASCII linefeed
public static final short TIME_HEADER = 255; //header byte for arduino serial time message 
public static final short FONT_SIZE = 12;
public static final short portIndex = 0;  // select the com port, 0 is the first port
color PCClock = color(153, 51, 0);
PFont fontA; 
long prevTime=0; // holds value of the previous time update

void setup() {  
  size(200, 200);
  println(Serial.list());
  println(" Connecting to -> " + Serial.list()[portIndex]);
  myPort = new Serial(this,Serial.list()[portIndex], 19200);

  fontA =  createFont("Arial", FONT_SIZE);  
  textFont(fontA); 
  textSize( FONT_SIZE);
  stroke(255);
  smooth();
  background(0);
  text("Mouse click to set Arduino time", 5,10);
}

long getTimeNow(){
  // java time is in ms, we want secs    
  GregorianCalendar cal = new GregorianCalendar();
  cal.setTime(new Date());
  int	tzo = cal.get(Calendar.ZONE_OFFSET);
  int	dst = cal.get(Calendar.DST_OFFSET);
  long now = (cal.getTimeInMillis() / 1000) ; 
  now = now + (tzo/1000) + (dst/1000); 
  return now;
}

void mousePressed() {  
  String time = String.valueOf(getTimeNow());  
  char header = 0xff;
  myPort.write(header);  // send header and time to arduino
  myPort.write(time);   
}

void serialEvent(Serial p) {
  String inString = myPort.readStringUntil(LF); 
  if(inString != null && inString.length() >= 13 && inString.charAt(0) == TIME_HEADER) {    
    int val = 0;
    long time = 0;
    for(int i = 1; i <= 10; i++)        
      time = time * 10 + (inString.charAt(i) - '0'); 
    UpdateClock(time);
  }         
  else if(inString != null && inString.length() > 0 )     
    println(inString);  // display serial input that is not a time messgage
}

void UpdateClock( long t){
  int sec, min, hr;
  Long time = new Long(t);
  sec = time.intValue() % 60;
  min = (time.intValue() / 60) % 60 ;
  hr =  (time.intValue() % 86400) / 3660;   
  UpdateClock(sec, min, hr );   
}

void UpdateClock( int sec, int min, int hour){        
  fill(80);
  noStroke();
  // Angles for sin() and cos() start at 3 o'clock;
  // subtract HALF_PI to make them start at the top
  ellipse(100, 100, 160, 160);
  float s = map(sec, 0, 60, 0, TWO_PI) - HALF_PI;
  float m = map(min, 0, 60, 0, TWO_PI) - HALF_PI;
  float h = map(hour % 12, 0, 12, 0, TWO_PI) - HALF_PI;
  stroke(255);
  strokeWeight(1);
  line(100, 100, cos(s) * 72 + 100, sin(s) * 72 + 100);
  strokeWeight(2);
  line(100, 100, cos(m) * 60 + 100, sin(m) * 60 + 100);
  strokeWeight(4);
  line(100, 100, cos(h) * 50 + 100, sin(h) * 50 + 100);  
  // now show the seconds on the computer running this app
  s = map(second(), 0, 60, 0, TWO_PI) - HALF_PI;
  strokeWeight(1);
  stroke(PCClock);  // another second hand in red to indicate Java time
  line(100, 100, cos(s) * 72 + 100, sin(s) * 72 + 100);
}

void draw() { 

}
