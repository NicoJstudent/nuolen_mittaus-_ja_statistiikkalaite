#include <LiquidCrystal.h>
LiquidCrystal lcd(8,9,4,5,6,7);  // näyttökirjasto

int keypad_pin = A0;
int keypad_value = 0;
int keypad_value_old = 0;

const int anturi1 = A1; // mittausanturit
const int anturi2 = A2;

// muuttujat
float tulosTaulukko [5] = {0, 0, 0, 0, 0}; // 5 alkiota mihin tallennetaan mittausten tulokset
int taulukkoIndeksi = 0; // osoittaa paikan taulukossa
int menutulosindeksi=0; // lcd-näytön muistipaikat
float keskiarvo=0;
long duration;
long duration2;
int distance1=0;
int distance2=0;
float aikaleima1=0;
float aikaleima2=0;
void ReadKeyPad();
//unsigned long aikaleima1=0;
//unsigned long aikaleima2=0;
int distance=0;
//unsigned long odotus=0;
//float ultrasonicRead();
//float ultrasonicRead2();
float vauhti_fps;

char btn_push;

byte mainMenuPage = 1;
byte mainMenuPageOld = 1;
byte mainMenuTotal = 4;

/*float ultrasonicRead ()
{
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(1);

  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

  //calculating distance
  distance= duration*0.0343/2;

   // Prints the distance on the Serial Monitor
  if (distance < 60){
  // Serial.print("Distance in cm : "); näitä ei tarvitse
  // Serial.println(distance);

  aikaleima1 = micros()/1000;

  return distance;
  }
}

float ultrasonicRead2 ()
{
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(1);

  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);

  duration2 = pulseIn(echoPin2, HIGH);

  distance2= duration2*0.0343/2;

  if (distance2 < 60){
    //  Serial.print("Distance 2 in cm : ");
    //  Serial.println(distance2);

    aikaleima2 = micros()/1000;

  return distance2;
 }
  
}*/

void setup()
{
    pinMode(anturi1, INPUT);
    pinMode(anturi2, INPUT); //Asettaa antureiden pinnit Input tilaan
    lcd.begin(16,2);  //lcd-näytön alustus
    Serial.begin(9600); //serial monitorin alustus
    MainMenuDisplay(); //päämenu
    delay(1000); 
}
void loop()
{
    btn_push = ReadKeypad();

    MainMenuBtn();

    if(btn_push == 'S')//enter selected menu
    {
        WaitBtnRelease();
        switch (mainMenuPage)
        {
            case 1:
              MenuA();
              break;
            case 2:
              MenuB();
              break;
            case 3:
              MenuC();
              break;
        }

          MainMenuDisplay();
          WaitBtnRelease();
    }



    delay(10);

}//--------------- End of loop() loop ---------------------

void MainMenuDisplay()
{
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Valitse:");
    lcd.setCursor(0,1);
    switch (mainMenuPage)
    {
        case 1:
          lcd.print("1. Tee mittaus");
          break;
        case 2:
          lcd.print("2. Historia");
          break;
        case 3:
          lcd.print("3. Resetointi");
          break;
    }
}

void MenuA()
{  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Mitataan -->");
  Serial.println("testi1");
  if (btn_push == 'S'){

    //Serial.println("testi");

    while(ReadKeypad()!= 'L'){
      btn_push = ReadKeypad();
      if (aikaleima1 == 0){
        //Serial.println("testi2");
        anturiData1();

      }
      else if (aikaleima2 == 0){
        //Serial.println("testi3");
        anturiData2(); 
      }
      else { //tehdään laskelmat aikaleimoihin perustuen ja nollataan aikaleimat uutta mittausta varten
        float lentoaika = (aikaleima2-aikaleima1)/10;
        float sensorivalimatka = 990; //antureiden välinen etäisyys 990 mm
        // vauhti mm/ms
        float vauhti = sensorivalimatka/lentoaika; //tarkka mittaus pitää tehdä ja päivittää tähän
        float vauhti_fps = vauhti * 3.28; // feet per second
        // aikaleima1 

        // nopeus = v, matka = s, aika = t, näiden avulla voidaan kirjoittaa nopeuden laskukaava lyhyesti: v = s / t
        // antureiden välinen matka on 1 m eli 1000 mm
        Serial.print(lentoaika);
        if (lentoaika < 10000 && lentoaika > 1){
          Serial.println(String("indeksi: ") + taulukkoIndeksi);
          tulosTaulukko [taulukkoIndeksi] = vauhti; // Tämä sinne missä lasketaan vauhti = sensirovalimatka/lentoaika
          taulukkoIndeksi++;

          Serial.println(aikaleima1);
          Serial.println(aikaleima2);
          Serial.println(String("Lentoaika: ") + lentoaika);
          Serial.println(String("Vauhti: ") + vauhti);
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print(String("Vauhti: ") + vauhti + " m/s");
          lcd.setCursor(0,1);
          lcd.print(String("Vauhti: ") + vauhti_fps + " fps");
          aikaleima1=0;
          aikaleima2=0;

          if (taulukkoIndeksi == 5){
            float summa = 0;

            for (int i=0; i < 5; i++){
              //(taulukkoIndeksi; taulukkoIndeksi == 0, taulukkoIndeksi--;){  // Tämä myös sinne perään
              //keskiarvo = tulosTaulukko/5;
              //(int i=0; i < 5; i++){  // Tämä myös sinne perään
              summa = summa + tulosTaulukko[i];
              Serial.println(tulosTaulukko[i]);
            }
            keskiarvo = summa/5.0;
            Serial.println(String("Keskiarvo: ") + keskiarvo);
          }
        }
        else {
          aikaleima1=0;
          aikaleima2=0;
        }
      }
    }
  }
}


void MenuB()
{  
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(String("") + (menutulosindeksi+1) + "/5: " + tulosTaulukko[menutulosindeksi] + " m/s");
    lcd.setCursor(0,1);
    lcd.print(String("") + (menutulosindeksi+1) + "/5: " + tulosTaulukko[menutulosindeksi]*3.28 + " fps");

    while(ReadKeypad()!= 'L')
    {
      btn_push = ReadKeypad();
      if (btn_push == 'U'){

        menutulosindeksi++;

        if (menutulosindeksi < 5){
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print(String("") + (menutulosindeksi+1) + "/5: " + tulosTaulukko[menutulosindeksi] + " m/s");
          lcd.setCursor(0,1);
          lcd.print(String("") + (menutulosindeksi+1) + "/5: " + tulosTaulukko[menutulosindeksi]*3.28 + " fps");
        }
        else if (menutulosindeksi == 5){
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print(String("Avg: ") + keskiarvo + " m/s");
          lcd.setCursor(0,1);
          lcd.print(String("Avg: ") + keskiarvo*3.28 + " fps");
        }
        else{

          menutulosindeksi = -1;

        }
        
        WaitBtnRelease();
      }

      if (btn_push == 'D'){

        menutulosindeksi--;

        if (menutulosindeksi < 0){

          menutulosindeksi = 5;

        }

        if (menutulosindeksi < 5){
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print(String("") + (menutulosindeksi+1) + "/5: " + tulosTaulukko[menutulosindeksi] + " m/s");
          lcd.setCursor(0,1);
          lcd.print(String("") + (menutulosindeksi+1) + "/5: " + tulosTaulukko[menutulosindeksi]*3.28 + " fps");
        }
        else if (menutulosindeksi == 5){
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print(String("Avg ") + keskiarvo + " m/s");
          lcd.setCursor(0,1);
          lcd.print(String("Avg ") + keskiarvo*3.28 + " fps");
        }
        else{

          menutulosindeksi = -1;

        }
        
        WaitBtnRelease();
      }

    }
}

void MenuC()
{  
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Resetoi:");
    lcd.setCursor(0,1);
    lcd.print("paina 'select'");

    while(ReadKeypad()!= 'L')
    {
      if (ReadKeypad() == 'S'){
       for (int i=0; i < 5; i++){ 
            tulosTaulukko[i]=0;
       }
       keskiarvo=0;
       taulukkoIndeksi=0;
       menutulosindeksi=0;
       lcd.clear();
       lcd.setCursor(0,0);
       lcd.print("Arvot nollattu");
       delay(1000);
       mainMenuPage=1;
       break;
      }
    }
}

void MainMenuBtn()
{
    WaitBtnRelease();
    if(btn_push == 'U')
    {
        mainMenuPage++;
        if(mainMenuPage > mainMenuTotal)
          mainMenuPage = 1;
    }
    else if(btn_push == 'D')
    {
        mainMenuPage--;
        if(mainMenuPage == 0)
          mainMenuPage = mainMenuTotal;    
    }

    if(mainMenuPage != mainMenuPageOld) //only update display when page change
    {
        MainMenuDisplay();
        mainMenuPageOld = mainMenuPage;
    }
}

char ReadKeypad()
{
  /* Keypad button analog Value
  no button pressed 1023
  select  741
  left    503
  up      326
  down    142
  right   0
  */
  keypad_value = analogRead(keypad_pin);

  if(keypad_value < 100)
    return 'R';
  else if(keypad_value < 200)
    return 'D';
  else if(keypad_value < 400)
    return 'U';
  else if(keypad_value < 600)
    return 'L';
  else if(keypad_value < 800)
    return 'S';
  else
    return 'N';

}

void WaitBtnRelease(){

    while( analogRead(keypad_pin) < 800){}
}

float anturiData1 ()
{
  float ekaAnturi = digitalRead(anturi1);
  if (ekaAnturi == LOW){
    aikaleima1 = micros()/100;
  }
}

float anturiData2 ()
{
  float tokaAnturi = digitalRead(anturi2);
  if (tokaAnturi == LOW){
    aikaleima2 = micros()/100;
  }
}
