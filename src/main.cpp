
#include <SFML/Graphics.hpp>
#include <iostream>  
#include <string>     
#include <thread>
#include <chrono>
#include <iostream>
#include <array>

#define delay 1ms						// Delay tussen stappen 
#define mmperrotationstepperx 1			// Aantal mm per volledige rotatie van de stapper van de x as
#define mmperrotationsteppery 1			// Aantal mm per volledige rotatie van de stapper van de y as
#define pixelspermm 2					// Aantal pixels per mm (simulator)
#define maxrow1 596						// Maximale aantal rijen voor rij 1 (simulator)
#define maxrow2 420						// Maximale aantal rijen voor rij 2 (simulator)
#define mmperrotationstepper1 1			// Aantal mm per volledige rotatie van de stapper van rij 1 (simulator)
#define mmperrotationstepper2 1			// Aantal mm per volledige rotatie van de stapper van rij 2 (simulator)

void findLine();
void loadStart();
void loadLine();
void run(bool laser);
std::array<bool, 4> getOutputstepper(int nextStep);
void setOutput(bool c1, bool c2, bool c3, bool c4, bool c5, bool c6, bool c7, bool c8, bool c9);
void Simulator();
void setInput(bool c1, bool c2, bool c3, bool c4, bool c5, bool c6, bool c7, bool c8, bool c9);
void closeWindow();
int calcrotation(bool c1, bool c2, bool c3, bool c4, int stepper);
void moverow(double angle, int row);
void init();
void drawwindow();
void uselaser();

int currentanglestepper1 = 0;			// huidige positie van stepper 1 (simulator)
int currentanglestepper2 = 0;			// huidige positie stepper 2 (simulator)
int positionrow1 = 0;					// plek van rij 1 (simulator)
int positionrow2 = 0;					// plek van rij 2 (simulator)
int pixels_used = 0;					// aantal zwarte pixels die ingebruik zijn (simulator)

int map[420][594];						// dubbele array die staan voor de pixels van de foto met de kleur als waarde
int startvector[2];						// begin punt van de lijn
int endvector[2];						// eind punt van de lijn
int instructions[2];					// aantal rotaties die de stepper zal moeten maken 
int stepperxlocation = 1;				// huidige positie van stepper 1
int stepperylocation = 1;				// huidige positie van stepper 2


// Objecten voor de simulator
sf::RectangleShape pixel[10000];								

sf::RenderWindow window(sf::VideoMode(1200, 800), "simulator");
sf::RectangleShape stepper1(sf::Vector2f(200, 200));
sf::RectangleShape stepper2(sf::Vector2f(200, 200));
sf::RectangleShape stepper1shaft(sf::Vector2f(4, 160));
sf::RectangleShape stepper2shaft(sf::Vector2f(4, 160));
sf::RectangleShape paper(sf::Vector2f(420, 594));
sf::RectangleShape row1(sf::Vector2f(16, 610));
sf::RectangleShape row2(sf::Vector2f(420, 16));
sf::RectangleShape laser(sf::Vector2f(30, 30));



int main()
{
	Simulator();														// start de simulatie
	setOutput(true,false,false,false,true,false,false,false,false);		// eerste uitput zodat de steppers allemaal goed staan
	map[100][150] = 255;												// hard coded map set als voorbeeld
	map[20] [400] = 255;

	findLine();															// zoekt de lijn uit de foto
	loadStart();														// laad het startpunt in de instructie set
	run(false);															// voert de instructies uit met de laser uit
	loadLine();															// laad berekend de instructies voor de lijn en zet deze in de instructie set
	run(true);															// voert de instructies uit met de laser aan

	while (1) {
																		// oneindige loop als alles klaar is zodat de simulator niet afsluit
	}

	return 0;
}

void findLine() {														// zoek de lijn door:
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

void loadStart() {
	instructions[0] = startvector[0] / mmperrotationstepperx;			// berekend het aantal rotaties om naar het begin punt te komen
	instructions[1] = startvector[1] / mmperrotationsteppery;			// berekend het aantal rotaties om naar het begin punt te komen
}

void loadLine() {
	instructions[0] = (endvector[0] - startvector[0]) / mmperrotationstepperx;	// berekend het aantal rotaties om naar het eind punt te komen
	instructions[1] = (endvector[1] - startvector[1]) / mmperrotationsteppery;	// berekend het aantal rotaties om naar het eind punt te komen
}

void run(bool laser) {									// zorgt dat de instructies worden uitgevoerd
	using namespace std::this_thread;					
	using namespace std::chrono; 

	double stepsx = (instructions[0] * 8);				// berekend het aantal stappen voor de x as
	double stepsy = (instructions[1] * 8);				// berekend het aantal stappen voor de y as
	int turnsidex;										
	int turnsidey;

	if (stepsx < 0) {									// kijkt of de stappen negatief of posietief zijn
		stepsx = stepsx * -1;							// negatieve stappen worden positief gemaakt
		turnsidex = -1;									// de rotatie richting word naar links 
	}
	else {
		turnsidex = 1;									// de rotatie richting word naar rechts
	}

	if (stepsy < 0) {									// kijkt of de stappen negatief of posietief zijn
		stepsy = stepsy * -1;							// negatieve stappen worden positief gemaakt
		turnsidey = -1;									// de rotatie richting word naar boven 
	}
	else {
		turnsidey = 1;									// de rotatie richting word naar onder
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
	}
}

std::array<bool, 4> getOutputstepper(int nextStep) {	// Switch die een 4 bools returned aan de hand van de ingevoerde waarde
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

void setOutput(bool c1, bool c2, bool c3, bool c4, bool c5, bool c6, bool c7, bool c8, bool c9) {
	setInput(c1, c2, c3, c4, c5, c6, c7, c8, c9);	// set de pint output voor de simulator
}

void Simulator()
{
	init();						// laad alle objecten
	drawwindow();				// geef alle objecten weer op het scherm 
}

void closeWindow() {
	window.close();				// mogelijkheid de simulator af te sluiten vanaf buitenaf
}
void setInput(bool c1, bool c2, bool c3, bool c4, bool c5, bool c6, bool c7, bool c8, bool c9) { //set de pin input voor de simulator
	double angle;

	if (c9) {
		uselaser();
	}

	angle = calcrotation(c1, c2, c3, c4, 2);
	moverow(angle, 2);
	stepper1shaft.rotate(angle);

	angle = calcrotation(c5, c6, c7, c8, 1);
	moverow(angle, 1);
	stepper2shaft.rotate(angle);

	drawwindow();
}

void init() {											// maakt de vaste vormen aan voor de simulator
	stepper1.setFillColor(sf::Color::Green);
	stepper1.setPosition(sf::Vector2f(100, 100));


	stepper2.setFillColor(sf::Color::Blue);
	stepper2.setPosition(sf::Vector2f(100, 500));


	stepper1shaft.setFillColor(sf::Color::Black);
	stepper1shaft.setOutlineThickness(4);
	stepper1shaft.setOutlineColor(sf::Color::Red);
	stepper1shaft.setPosition(sf::Vector2f(198, 120));
	stepper1shaft.move(sf::Vector2f(0, 80));
	stepper1shaft.rotate(180);


	stepper2shaft.setFillColor(sf::Color::Black);
	stepper2shaft.setOutlineThickness(4);
	stepper2shaft.setOutlineColor(sf::Color::Red);
	stepper2shaft.setPosition(sf::Vector2f(198, 520));
	stepper2shaft.move(sf::Vector2f(0, 80));
	stepper2shaft.rotate(180);


	paper.setFillColor(sf::Color::White);
	paper.setPosition(sf::Vector2f(600, 120));


	row1.setFillColor(sf::Color::Green);
	row1.setPosition(sf::Vector2f(584, 112));


	row2.setFillColor(sf::Color::Blue);
	row2.setPosition(sf::Vector2f(600, 112));


	laser.setFillColor(sf::Color::Red);
	laser.setPosition(sf::Vector2f(586, 106));
}
void drawwindow() {											// geeft alle objecten weer op het scherm
	window.clear();
	window.draw(stepper1);
	window.draw(stepper2);
	window.draw(stepper1shaft);
	window.draw(stepper2shaft);
	window.draw(paper);

	for (int i = 0; i < pixels_used; i++) {
		window.draw(pixel[i]);
	}

	window.draw(row2);
	window.draw(row1);
	window.draw(laser);
	window.display();
}
void uselaser() {											// simuleerd de laser en zorgt voor een zwarte stip op het papier
	pixel[pixels_used].setSize(sf::Vector2f(1, 1));
	pixel[pixels_used].setFillColor(sf::Color::Black);
	pixel[pixels_used].setPosition((laser.getPosition().x + 14), (laser.getPosition().y + 14));
	pixels_used++;
}
void moverow(double angle, int row) {						// beweegt de x of de y as
	double currentposition;
	int currentmax;
	double mmperrotation;
	double movemm;
	double movepixels;

	if (row == 1) {
		currentposition = positionrow1;
		currentmax = maxrow1;
		mmperrotation = mmperrotationstepper1;
	}
	else {
		currentposition = positionrow2;
		currentmax = maxrow2;
		mmperrotation = mmperrotationstepper2;
	}

	movemm = ((angle / 360)*mmperrotation);

	if ((currentposition + movemm) >= currentmax) {
		movemm = 0;
	}
	else {
		movemm = movemm;
	}
	
	movepixels = movemm*pixelspermm;
	currentposition = currentposition + movepixels;

	if (row == 1) {
		positionrow1 = currentposition;
		row2.setPosition(sf::Vector2f(row2.getPosition().x, (row2.getPosition().y + movepixels)));
		laser.setPosition(sf::Vector2f(laser.getPosition().x, (laser.getPosition().y + movepixels)));
	}
	else {
		positionrow2 = currentposition;
		laser.setPosition(sf::Vector2f((laser.getPosition().x + movepixels), laser.getPosition().y));
	}
}

int calcrotation(bool c1, bool c2, bool c3, bool c4, int stepper) {				// berekent de rotatie in graden
	int angle;
	int returnangle;
	int currentangle;

	if (stepper == 1) {
		currentangle = currentanglestepper1;
	}
	else {
		currentangle = currentanglestepper2;
	}

	if (c1 == true && c2 == false && c3 == false && c4 == false) {
		angle = 0;
	}
	else if (c1 == true && c2 == true && c3 == false && c4 == false) {
		angle = 45;
	}
	else if (c1 == false && c2 == true && c3 == false && c4 == false) {
		angle = 90;
	}
	else if (c1 == false && c2 == true && c3 == true && c4 == false) {
		angle = 135;
	}
	else if (c1 == false && c2 == false && c3 == true && c4 == false) {
		angle = 180;
	}
	else if (c1 == false && c2 == false && c3 == true && c4 == true) {
		angle = 225;
	}
	else if (c1 == false && c2 == false && c3 == false && c4 == true) {
		angle = 270;
	}
	else if (c1 == true && c2 == false && c3 == false && c4 == true) {
		angle = 315;
	}
	else {
		angle = currentangle;
	}

	if (angle - currentangle < 180 && angle - currentangle > -180) {
		returnangle = angle - currentangle;
	}
	else if (angle - currentangle > 0) {
		returnangle = (angle - 360) - currentangle;
	}
	else {
		returnangle = (360 - currentangle) + angle;
	}

	if (stepper == 1) {
		currentanglestepper1 = angle;
	}
	else {
		currentanglestepper2 = angle;
	}

	return (double)returnangle;
}
