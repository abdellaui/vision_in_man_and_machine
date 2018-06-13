/*
Tolga Akkiraz
Abdullah Sahin
*/

#include "ImageManipulator.h"
#include <pragma.h>
#include <iostream>

//Main function
int pragma::main(int argc, char *argv[])
{
	std::cout
		<< "Vorlesung Sehen in Mensch und Maschine Aufgabe 2"
		<< endl
		<< endl;

	//ByteImagePointer anlegen
	pragma::Image::ByteImagePointer testImageL;
	//Testbild einlesen
	testImageL->read("../images/einstein.jpg");
	//Fenster für Ausgabe anlegen
	pragma::Graphics::WindowPointer windowL("Ausgabefenster1", 0, 0, 128, 128);
	pragma::Graphics::WindowPointer window2L("Ausgabefenster2", 0, 0, 128, 128);
	pragma::Graphics::WindowPointer window3L("Ausgabefenster3", 0, 0, 128, 128);

	//Variablen für Bildmanipulator anlegen
	// FIXME: done!
	pragma::BOOL _addNoiseE = pragma::FALSE;	   // Rauschen hinzufügen oder nicht
	pragma::BOOL _scaleImageE = pragma::FALSE;	  // Bild skalieren oder nicht
	pragma::BOOL _createSubImageE = pragma::FALSE; // Bildausschnitt ausschneiden
	pragma::BOOL _rotateImageE = pragma::TRUE;	// Bild rotieren
	pragma::UINT32 _xResE = 10;					   // x-Auflösung
	pragma::UINT32 _yResE = 500;				   // y-Auflösung
	pragma::UINT32 _minXE = 100;				   // minimum x-Koordinate von Bildausschnitt
	pragma::UINT32 _maxXE = 200 + _minXE;		   // max x-Koordinate von Bildausschnitt
	pragma::UINT32 _minYE = 100;				   // minimum y-Koordinate von Bildausschnitt
	pragma::UINT32 _maxYE = 200 + _minYE;		   // max y-Koordinate von Bildausschnitt
	pragma::REAL _noiseStrengthE = 0.0f;		   // Stärke des Rauschens

	//Bildmanipulator mit geeigneten Werten erzeugen und Bild verarbeiten
	// FIXME: done!
	ImageManipulator imageManipulatorJ(_addNoiseE, _scaleImageE,
									   _rotateImageE, _createSubImageE,
									   _xResE, _yResE,
									   _minXE, _maxXE,
									   _minYE, _maxYE,
									   _noiseStrengthE);
	ImageManipulator imageManipulatorK = imageManipulatorJ;

	ImageManipulator imageManipulatorL(imageManipulatorK);


	pragma::Image::ByteImagePointer processedImageL = imageManipulatorL.processImage(testImageL);

	//Eingabebild und manipuliertes anzeigen
	windowL->image(testImageL);
	window2L->image(processedImageL);

	// own test
	imageManipulatorK.setRotateImage(pragma::FALSE);
	imageManipulatorK.setAddNoise(pragma::TRUE);
	imageManipulatorK.setNoiseStrength(1);
	pragma::Image::ByteImagePointer processTest = imageManipulatorK.processImage(testImageL);
	window3L->image(processTest);

	

	//Beenden wenn eine Taste gedrückt wurde
	std::getchar();

	return 0;
}
