#include <SFML/Graphics.hpp>
#include <iostream>  
#include <string>     
#include <thread>
#include <chrono>
#include <iostream>
#include <array>

#include "Gpio/Gpio.hpp"      
      
#define delay 1ms						// Delay tussen stappen 
#define mmperrotationstepperx 1			// Aantal mm per volledige rotatie van de stapper van de x as
#define mmperrotationsteppery 1			// Aantal mm per volledige rotatie van de stapper van de y as

void moveTo(int x, int y, bool laser);
void findLine();
void loadStart();
void loadLine();
void run(bool laser);
std::array<bool, 4> getOutputstepper(int nextStep);
void setOutput(bool c1, bool c2, bool c3, bool c4, bool c5, bool c6, bool c7, bool c8, bool c9);

int map[420][594];						// dubbele array die staan voor de pixels van de foto met de kleur als waarde
int currentvector[2] = { 0,0 };			// huidige locatie van de laser
int startvector[2];						// begin punt van de lijn
int endvector[2];						// eind punt van de lijn
int instructions[2];					// aantal rotaties die de stepper zal moeten maken 
int stepperxlocation = 1;				// huidige positie van stepper 1
int stepperylocation = 1;				// huidige positie van stepper 2


Plotter::Plotter()
{
	setOutput(true,false,false,false,true,false,false,false,false);		// eerste uitput zodat de steppers allemaal goed staan
}

void Plotter::moveTo(int x, int y, bool laser) {
	instructions[0] = 0;
	instructions[1] = 0;
	instructions[0] = (x - currentvector[0]) / mmperrotationstepperx;	// berekend het aantal rotaties om naar het aangegeven punt te komen
	instructions[1] = (y - currentvector[1]) / mmperrotationsteppery;	// berekend het aantal rotaties om naar het aangegeven punt te komen
	run(laser);
	currentvector[0] = x;
	currentvector[1] = y;
} 
      
void Plotter::findLine() {														// zoek de lijn door:
	int i, j;
	bool stop = false;
	for (i = 0; i < 594 && stop == false; i++) {					    // alle pixels van links naar rechts
		for (j = 0; j < 420 && stop == false; j++) {					// en van boven naar onder door te lopen
			if (map[j][i] == 255) {
				startvector[0] = j;
				startvector[1] = i;
				stop = true;											// eerste zwarte pixel word opgeslagen als begin punt
			}
		}																
	}
	stop = false;
	for (i = 594; i >= 0 && stop == false; i--) {						// alle pixels van rechts naar links
		for (j = 420; j >= 0 && stop == false; j--) {					// en van onder naar boven door te lopen
			if (map[j][i] == 255) {
				endvector[0] = j;
				endvector[1] = i;
				stop = true;											// eerste zwarte pixel word opgeslagen als begin punt
			}
		}
	}

}

void Plotter::loadStart() {
	instructions[0] = startvector[0] / mmperrotationstepperx;			// berekend het aantal rotaties om naar het begin punt te komen
	instructions[1] = startvector[1] / mmperrotationsteppery;			// berekend het aantal rotaties om naar het begin punt te komen
}

void Plotter::loadLine() {
	instructions[0] = (endvector[0] - startvector[0]) / mmperrotationstepperx;	// berekend het aantal rotaties om naar het eind punt te komen
	instructions[1] = (endvector[1] - startvector[1]) / mmperrotationsteppery;	// berekend het aantal rotaties om naar het eind punt te komen
}

void Plotter::run(bool laser) {									// zorgt dat de instructies worden uitgevoerd
	using namespace std::this_thread;					
	using namespace std::chrono; 

	double stepsx = (instructions[0] * 8);				// berekend het aantal stappen voor de x as
	double stepsy = (instructions[1] * 8);				// berekend het aantal stappen voor de y as
	int turnsidex;										
	int turnsidey;

	if (stepsx < 0) {									// kijkt of de stappen negatief of posietief zijn
		stepsx = stepsx * -1;							// negatieve stappen worden positief gemaakt
		turnsidex = -1;									// de rotatie richting word naar links
      		gpio::set_value(9,0);
	}
	else {
		turnsidex = 1;									// de rotatie richting word naar rechts
		gpio::set_value(9,1);
      	}

	if (stepsy < 0) {									// kijkt of de stappen negatief of posietief zijn
		stepsy = stepsy * -1;							// negatieve stappen worden positief gemaakt
		turnsidey = -1;									// de rotatie richting word naar boven 
		gpio::set_value(8,0);
        }
	else {
		turnsidey = 1;									// de rotatie richting word naar onder
		gpio::set_value(8,1);
        }

	double slopex = stepsx / stepsy;					// berekend hoeveel stappen x in relatie tot y
	double slopey = stepsy / stepsx;					// berekend hoeveel stappen y in relatie tot x
	double slopexcount = slopex;						// zet het aantal benodigde stappen in aparte variable
	double slopeycount = slopey;						// zet het aantal benodigde stappen in aparte variable
	int avrstep;										// gemiddelde aantal stappen dat gemaakt moet worden
	bool stepx;											// geeft voor x aan of er een stap gemaakt moet worden of niet
	bool stepy;											// geeft voor y aan of er een stap gemaakt moet worden of niet
	std::array<bool, 4> boolarrayx;						// tijdelijke opslag voor stepper output voor de x as
	std::array<bool, 4> boolarrayy;						// tijdelijke opslag voor stepper output voor de y as

	avrstep = ((stepsx + stepsy)) / 2;					// bereken het gemiddelde aantal stappen dat gemaakt moet worden

	for (int i = 0; i < avrstep; i++) {					// loopt het gemiddelde aantal stappen dat gemaakt moet worden keer door de code
		
		if (slopexcount >= 1) {							// als slopexcount >= 1 haal er dan 1 vanaf en zet stapx op true zodat deze gaat draaien
			slopexcount = slopexcount - 1;
			stepx = true;
		}
		else {											// als de slopexcount nog < is dan 1 tell slopex er bij op en zet/laat stepx op false zodat deze niet gaat draaien
			stepx = false;
			slopexcount = slopexcount + slopex;
		}
		if (slopeycount >= 1) {							// als slopeycount > 1 haal er dan 1 vanaf en zet stapy op true zodat deze gaat draaien
			slopeycount = slopeycount - 1;
			stepy = true;
		}
		else {											// als de slopeycount nog < is dan 1 tell slopey er bij op en zet/laat stepy op false zodat deze niet gaat draaien
			stepy = false;
			slopeycount = slopeycount + slopey;
		}
		if (stepx) {											// als stepx true is moet stepper x 1 stap draaien
			gpio::set_value(11,1);
      			stepperxlocation = stepperxlocation + turnsidex;	// pas de locatie van de stepper aan 
			
			if(stepperxlocation >= 9) {							// check voor als de stepper buiten het berijk van 0-8 gaat
				stepperxlocation = 1;
			}
			else if (stepperxlocation <= 0) {
				stepperxlocation = 8;
			}

			boolarrayx = getOutputstepper(stepperxlocation);    // stuur de nieuwe locatie naar getOutputstepper en ontvang de pin output waarden voor de stepper
      		}
		if (stepy) {											// als stepy true is moet stepper x 1 stap draaien
			gpio::set_value(7,1);
      			stepperylocation = stepperylocation + turnsidey;	// pas de locatie van de stepper aan 

			if (stepperylocation >= 9) {
				stepperylocation = 1;							// check voor als de stepper buiten het berijk van 0-8 gaat
			}
			else if (stepperylocation <= 0) {
				stepperylocation = 8;
			}

			boolarrayy = getOutputstepper(stepperylocation);	// stuur de nieuwe locatie naar getOutputstepper en ontvang de pin output waarden voor de stepper
      		}
		
		setOutput(boolarrayx[0], boolarrayx[1], boolarrayx[2], boolarrayx[3], boolarrayy[0], boolarrayy[1], boolarrayy[2], boolarrayy[3], laser); // set the nieuwe pin output
		sleep_for(delay);	// wacht x aantal ms zodat de stepper tijd heeft om te draaien 
		gpio::set_value(11,0);
      		gpio::set_value(7,0);
      	}
}

std::array<bool, 4> Plotter::getOutputstepper(int nextStep) {	// Switch die een 4 bools returned aan de hand van de ingevoerde waarde
	std::array<bool, 4> boolarray;
	switch (nextStep) {
	case 1: boolarray[0] = true; boolarray[1] = false; boolarray[2] = false; boolarray[3] = false; break;
	case 2: boolarray[0] = true; boolarray[1] = true; boolarray[2] = false; boolarray[3] = false; break;
	case 3: boolarray[0] = false; boolarray[1] = true; boolarray[2] = false; boolarray[3] = false; break;
	case 4: boolarray[0] = false; boolarray[1] = true; boolarray[2] = true; boolarray[3] = false; break;
	case 5: boolarray[0] = false; boolarray[1] = false; boolarray[2] = true; boolarray[3] = false; break;
	case 6: boolarray[0] = false; boolarray[1] = false; boolarray[2] = true; boolarray[3] = true; break;
	case 7: boolarray[0] = false; boolarray[1] = false; boolarray[2] = false; boolarray[3] = true; break;
	case 8: boolarray[0] = true; boolarray[1] = false; boolarray[2] = false; boolarray[3] = true; break;
	}
		return boolarray;
}

void Plotter::setOutput(bool c1, bool c2, bool c3, bool c4, bool c5, bool c6, bool c7, bool c8, bool c9) {
	// 
}

