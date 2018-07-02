/*
Abdullah Sahin
Tolga Akkiraz
*/
#include <pragma.h>
#include <iostream>
#include "ImageManipulator.h"

/*
6.1.2)
******
Wir sind strategisch wie folgt vorgegangen:
-Für jede nicht triviale Permutation von Größenskalierung und Rauschstärke ein Bild erzeugt.
	-Skalierungsfaktor 	von 0.1 bis -2.0 mit 0.1  Schritten (18 Iterattionen)
	-Rauschstärke 		von 0.0 bis  1.0 mit 0.05 Schritten (20 Iterationen)

-Somit hatten wir 360 Bilder zum auswerten. (Bei Nachfrage per Mail nachreichbar.)

Auswertung:
Faktor für Gößenskalierung     	Rauschstärke
x>1.8							-- keine --
1.8								0.15, 0.20
1.7								-- keine --
1.6								0.05, 0.10, 0.15, 0.20
1.5								0.05, 0.10
1.4								0.05
1.3								0.00, 0.05, 0.10
1.2								0.00
1.1								0.00, 0.15, 0.20
1.0								0.00, 0.05, 0.20
0.9								0.00, 0.05, 0.10
0.8								0.00
0.7								0.00, 0.05
0.6								0.00, 0.05
0.5								0.00, 0.05
x<0.5							-- keine --

Erkenntnis:
Die minimale Skalierungsfaktor ist 0.5, die maximal ist 1.8, 
wobei dies einem Zufall geschuldet sein kann.
Die maximale Rauschstärke, bei dem die Nase auffindbar war, ist 0.20.
Bei der Rauschstärke ist keine Linearität zu erkennen, 
da diese einem pseudo-zufälligem Prozess unterworfen ist.
Zwischen 0.5 - 1.3 Skalierungsfaktor ist die Nase 
mit keinem Rauschen noch wiederfindbar.
Daraus erschließt sich, dass der Intervall [0.5, 1.3] eine untere bzw.
obere Schranke für den Skalierungsfaktor sein kann.
Bei der Rauschstärke ist eine Beschränkung nicht so leicht ersichtlich, 
jedoch ist die Nase mit einer Rauschstärke im Intervall [0.00, 0.05]
in den meisten Skalierungsfaktoren wiederfindbar.
Wobei man im Hinterkopf behalten muss, 
dass die durch zufügen eines Rausches enstehende Artefakte 
unglücklich positioniert werden könnten 
und somit die obige Auswertung manipulieren.

Fazit:
Beim Rauschen höher als 0.05 können die Ergebnisse stark gefälscht werden,
da diese an zufällige Positionen Werte von 0 oder 255 einträgt. Dies hat
zufolge, dass der globale Extrema verschoben werden kann und somit
das Template falsch gematcht wird. Ebenso kann es einen positiven Effekt haben, 
in dem der globale Wert zufällig an der gewünschten Position ensteht.

Beim Skalieren des Bildes steht der Faktor mit dem Template im Zusammenspiel.
Ebenso darf das Bild nicht zu groß werden, da die Berechnung des erwünschten globalen Extremas
über die Größe des Templates hinausgeht.
Bei zu kleinen Bildern ensteht ein globales Extrema in der Umgebung des gewünschtens Positiones,
da diese schon in die Berechnung einfliesen.

6.1.3)
Ohne Beachtung der Bildstörung verspricht ein größeres Template eine bessere Wiederfindungs-Quote.
Ein kleines Template, wie 3x3 enthält nur 9 Pixel, also fließen nur 9 Werte in eine Berechnung.
Bei so einer niedrigen Anzahl ist die Wahrscheinlichkeit hoch, dass andere Bereiche des Bildes
die selben Ergebnisse liefern.
Je größer das Template, desto unterschiedlicher - ausgehend von einem unebenen Template -
sind die Werte und desto unwahrscheinlicher ist, das diese Werte in einem andere Bereich des Bildes
die selben Ergebnisse liefern.

*/
// Faltung
pragma::Image::RealImagePointer falte(const pragma::Image::RealImagePointer image, const pragma::Image::RealImagePointer filter)
{
	// Das gefaltete Bild
	pragma::Image::RealImagePointer resultL(image->xResolution(), image->yResolution());
	resultL->init(0);

	for (unsigned int iX = 0; iX < image->xResolution(); iX++)
	{
		for (unsigned int iY = 0; iY < image->yResolution(); iY++)
		{
			pragma::REAL currValue = 0.0;
			pragma::REAL normValue = 0.0;
			for (unsigned int fX = filter->xResolution()-1; fX > 0 ; fX--)
			{
				for (unsigned int fY = filter->yResolution()-1; fY > 0; fY--)
				{
					
					int tempX = iX + fX - 1;
					int tempY = iY + fY - 1;

					// add image resolution to prevent negative values
					tempX = (tempX + image->xResolution()) % image->xResolution();
					tempY = (tempY + image->yResolution()) % image->yResolution();

					pragma::REAL tempValue = image->pixel(tempX, tempY);
					normValue += pragma::sqr(tempValue);
					currValue += tempValue * filter->pixel(fX, fY);
				}
			}
			pragma::REAL normedValue = currValue / std::sqrt(normValue);
			resultL->pixel(iX, iY, normedValue);
		}
	}

	return resultL;
}
// ========== Main function ==========
int pragma::main(int argc, char *argv[])
{
	std::cout << "Vorlesung Sehen in Mensch und Maschine Aufgabe 6\n\n";

	// Variablen anlegen (und default-Werte festlegen) fuer:
	// Name des Eingabebilds, Name des Testbilds, Position des Ausschnitts im Eingabebild, Breite und Hoehe des Ausschnitts, Skalierung und Rauschstaerke
	string imageName = "../images/face0.tiff";
	string testImageName = "../images/face1.tiff";
	// TODO: done!

	int posSubImageX = 60;
	int posSubImageY = 58;
	int widthSubImage = 16;
	int heightSubImage = 20;
	pragma::REAL scaleSubImage = 1.0;
	pragma::REAL noiseStrength = 0.0;

	// Parameter ueber Benutzereingaben (cin) oder Kommandozeile einlesen
	// TODO: done!

	if (argc != 9)
	{
		std::cout
			<< "Usage:"
			<< std::endl
			<< argv[0]
			<< " <InputImage> <TestImage> <X-Pos SubImage> <Y-Pos SubImage> <Width SubImage> <Height SubImage> <Scalefactor> <NoiseStrength>\n";

		std::cout
			<< "** Demo:"
			<< std::endl
			<< argv[0] << " "
			<< imageName << " "
			<< testImageName << " "
			<< posSubImageX << " "
			<< posSubImageY << " "
			<< widthSubImage << " "
			<< heightSubImage << " "
			<< scaleSubImage << " "
			<< noiseStrength << std::endl;
		return -1;
	}

	imageName = pragma::string(argv[1]);
	testImageName = pragma::string(argv[2]);
	posSubImageX = atoi(argv[3]);
	posSubImageY = atoi(argv[4]);
	widthSubImage = atoi(argv[5]);
	heightSubImage = atoi(argv[6]);
	scaleSubImage = atof(argv[7]);
	noiseStrength = atof(argv[8]);

	pragma_assert(scaleSubImage>0);
	pragma_assert(noiseStrength>=0 && noiseStrength<=1);

	std::cout
		<< "Inputs:"
		<< std::endl
		<< "<InputImage>       : " << imageName << std::endl
		<< "<TestImage>        : " << testImageName << std::endl
		<< "<X-Pos SubImage>   : " << posSubImageX << std::endl
		<< "<Y-Pos SubImage>   : " << posSubImageY << std::endl
		<< "<Width SubImage>   : " << widthSubImage << std::endl
		<< "<Height SubImage>  : " << heightSubImage << std::endl
		<< "<Scalefactor>      : " << scaleSubImage << std::endl
		<< "<NoiseStrength>    : " << noiseStrength << std::endl;

		
	// Eingabebild einlesen
	pragma::Image::ByteImagePointer image;
	image->read(imageName);
	image->write("image.png");

	// Bildausschnitt ausschneiden
	// TODO: done!
	ImageManipulator imageManipulator(posSubImageX, posSubImageX + widthSubImage , posSubImageY, posSubImageY + heightSubImage);
	pragma::Image::ByteImagePointer subImage = imageManipulator.createSubImage(image);
	subImage->writePNG("subImage.png");
	// Filter aus Bildausschnitt erzeugen
	// TODO: done!
	pragma::Image::RealImagePointer filter(subImage);
	// Testbild einlesen
	pragma::Image::ByteImagePointer image2;
	image2->read(testImageName);
	image2->writePNG("image2.png");
	// Mit ImageManipulator verarbeiten (Rauschen und Skalierung)
	// TODO: done!
	ImageManipulator imageManipulator2(noiseStrength);
	imageManipulator2.setScaleImage(true);
	imageManipulator2.setResolution(scaleSubImage*image2->xResolution(), scaleSubImage*image2->yResolution());

	pragma::Image::ByteImagePointer testImage = imageManipulator2.processImage(image2);
	testImage->writePNG("testImage.png");

	// Filter mit Bild falten (inklusive Normalisierung)
	// TODO: done!

	pragma::Image::RealImagePointer convolvedImage = falte(testImage, filter);

	// Position im gefalteten Bild mit maximaler Antwort bestimmen
	// TODO: done!
	int globalMax_X = 0;
	int globalMax_Y = 0;
	convolvedImage->findGlobalMax(globalMax_X, globalMax_Y);

	std::cout << "gX: " << globalMax_X << " // gY: " << globalMax_Y << std::endl;

	// Filter in Testbild kopieren (sodass z.B. die ausgeschnittene Nase auf der gefundenen Nase angezeigt wird)
	pragma::Image::RealImagePointer finalImage(testImage);

	if (globalMax_X > -1 && globalMax_Y > -1)
		finalImage->put(filter, globalMax_X, globalMax_Y);
	else
		std::cout << "globalMax_X | globalMax_Y out of range"<< std::endl;
	
	// convolvedImage und finalImage auf Festplatte speichern (Konvertierung in ByteImagePointer ist erlaubt)
	// TODO: done!

	pragma::Image::ByteImagePointer store_convolvedImage(convolvedImage);
	store_convolvedImage->writePNG("convolvedImage.png");

	pragma::Image::ByteImagePointer store_finalImage(finalImage);
	store_finalImage->writePNG(pragma::string("fI_s_%.3f", scaleSubImage) + pragma::string("_n_%.3f_.png", noiseStrength));
	return 0;
}