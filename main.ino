#include <Wire.h>
#include <LiquidCrystal.h>
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"
#include "RTClib.h"


int pinTh = 0, pinLux = 1, b1 =4, b2 = 5, buzzer = 6, bp2 = 0, bs2 =0 ,pinPot = 2, led = 13;

int nuit=0;

double temp1;

uint8_t alarmeh=0, alarmem=0;
int alarmeon=0;

int APPUI_DELAI = 1000;
unsigned long debutAppui;

Adafruit_7segment seg = Adafruit_7segment();
RTC_DS1307 RTC;
//RTC_Millis RTC;
LiquidCrystal lcd(12,11,10,9,8,7);

void setup(){

	analogReference(EXTERNAL);
	seg.begin(0x70);
	RTC.begin;
        lcd.begin(16, 2);

	//RTC.begin(DateTime(__DATE__, __TIME__));
        //RTC.adjust(DateTime(__DATE__, __TIME__));

	pinMode(b1, INPUT);
	pinMode(b2, INPUT);

	pinMode(buzzer, OUTPUT);
	pinMode(led, OUTPUT);

        alarmeh = RTC.readnvram(0x08);
	alarmem = RTC.readnvram(0x09);
	
}
void loop(){                                 
	
	
	temp1 = mesureTemp(pinTh);
	controlLum(mesureLum(pinLux));
	afficherHeure();
	afficherLcd();

	if (digitalRead(b1)==HIGH)
	  {
	    delay(200); // rebonds
	    afficherMenu();
	  }
	if (alarmeon==1) verifierAlarme(); 
	delay(250); 

	
}


int mesureLum(int pl){
	int lux = analogRead(pl);
	return lux;	
}

double mesureTemp(int pth){
	int B = 3977, To = 298.15, Ro = 10000;
	return 1/(1/double(B)*log(Ro / ( 1023/double(analogRead (pth)) - 1) / Ro)+1/double(298.15))-273.15;
}

void controlLum(int lx){
        if(lx>800)  seg.setBrightness(1);
        else if (lx < 800 && lx > 600)  seg.setBrightness(6);
        else seg.setBrightness(10);
}

void afficherHeure(){
	
	DateTime now = RTC.now();
	
	seg.writeDigitRaw(2, 0x02);
	seg.writeDigitNum(4, now.minute()%10); // minutes unités
	seg.writeDigitNum(3, ((now.minute()-now.minute()%10)/10)%6); // minutes dizaines
	seg.writeDigitNum(1, now.hour()%10); // heures unités
	seg.writeDigitNum(0, ((now.hour()-now.hour()%10)/10)%10); // heures dizaines
	if (nuit == 1) seg.print(0);
	seg.writeDisplay();
}

void afficherTemp(){
	seg.printFloat(temp1, 2, DEC);
	seg.writeDisplay();
}


char* jourSemaine(int j){
	switch(j){
		case 0 : return "Dim"; 
	    	break;
		case 1 : return "Lun"; 
	    	break;
		case 2 : return "Mar"; 
	    	break;
		case 3 : return "Mer"; 
	    	break;
		case 4 : return "Jeu"; 
	    	break;
		case 5 : return "Ven"; 
	    	break;
		case 6 : return "Sam"; 
	    	break;
	}
}

void  afficherLcd(){
        
	DateTime now = RTC.now();
        lcd.clear();
	if (now.hour() < 10 ) lcd.print("0");
        lcd.print(now.hour());
        lcd.print(":");
	if (now.minute() < 10 ) lcd.print("0");
        lcd.print(now.minute());
	lcd.print("    ");
	if (alarmeon==1) lcd.print("* ");
	lcd.setCursor(11,0);
	lcd.print(temp1);
        lcd.setCursor(0,1);
        lcd.print(jourSemaine(now.dayOfWeek()));
	lcd.print("     ");
	if (now.day() < 10 ) lcd.print("0");
	lcd.print(now.day());
	lcd.print("/");
	if (now.month() < 10 ) lcd.print("0");
	lcd.print(now.month());
	lcd.print("/");
	lcd.print(now.year()%2000);
}

void rheures( int a)
{
	  DateTime now = RTC.now();
	  int heure = now.hour();
	  delay(300);
	  lcd.clear();  	  

          while (digitalRead(b1)==LOW)
	  {
		afficherHeure();//ds tous les sous-menus
		
		lcd.setCursor(0,0);
		lcd.print("Heure : 0-23");
		    	    
		lcd.setCursor(7,1);
		lcd.print(heure);
		bs2 = digitalRead(b2);

		if(bs2 == 0 && bp2 == 1) debutAppui = millis();
		if(bs2 == 1 && bp2 == 1 && (millis() - debutAppui <= APPUI_DELAI)){
			heure++;
			if(heure >= 24) {
				heure =0;
				lcd.setCursor(8,1);
				lcd.print(" ");}
			lcd.setCursor(7,1);
			lcd.print(heure);
			delay(400);}
		if(bs2 == 1 && bp2 == 1 && (millis() - debutAppui >= APPUI_DELAI)){
			heure++;
			if(heure >= 24) {
				heure =0;
				lcd.setCursor(8,1);
				lcd.print(" ");}
			lcd.setCursor(7,1);
			lcd.print(heure);
			delay(100);}

		bp2 = bs2;
	  }
	
	  
           if (a == 1 ) {
		alarmeh = heure;
		RTC.writenvram(0x08, alarmeh);
		delay(200);
		menua();}

	  else {	
		RTC.adjust(DateTime(now.year(), now.month(), now.day(), heure, now.minute(), now.second()));
		delay(200); // rebonds
	 	menuh(); }

}

void rminutes( int a)
{
		  

	  DateTime now = RTC.now();
	  int minute = now.minute();
	  delay(300);
	  lcd.clear();  
	  
	  while (digitalRead(b1)==LOW)
	 {

		afficherHeure();//ds tous les sous-menus		

		lcd.setCursor(0,0);
		lcd.print("Minutes : 0-59");
		    	    
		lcd.setCursor(7,1);
		lcd.print(minute);
		bs2 = digitalRead(b2);

		if(bs2 == 0 && bp2 == 1) debutAppui = millis();
		if(bs2 == 1 && bp2 == 1 && (millis() - debutAppui <= APPUI_DELAI)){
			minute++;
			if(minute >= 60) {
				minute =0;
				lcd.setCursor(8,1);
				lcd.print(" ");}
			lcd.setCursor(7,1);
			lcd.print(minute);
			delay(400);}
		if(bs2 == 1 && bp2 == 1 && (millis() - debutAppui >= APPUI_DELAI)){
			minute++;
			if(minute >= 60) {
				minute =0;
				lcd.setCursor(8,1);
				lcd.print(" ");}
			lcd.setCursor(7,1);
			lcd.print(minute);
			delay(100);}

		bp2 = bs2;
	  }
	  
          if (a == 1 ) {
		alarmem = minute;
		delay(200);
		RTC.writenvram(0x09, alarmem);
		menua();}

	  else {	
		RTC.adjust(DateTime(now.year(), now.month(), now.day(), now.hour(), minute, now.second()));
		delay(200); // rebonds
	 	menuh(); }

}

void rjour()
{
	  DateTime now = RTC.now();
	  int jour = now.day();
	  delay(300);
	  lcd.clear();  
	  
	  while (digitalRead(b1)==LOW)
	 {

		afficherHeure();//ds tous les sous-menus

		lcd.setCursor(0,0);
		lcd.print("Jour : 1-31");
		    	    
		lcd.setCursor(7,1);
		lcd.print(jour);
		bs2 = digitalRead(b2);

		if(bs2 == 0 && bp2 == 1) debutAppui = millis();
		if(bs2 == 1 && bp2 == 1 && (millis() - debutAppui <= APPUI_DELAI)){
			jour++;
			if(jour >= 32) {
				jour =1;
				lcd.setCursor(8,1);
				lcd.print(" ");}
			lcd.setCursor(7,1);
			lcd.print(jour);
			delay(400);}
		if(bs2 == 1 && bp2 == 1 && (millis() - debutAppui >= APPUI_DELAI)){
			jour++;
			if(jour >= 32) {
				jour =1;
				lcd.setCursor(8,1);
				lcd.print(" ");}
			lcd.setCursor(7,1);
			lcd.print(jour);
			delay(100);}

		bp2 = bs2;
	  }
          RTC.adjust(DateTime(now.year(), now.month(), jour, now.hour(), now.minute(), now.second()));
}


void rmois()
{
	  DateTime now = RTC.now();
	  int mois = now.month();
	  delay(300);
	  lcd.clear();  
	  
	  while (digitalRead(b1)==LOW)
	 {
		
		afficherHeure();//ds tous les sous-menus

		lcd.setCursor(0,0);
		lcd.print("Mois : 1-12");
		    	    
		lcd.setCursor(7,1);
		lcd.print(mois);
		bs2 = digitalRead(b2);

		if(bs2 == 0 && bp2 == 1) debutAppui = millis();
		if(bs2 == 1 && bp2 == 1 && (millis() - debutAppui <= APPUI_DELAI)){
			mois++;
			if(mois >= 13) {
				mois =1;
				lcd.setCursor(8,1);
				lcd.print(" ");}
			lcd.setCursor(7,1);
			lcd.print(mois);
			delay(400);}
		if(bs2 == 1 && bp2 == 1 && (millis() - debutAppui >= APPUI_DELAI)){
			mois++;
			if(mois >= 13) {
				mois =1;
				lcd.setCursor(8,1);
				lcd.print(" ");}
			lcd.setCursor(7,1);
			lcd.print(mois);
			delay(100);}

		bp2 = bs2;
	  }
          RTC.adjust(DateTime(now.year(), mois, now.day(), now.hour(), now.minute(), now.second()));
}


void rannee()
{
	  DateTime now = RTC.now();
	  int annee = now.year();
	  delay(300);
	  lcd.clear();  
	  
	  while (digitalRead(b1)==LOW)
	 {

		afficherHeure();//ds tous les sous-menus

		lcd.setCursor(0,0);
		lcd.print("Annee : 00-99");
		    	    
		lcd.setCursor(7,1);
		lcd.print(annee);
		bs2 = digitalRead(b2);

		if(bs2 == 0 && bp2 == 1) debutAppui = millis();
		if(bs2 == 1 && bp2 == 1 && (millis() - debutAppui <= APPUI_DELAI)){
			annee++;
			if(annee >= 100) {
				annee =0;
				lcd.setCursor(8,1);
				lcd.print("   ");}
			lcd.setCursor(7,1);
			lcd.print(2000+annee);
			delay(400);}
		if(bs2 == 1 && bp2 == 1 && (millis() - debutAppui >= APPUI_DELAI)){
			annee++;
			if(annee >= 100) {
				annee =0;
				lcd.setCursor(8,1);
				lcd.print("   ");}
			lcd.setCursor(7,1);
			lcd.print(2000+annee);
			delay(100);}

		bp2 = bs2;
	  }
          RTC.adjust(DateTime(annee+2000, now.month(), now.day(), now.hour(), now.minute(), now.second()));
}

void mmeteo()
{
	delay(300);
	lcd.clear();  

	while(digitalRead(b1)==LOW)
		{

				afficherHeure();//ds tous les sous-menus
				
				temp1 = mesureTemp(pinTh);

				lcd.setCursor(0,0);
				lcd.print("IN : ");
				lcd.print(temp1);
				lcd.setCursor(0,1);
				lcd.print("OUT : ");
				delay(500); 
		}
}

void malarmOnOff()
{
	int nbappui = 0;
	delay(300);
	lcd.clear();
	lcd.print("Prev. ");
	if(alarmeh < 10) lcd.print("0");
	lcd.print(alarmeh);
	lcd.print(":");
	if(alarmem < 10) lcd.print("0");
	lcd.print(alarmem);

	while (digitalRead(b1)==LOW)
 	 {
				afficherHeure();//ds tous les sous-menus
	    bs2 = digitalRead(b2);
	    if((bs2 != bp2) && (bs2 == LOW) ) alarmeon = !alarmeon;
	    bp2 = bs2;
	   if (!alarmeon) {
		 lcd.setCursor(0,1); 
		 lcd.print("Alarme off  "); 
   	 }
	    else if (alarmeon) {
	      lcd.setCursor(0,1); 
	      lcd.print("Alarme on  "); 
		    }
	    delay(100); 
	  }

}

void verifierAlarme()
{
	DateTime now = RTC.now();
	if (alarmem==now.minute() && alarmeh==now.hour())
	{
		while(digitalRead(b2)==LOW)
			{
				afficherHeure();//ds tous les sous-menus				
				nuit =0; // sortie du mode nuit	
				lcd.clear();
				lcd.print("  ALARME !  ");
				lcd.setCursor(0,1);
				lcd.setCursor(6,1);
				if(alarmeh < 10) lcd.print("0");
				lcd.print(alarmeh);
				lcd.print(":");
				if(alarmem < 10) lcd.print("0");
				lcd.print(alarmem);
				digitalWrite(buzzer,HIGH);
			       digitalWrite(led,HIGH);				
				delay(400);
				digitalWrite(buzzer,LOW);
				 digitalWrite(led, LOW); 
				delay(400);
				 digitalWrite(led, HIGH); 				
				digitalWrite(buzzer,HIGH);
				delay(400);
				digitalWrite(buzzer,LOW);
				digitalWrite(led, LOW); 
				delay(1500);
					//boum !!!
			}
		delay(500);
		malarmOnOff(); 
		lcd.clear();
		lcd.print("Debout!");
		delay(5000); 
	}
}

void mnuit()
{
	delay(300);
	lcd.clear();
	lcd.print("Mode nuit");

	while (digitalRead(b1)==LOW)
 	 {
	    afficherHeure();//ds tous les sous-menus
	    bs2 = digitalRead(b2);
	    if((bs2 != bp2) && (bs2 == LOW) ) nuit = !nuit;
	    bp2 = bs2;
	   if (nuit == 0) {
		 lcd.setCursor(0,1); 
		 lcd.print(" off  "); 
   	 }
	    else if (nuit == 1) {
	      lcd.setCursor(0,1); 
	      lcd.print(" on  "); 
		    }
	    delay(100); 
	  }

}

void afficherMenu()
{
	delay(300); // rebonds bouton : voir avec le condo si utile
	int nbappui = 0;
	lcd.clear();  
	lcd.print("Menu"); 

	while (digitalRead(b1)==LOW)
	  {
	    afficherHeure();//ds tous les sous-menus
	 
	
	    bs2 = digitalRead(b2);
	    if((bs2 != bp2) && (bs2 == LOW) ) nbappui ++;
	    bp2 = bs2;


	    if(nbappui== 0) {
	      lcd.clear();
	      lcd.print("     Menu     ");
	      lcd.setCursor(0,1); 
	      lcd.print(" Meteo"); 
	    }
	    else if (nbappui==1) {
	      lcd.clear();
	      lcd.print("     Mode     ");
	      lcd.setCursor(0,1); 
	      lcd.print(" Nuit");  
	    }
	    else if (nbappui==2) {
	      lcd.clear();
	      lcd.print("     Menu     ");
	      lcd.setCursor(0,1); 
	      lcd.print(" Alarme"); 
	    }
	    else if (nbappui==3) {
	      lcd.clear();
	      lcd.print("     Menu     ");
	      lcd.setCursor(0,1); 
	      lcd.print(" Heure"); 
	    }
	    else if (nbappui==4) {
	      lcd.clear();
	      lcd.print("     Retour    ");  
	    }    
	    delay(150); 
	  }

	  switch(nbappui)
	  {
	  case 0: 
	    	mmeteo(); 
	    break;
	  case 1: 
	   	mnuit(); 
	    break;
	  case 2: 
	   	menua();
	    break;
	  case 3: 
	    	menuh();
	    break;
	  }
}



void menuh()
{
	delay(300); // rebonds bouton : voir avec le condo si utile
	int nbappui = 0;
	lcd.clear();  
	lcd.print("Menu"); 

	while (digitalRead(b1)==LOW)
	  {
			afficherHeure();//ds tous les sous-menus
	
	    bs2 = digitalRead(b2);
	    if((bs2 != bp2) && (bs2 == LOW) ) nbappui ++;
	    bp2 = bs2;

	    if (nbappui==0) {
	      lcd.clear();
	      lcd.print("     Regler     ");
	      lcd.setCursor(0,1); 
	      lcd.print(" Heure");  
	    }
	    else if (nbappui==1) {
	      lcd.clear();
	      lcd.print("     Regler     ");
	      lcd.setCursor(0,1); 
	      lcd.print(" Minutes");  
	    }
	    else if (nbappui==2) {
	      lcd.clear();
	      lcd.print("     Regler     ");
	      lcd.setCursor(0,1); 
	      lcd.print(" Jour");  
	    }
	    else if (nbappui==3) {
	      lcd.clear();
	      lcd.print("     Regler     ");
	      lcd.setCursor(0,1); 
	      lcd.print(" Mois");  
	    }
	    else if (nbappui==4) {
	      lcd.clear();
	      lcd.print("     Regler     ");
	      lcd.setCursor(0,1); 
	      lcd.print(" Annee");  
	    }
	    else if (nbappui==5) {
	      lcd.clear();
	      lcd.print("     Retour    ");  
	    }    
	    delay(150); 
	  }

	  switch(nbappui)
	  {
	  case 0: 
	    rheures(0); 
	    break;
	  case 1: 
	    rminutes(0);  
	    break;
	  case 2: 
	    rjour(); 
	    break;
	  case 3: 
	    rmois(); 
	    break;
          case 4: 
	    rannee(); 
	    break;  
	  }
}

void menua()
{
	delay(300); // rebonds bouton : voir avec le condo si utile
	int nbappui = 0;
	lcd.clear();  
	lcd.print("Menu"); 

	while (digitalRead(b1)==LOW)
	  {
	
            afficherHeure();//ds tous les sous-menus
	    bs2 = digitalRead(b2);
	    if((bs2 != bp2) && (bs2 == LOW) ) nbappui ++;
	    bp2 = bs2;


	  if (nbappui==0) {
	      lcd.clear();
	      lcd.print("     Regler     ");
	      lcd.setCursor(0,1); 
	      lcd.print(" Alarme heures");  
	    }
	    else if (nbappui==1) {
	      lcd.clear();
	      lcd.print("     Regler     ");
	      lcd.setCursor(0,1); 
	      lcd.print(" Alarme minutes"); 
	    }
	    else if (nbappui==2) {
	      lcd.clear();
	      lcd.print("     Regler     ");
	      lcd.setCursor(0,1); 
	      lcd.print("Alarme On / Off");  
	    }
	    else if (nbappui==3) {
	      lcd.clear();
	      lcd.print("     Retour    ");  
	    }    
	    delay(150); 
	  }

	  switch(nbappui)
	  {
	  case 0: 
	    rheures(1); 
	    break;
	  case 1: 
	    rminutes(1); 
	    break;
	  case 2: 
	    malarmOnOff(); 
	    break;
	  }
}




