#ifndef IMAGE_MANIPULATOR_H
#define IMAGE_MANIPULATOR_H

#include <pragma.h>

// Diese Klasse dient zur Manipulation von Bildern. Sie kann rechteckige
// Subbilder aus größeren Bildern ausschneiden, das Bild skalieren, um 180
// Grad rotieren und Rauschen hinzufügen. Welche Manipulation durchgeführt
// wird, hängt von den 4 bool-Variablen ab, die restlichen Parameter werden
// für die entsprechenden Manipulationen benötigt: xResE und yResE sind die
// neue Auflösung (Größe) des Bildes, wenn es skaliert wird, xMinE, xMaxE,
// yMinE und yMaxE legen den Bereich fest, der aus einem Eingabebild
// ausgeschnitten werden soll. Die Bildmanipulatorklasse wird mit den
// gewünschten Parameterwerten erzeugt. Dann kann für jeden ByteImagePointer
// (Pragmas Typ für Schwarzweißbilder) processImage aufgerufen werden. Diese
// Funktion gibt eine den Parameterwerten entsprechend veränderte Version
// des Eingabebildes zurück, das Eingabebild selbst bleibt unverändert.
// Parameterwerte eines Bildmanipulators können auch nach dem Erzeugen
// mittels Setter-Funktionen geändert und mittels Getter-Funktionen
// abgefragt werden. Die hier verwendeten einfachen Datentypen wie zum
// Beispiel pragma::UINT32 sind einfach andere speziell in Pragma definierte
// Namen für die normalen einfachen Datentypen von C++. Ein pragma::UINT32
// ist ein unsigned int, pragma::INT32 entspricht int, pragma::BOOL ist ein
// bool und pragma::REAL ein float oder double, je nach Konfiguration von
// pragma. Das kann man in der Datei src/pragma.cfg nachgucken.

class ImageManipulator
{
  private:
	pragma::BOOL addNoiseE;		  // Rauschen hinzufügen oder nicht
	pragma::BOOL scaleImageE;	 // Bild skalieren oder nicht
	pragma::BOOL createSubImageE; // Bildausschnitt ausschneiden
	pragma::BOOL rotateImageE;	// Bild rotieren
	pragma::UINT32 xResE;		  // x-Auflösung
	pragma::UINT32 yResE;		  // y-Auflösung
	pragma::UINT32 minXE;		  // minimum x-Koordinate von Bildausschnitt
	pragma::UINT32 maxXE;		  // max x-Koordinate von Bildausschnitt
	pragma::UINT32 minYE;		  // minimum y-Koordinate von Bildausschnitt
	pragma::UINT32 maxYE;		  // max y-Koordinate von Bildausschnitt
	pragma::REAL noiseStrengthE;  // Stärke des Rauschens

  protected:
  public:
	//===== Constructors =====
	ImageManipulator(
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
		pragma::REAL noiseStrengthA);							  // Setzt alle Parameter
	ImageManipulator(pragma::REAL noiseStrengthA);				  // Nur für Hinzufügen von gaussschem Rauschen
	ImageManipulator(pragma::BOOL rotateImageA = pragma::FALSE);  // Nur für Rotation
	ImageManipulator(pragma::UINT32 xResA, pragma::UINT32 yResA); // Nur zum Skalieren, setzt x und y-Auflösung
	ImageManipulator(pragma::UINT32 minXA, pragma::UINT32 maxXA,
					 pragma::UINT32 minYA, pragma::UINT32 maxYA); // Nur Bildausschnitt, setzt min und max-Koordinaten für x und y
	ImageManipulator(const ImageManipulator &blueprintA);		  // Copy Constructor, kopiert alle Parameter von blueprintA in diesen ImageManipulator

	//Destructor
	virtual ~ImageManipulator();

	//===== Operators =====
	ImageManipulator &operator=(const ImageManipulator &blueprintA); // Assignment Operator, kopiert alle Parameter von blueprintA in diesen ImageManipulator

	//===== Functions =====
	pragma::Image::ByteImagePointer processImage(pragma::Image::ByteImagePointer imageA);   // Führe alle Bildmanipulationen aus und gib das Ergebnis zurück
	pragma::Image::ByteImagePointer addNoise(pragma::Image::ByteImagePointer imageA);		// Füge sogennantes Salz und Pfeffer-Rauschen zum Bild hinzu
	pragma::Image::ByteImagePointer createSubImage(pragma::Image::ByteImagePointer imageA); // Schneide ein Subbild aus dem gegebenen Bild aus

	//===== Getter =====
	pragma::BOOL getAddNoise(PRAGMA_VOID) PRAGMA_CONST;
	pragma::BOOL getScaleImage(PRAGMA_VOID) PRAGMA_CONST;
	pragma::BOOL getCreateSubImage(PRAGMA_VOID) PRAGMA_CONST;
	pragma::BOOL getRotateImage(PRAGMA_VOID) PRAGMA_CONST;
	pragma::UINT32 getXRes(PRAGMA_VOID) PRAGMA_CONST;
	pragma::UINT32 getYRes(PRAGMA_VOID) PRAGMA_CONST;
	pragma::UINT32 getMinX(PRAGMA_VOID) PRAGMA_CONST;
	pragma::UINT32 getMaxX(PRAGMA_VOID) PRAGMA_CONST;
	pragma::UINT32 getMinY(PRAGMA_VOID) PRAGMA_CONST;
	pragma::UINT32 getMaxY(PRAGMA_VOID) PRAGMA_CONST;
	pragma::REAL getNoiseStrength(PRAGMA_VOID) PRAGMA_CONST;

	//===== Setter=====
	PRAGMA_VOID setAddNoise(pragma::BOOL addNoiseA);
	PRAGMA_VOID setScaleImage(pragma::BOOL scaleImageA);
	PRAGMA_VOID setCreateSubImage(pragma::BOOL createSubImageA);
	PRAGMA_VOID setRotateImage(pragma::BOOL rotateImageA);
	PRAGMA_VOID setResolution(pragma::UINT32 xResA, pragma::UINT32 yResA);
	PRAGMA_VOID setCornerPoints(pragma::UINT32 minXA, pragma::UINT32 maxXA, pragma::UINT32 minYA, pragma::UINT32 maxYA);
	PRAGMA_VOID setNoiseStrength(pragma::REAL noiseStrengthA);
};

#endif // IMAGE_MANIPULATOR_H