#include <Arduino.h>
#include<OneButton.h>
#include<TM1637Display.h>

#define led 13
#define CLK 6
#define DIO 7

int pool_lenght = 16;
int lap_counter = 0;
int button_delay = 2500;
double max_speed = 0;
unsigned long lap_time_1 = 0;
unsigned long lap_time_2 = 0;
unsigned long start_time = 0;
OneButton MainButton(2, false);
//Wprowadzanie długości basenu


TM1637Display Lap_display = TM1637Display(CLK, DIO); //deklaracja wyśweitlacza

//Wyświetlanie czasu w formacie 00:00:00
void Time_Display(unsigned long time_in_milisec, bool milis_display){
  /*Funkcja wyświetla czas w formacie MM:SS:MMM. przyjmuje jako argumenty:
  usigned long time_in-milisec - czas w milisekundach 
  bool milis_display - jeśli jest równe TRUE wyświetla format MM:SS:MMM */

   int time_in_milisec_int = (long)time_in_milisec;
   int miliseconds = 0;
   int seconds = 0;
   int minutes = 0;

   miliseconds = time_in_milisec_int % 1000;
   time_in_milisec -=miliseconds;
   seconds = time_in_milisec/1000;
   minutes = seconds/60;
   seconds = seconds % 60;

  if( milis_display == true){
    if(minutes < 10){
      Serial.print("0");
      Serial.print(minutes);
      Serial.print(":");
      if(seconds < 10){
        Serial.print("0");
        Serial.print(seconds);
        Serial.print(":");
        if(miliseconds < 100 && miliseconds > 10){
          Serial.print("0");
          Serial.print(miliseconds);
        }
        else if(miliseconds < 10){
          Serial.print("00");
          Serial.print(miliseconds);
        }
        else{
          Serial.print(miliseconds);
        }
      }
      else{
        Serial.print(seconds);
        Serial.print(":");
        if(miliseconds < 100 && miliseconds > 10){
          Serial.print("0");
          Serial.print(miliseconds);
        }
        else if(miliseconds < 10){
          Serial.print("00");
          Serial.print(miliseconds);
        }
        else{
          Serial.print(miliseconds);
        }
      }
    }
    else{
      Serial.print(minutes);
      Serial.print(":");
      if(seconds < 10){
        Serial.print("0");
        Serial.print(seconds);
        Serial.print(":");
      }
      else{
        Serial.print(seconds);
        Serial.print(":");
      }
    }
  }
  else{
    if(minutes < 10){
      Serial.print("0");
      Serial.print(minutes);
      Serial.print(":");
      if(seconds < 10){
        Serial.print("0");
        Serial.print(seconds);
      }
      else{
        Serial.print(seconds);
      }
    }
    else{
      Serial.print(minutes);
      Serial.print(":");
      if(seconds < 10){
        Serial.print("0");
        Serial.print(seconds);
      }
      else{
        Serial.print(seconds);
      }

    }
  }

   
}

//Funkcje podpięte pod przycisk
void MainButtonClick (){
  if(lap_counter == 0){ //Rozpocz?cie dzia?ania timera
    start_time = millis();
    Serial.println("-------  Timer Started  --------");
    Serial.println("COUNT     TIME         TIME TOT.");
    digitalWrite(led, HIGH);
    lap_counter++;
    Lap_display.setSegments(0b00010000,4,2);
    delay(500);

  }
  else{ //Zliczanie okr?ze?
    lap_time_1 = millis() - start_time; //Zliczenie czasu okr??enia
    //Wy?wietlenie nmeru okr??enia
    if(lap_counter < 10){
      Serial.print("Lap00");
    }
    else if(lap_counter >= 10){
      Serial.print("Lap0");
    }
    else{
      Serial.print("Lap");
    }
    Serial.print(lap_counter);
    Serial.print("    ");
    //Wy?wietlenie czasu danego okr??enia
    Time_Display((lap_time_1-lap_time_2),1);
    Serial.print("    ");
    //Wy?wietlenie czasu sumy okr??e? 
    Time_Display(lap_time_1,1);
    //Wyswietlenie predkosci danego okrazenia
    Serial.print("    ");
    Serial.print(50.00/((lap_time_1-lap_time_2)/1000.00));
    //Zapisanie prędkości maksymalnej
    if((50.00/((lap_time_1-lap_time_2)/1000.00)) > max_speed){
      max_speed = (50.00/((lap_time_1-lap_time_2)/1000.00));
    }

    lap_time_2 = lap_time_1; //Zapisanie czasu okra?enia
    digitalWrite(led, HIGH); //Zapalenie diody przy wci?ni?ciu przycisku
    lap_counter++; //Zapisanie numeru okra?enia
    delay(500); //uniemozliwia double click
    Serial.println(); //przechodzi do nastepnej linii
    Lap_display.showNumberDec(pool_lenght*2*lap_counter);
  }

}

void myDoubleClickFunction (){ //Zakończenie działania timera
   lap_time_1 = millis() - start_time; //Zliczenie czasu ostatniego okr??enia
  //Wy?wietlenie numeru ostatniego okra?enia
    if(lap_counter < 10){
      Serial.print("Lap00");
    }
    else if(lap_counter >= 10){
      Serial.print("Lap0");
    }
    else{
      Serial.print("Lap");
    }
    Serial.print(lap_counter);
    Serial.print("    ");
    Time_Display((lap_time_1-lap_time_2-button_delay),1);
    Serial.print("    ");
    //Wy?wietlenie czasu sumy okre? w ostatnim okr??eniu 
    Time_Display(lap_time_1-button_delay,1);
    //Wyswietlenie predkosci ostatniego okrozenia
    Serial.print("    ");
    Serial.print(50.00/((lap_time_1-lap_time_2-button_delay)/1000.00));

   Serial.println();//Przeskoczenie do następnej linii
    //Wy?wietlenie statystyk zako??zenie komunikacji
    for (int i = 0;i<3;i++){
      digitalWrite(led,HIGH);
      delay(100);
      digitalWrite(led,LOW);
      delay(100);
    }
    Serial.println("-------  Timer Stopped  --------");
    Serial.print("Distance: ");
    Serial.print(pool_lenght*2*lap_counter, DEC);
    Serial.println("m");
    Serial.print("Time: ");
    Time_Display((lap_time_1-button_delay),1);
    Serial.println("s");
    Serial.print("Max speed: ");
    Serial.print(max_speed);
    Serial.println("m/s");
    Serial.print("Avarage speed: ");
    Serial.print((pool_lenght*2*lap_counter)/((lap_time_1-button_delay)/1000.00));
    Serial.println("m/s");
    lap_counter = 0;
    max_speed = 0;
    lap_time_1 = 0;
    lap_time_2 = 0;
    start_time = 0;

    delay(1000);
}

void setup() {
  Serial.begin(9600);
  MainButton.attachClick(MainButtonClick);
  MainButton.attachLongPressStart(myDoubleClickFunction);
  pinMode(led, OUTPUT);
  Lap_display.clear();
  Lap_display.setBrightness(7);
  Lap_display.showNumberDec(0, true);
  Serial.println("------- Swim Trainer 1.0 -------");
  Serial.println("Insert the pool lenght (typical 25m)");
  Serial.print("Pool Lenght: ");
  Serial.print(pool_lenght);
  Serial.println("m");
  Serial.println("Press button to start");  
  
  
  
 }


void loop() {
  MainButton.tick();
  
  digitalWrite(led, LOW);
//Ustawienia guzika 
  MainButton.setDebounceMs(80);
  MainButton.setPressTicks(button_delay);
  MainButton.setClickTicks(200);

}

