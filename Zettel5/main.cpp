/*
Tolga Akkiraz
Abdullah Sahin
*/
#include <pragma.h>
#include <iostream>

/*
OWN_DEBUGGING needs following relative dirs:
- convolve
- wavelet
*/
#define OWN_DEBUGGING

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

// Gaborwavelet erzeugen
void createGaborWavelet(
	pragma::Image::ComplexImagePointer wavelet,
	pragma::UINT32 l,
	pragma::UINT32 m,
	pragma::UINT32 L,
	pragma::UINT32 M,
	pragma::REAL kMax,
	pragma::REAL k,
	pragma::REAL sigma)
{
	// Frequenzvektor k fuer aktuelles m bzw. l besimmen (Gleichung 8)
	// TODO

	// std::pow(base, exp) , exp needs to be double, so use -1.0 as factor
	pragma::REAL tempFactor = kMax * std::pow(k, -1.0 * m);
	pragma::REAL tempParameter = pragma::PI * l / L;

	std::vector<pragma::REAL> k_ml = {tempFactor * std::cos(tempParameter), tempFactor * std::sin(tempParameter)};

	// Waveletfamilie im Frequenzraum berechnen (Gleichung 7)
	// omega wird hier genau wie in Aufgabe 4.2.1 berechnet. Der Code kann wiederverwendet werden.
	// TODO
	pragma::Trafo::FastFourierTransform tempFFT;
	tempFFT.transformForward(wavelet);

	int tempXRes = wavelet->xResolution();
	int tempYRes = wavelet->yResolution();

	for (int x = 0; x < tempXRes; x++)
	{
		for (int y = 0; y < tempYRes; y++)
		{

			pragma::REAL tempX = 2 * pragma::PI / tempXRes;
			tempX *= (x >= tempXRes / 2) ? -1 * (tempXRes - x) : x;

			pragma::REAL tempY = 2 * pragma::PI / tempYRes;
			tempY *= (y >= tempYRes / 2) ? -1 * (tempYRes - y) : y;

			pragma::REAL tempOmega = std::sqrt(pragma::sqr(tempX) + pragma::sqr(tempY));
			pragma::REAL currK = std::sqrt(pragma::sqr(k_ml.at(0)) + pragma::sqr(k_ml.at(1)));

			pragma::REAL tempPotenz = pragma::sqr(sigma) / -2 * pragma::sqr(currK);
			pragma::REAL tempPotenzFaktor1 = pragma::sqr(tempOmega - currK);
			//pragma::REAL tempPotenzFaktor1 = pragma::sqr(tempX-k_ml.at(0)) + pragma::sqr(tempY-k_ml.at(1));
			pragma::REAL tempPotenzFaktor2 = pragma::sqr(tempOmega) + pragma::sqr(currK);

			pragma::REAL currVal = std::exp(tempPotenz * tempPotenzFaktor1) - std::exp(tempPotenz * tempPotenzFaktor2);

			wavelet->pixel(x, y, pragma::Complex(currVal));
		}
	}
}

// ====== Main function ======
int pragma::main(int argc, char *argv[])
{
	cout << "Vorlesung Sehen in Mensch und Maschine Aufgabe 5" << endl
		 << endl;

	pragma::Trafo::FastFourierTransform dft;

	// Bild einlesen und anzeigen
	pragma::string name = "../images/monroe1.png";
	//pragma::string name = "../images/Kante_22.jpg";
	pragma::Image::ByteImagePointer bild;
	bild->read(name);

	pragma::UINT32 xRes = bild->xResolution();
	pragma::UINT32 yRes = bild->yResolution();
	pragma::Graphics::WindowPointer wEingabe("Eingabebild", xRes, yRes);
	wEingabe->image(bild);

	// Parameter festlegen
	pragma::UINT32 M = 5;				// Anzahl der Skalierungen
	pragma::REAL k = std::sqrt(2);		// Skalierungsfaktor
	pragma::REAL kMax = pragma::PI / 2; // Maximale Frequenz
	pragma::UINT32 L = 8;				// Anzahl der Richtungen
	pragma::REAL sigma = 2 * pragma::PI;

	// Ueber alle Richtungen L und Skalierungen M iterieren
	for (pragma::UINT32 l = 0; l < L; ++l)
	{
		for (pragma::UINT32 m = 0; m < M; ++m)
		{
			// ====== Aufgabe 5.1.1 ======
			// Gaborwavelet erzeugen
			pragma::Image::ComplexImagePointer wavelet(xRes, yRes);
			wavelet->init(pragma::Complex(0, 0));
			createGaborWavelet(wavelet, l, m, L, M, kMax, k, sigma);

			// ====== Aufgabe 5.1.2 ======
			// Frequenzraumfaltung von komplexem Bild mit Wavelet durchfuehren
			// TODO
			pragma::Image::ComplexImagePointer currComplexImg(bild);

			pragma::Trafo::FastFourierTransform tempFFT;
			tempFFT.transformForward(currComplexImg);

			*currComplexImg *= *wavelet;

			tempFFT.transformBackward(currComplexImg);

			// ====== Aufgabe 5.1.3 ======
			// Betraege der Teilbandbilder ausgeben
			// TODO
			pragma::string currIterationName("l%d_m%d", l, m);

			pragma::Graphics::WindowPointer window_1("convolve_" + currIterationName);
			window_1->image(currComplexImg, pragma::Image::ABS_PART);

			pragma::Graphics::WindowPointer window_2("walvelet_" + currIterationName, yRes);
			window_2->image(wavelet, pragma::Image::ABS_PART);

#ifdef OWN_DEBUGGING
			storeComplexImage("convolve/" + currIterationName, currComplexImg);
			storeComplexImage("wavelet/" + currIterationName, wavelet);
			std::cout << "enter for next picture" << std::endl;
			std::getchar();
#endif
		}
	}
	std::cout << "done!" << std::endl;
	std::getchar();

	return 0;
}
