// Bewertung:  3.5 +  4  +  2  =  9.5
// -----------------------------------------------------------------------------
// Maximum:    4  +  4  +  2  =   10
// -----------------------------------------------------------------------------
// @Error: Markiert Fehler mit Punktabzug.
// @Warning: Markiert Fehler, die zukünftig zu Punktabzug führen können.
// @Note: Markiert Anmerkungen und Hinweise.

/*
Tolga Akkiraz
Abdullah Sahin
*/

#include <pragma.h>
#include <iostream>

/*  Hinweis:
	PRaGMA besitzt die Klasse pragma::TwoDimPoint, die einen zweidimensionalen Vektor darstellt.
	Dieser besitzt einige Funktionen, die die Loesung dieser Aufgabe erleichtern:
	- .x(pragma::REAL): Setzt die x-Komponente auf einen Wert
	- .y(pragma::REAL): Setzt die y-Komponente auf einen Wert
	- .norm2(): Quadrierte euklidische Norm
	- Kann durch *= mit anderen Werten multipliziert werden
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

// Omega ermitteln
pragma::REAL omegaOfIndex(pragma::UINT32 indexA, pragma::UINT32 sizeA)
{
	pragma_assert(indexA < sizeA);

	if (sizeA - 1 - indexA < sizeA / 2) // negative omega
	{
		return (sizeA - indexA) * -2.0 * pragma::PI / sizeA;
	}
	else // positive omega (indexA < (sizeA+1)/2)
	{
		return indexA * 2.0 * pragma::PI / sizeA;
	}
}

// Gaborwavelet erzeugen
void createGaborWavelet(
	pragma::Image::ComplexImagePointer wavelet, pragma::UINT32 l, pragma::UINT32 m, pragma::UINT32 L, pragma::UINT32 M, pragma::REAL kMax, pragma::REAL k, pragma::REAL sigma)
{
	// Frequenzvektor k fuer aktuelles m bzw. l bestimmen (Gleichung 8)
	// TODO: 
	pragma::TwoDimPoint trigoFactor;

	pragma::REAL fac = kMax * std::pow(k, (-1.0*m));
	trigoFactor.x(fac * std::cos(pragma::PI*l / L));
	trigoFactor.y(fac * std::sin(pragma::PI*l / L));

	pragma::TwoDimPoint k_ = trigoFactor;

	// Waveletfamilie im Frequenzraum berechnen (Gleichung 7)
	// omega wird hier genau wie in Aufgabe 4.2.1 berechnet. Dafuer kann die omegaOfIndex-Funktion benutzt werden.
	// TODO


	// @Error: Die Stelle hier folgt einem falschen Gedankengang, auch wenn das Ergebnis sich nicht ändert.
	//         Der ComplexImagePointer wavelet ist an dieser Stelle komplett mit 0.0 initialisiert. Wenn man ihn transformiert, ist immer noch
	//         alles null. Den Punktabzug gibt es, weil die Wavelets direkt im Frequenzraum erzeugt werden sollten und keine Transformation
	//         in diese Funktion gehört. Das einzige, was transformiert werden muss, ist das Bild. Entweder habt ihr vergessen das hier
	//         rauszulöschen, oder es ist nur aus versehen vor der eigentlichen Änderung des Bildes und deshalb zufällig noch richtig, weil
	//         die Transformation das Bild nicht ändert (es aber trotzdem transformiert), daher kein voller Punkt Abzug.
	// -0.5

	pragma::Trafo::FastFourierTransform dftL;
	dftL.transformForward(wavelet);



	pragma::INT32 xRes = wavelet->xResolution();
	pragma::INT32 yRes = wavelet->yResolution();

	for (int x = 0; x < xRes; x++)
	{
		for (int y = 0; y < yRes; y++)
		{
			pragma::TwoDimPoint omega;
			omega.x(omegaOfIndex(x, xRes));
			omega.y(omegaOfIndex(y, yRes));

			pragma::REAL factor = pragma::sqr(sigma) / (-2 * k_.norm2());
			pragma::REAL expFactor1 = (omega - k_).norm2();
			pragma::REAL expFactor2 = omega.norm2() + k_.norm2();

			pragma::REAL value = std::exp(factor * expFactor1) - std::exp(factor * expFactor2);

			wavelet->pixel(x, y, pragma::Complex(value));
		}
	}



}

// ====== Main function ======
int pragma::main(int argc, char *argv[])
{
	cout << "Vorlesung Sehen in Mensch und Maschine Aufgabe 5" << endl << endl;

	pragma::Trafo::FastFourierTransform dft;

	// Bild einlesen und anzeigen
	pragma::string name = "../../images/monroe1.png";
	pragma::Image::ByteImagePointer bild;
	bild->read(name);

	pragma::UINT32 xRes = bild->xResolution();
	pragma::UINT32 yRes = bild->yResolution();
	pragma::Graphics::WindowPointer wEingabe("Eingabebild", xRes, yRes);
	wEingabe->image(bild);
	getchar();
	// Parameter festlegen
	pragma::UINT32 M = 5;					// Anzahl der Skalierungen
	pragma::REAL k = std::sqrt(2);					// Skalierungsfaktor
	pragma::REAL kMax = pragma::PI/2;				// Maximale Frequenz
	pragma::UINT32 L = 8;					// Anzahl der Richtungen
	pragma::REAL sigma = 2 * pragma::PI;

	// Ueber alle Richtungen L und Skalierungen M iterieren
	for (pragma::UINT32 l = 0; l < L; ++l)
	{
		for (pragma::UINT32 m = 0; m < M; ++m)
		{
			// ====== Aufgabe 5.1.1 ====== 
			std::cout << "Aufgabe 5.1.1 Gaborwavelet erzeugen" << std::endl;
			// Gaborwavelet erzeugen
			pragma::Image::ComplexImagePointer wavelet(xRes, yRes);
			wavelet->init(pragma::Complex(0, 0));
			createGaborWavelet(wavelet, l, m, L, M, kMax, k, sigma);

			// ====== Aufgabe 5.1.2 ====== 
			std::cout << "Aufgabe 5.1.2 Frequenzraumfaltung" << std::endl;
			// Frequenzraumfaltung von komplexem Bild mit Wavelet durchfuehren
			// TODO
			pragma::Image::ComplexImagePointer complexImage(bild);
			pragma::Trafo::FastFourierTransform dftL;
			dftL.transformForward(complexImage);
			*complexImage *= *wavelet;
			dftL.transformBackward(complexImage);
			// ====== Aufgabe 5.1.3 ====== 
			std::cout << "Aufgabe 5.1.3 Betraege ausgeben" << std::endl;
			// Betraege der Teilbandbilder ausgeben
			// TODO
			pragma::string currIterationName("l%d_m%d", l, m);

			pragma::Graphics::WindowPointer window_1("convolve_" + currIterationName,0,0);
			window_1->image(complexImage, pragma::Image::ABS_PART);
			pragma::Graphics::WindowPointer window_2("wavelet_" + currIterationName,xRes, yRes);
			window_2->image(wavelet, pragma::Image::ABS_PART);
			getchar();
#ifdef OWN_DEBUGGING
			storeComplexImage("convolve/" + currIterationName, complexImage);
			storeComplexImage("wavelet/" + currIterationName, wavelet);
			std::cout << "enter for next picture" << std::endl;
			getchar();
#endif
		}
	}
	std::cout << "Done !" << std::endl;
	std::getchar();

	return 0;
}
