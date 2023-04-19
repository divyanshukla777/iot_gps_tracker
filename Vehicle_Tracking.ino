#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <String.h>

boolean GetGPS_flag = true;
boolean Location_isValid_flag = true;
boolean Password_flag = true;
String value;
String message_text;
String password = "Hello";
String Message,lat,lon,url;
int i=1;

SoftwareSerial mySerial(2,3);
TinyGPSPlus gps;

String SIM808(String value) 
{
  String Out;

  mySerial.println(value);
  delay(10);
  while (mySerial.available()) 
  {
    Out = (mySerial.readString());
  }

  Out.trim();
  Out.remove(0, value.length() + 3);
  return Out;
}
String SIM808(String value, int count) 
{
  String Out;

  mySerial.println(value);
  delay(10);
  while (mySerial.available()) 
  {
    Out = (mySerial.readString());
  }

  Out.trim();
  Out.remove(0, value.length() + 3 + count);
  return Out;
}

void setup() 
{
  Serial.begin(9600);
  mySerial.begin(9600);
  call();
  mySerial.println("AT+CGNSPWR=1");
  delay(1000);
  mySerial.println("AT+CGNSSEQ=\"RMC\"");
  delay(1000);
}

void loop() 
{
   //******* GET GPS *******
  if (GetGPS_flag) 
  {
    do 
    {
      Serial.println("Power Down The GPS");
      SIM808("AT+CGPSPWR=0");
      //Serial.println("end");

      Serial.println("Check The GPS Power");
      value = SIM808("AT+CGPSPWR?");
      value.remove(11, 17);
      Serial.println(value);
      //Serial.println("end");

      Serial.println("Power Up The GPS");
      SIM808("AT+CGPSPWR=1");
      //Serial.println("end");

      Serial.println("Disable The GPS Output");
      SIM808("AT+CGPSOUT=0");
      //Serial.println("end\n");

      Serial.println("Wait For The GPS To Find Location");
      while (!SIM808("AT+CGPSSTATUS?" , 13).startsWith("Location 3D Fix")) 
      {
        Serial.println("Location Not Fixed Yet, Please Wait!");
        delay(1000);
      }
      Serial.println("Location Found!");
      Serial.println("end\n");



      //Get and Analyse The GPS Output
      Serial.println("Get and Analyse The GPS Output");
      String Out = "";
      mySerial.println("AT+CGPSOUT=2");

      delay(10);
      while (mySerial.available()) 
      {
        mySerial.readStringUntil('\n');
        mySerial.readStringUntil('\n');
        Out = (mySerial.readStringUntil('\n'));  Out += "\r\n";
      }
      mySerial.println("AT+CGPSOUT=0");
      delay(10);
      mySerial.println("AT+CGPSOUT=32");
      delay(10);
      while (mySerial.available()) 
      {
        mySerial.readStringUntil('\n');
        mySerial.readStringUntil('\n');
        Out += (mySerial.readStringUntil('\n'));  Out += "\r\n";
      }
      mySerial.println("AT+CGPSOUT=0");
      Out.trim();
      Serial.println(Out);
      Serial.println("");
      //GPS Output Analized


      int buffint = Out.length();
      char buff[buffint];
      Out.toCharArray(buff, buffint);
      const char *gpsStream = buff;

      while (*gpsStream)
        if (gps.encode(*gpsStream++))
          displayInfo();

      Serial.println("");
      Serial.println("");

      delay(1000);

      if (gps.location.isValid())
      {
        Location_isValid_flag = true;
        Message = String(gps.location.lat(), 6);
        lat = String(gps.location.lat(), 6);
        Message += ",";
        Message += String(gps.location.lng(), 6);
        lon = String(gps.location.lng(), 6);
        Message += " ";
      }
    } while (!Location_isValid_flag);

  }

  if (Password_flag == true) 
  {
    Serial.println("Open This Link:");
    url = "http://maps.google.com/maps?q=" + Message;
    Serial.println(url);
    mySerial.println(url);
    SIM808(Message);
    delay(5000);
    ShowSerialData();
  }
    
  while(i>0)
  {
      sms(url);
      delay(5000);
      i--;
  }

  //******* GET GPS END *******
 
  if (mySerial.available())
    Serial.write(mySerial.read());
 
  mySerial.println("AT");
  delay(1000);
 
  mySerial.println("AT+CPIN?");
  delay(1000);
 
  mySerial.println("AT+CREG?");
  delay(1000);
 
  mySerial.println("AT+CGATT?");
  delay(1000);
 
  mySerial.println("AT+CIPSHUT");
  delay(1000);
 
  mySerial.println("AT+CIPSTATUS");
  delay(1000);
 
  mySerial.println("AT+CIPMUX=0");
  delay(1000);
 
  ShowSerialData();
 
  mySerial.println("AT+CSTT=\"live.vodafone.com\""); //start task and setting the APN, change this with your APN 
  delay(1000);
 
  ShowSerialData();
 
  mySerial.println("AT+CIICR");//bring up wireless connection
  delay(1000);
 
  ShowSerialData();
 
  mySerial.println("AT+CIFSR");//get local IP adress
  delay(1000);
 
  ShowSerialData();
 
  mySerial.println("AT+CIPSPRT=0");
  delay(1000);
 
  ShowSerialData();
  
  mySerial.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"");//start up the connection
  delay(1000);
 
  ShowSerialData();
 
  mySerial.println("AT+CIPSEND");//begin send data to remote server
  delay(1000);
  ShowSerialData();
  
  // REPLACE THIS API KEY WITH YOUR API KEY
  
  String str="GET https://api.thingspeak.com/update?api_key=90H0WOQ7227E2VLA&field1=" + String(lat) +"&field2="+String(lon);
  Serial.println(str);
  mySerial.println(str);//begin send data to remote server
  
  delay(4000);
  ShowSerialData();
 
  mySerial.println((char)26);//sending
  delay(1000);//waitting for reply, important! the time is base on the condition of internet 
  mySerial.println();
 
  ShowSerialData();
 
  mySerial.println("AT+CIPSHUT");//close the connection
  delay(1000);
  ShowSerialData();
}

void ShowSerialData()
{
  while(mySerial.available()!=0)
  Serial.write(mySerial.read());
  delay(5000); 
  
}

void call()
{
  Serial.println("Calling.......");
  mySerial.print("ATD");                      // Call command ATD
  mySerial.print("9123712634");                // change the mobile number       
  mySerial.println(";");
  delay(10000);
  Serial.println("Call disconected");
  mySerial.println("ATH");
  delay(1000); 
}

 void sms(String url)
 {    
      Serial.println("Sending.......");
      mySerial.println("AT+CMGF=1\r\n");                // Select Text mode for messsaging
      delay(1000);
      mySerial.print("AT+CMGS=\"9123712634\"\r\n");            // change the mobile no   
      delay(1000);   
      mySerial.print(url);
      delay(1000);     
      mySerial.println((char)26);
      delay(1000);
      Serial.println("Message is sent");
      delay(1000);
 }

void displayInfo()
{
  Serial.print(F("Location: "));
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.println();
}

char * floatToString(char * outstr, double val, byte precision, byte widthp) 
{
  char temp[16];
  byte i;

  // compute the rounding factor and fractional multiplier
  double roundingFactor = 0.5;
  unsigned long mult = 1;
  for (i = 0; i < precision; i++)
  {
    roundingFactor /= 10.0;
    mult *= 10;
  }

  temp[0] = '\0';
  outstr[0] = '\0';

  if (val < 0.0) 
  {
    strcpy(outstr, "-\0");
    val = -val;
  }

  val += roundingFactor;

  strcat(outstr, itoa(int(val), temp, 10)); //prints the int part
  if ( precision > 0) 
  {
    strcat(outstr, ".\0"); // print the decimal point
    unsigned long frac;
    unsigned long mult = 1;
    byte padding = precision - 1;
    while (precision--)
      mult *= 10;

    if (val >= 0)
      frac = (val - int(val)) * mult;
    else
      frac = (int(val) - val ) * mult;
    unsigned long frac1 = frac;

    while (frac1 /= 10)
      padding--;

    while (padding--)
      strcat(outstr, "0\0");

    strcat(outstr, itoa(frac, temp, 10));
  }

  // generate space padding
  if ((widthp != 0) && (widthp >= strlen(outstr))) 
  {
    byte J = 0;
    J = widthp - strlen(outstr);

    for (i = 0; i < J; i++) 
    {
      temp[i] = ' ';
    }

    temp[i++] = '\0';
    strcat(temp, outstr);
    strcpy(outstr, temp);
  }
  return outstr;
}