#include "Faltung.h"
//Tolga Akkiraz
//Abdullah Sahin

// Bewertung:  7  +  3  =   10
// -----------------------------------------------------------------------------
// Maximum:    7  +  3  =   10
// -----------------------------------------------------------------------------
// @Error: Markiert Fehler mit Punktabzug.
// @Warning: Markiert Fehler, die zukünftig zu Punktabzug führen können.
// @Note: Markiert Anmerkungen und Hinweise.




// Main function
int pragma::main(int argc, char *argv[])
{
	std::cout << "Sehen in Mensch und Maschine Aufgabe 3\n\n";

	// Faltungsobjekt
	Faltung faltung;

	// Name des Eingabebildes
	pragma::string bildNameL("../../images/Kante_22.jpg");
	if (argc > 1)
		bildNameL = argv[1];

	// Winkel (Umwandlung von deg in rad)
	pragma::REAL winkelL = pragma::PI / 4.0;
	if (argc > 2)
	{
			pragma::REAL gradL = std::atof(argv[2]);
			winkelL = pragma::PI * gradL / 180.0;
	}

	// Bild einlesen
	pragma::Image::ByteImagePointer bildL;
	bildL->read(bildNameL);

	// Bild anzeigen
	pragma::Graphics::WindowPointer fensterL;
	fensterL->image(bildL);

	std::getchar();

	// ====== Aufgabe 3.1 ======
	//Faltungsobjekt erstellen
	Faltung faltungL;

	// Sobelfilter erstellen
	std::cout << "Creating Sobel filters..." << std::flush;
	// TODO:DONE

	Filter sobelFilterX = Filter::getSobelX();
	Filter sobelFilterY = Filter::getSobelY();

	std::cout << " done\n";

	// bildL mit Sx falten (beliebige Randbedingung)
	std::cout << "Convolving with vertical edge detector..." << std::flush;
	// TODO: DONE
	pragma::Image::RealImagePointer xGradientsL = faltung.falte(bildL,sobelFilterX, 'w');
	fensterL->image(xGradientsL);
	std::cout << " done\n";
	getchar();

	// bildL mit Sy falten (beliebige Randbedingung)
	std::cout << "Convolving with horizontal edge detector..." << std::flush;
	// TODO: DONE
	pragma::Image::RealImagePointer yGradientsL = faltung.falte(bildL, sobelFilterY, 'w');
	fensterL->image(yGradientsL);
	std::cout << " done\n";
	getchar();

	// Richtungsunabhängige Kantenstärke ermitteln
	std::cout << "Richtungsunabhaengige Kantenstaerke ermitteln..." << std::flush;
	// TODO: DONE
	pragma::Image::RealImagePointer ungerichtetL = faltungL.getKSUngerichtet(xGradientsL, yGradientsL);
	fensterL->image(ungerichtetL);
	std::cout << " done\n";
	getchar();

	// Richtungsabhängige Kantenstärke für bestimmte Richtung ermitteln 
	std::cout << "Richtungsabhaengige Kantenstaerke ermitteln..." << std::flush;
	// TODO: DONE
	pragma::Image::RealImagePointer gerichtetL = faltungL.getKSGerichtet(xGradientsL, yGradientsL, winkelL);
	fensterL->image(gerichtetL);
	std::cout << " done\n";
	getchar();

	// ====== Aufgabe 3.3 ======
	// Verschiedene Gabor laden um Bild damit zu falten

	// Name des ersten Gabors
	pragma::string gabor1NameL("../../gabors/kernel_l4_d7_real.txt");
	pragma::Image::RealImagePointer gabor1L;
	gabor1L->read(gabor1NameL);

	// Stoppuhr-Objekt anlegen um Faltungszeit zu messen
	Stopwatch stopperL(Stopwatch::REAL_TIME);
	std::cout << gabor1NameL << std::endl;
	std::cout << "Beginne Messung " << gabor1NameL << ": " << std::flush;



	// Bild mit Gaborfilter falten
	// TODO: DONE
	Filter gabor1F(gabor1L, gabor1L->xResolution() / 2, gabor1L->yResolution() / 2);
	stopperL.start();
	pragma::Image::RealImagePointer gaborI1 = faltungL.falte(bildL, gabor1F, 'w');
	stopperL.stop();
	stopperL.report(std::cout);
	std::cout << std::endl;
	std::cout << "X: " << gabor1L->xResolution() << " Y: " << gabor1L->yResolution() << std::endl;


	std::getchar();


	return 0;
}
