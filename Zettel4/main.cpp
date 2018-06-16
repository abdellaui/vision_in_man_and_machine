/*
Tolga Akkiraz
Abdullah Sahin
*/
#include <pragma.h>
#include <iostream>

/*
OWN_DEBUGGING needs following relative dirs:
-disp
---afterFFT
---beforeFFT
---gauss
---mini
-----afterFFT
-----beforeFFT
-----text
-------afterFFT
-------beforeFFT

you can rename _disp to disp its in images.zip
*/
//#define OWN_DEBUGGING

void storeComplexImage(const pragma::string &path, pragma::Image::ComplexImagePointer source)
{
#ifdef OWN_DEBUGGING
	pragma::Image::ByteImagePointer tempByteImage_REAL(source, pragma::Image::REAL_PART);
	tempByteImage_REAL->writePNG(path + "_real.png");

	pragma::Image::ByteImagePointer tempByteImage_IMAG(source, pragma::Image::IMAG_PART);
	tempByteImage_IMAG->writePNG(path + "_imag.png");

	pragma::Image::ByteImagePointer tempByteImage_ABSO(source, pragma::Image::ABS_PART);
	tempByteImage_ABSO->writePNG(path + "_abso.png");

	pragma::Image::ByteImagePointer tempByteImage_PHAS(source, pragma::Image::PHASE_PART);
	tempByteImage_PHAS->writePNG(path + "_phas.png");
#endif
// else nop
}

void forEach(pragma::Image::RealImagePointer source,
			 std::function<pragma::REAL(int, int)> calcVal)
{
	for (int x = 0; x < source->xResolution(); x++)
		for (int y = 0; y < source->yResolution(); y++)
			source->pixel(x, y, calcVal(x, y));
}

void forEach(pragma::Image::ComplexImagePointer source,
			 std::function<pragma::Complex(int, int)> calcVal)
{
	for (int x = 0; x < source->xResolution(); x++)
		for (int y = 0; y < source->yResolution(); y++)
			source->pixel(x, y, calcVal(x, y));
}

// Sobel Sx
pragma::Image::ComplexImagePointer xSobelDFT(pragma::UINT32 widthA, pragma::UINT32 heightA)
{
	// Komplexes Bild anlegen und mit Nullen fuellen
	pragma::Image::ComplexImagePointer sobelL(widthA, heightA);
	sobelL->init(0.0);

	// Sobelfilter Sx erzeugen und Fouriertransformation ausfuehren
	// TODO:

	int tempXOffset = widthA - 1;
	int tempYOffset = heightA - 1;

	// col 1
	sobelL->pixel(tempXOffset, tempYOffset, 1.0 / 8.0);
	sobelL->pixel(tempXOffset, 0, 2.0 / 8.0);
	sobelL->pixel(tempXOffset, 1, 1.0 / 8.0);

	// col 3
	sobelL->pixel(2, tempYOffset, -1.0 / 8.0);
	sobelL->pixel(1, 0, -2.0 / 8.0);
	sobelL->pixel(1, 1, -1.0 / 8.0);

	pragma::Trafo::FastFourierTransform tempFFT;

	storeComplexImage("disp/beforeFFT/sobelLX", sobelL);
	tempFFT.transformForward(sobelL);
	storeComplexImage("disp/afterFFT/sobelLX", sobelL);

	return sobelL;
}

// Sobel Sy
pragma::Image::ComplexImagePointer ySobelDFT(pragma::UINT32 widthA, pragma::UINT32 heightA)
{
	// Komplexes Bild anlegen und mit Nullen fuellen
	pragma::Image::ComplexImagePointer sobelL(widthA, heightA);
	sobelL->init(0.0);

	// Sobelfilter Sy erzeugen und Fouriertransformation ausfuehren
	// TODO:

	int tempXOffset = widthA - 1;
	int tempYOffset = heightA - 1;

	// row 1
	sobelL->pixel(tempXOffset, tempYOffset, 1.0 / 8.0);
	sobelL->pixel(0, tempYOffset, 2.0 / 8.0);
	sobelL->pixel(1, tempYOffset, 1.0 / 8.0);

	// row 3
	sobelL->pixel(tempXOffset, 1, -1.0 / 8.0);
	sobelL->pixel(0, 1, -2.0 / 8.0);
	sobelL->pixel(1, 1, -1.0 / 8.0);

	pragma::Trafo::FastFourierTransform tempFFT;

	storeComplexImage("disp/beforeFFT/sobelLY", sobelL);
	tempFFT.transformForward(sobelL);
	storeComplexImage("disp/afterFFT/sobelLY", sobelL);
	return sobelL;
}

pragma::Image::ComplexImagePointer convolve(pragma::Image::ComplexImagePointer source, pragma::Image::ComplexImagePointer filter)
{

	pragma::Image::ComplexImagePointer copySource(source, true);

	*copySource *= *filter;

	pragma::Trafo::FastFourierTransform tempFFT;
	tempFFT.transformBackward(copySource);

	return copySource;
}

// ========== Main function ==========
int pragma::main(int argc, char *argv[])
{
	std::cout << "Vorlesung Sehen in Mensch und Maschine Aufgabe 4\n\n";

	// Objekt fuer Fouriertransformation erzeugen
	pragma::Trafo::FastFourierTransform dftL;

	// ====== Aufgabe 4.1 ======
	std::cout << "Aufgabe 4.1 (Testdaten generieren und Ergebnisse auswerten):" << endl;

	int xyRes = 360;
	// Konstante Funktionen
	// TODO:

	pragma::Image::ComplexImagePointer constFunction(xyRes, xyRes);
	dftL.transformForward(constFunction);

	std::function<pragma::Complex(int, int)> constFunctionCalculator = [](int x, int y) {
		return 5.0;
	};

	forEach(constFunction, constFunctionCalculator);

	storeComplexImage("disp/beforeFFT/constFunction", constFunction);

	pragma::Graphics::WindowPointer window1;
	window1->image(constFunction, pragma::Image::REAL_PART);

	dftL.transformBackward(constFunction);
	storeComplexImage("disp/afterFFT/constFunction", constFunction);

	pragma::Image::ComplexImagePointer trigonomialFunction(18, 18);
	trigonomialFunction->init(0.0);
	dftL.transformForward(trigonomialFunction);

	std::function<pragma::Complex(int, int)> trigonomialFunctionCalculator = [](int x, int y) {
		return std::sin((x+y)*2*pragma::PI/18);

	};
	forEach(trigonomialFunction, trigonomialFunctionCalculator);

#ifdef OWN_DEBUGGING
	pragma::string currFunctionName = "scaled_sin_X+Y";
	storeComplexImage("disp/mini/beforeFFT/"+currFunctionName, trigonomialFunction);

	pragma::Image::RealImagePointer tempWriter(trigonomialFunction, pragma::Image::REAL_PART);
	tempWriter->writeText("disp/mini/text/beforeFFT/"+currFunctionName+".txt");
#endif

	pragma::Graphics::WindowPointer window2;
	window2->image(trigonomialFunction, pragma::Image::REAL_PART);
	
#ifdef OWN_DEBUGGING
	dftL.transformBackward(trigonomialFunction);
	storeComplexImage("disp/mini/afterFFT/"+currFunctionName, trigonomialFunction);
	trigonomialFunction->writeText("disp/mini/text/afterFFT/"+currFunctionName+".txt");
#endif
	getchar();

	/*

		Anmerkung zum Koardinatensystem:
		Die Anordnung des Koardinatensystems in Pragma ist wie folgt:
		+---------+---------+
		|A(0,0)   |   B(x,0)|
		|.a.      |      .b.|
		+---------#---------+  
		|.c.      |      .d.|
		|C(0,y)   |   D(x,y)|
		+---------+---------+
		wobei # den Mittelpunkt des Feldes darstellt.

		diese wird bei Berechnungen, wie in der Mathematik bekannten Koardinatensystem, umgewandelt.

		+---------+---------+
		|      .d.|.c.      |
		|   D(x,y)|C(0,y)   |
		+---------#---------+  
		|   B(x,0)|A(0,0)   |
		|      .b.|.a.      |
		+---------+---------+
		wobei # den Ursprung des Koardinatensystem darstellt.

		//---------------------
		
		Anmerkung zum Frequenzraum:
		Von Koardinaten (0,0) links oben bis zur Bildmitte steigen die Werte mit der Steigung 2*PI/Bild(breite\/höhe)
		Ab der Bildmitte fallen die Werte mit mit der Steigung -2*PI/Bild(breite\/höhe)

		Visuelle Darstellung:
		+------------------+------------------+
		| f'(x) =  2*PI/X  | f'(x) = -2*PI/X  |
		| f'(y) =  2*PI/Y  | f'(y) =  2*PI/Y  |
		+------------------#------------------+  
		| f'(x) =  2*PI/X  | f'(x) = -2*PI/X  |
		| f'(y) = -2*PI/Y  | f'(y) = -2*PI/Y  |
		+------------------+------------------+
		
		wobei:
		- X = [0, Bildbreite-1], 
		- Y = [0, Bildhöhe-1],
		- x € X, 
		- y € Y, 
		- f'(x) = Steigung von f(x)
		- # in der Zeichnung soll Bildmitte sein
	*/

	// ====== Aufgabe 4.2.1 ======
	std::cout << "Aufgabe 4.2.1 (Tiefpass-Filterung mit Gauss-Funktion):" << endl;

	// Bild einlesen
	pragma::Image::ByteImagePointer bildL;
	bildL->read("../images/einstein.jpg");

	// Tiefpass-Filterung fuer verschiedene Sigma durchfuehren
	// FIXME: sigmaL < 1.0 to sigmaL < 10.0
	for (pragma::REAL sigmaL = 1.0; sigmaL < 10.0; sigmaL += sigmaL)
	{
		// Komplexes Bild anlegen und transformieren
		pragma::Image::ComplexImagePointer complexImageL(bildL);
		dftL.transformForward(complexImageL);

		// Gaussglocke erzeugen und Bild falten
		// TODO:
		pragma::REAL currSigmaSqr = pragma::sqr(sigmaL);
		int tempXRes = complexImageL->xResolution();
		int tempYRes = complexImageL->yResolution();

		std::function<pragma::Complex(int, int)> gaussianCalculator = [currSigmaSqr, tempXRes, tempYRes](int x, int y) {

			// make a circle which is drawable by tempXRes-steps
			pragma::REAL tempX = 2 * pragma::PI / tempXRes;
			// center at (0,0) and if x > half of tempXRes, go right to left, otherwise go left to right
			tempX *= (x >= tempXRes / 2) ? -1 * (tempXRes - x) : x;

			pragma::REAL tempY = 2 * pragma::PI / tempYRes;
			tempY *= (y >= tempYRes / 2) ? -1 * (tempYRes - y) : y;

			pragma::TwoDimPoint currOmega(tempX,tempY);

			pragma::REAL currVal = 2 * pragma::PI * currSigmaSqr;
			currVal *= std::exp(currSigmaSqr * currOmega.norm2() / -2);

			return currVal;
		};
		pragma::Image::ComplexImagePointer gaussianFilter(tempXRes, tempYRes);
		gaussianFilter->init(0.0);

		forEach(gaussianFilter, gaussianCalculator);
		storeComplexImage(pragma::string("disp/gauss/sigma%f", sigmaL), gaussianFilter);

		// Bild ausgeben
		pragma::Image::ByteImagePointer blurredL(convolve(complexImageL, gaussianFilter), pragma::Image::REAL_PART);
		blurredL->write(pragma::string("blur_sigma%f_einstein.png", sigmaL));
	}

	getchar();

	// ====== Aufgabe 4.2.2 ======
	std::cout << "Aufgabe 4.2.2 (Sobelfilter):" << endl;

	// Bild einlesen
	bildL->read("../images/einstein.jpg");

	pragma::Image::ComplexImagePointer complexImageL(bildL);
	dftL.transformForward(complexImageL);

	// Sobelfilter anlegen
	pragma::Image::ComplexImagePointer xSobelL = xSobelDFT(bildL->xResolution(), bildL->yResolution());
	pragma::Image::ComplexImagePointer ySobelL = ySobelDFT(bildL->xResolution(), bildL->yResolution());

	pragma::Image::ComplexImagePointer gX = convolve(complexImageL, xSobelL);
	pragma::Image::ComplexImagePointer gY = convolve(complexImageL, ySobelL);
	// Betragsspektrum Sx anlegen
	// TODO:
	pragma::Image::RealImagePointer gXAbs(gX, pragma::Image::ABS_PART);

	pragma::Graphics::WindowPointer window4;
	window4->image(gXAbs);

	// Betragsspektrum Sy anlegen
	// TODO:
	pragma::Image::RealImagePointer gYAbs(gY, pragma::Image::ABS_PART);

	pragma::Graphics::WindowPointer window5;
	window5->image(gYAbs);
	// Richtungsunabhaengiges Kantenbild berechnen
	// TODO:

	pragma::Image::RealImagePointer absImg(bildL->xResolution(), bildL->yResolution());

	std::function<pragma::REAL(int, int)> calcAbs = [gX, gY](int x, int y) {
		return std::sqrt(gX->pixel(x, y).sqrNorm() + gY->pixel(x, y).sqrNorm());
	};
	forEach(absImg, calcAbs);

	pragma::Graphics::WindowPointer window6;
	window6->image(absImg);

	getchar();

	/*
	Der Sobelfilter eignet sich für ein Teil der Kantendetektion, 
	da diese die 1. Abbleitung darstellt und somit die "Steigung" 
	des Helligkeits in der jeweiligen Richtung angibt.
	Im resultierenden Bild sind die Bereiche mit der größten Intensität dort, 
	wo sich die Helligkeit des Originalbildes am stärksten ändert und somit 
	die größten Kanten darstellt. 
	*/
	return 0;
}
