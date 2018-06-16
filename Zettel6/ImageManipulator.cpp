// Bewertung: 10
// -----------------------------------------------------------------------------
// Maximum:   10
// -----------------------------------------------------------------------------
// @Error: Markiert Fehler mit Punktabzug.
// @Warning: Markiert Fehler, die zukünftig zu Punktabzug führen können.
// @Note: Markiert Anmerkungen und Hinweise.

#include "ImageManipulator.h"
#include <string>
#include <vector>
#include <cmath>
#include <time.h>
#include <iostream>

using namespace std;

//===== Constructors =====
// Setzt alle Parameter
ImageManipulator::ImageManipulator(
	pragma::BOOL addNoiseA,
	pragma::BOOL scaleImageA,
	pragma::BOOL rotateImageA,
	pragma::BOOL createSubImageA,
	pragma::UINT32 xResA,
	pragma::UINT32 yResA,
	pragma::UINT32 minXA,
	pragma::UINT32 maxXA,
	pragma::UINT32 minYA,
	pragma::UINT32 maxYA,
	pragma::REAL noiseStrengthA) : addNoiseE(addNoiseA),
								   scaleImageE(scaleImageA),
								   rotateImageE(rotateImageA),
								   createSubImageE(createSubImageA),
								   xResE(xResA),
								   yResE(yResA),
								   minXE(minXA),
								   maxXE(maxXA),
								   minYE(minYA),
								   maxYE(maxYA),
								   noiseStrengthE(noiseStrengthA)
{
	
	// @Note: Es empfiehlt sich, diese Initialisierung in eine eigene Funktion auszulagern,
	// da sie in jedem Constructor aufgerufen wird.

	// lokale Zeit abfrage um RNG zu initialisieren
	time_t t1 = time(NULL);
	localtime(&t1);
	// Pragma-Pseudo-Zufallszahlengenerator (RNG) initialisieren
	pragma::seedRandom(t1);
}

// Nur für Hinzufügen von gaussschem Rauschen
ImageManipulator::ImageManipulator(pragma::REAL noiseStrengthA) : addNoiseE(pragma::TRUE),
																  scaleImageE(pragma::FALSE),
																  rotateImageE(pragma::FALSE),
																  createSubImageE(pragma::FALSE)
{
	// FIXME: done!
	noiseStrengthE = noiseStrengthA;

	// lokale Zeit abfrage um RNG zu initialisieren
	time_t t1 = time(NULL);
	localtime(&t1);
	// Pragma-Pseudo-Zufallszahlengenerator (RNG) initialisieren
	pragma::seedRandom(t1);
}

// Nur für Rotation
ImageManipulator::ImageManipulator(pragma::BOOL rotateImageA) : addNoiseE(pragma::FALSE),
																scaleImageE(pragma::FALSE),
																rotateImageE(pragma::TRUE),
																createSubImageE(pragma::FALSE)
{
	// FIXME: done!
	rotateImageE = rotateImageA;

	// lokale Zeit abfrage um RNG zu initialisieren
	time_t t1 = time(NULL);
	localtime(&t1);
	// Pragma-Pseudo-Zufallszahlengenerator (RNG) initialisieren
	pragma::seedRandom(t1);
}

// Nur zum Skalieren, setzt x und y-Auflösung
ImageManipulator::ImageManipulator(pragma::UINT32 xResA, pragma::UINT32 yResA) : addNoiseE(pragma::FALSE),
																				 scaleImageE(pragma::TRUE),
																				 rotateImageE(pragma::FALSE),
																				 createSubImageE(pragma::FALSE)
{
	//FIXME: done!
	xResE = xResA;
	yResE = yResA;

	// lokale Zeit abfrage um RNG zu initialisieren
	time_t t1 = time(NULL);
	localtime(&t1);
	// Pragma-Pseudo-Zufallszahlengenerator (RNG) initialisieren
	pragma::seedRandom(t1);
}

//Nur Bildausschnitt, setzt min und max-Koordinaten für x und y
ImageManipulator::ImageManipulator(pragma::UINT32 minXA, pragma::UINT32 maxXA, pragma::UINT32 minYA, pragma::UINT32 maxYA) : addNoiseE(pragma::FALSE),
																															 scaleImageE(pragma::FALSE),
																															 rotateImageE(pragma::FALSE),
																															 createSubImageE(pragma::TRUE)
{
	// FIXME: done!
	minXE = minXA;
	maxXE = maxXA;
	minYE = minYA;
	maxYE = maxYA;

	// lokale Zeit abfrage um RNG zu initialisieren
	time_t t1 = time(NULL);
	localtime(&t1);
	// Pragma-Pseudo-Zufallszahlengenerator (RNG) initialisieren
	pragma::seedRandom(t1);
}

// Copy Constructor, kopiert alle Parameter von blueprintA in diesen ImageManipulator
ImageManipulator::ImageManipulator(PRAGMA_CONST ImageManipulator &blueprintA)
{
	// @Note: Hier bietet sich die Constructor-Syntax an, die schon oben verwendet wurde 
	// (mit dem Doppelpunkt und der anschließenden Parameterliste).
	// Die Member auf diese Weise zu initialisieren hat in erster
	// Linie den Vorteil, dass die Variablen direkt bei ihrer Erstellung einen Wert
	// erhalten. Sobald der Code im Funktionsrumpf ausgeführt wird, muss dieses
	// ImageManipulator-Objekt vollständig erzeugt sein.
	// Bei komplexeren Objekten ist es also ineffizient, wenn das Objekt erst im
	// Rumpf des Constructors explizit initialisiert wird, weil es dann effektiv
	// zweimal erzeugt werden muss.

	// FIXME: done!
	addNoiseE = blueprintA.getAddNoise();
	scaleImageE = blueprintA.getScaleImage();
	rotateImageE = blueprintA.getRotateImage();
	createSubImageE = blueprintA.getCreateSubImage();
	xResE = blueprintA.getXRes();
	yResE = blueprintA.getYRes();
	minXE = blueprintA.getMinX();
	maxXE = blueprintA.getMaxX();
	minYE = blueprintA.getMinY();
	maxYE = blueprintA.getMaxY();
	noiseStrengthE = blueprintA.getNoiseStrength();
}

//Destructor
ImageManipulator::~ImageManipulator(PRAGMA_VOID) {}

// Assignment Operator, kopiert alle Parameter von blueprintA in diesen ImageManipulator
ImageManipulator &ImageManipulator::operator=(PRAGMA_CONST ImageManipulator &blueprintA)
{
	// FIXME: done!
	addNoiseE = blueprintA.getAddNoise();
	scaleImageE = blueprintA.getScaleImage();
	rotateImageE = blueprintA.getRotateImage();
	createSubImageE = blueprintA.getCreateSubImage();
	xResE = blueprintA.getXRes();
	yResE = blueprintA.getYRes();
	minXE = blueprintA.getMinX();
	maxXE = blueprintA.getMaxX();
	minYE = blueprintA.getMinY();
	maxYE = blueprintA.getMaxY();
	noiseStrengthE = blueprintA.getNoiseStrength();

	return *this;
}

// Führe alle Bildmanipulationen aus und gib das Ergebnis zurück
pragma::Image::ByteImagePointer ImageManipulator::processImage(PRAGMA_CONST pragma::Image::ByteImagePointer imageA)
{
	//Eingabebild kopieren, damit es nicht verändert wird
	pragma::Image::ByteImagePointer processedImageL(imageA, pragma::TRUE);
	// Wenn gefordert, Bildausschnitt erzeugen
	if (createSubImageE)
	{
		// FIXME: done!
		processedImageL = createSubImage(processedImageL);
	}
	// Wenn gefordert, Bild skalieren
	if (scaleImageE)
	{
		// FIXME: done!
		processedImageL->resize(getXRes(), getYRes());
		//Einfach entsprechende Pragma-Funktion für Größenänderung von Bildern (siehe Doku) for processedImageL aufrufen
	}
	//Wenn gefordert, Bild um 180 Grad drehen
	if (rotateImageE)
	{
		// FIXME: done!
		processedImageL->rotate180();
	}
	//Wenn gefordert Rauschen zum Bild hinzufügen
	if (addNoiseE)
	{
		// FIXME: done!
		processedImageL = addNoise(processedImageL);
	}

	return processedImageL;
}

// Füge sogennantes Salz und Pfeffer-Rauschen zum Bild hinzu
pragma::Image::ByteImagePointer ImageManipulator::addNoise(pragma::Image::ByteImagePointer imageA)
{
	// Diese Funktion fügt sogenanntes Salz- und Pfefferrauschen zum Bild hinzu, das heißt,
	// das noiseStrength aller Pixel zufällig ausgewählt und dann entweder auf Schwarz oder
	// auf Weiß gesetzt werden. Dabei soll sichergestellt werden, daß kein Pixel mehrfach ausgewählt wird.
	//Eingabebild kopieren
	pragma::Image::ByteImagePointer noisyImageL(imageA, pragma::TRUE);

	// @Note: Für das Ergebnis irrelevant, aber der Konsistenz halber sollte auf imageA
	// nach dem Kopieren in noisyImageL nicht mehr zugegriffen werden (also 
	// eher mit noisyImageL->xResolution etc. arbeiten).

	// Anzahl an zu verändernden Pixeln festlegen und Zähler für veränderte Pixel festlegen
	// FIXME: done
	pragma::UINT32 maxNoisePixelCountL = getNoiseStrength() * (imageA->xResolution() * imageA->yResolution());

	// Bild anlegen, das speichert ob ein Pixel schon verändert wurde
	// FIXME: done!
	pragma::Image::ByteImagePointer usedPixelsL(imageA->xResolution(), imageA->yResolution(), pragma::FALSE);
	usedPixelsL->init(0);

	// Solange noch nicht genug Pixel verändert wurden
	// FIXME: done!
	while (maxNoisePixelCountL > 0)
	{
		// @Note: Für ganze Zahlen gibt es auch pragma::intRandomFromInterval, wodurch man
		// sich die Umwandlung spart.

		// Ganzzahlige Zufallszahlen innerhalb der x- und y-Auflösung mittels pragma-Funktion erzeugen
		// FIXME: done!
		pragma::UINT32 xL = pragma::randomFromInterval(0, imageA->xResolution());
		// FIXME: done!
		pragma::UINT32 yL = pragma::randomFromInterval(0, imageA->yResolution());
		// Wenn entsprechendes Pixel noch nicht verändert wurde es mit 50% Wahrscheinlichkeit auf 0 oder 255 setzen und Pixel als verändert markieren
		// FIXME:
		if (usedPixelsL->pixel(xL, yL) == 0)
		{
			// @Note: Sehr elegante Lösung, schön gemacht.

			// FIXME: done
			pragma::UINT32 fiftyFifty = pragma::randomFromInterval(0, 100);
			pragma::UINT32 currPixelColor = (fiftyFifty % 2 == 0) ? 255 : 0;

			noisyImageL->pixel(xL, yL, currPixelColor);

			usedPixelsL->pixel(xL, yL, 1);
			maxNoisePixelCountL--;
		}
	}

	return noisyImageL;
}

// Schneide ein Subbild aus dem gegebenen Bild aus
pragma::Image::ByteImagePointer ImageManipulator::createSubImage(pragma::Image::ByteImagePointer imageA)
{
	// Auflösung des Bildausschnitts bestimmen (festgelegt durch minimale und maximale
	// x und y-Koordinaten, die die Ecken des auszuschneidenen Bereiches festlegen
	// FIXME: done!

	// @Warning: Der Ausschnitt sollte eigentlich auch maxXE bzw. maxYE enthalten.
	// Wenn das Bild 10x10 ist, gehen die Koordinaten jeweils von 0 bis 9.
	// Soll z.B. das ganze Bild ausgeschnitten werden, müsste man minX = 0 und maxX = 9 
	// wählen. In diesem Fall wird hier xResL = (9 - 0) = 9 berechnet, weswegen der
	// Ausschnitt einen Pixel zu klein wird. Hier fehlt also ein +1.

	pragma::UINT32 xResL = (getMaxX() - getMinX() + 1);
	pragma::UINT32 yResL = (getMaxY() - getMinY() + 1);
	//Vorsichtshalber testen, dass Bildausschnitt in imageA liegt und min kleiner als max ist
	// FIXME: done

	// @Warning: Wenn maxX <= minX ist, hat der Ausschnitt ebenfalls keine Fläche. Mit <
	// wird nur auf eine negative Fläche geprüft.
	bool minIsBiggerMax_X = getMaxX() <= getMinX();
	bool minIsBiggerMax_Y = getMaxY() <= getMinY();

	// @Warning: Siehe andere Kommentare. Für z.B. maxX = 10 kommt hier 'true' raus, obwohl
	// Die Koordinate gar nicht mehr im Bild liegt.
	bool imageNotIncludesX = imageA->xResolution() <= getMaxX();
	bool imageNotIncludesY = imageA->yResolution() <= getMaxY();
	
	if (minIsBiggerMax_X || minIsBiggerMax_Y || imageNotIncludesX || imageNotIncludesY)
	{
		std::cout << "ImageManipulation::createSubImage: error, corners of subimage are not inside imageA!\n";
		getchar();
	}
	// Bildausschitt aus imageA mit Schleifen durchlaufen und in subImageL kopieren (mit pixel-Funtionen von pragma)
	pragma::Image::ByteImagePointer subImageL(xResL, yResL);
	subImageL->init(0);
	// FIXME: done!
	for (int tmpX = 0; tmpX < xResL; tmpX++)
	{
		for (int tmpY = 0; tmpY < yResL; tmpY++)
		{
			pragma::BYTE tmpColor = imageA->pixel(tmpX + getMinX(), tmpY + getMinY());
			subImageL->pixel(tmpX, tmpY, tmpColor);
		}
	}
	return subImageL;
}

// Getter und Setter-Funktionen nicht vergessen.
// FIXME: done!

//===== Getter =====
pragma::BOOL ImageManipulator::getAddNoise(PRAGMA_VOID) PRAGMA_CONST
{
	return addNoiseE;
}
pragma::BOOL ImageManipulator::getScaleImage(PRAGMA_VOID) PRAGMA_CONST
{
	return scaleImageE;
}
pragma::BOOL ImageManipulator::getCreateSubImage(PRAGMA_VOID) PRAGMA_CONST
{
	return createSubImageE;
}
pragma::BOOL ImageManipulator::getRotateImage(PRAGMA_VOID) PRAGMA_CONST
{
	return rotateImageE;
}
pragma::UINT32 ImageManipulator::getXRes(PRAGMA_VOID) PRAGMA_CONST
{
	return xResE;
}
pragma::UINT32 ImageManipulator::getYRes(PRAGMA_VOID) PRAGMA_CONST
{
	return yResE;
}
pragma::UINT32 ImageManipulator::getMinX(PRAGMA_VOID) PRAGMA_CONST
{
	return minXE;
}
pragma::UINT32 ImageManipulator::getMaxX(PRAGMA_VOID) PRAGMA_CONST
{
	return maxXE;
}
pragma::UINT32 ImageManipulator::getMinY(PRAGMA_VOID) PRAGMA_CONST
{
	return minYE;
}
pragma::UINT32 ImageManipulator::getMaxY(PRAGMA_VOID) PRAGMA_CONST
{
	return maxYE;
}
pragma::REAL ImageManipulator::getNoiseStrength(PRAGMA_VOID) PRAGMA_CONST
{
	return noiseStrengthE;
}

//===== Setter=====
PRAGMA_VOID ImageManipulator::setAddNoise(pragma::BOOL addNoiseA)
{
	addNoiseE = addNoiseA;
}
PRAGMA_VOID ImageManipulator::setScaleImage(pragma::BOOL scaleImageA)
{
	scaleImageE = scaleImageA;
}
PRAGMA_VOID ImageManipulator::setCreateSubImage(pragma::BOOL createSubImageA)
{
	createSubImageE = createSubImageA;
}
PRAGMA_VOID ImageManipulator::setRotateImage(pragma::BOOL rotateImageA)
{
	rotateImageE = rotateImageA;
}
PRAGMA_VOID ImageManipulator::setResolution(pragma::UINT32 xResA, pragma::UINT32 yResA)
{
	xResE = xResA;
	yResE = yResA;
}
PRAGMA_VOID ImageManipulator::setCornerPoints(pragma::UINT32 minXA, pragma::UINT32 maxXA,
											  pragma::UINT32 minYA, pragma::UINT32 maxYA)
{
	minXE = minXA;
	maxXE = maxXA;
	minYE = minYA;
	maxYE = maxYA;
}
PRAGMA_VOID ImageManipulator::setNoiseStrength(pragma::REAL noiseStrengthA)
{
	noiseStrengthE = noiseStrengthA;
}