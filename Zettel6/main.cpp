#include <pragma.h>
#include <iostream>
#include "ImageManipulator.h"

/* Hinweis:
Da die Aufgaben ineinander uebergehen, gibt es bei dieser Vorlage keine klare Unterteilung in 6.1.1 und 6.1.2. 
Es sollen keine weiteren Dateien included werden (auch nicht Faltung.h). Der entsprechende Code wird in die falte()-Funktion eingefuegt und die
ImageManipulator.h bzw. .cpp muessen mit hochgeladen werden, da die Abgabe sonst nicht korrigiert werden kann.
Denken Sie daran, verschiedene Rauschstaerken und Skalierungen auszuprobieren und die entsprechenden Fragen fuer 6.1.2 und 6.1.3 zu beantworten.
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
			for (unsigned int fX = 0; fX < filter->xResolution(); fX++)
			{
				for (unsigned int fY = 0; fY < filter->yResolution(); fY++)
				{
					int tempX = iX - fX;
					int tempY = iY - fY;

					// add image resolution to prevent negative values
					tempX = (tempX + image->xResolution()) % image->xResolution();
					tempY = (tempY + image->yResolution()) % image->yResolution();

					pragma::REAL tempValue = image->pixel(tempX, tempY);
					normValue += pragma::sqr(tempValue);
					currValue += tempValue * filter->pixel(fX, fY);
				}
			}
			resultL->pixel(iX, iY, currValue / std::sqrt(normValue));
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
	// TODO

	int posSubImageX = 0;
	int posSubImageY = 0;
	int widthSubImage = 10;
	int heightSubImage = 10;
	int scaleSubImageX = 10;
	int scaleSubImageY = 10;
	pragma::REAL noiseStrength = 0.0;

	// Parameter ueber Benutzereingaben (pcin) oder Kommandozeile einlesen
	// TODO

	if (argc != 10)
	{
		std::cout
			<< "Usage:"
			<< std::endl
			<< argv[0]
			<< " <InputImage> <TestImage> <X-Pos SubImage> <Y-Pos SubImage> <Width SubImage> <Height SubImage> <Scale X> <Scale Y> <NoiseStrength>\n";

		std::cout
			<< "Demo:"
			<< std::endl
			<< argv[0] << " "
			<< imageName << " "
			<< testImageName << " "
			<< posSubImageX << " "
			<< posSubImageY << " "
			<< widthSubImage << " "
			<< heightSubImage << " "
			<< scaleSubImageX << " "
			<< scaleSubImageY << " "
			<< noiseStrength << std::endl;
		return -1;
	}

	imageName = pragma::string(argv[1]);
	testImageName = pragma::string(argv[2]);
	posSubImageX = atoi(argv[3]);
	posSubImageY = atoi(argv[4]);
	widthSubImage = atoi(argv[5]);
	heightSubImage = atoi(argv[6]);
	scaleSubImageX = atoi(argv[7]);
	scaleSubImageY = atoi(argv[8]);
	noiseStrength = atof(argv[9]);

	std::cout
		<< "Inputs:"
		<< std::endl
		<< "<InputImage>       : " << imageName << std::endl
		<< "<TestImage>        : " << testImageName << std::endl
		<< "<X-Pos SubImage>   : " << posSubImageX << std::endl
		<< "<Y-Pos SubImage>   : " << posSubImageY << std::endl
		<< "<Width SubImage>   : " << widthSubImage << std::endl
		<< "<Height SubImage>  : " << heightSubImage << std::endl
		<< "<Scale X>          : " << scaleSubImageX << std::endl
		<< "<Scale Y>          : " << scaleSubImageY << std::endl
		<< "<NoiseStrength>    : " << noiseStrength << std::endl;
	// Eingabebild einlesen
	pragma::Image::ByteImagePointer image;
	image->read(imageName);
	image->write("image.png");

	// Bildausschnitt ausschneiden
	// TODO
	ImageManipulator imageManipulator(posSubImageX, posSubImageX + widthSubImage, posSubImageY, posSubImageY + heightSubImage);
	pragma::Image::ByteImagePointer subImage = imageManipulator.createSubImage(image);
	subImage->writePNG("subImage.png");
	// Filter aus Bildausschnitt erzeugen
	// TODO
	pragma::Image::RealImagePointer filter(subImage);
	
	//filter->rotate180();

	// Testbild einlesen
	pragma::Image::ByteImagePointer image2;
	image2->read(testImageName);
	image2->writePNG("image2.png");
	// Mit ImageManipulator verarbeiten (Rauschen und Skalierung)
	// TODO
	ImageManipulator imageManipulator2(noiseStrength);
	imageManipulator2.setScaleImage(true);
	imageManipulator2.setResolution(scaleSubImageX, scaleSubImageY);

	pragma::Image::ByteImagePointer testImage = imageManipulator2.processImage(image2);
	testImage->writePNG("testImage.png");
	// Filter mit Bild falten (inklusive Normalisierung)
	// TODO
	pragma::Image::RealImagePointer convolvedImage = falte(testImage, filter);

	// Position im gefalteten Bild mit maximaler Antwort bestimmen
	// TODO
	int globalMax_X = 0;
	int globalMax_Y = 0;
	pragma::REAL maxValue = convolvedImage->findGlobalMax(globalMax_X, globalMax_Y);

	// Filter in Testbild kopieren (sodass z.B. die ausgeschnittene Nase auf der gefundenen Nase angezeigt wird)
	pragma::Image::RealImagePointer finalImage(testImage);

	//filter->rotate180();
	finalImage->put(filter, globalMax_X, globalMax_Y);

	// convolvedImage und finalImage auf Festplatte speichern (Konvertierung in ByteImagePointer ist erlaubt)
	// TODO

	pragma::Image::ByteImagePointer store_convolvedImage(convolvedImage);
	store_convolvedImage->writePNG("convolvedImage.png");

	pragma::Image::ByteImagePointer store_finalImage(finalImage);
	store_finalImage->writePNG("finalImage.png");
	return 0;
}