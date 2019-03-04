#define BLYNK_PRINT Serial
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
char auth[] = "7c37784fefb84892a3f76c6c29b5e663";
char ssid[] = "ugur";
char pass[] = "12345678";
#define EspSerial Serial
#define ESP8266_BAUD 115200
ESP8266 wifi(&EspSerial);
//-------------------------------------------------------------------------------------------------------------------
//SERVO  MOTOR PENCERE
        #include <Servo.h>  
        Servo servoPencere; 
        int pencereD=11; 

//----------------------------------------------------
//GAZ SENSÖRÜ
BLYNK_WRITE(V5){int pinValue = param.asInt(); }
        float sensor_gazin=A5;
        float gasValue;
        int buzzerPin = 10;  // buzzer alana kadar kapalı kalsın.
//-----------------------------------------------------
//DHT11 ISI VE NEM SENSÖRÜ
BLYNK_WRITE(V3){ int pinValue = param.asInt();}
BLYNK_WRITE(V4){ int pinValue = param.asInt();}
        #include "dht.h"
        #define dht_apin A2 // Analog Pin sensor is connected to
        dht DHT;


//-----------------------------------------------------------
//AlEV ALGILAMA SENSÖRÜ
BLYNK_WRITE(V2){int pinValue = param.asInt();}
        #include<SoftwareSerial.h>        
        int sensorValue = 0; // variable to store the value coming from the sensor   
        int buzzer = 7; // Output pin for Buzzer
        int sensorPin  = 6;
//-----------------------------------------------------------    
//TOPRAK NEM SENSÖRÜ
BLYNK_WRITE(V6){int pinValue = param.asInt();}
       const int ToprakNemA = A4;       
        int ToprakDeger = 0;  
//------------------------------------
//SERVO MOTOR KAPI
/*servo-motor baş*/
      //  #include <Servo.h>  
        Servo servoKapi;  
        int kapiD=3;
        
/*servo-motor son*/

//------------------------------------
//MAMA SİSTEMİ
#include <Servo.h>  
        Servo servoMama; 
        int mamaD=2; 
        #define trigPin 12
#define echoPin 13
BLYNK_WRITE(V0){int pinValue = param.asInt();}


//---------------------------------------------------------------------------------------------------
void setup()
{
  // Debug console
  Serial.begin(9600);
  EspSerial.begin(ESP8266_BAUD);
  delay(10);
  Blynk.begin(auth, wifi, ssid, pass);
  
  //------------------------------------------
//SERVO MOTOR PENCERE  
        servoPencere.attach(9);  // servo motorun takılı olduğu pini belirledik
//------------------------------------------

//GAZ SENSÖRÜ  
        pinMode(buzzerPin, OUTPUT); //buzzer gelene kadar kapalı kalsın
        pinMode(sensor_gazin,INPUT);
//------------------------------------------
//ALEV ALGILAMA SENSÖRÜ
        pinMode (sensorPin, INPUT);
        pinMode(buzzer,OUTPUT);
//------------------------------------------
//TOPRAK NEM SENSÖRÜ
pinMode(ToprakNemA,INPUT);
//------------------------------------------
//SERVO MOTOR KAPI
/*servo-motor baş*/
 servoKapi.attach(5);
/*servo-motor son*/
 //------------------------------------------
 //Mama sistemi
/*servo-motor baş*/
 servoMama.attach(4);
 pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);
}
//-------------------------------------------------------------------------------------------------------------------
void loop()
{
  Blynk.run();
  //------------------------------------------  
  //SERVO MOTOR PENCERE
  if(digitalRead(pencereD)==HIGH){servoPencere.write(180);}
  else {servoPencere.write(0);}
  //------------------------------------------ 

//GAZ SENSÖRÜ
/*gaz sensörü baş*/
        gasValue=analogRead(sensor_gazin);
        
        if( gasValue >145)
        { 
          Blynk.virtualWrite(V5, "!!!Gaz algılandı!!!");
           Serial.println("gaz deger");
          Serial.println(gasValue);
           servoPencere.write(180); 
          digitalWrite(buzzerPin, HIGH); 
        } 
        else
        {
          Blynk.virtualWrite(V5, "Gaz yok");    
           Serial.println("gaz deger");
          Serial.println(gasValue);   
        digitalWrite(buzzerPin, LOW);
        }
/*gaz sensörü son*/
//------------------------------------------
//ISI VE NEM SENSÖRÜ
  /*ısı-nem sensörü baş*/
        DHT.read11(dht_apin);
        int nem=DHT.humidity;
        int isi=DHT.temperature;
        Blynk.virtualWrite(V3, nem);
        Blynk.virtualWrite(V4, isi);
        delay(1000);
/*ısı-nem sensörü son*/
//------------------------------------------/
//ALEV ALGILAMA SENSÖRÜ
/*alev sensörü baş*/

        sensorValue =digitalRead (sensorPin);
        Serial.println(sensorValue);
        if (sensorValue == 0) // HIGH sabittir degeri bir dir ARDUINO tanimlanmadan kullanilabilir.
         {
     Blynk.virtualWrite(V2, "Ateş algılandı.");        
        digitalWrite(buzzer,HIGH);
         }
        else
      {
     Blynk.virtualWrite(V2, "Ateş yok.");        
        digitalWrite(buzzer,LOW);
      }
   
/*alev sensörü son*/
//------------------------------------------
  //TOPRAK NEM SENSÖRÜ
  /*toprak sensörü baş*/
  ToprakDeger = analogRead(ToprakNemA); // prob pininden okunan veriyi ölçüm sonucuna kayıt ediyoruz
if(ToprakDeger<900){Blynk.virtualWrite(V6, "Bahçeniz Nemli");}
else{ Blynk.virtualWrite(V6, "!!!Bahçeyi Sulayın !!!");}
/*toprak sensörü son*/
//------------------------------------------  
//SERVO MOTOR KAPI
  if(digitalRead(kapiD)==HIGH)
  {servoKapi.write(0);}
  else{servoKapi.write(180);}

  //------------------------------------------  
//MAMA SİSTEMİ
  if(digitalRead(mamaD)==HIGH)
  {servoMama.write(50);}
  else{servoMama.write(0);}

  long sure, mesafe;
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
sure = pulseIn(echoPin, HIGH);
mesafe = (sure/2) / 29.1;
   if (mesafe>4){
       Serial.print(mesafe);
   Serial.println(" cm");
   Blynk.virtualWrite(V0, "!!!Mama Kabı Boşalmak Üzere!!!");
   }
   else {  Blynk.virtualWrite(V0, "Mama Kabı Dolu");}


}
