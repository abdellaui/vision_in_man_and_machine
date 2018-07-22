#include "Faltung.h"
#include <math.h>

//Tolga Akkiraz
//Abdullah Sahin


// ============ Filter ============
// ===== Constructor =====
Filter::Filter(pragma::Image::RealImagePointer filterA,
	pragma::UINT32 xOffsetA = 0,
	pragma::UINT32 yOffsetA = 0)
	: filterE(filterA),
	xOffsetE(xOffsetA),
	yOffsetE(yOffsetA)
{
	// TODO: DONE
	// Sicherstellen, dass Aufsatzpunkt innerhalb der Filterdimensionen liegt
	if (xOffsetA >= filterA->xResolution() || yOffsetA >= filterA->yResolution())
	{
		std::cout << "Fehler in Filter(): Offset ist groesser als Dimensionen des Filters.\n";
		getchar();
	}
}

// ===== Destructor =====
Filter::~Filter()
{
}

// ===== Functions =====
// Gibt den Wert des Pixels an Stelle (xA, yA) zurück (Aufsatzpunkt beachten)
pragma::REAL Filter::getValueAt(pragma::INT32 xA, pragma::INT32 yA) const
{
	// TODO: DONE
	return this->filterE->pixel(xA, yA);
}

// x-Auflösung des Filters
pragma::UINT32 Filter::getWidth() const
{
	// TODO: DONE
	return this->filterE->xResolution();
}

// y-Auflösung des Filters
pragma::UINT32 Filter::getHeight() const
{
	// TODO: DONE
	return this->filterE->yResolution();
}

// x-Komponente des Aufsatzpunkts
pragma::UINT32 Filter::getXOffset() const
{
	// TODO: DONE
	return this->xOffsetE;
};

// y-Komponente des Aufsatzpunkts
pragma::UINT32 Filter::getYOffset() const
{
	// TODO: DONE
	return this->yOffsetE;
};

// Erzeugt den Sobelfilter Sx und gibt diesen zurück
Filter Filter::getSobelX()
{
	// TODO: DONE
	pragma::Image::RealImagePointer xSobelL(3, 3);
	xSobelL->init(0);

	xSobelL->pixel(0, 0) = 1. * (1. / 8.);
	xSobelL->pixel(0, 1) = 2. * (1. / 8.);
	xSobelL->pixel(0, 2) = 1. * (1. / 8.);

	xSobelL->pixel(2, 0) = -1. * (1. / 8.);
	xSobelL->pixel(2, 1) = -2. * (1. / 8.);
	xSobelL->pixel(2, 2) = -1. * (1. / 8.);

	Filter xSobelFilter(xSobelL, 1,1);

	return xSobelFilter;
}

// Erzeugt den Sobelfilter Sy und gibt diesen zurück
Filter Filter::getSobelY()
{
	// TODO: DONE
	pragma::Image::RealImagePointer ySobelL(3,3);
	ySobelL->init(0);

	ySobelL->pixel(0, 0) = 1. * (1. / 8.);
	ySobelL->pixel(1,0) = 2. * (1. / 8.);
	ySobelL->pixel(2,0) = 1. * (1. / 8.);

	ySobelL->pixel(0,2) = -1. * (1. / 8.);
	ySobelL->pixel(1,2) = -2. * (1. / 8.);
	ySobelL->pixel(2,2) = -1. * (1. / 8.);

	Filter ySobelFilter(ySobelL, 1, 1);

	return ySobelFilter;
}


// ============ Faltung ============

// ===== Constructor =====
Faltung::Faltung()
{
}

// ===== Destructor =====
Faltung::~Faltung()
{
}

// ===== Functions =====

// Diese Funktion faltet ein Bild bei gegebenem Filter und festgelegten Randbedingungen.
// Rückgabewert ist das gefaltete Bild. Implementiert über modeA Randbedingungen für
// - Unused / Weglassen der nicht-definierten Pixel (= 'u')
// - Wrap-around (= 'w')
// - Zero-padding (= 'z')
pragma::Image::RealImagePointer Faltung::falte(const pragma::Image::ByteImagePointer& imageA, const Filter& filterA, pragma::CHAR modeA) const
{
	// TODO: DONE
	// Sicherstellen, dass die Bildauflösung größer ist als die Dimensionen des Filters
	if (imageA->xResolution() < filterA.getWidth() || imageA->yResolution() < filterA.getHeight())
	{
		std::cout << "Fehler in falte: Bildaufloesung ist kleiner als Filter.\n";
		getchar();
	}

	// Ergebnis- und temporäres Bild erstellen
	pragma::Image::RealImagePointer resultL;
	pragma::Image::RealImagePointer imageL;

	// Variablen für den Filter erstellen
	/* Diese Werte geben die Anzahl der Pixel an, die vom Aufsatzpunkt aus in x- bzw. y-Richtung benötigt werden,
	   um die Ränder des Filters zu erreichen. Beispiele für einen 3x3 Filter mit Aufsatzpunkt X:

	   0 0 0
	   0 X 0
	   0 0 0

	   x_off = (1,1)
	   minX = -1, maxX = 1, minY = -1, maxY = 1


	   0 0 0
	   0 0 0
	   X 0 0

	   x_off = (0,2)
	   minX = 0, maxX = 2, minY = -2, maxY = 0

	   Mit Hilfe dieser Variablen können leicht die Anfangs- bzw. Endwerte für die Iteration bei
	   der eigentlichen Faltung und die Position des Bildes für die Behandlung der Randbedingungen bestimmt werden.

	   Achtung: Diese Werte können Vorzeichen haben und sind daher INT32 (im Gegensatz zu den Gettern des Filters).
	   Falls der Compiler eine Warnung wirft wie "result still unsigned", kann diese ignoriert werden.*/

	// TODO: DONE
	pragma::INT32 minX = 0 - filterA.getXOffset();
	pragma::INT32 maxX = (filterA.getWidth() - 1) - filterA.getXOffset();
	pragma::INT32 minY = 0 - filterA.getYOffset();
	pragma::INT32 maxY = (filterA.getHeight() - 1) - filterA.getYOffset();

	// ===== Randbedingungen abhandeln =====
	// Randbedingung 'unused' / Weglassen der nicht-definierten Pixel
	if (modeA == 'u')
	{
		// TODO: DONE
		// Auflösung des Ergebnisbilds anpassen (verkleinern um nicht-definierte Pixel)
		pragma::UINT32 resizedX = imageA->xResolution() - pragma::abs(minX) - pragma::abs(maxX);
		pragma::UINT32 resizedY = imageA->yResolution() - pragma::abs(minY) - pragma::abs(maxY);

		resultL->resolution(resizedX, resizedY);

		// Eingabebild in temporäres Bild kopieren
		imageL = imageA;
	}
	// Randbedingung 'wrap-around' 
	else if (modeA == 'w')
	{
		// TODO: DONE
		// Auflösung des Ergebnisbilds anpassen
		pragma::UINT32 resizedX = imageA->xResolution() + pragma::abs(minX) + pragma::abs(maxX);
		pragma::UINT32 resizedY = imageA->yResolution() + pragma::abs(minY) + pragma::abs(maxY);

		resultL->resolution(imageA->xResolution(), imageA->yResolution());

		// Auflösung des temporären Bilds anpassen (Vergrößern um durch Filter dazukommende Pixel)
		imageL->resolution(resizedX, resizedY);
		imageL->init(0);
		// Über das temporäre Bild iterieren und Formel für 'wrap-around' anwenden
		// TODO:DONE
		pragma::UINT32 x_Image;
		pragma::UINT32 y_Image;
		for (pragma::UINT32 x_I = 0; x_I < imageL->xResolution(); x_I++)
		{
			for (pragma::UINT32 y_I = 0; y_I < imageL->yResolution(); y_I++)
			{
				x_Image = (x_I + imageA->xResolution()) % imageA->xResolution();
				y_Image = (y_I + imageA->yResolution()) % imageA->yResolution();

				imageL->pixel(x_I, y_I, imageA->pixel(x_Image, y_Image));
			}
		}

	}
	// Randbedingung 'zero-padding'
	else if (modeA == 'z')
	{
		// TODO: DONE
		// Auflösung des Ergebnisbilds anpassen 
		resultL->resolution(imageA->xResolution(), imageA->yResolution());

		// Auflösung des temporären Bilds anpassen (Vergrößern um durch Filter dazukommende Pixel)
		pragma::UINT32 resizedX = imageA->xResolution() + pragma::abs(minX) + pragma::abs(maxX);
		pragma::UINT32 resizedY = imageA->yResolution() + pragma::abs(minY) + pragma::abs(maxY);
		imageL->resolution(resizedX, resizedY);

		// Bild mit Nullen füllen
		imageL->init(0);
		// Originales Bild an entsprechender Stelle einfügen
		imageL->put(imageA, pragma::abs(minX), pragma::abs(minY));

	}
	else
	{
		std::cout << "Fehler in falte: Unbekannte Randbedingung.\n";
		getchar();
	}

	// ===== Faltung durchführen (Skalarprodukt von Bild und Filter für den aktuellen Aufsatzpunkt) =====
	// Tipp: Bei der Bestimmung von Anfangs- bzw. Endwerten der Iteration helfen die zuvor festgelegten Variablen
	// minX, maxX, minY und maxY.

	// TODO:DONE

	pragma::UINT32 res_posX = 0;
	pragma::UINT32 res_posY = 0;

	pragma::REAL newPixel;
	//Man faengt beim Offset min an und endet bei Bildgroesse - Offset Max
	for (pragma::UINT32 imgL_posX = pragma::abs(minX); imgL_posX < (imageL->xResolution()-pragma::abs(maxX)) ; imgL_posX++)
	{
		res_posY = 0;
		for (pragma::UINT32 imgL_posY = pragma::abs(minY); imgL_posY < (imageL->yResolution() - pragma::abs(maxY)); imgL_posY++)
		{
			newPixel = 0.0;
			for (int filterA_posX = minX; filterA_posX <= maxX; filterA_posX++)
			{
				
				for (int filterA_posY = minY; filterA_posY <= maxY; filterA_posY++)
				{
					//Filterwertzugriff gespiegelt von pixelzugriff durch Vorzeichen Variation
					newPixel = newPixel + imageL->pixel(imgL_posX + filterA_posX, imgL_posY + filterA_posY)  * filterA.getValueAt(filterA.getXOffset() - filterA_posX, filterA.getYOffset() - filterA_posY);
				}
			}

			resultL->pixel(res_posX, res_posY, newPixel);
			res_posY++;
		}
		res_posX++;

	}

	return resultL;
}

// ===== Funktionen für richtungs(un)abhängige Kantenstärke, Sobelfilter usw. =====
// Ungerichtete Kantenstärke
pragma::Image::RealImagePointer Faltung::getKSUngerichtet(
	const pragma::Image::RealImagePointer& sxResultatA,
	const pragma::Image::RealImagePointer& syResultatA) const
{
	// TODO: DONE
	// Sicherstellen, dass Bilder gleichgroß sind (bzw. dieselbe Randbedingung verwendet wurde)
	if (sxResultatA->xResolution() != syResultatA->xResolution() || sxResultatA->yResolution() != syResultatA->yResolution())
	{
		std::cout << "Fehler in getKSUngerichtet: SxResultat und SyResultat haben nicht dieselbe Groesse.\n";
		getchar();
	}

	// Ergebnisbild erstellen 
	pragma::Image::RealImagePointer resultL(sxResultatA->xResolution(), sxResultatA->yResolution());

	// Über alle Pixel iterieren und absolute Kantenstärke berechnen (Wurzel aus der Summe der quadrierten Gradienten)
	// TODO: DONE
	pragma::REAL newPixel = 0;
	for (pragma::UINT32 pos_X = 0; pos_X < resultL->xResolution(); pos_X++){
		newPixel = 0;
		for (pragma::UINT32 pos_Y = 0; pos_Y < resultL->yResolution(); pos_Y++){
			newPixel = sxResultatA->pixel(pos_X, pos_Y) * sxResultatA->pixel(pos_X, pos_Y);
			newPixel += syResultatA->pixel(pos_X, pos_Y) * syResultatA->pixel(pos_X, pos_Y);
			newPixel = pragma::sqr(newPixel);

			resultL->pixel(pos_X, pos_Y, newPixel);
		}
	}

	return resultL;
}

// Gerichtete Kantenstärke
pragma::Image::RealImagePointer Faltung::getKSGerichtet(
	const pragma::Image::RealImagePointer& sxResultatA,
	const pragma::Image::RealImagePointer& syResultatA,
	pragma::REAL winkelA) const
{
	// TODO: DONE
	// Sicherstellen, dass die Bilder gleichgroß sind (bzw. dieselbe Randbedingung verwendet wurde)
	if (sxResultatA->xResolution() != syResultatA->xResolution() || sxResultatA->yResolution() != syResultatA->yResolution())
	{
		std::cout << "Fehler in getKSGerichtet: SxResultat und SyResultat haben nicht dieselbe Groesse.\n";
		getchar();
	}

	// Ergebnisbild anlegen
	pragma::Image::RealImagePointer resultL(sxResultatA->xResolution(), sxResultatA->yResolution());

	// Über alle Pixel iterieren und richtungsabhängige Intensität berechnen (SxResultat * cos + SyResultat * sin)
	// Es kann davon ausgegangen werden, dass winkelA in rad vorliegt. Die entsprechende Umwandlung von deg
	// ist in der main.cpp zu finden.

	// TODO: DONE

	pragma::REAL newPixel = 0;


	for (pragma::UINT32 pos_X = 0; pos_X < resultL->xResolution(); pos_X++){
		for (pragma::UINT32 pos_Y = 0; pos_Y < resultL->yResolution(); pos_Y++){

			newPixel = sxResultatA->pixel(pos_X, pos_Y) * cos(winkelA) + syResultatA->pixel(pos_X, pos_Y) * sin(winkelA);
			resultL->pixel(pos_X, pos_Y,newPixel);
		}
	}

	return resultL;
}


