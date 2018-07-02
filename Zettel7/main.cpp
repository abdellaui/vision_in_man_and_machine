#include "pragma.h"

// TODO: done!
/* Hinweise:
Achten Sie darauf, bei 7.1.1 das Verhalten zu kommentieren und bei 7.1.2 die Ergebnisse zu vergleichen sowie die Aehnlichkeitswerte anzugeben.
*/


/*
7.1.1)
******

Phase:
Maximale Aehnlichkeit (gallery) 	1        	bei x: 66 y: 64
Maximale Aehnlichkeit (test)    	0.77234  	bei x: 64 y: 95

Absolutewert:
Maximale Aehnlichkeit (gallery) 	1 			bei x: 66 y: 64
Maximale Aehnlichkeit (test)    	0.918852 	bei x: 79 y: 56

Bei der Ähnlichkeitsfunktion mit der Phase ist eine niedrigere Ähnlichkeit festzustellen. Diese liefert
auch einen falschen Ergebnis, denn die Koardinate liegt im Mundbereich.

PS: wenn wir die linke Auge als Feast übergeben, erhalte wir ebenso eine Koardinate im Mundbereich.

Bei der Ähnlichkeitsfunktion mit dem Absolutwert ist eine höhere Ähnlichkeit festzustellen. Diese liefert
einen richtigen Ergebnis.

PS: wenn wir die linke Auge als Feast übergeben, erhalten wir das gleiche Ergebnis, also die Koardinaten
des rechten Auges im Testbild.

7.1.2)
******

./zettel7_checks_Linux ../images/face1.tiff ../images/face0.tiff output.png 40 64 26 2 1 0

Phase:
Maximale Aehnlichkeit (graph)   	0.759695 	bei x: 53 y: 58

./zettel7_checks_Linux ../images/face1.tiff ../images/face0.tiff output.png 40 64 26 2 1 1

Absolutewert:
Maximale Aehnlichkeit (graph)   	0.90107 	bei x: 53 y: 55

Die Ähnlichkeitwerte sind minimal kleiner als die Werte aus 7.1.2.
Diesmal liefern beide Verfahren richtige Ergebnisse.
Merkwürdigerweißte ist beim output-Bild zu erkennen (ebenso an den obigen Werten), das bei der Ähnlichkeitsfunktion mit dem Absolutewert
leicht das Ziel verfehlt wird. (um 3 Pixeln in y)

###### face0.tiff #####

./zettel7_checks_Linux ../images/face1.tiff ../images/face0.tiff output.png 31 37 9 6 9 0

Maximale Aehnlichkeit (graph)   	0.723445 	bei x: 45 y: 31

./zettel7_checks_Linux ../images/face1.tiff ../images/face0.tiff output.png 31 37 9 6 9 1

Maximale Aehnlichkeit (graph)   	0.894243 	bei x: 44 y: 31

##### face2.tiff #####

./zettel7_checks_Linux ../images/face1.tiff ../images/face2.tiff output.png 31 37 9 6 9 0

Maximale Aehnlichkeit (graph)   	0.614808 	bei x: 65 y: 8

./zettel7_checks_Linux ../images/face1.tiff ../images/face2.tiff output.png 31 37 9 6 9 1

Maximale Aehnlichkeit (graph)   	0.811793 	bei x: 36 y: 29

##### face3.tiff #####

./zettel7_checks_Linux ../images/face1.tiff ../images/face3.tiff output.png 31 37 9 6 9 0

Maximale Aehnlichkeit (graph)   	0.622991 	bei x: 74 y: 18

./zettel7_checks_Linux ../images/face1.tiff ../images/face3.tiff output.png 31 37 9 6 9 1

Maximale Aehnlichkeit (graph)   	0.75175 	bei x: 41 y: 34
*/



// Diese Funktion kopiert das uebergebene Bild und markiert darin jede Stelle des Gitters, dessen LINKE OBERE ECKE (nicht der Mittelpunkt) 
// bei (x,y) liegt, mit einem 5*5-Pixel großen Quadrat.
pragma::Image::ByteImagePointer SetGraph(
	const pragma::Image::ByteImagePointer& byteImagePointerA,
	const pragma::UINT32& x,
	const pragma::UINT32& y,
	const pragma::UINT32& stepSize,
	const pragma::UINT32& nodesInRow,
	const pragma::UINT32& nodesInColumn)
{
	pragma::Image::ByteImagePointer byteImagePointer(byteImagePointerA, pragma::TRUE);

	for (pragma::UINT32 nX = 0; nX < nodesInRow; nX++)
	{
		for (pragma::UINT32 nY = 0; nY < nodesInColumn; nY++)
		{
			pragma::UINT32 xNode = x + nX * stepSize;
			pragma::UINT32 yNode = y + nY * stepSize;

			for (pragma::INT32 x = -5; x < 5; x++)
			{
				for (pragma::INT32 y = -5; y < 5; y++)
				{
					if ((xNode + x >= 0) && (xNode + x < byteImagePointer->xResolution())
						&& (yNode + y >= 0) && (yNode + y < byteImagePointer->yResolution()))
					{
						if (std::sqrt(static_cast<pragma::REAL>(x*x + y*y)) < 2)
							byteImagePointer->pixel(xNode + x, yNode + y) = 255;
						if (std::sqrt(static_cast<pragma::REAL>(x*x + y*y)) < 1)
							byteImagePointer->pixel(xNode + x, yNode + y) = 0;
					}
				}
			}
		}
	}

	return byteImagePointer;
}


// Plottet die Aehnlichkeit eines einzelnen FeaSts zu einem FeaStImage
pragma::Image::RealImagePointer PlotSimilarity(pragma::FeaSt::FeaStImagePointer image, pragma::FeaSt::FeaStPointer feaSt, pragma::FeaSt::SimFctPointer simFct)
{
	int imgX = image->xResolution();
	int imgY = image->yResolution();
	pragma::Image::RealImagePointer result(imgX, imgY);

	// TODO: done!
	for(int x = 0; x < imgX; x++)
	{
		for(int y = 0; y < imgY; y++)
		{
			pragma::REAL currValue = simFct->similarity(image->pixel(x,y),feaSt);
			result->pixel(x,y,currValue);
		}
	}

	return result;
}

// Berechnet die Aehnlichkeit zwischen zwei Feast-Graphen ueber den Durchschnitt der Aehnlichkeiten ihrer FeaSts
pragma::Image::RealImagePointer PlotSimilarity(pragma::FeaSt::FeaStImagePointer image, const pragma::vector<pragma::vector<pragma::FeaSt::FeaStPointer>> graph,
	pragma::UINT32 stepSize, pragma::FeaSt::SimFctPointer simFct)
{
	int imgX = image->xResolution();
	int imgY = image->yResolution();
	pragma::Image::RealImagePointer result(imgX, imgY);

	unsigned int countX = graph.size();
	unsigned int countY = graph[0].size();
	// TODO: done!
	for(int x = 0; x < imgX; x++)
	{
		for(int y = 0; y < imgY; y++)
		{
			pragma::REAL tempValue = 0.0;
			for (unsigned int nX = 0; nX < countX; nX++)
			{
				for (unsigned int nY = 0; nY < countY; nY++)
				{
					unsigned int tempX = (x + nX * stepSize) % imgX;
					unsigned int tempY = (y + nY * stepSize) % imgY;

					tempValue += simFct->similarity(image->pixel(tempX, tempY), graph[nX][nY]);
				}
			}
			// avg
			tempValue /= (countX*countY);
			result->pixel(x,y, tempValue);
		}
	}
	return result;
}



// ========== Main function ==========
int pragma::main(int argc, char *argv[])
{
	std::cout << "Vorlesung Sehen in Mensch und Maschine Aufgabe 7\n\n";

	// Argumente fehlen
	if (argc != 10)
	{
		cout << "Argc muss 10 sein, ist aber: " << argc << endl;
		std::cout << "Nutzung:\n" << " <Galeriebild> <Testbild> <Ausgabebild> <X> <Y> <Schrittweite> <Knoten pro Reihe> <Knoten pro Spalte> <Aehnlichkeitsfunktion (0 oder 1)>\n";
		
		std::cout << "*** Demo:\n" << std::endl;
		std::cout << argv[0] << " ";
		std::cout << "../images/face1.tiff ";
		std::cout << "../images/face0.tiff ";
		std::cout << "output.png ";
		std::cout << "31 ";
		std::cout << "37 ";
		std::cout << "9 ";
		std::cout << "6 ";
		std::cout << "9 ";
		std::cout << "0 "<<std::endl;
		getchar();

		return -1;
	}

	// Argumente ueber Kommandozeile einlesen
	pragma::string galleryImageName(argv[1]);		// Pfad des Galeriebildes
	pragma::string testimageName(argv[2]);			// Pfad des Testbildes
	pragma::string outputImageName(argv[3]);		// Pfad des Ausgabebildes
	pragma::INT32 posX = atoi(argv[4]);				// x-Koordinate des Startpunktes
	pragma::INT32 posY = atoi(argv[5]);				// y-Koordinate des Startpunktes
	pragma::UINT32 stepSize = atoi(argv[6]);		// Abstand zwischen den einzelnen Knoten
	pragma::UINT32 nodesInRow = atoi(argv[7]);		// Anzahl der horizontalen Knoten
	pragma::UINT32 nodesInColumn = atoi(argv[8]);	// Anzahl der vertikalen Knoten
	pragma::UINT8 simFunction = atoi(argv[9]);		// Aehnlichkeitsfunktion waehlen 

	// Bilder einlesen und anzeigen
	// Galeriebild
	pragma::Image::ByteImagePointer galleryImage;
	galleryImage->read(galleryImageName);
	pragma::Graphics::WindowPointer galleryImageWindow("Galeriebild", 20, 10);
	galleryImageWindow->image(galleryImage);

	// Testbild
	pragma::Image::ByteImagePointer testImage;
	testImage->read(testimageName);
	pragma::Graphics::WindowPointer testImageWindow("Testbild", 220, 10);
	testImageWindow->image(testImage);

	// FeaStImages erzeugen, jeder Pixel eines FeaSts ist ein pragma::FeaSt::FeaStPointer
	// Galeriebild
	pragma::Trafo::ComplexTrafoImagePointer galleryImageComplex;
	pragma::Trafo::GaborTrafoPointer galleryTrafoPointer;
	galleryTrafoPointer->transform(galleryImage, galleryImageComplex);
	pragma::FeaSt::FeaStImagePointer galleryFeastImagePointer(pragma::FeaSt::CUBIC_ABS_PHASE_FEAST, galleryImageComplex);

	// Testbild
	pragma::Trafo::ComplexTrafoImagePointer testImageComplex;
	pragma::Trafo::GaborTrafoPointer testTrafoPointer;
	testTrafoPointer->transform(testImage, testImageComplex);
	pragma::FeaSt::FeaStImagePointer testFeastImagePointer(pragma::FeaSt::CUBIC_ABS_PHASE_FEAST, testImageComplex);

	// Aehnlichkeitsfunktion auswaehlen (0 = Phase, 1 = Absolutwert)
	pragma::FeaSt::SimFctPointer simFctPointer;
	if (simFunction == 0)
		simFctPointer = pragma::FeaSt::Cubic::SimFct::AbsPhaseSimFctPointer();
	else
		simFctPointer = pragma::FeaSt::Cubic::SimFct::AbsSimFctPointer();


	// ========== Aufgabe 7.1.1 ==========

	// a) Aehnlichkeit eines FeaSts vom Galeriebild zu den anderen FeaSts des Bildes
	// FeaStPointer anlegen (z.B. von einem Auge)
	// TODO: done!
	
	// right eye causes big problems
	pragma::FeaSt::FeaStPointer feastPointer = galleryFeastImagePointer->pixel(66, 64);
	
	// Aehnlichkeit berechnen
	pragma::Image::RealImagePointer responseImage1 = PlotSimilarity(galleryFeastImagePointer, feastPointer, simFctPointer);
	int globalX = 0;
	int globalY = 0; 
	pragma::REAL globalVal = responseImage1->findGlobalMax(globalX,globalY);

	// Maximale Aehnlichkeit in Konsole ausgeben
    // TODO: done!
	std::cout << "Maximale Aehnlichkeit (gallery) " << globalVal << " bei x: "<< globalX << " y: " << globalY << std::endl;

	// b) Aehnlichstes FeaSt in Testbild bestimmen
	// Aehnlichkeit berechnen
	pragma::Image::RealImagePointer responseImage2 = PlotSimilarity(testFeastImagePointer, feastPointer, simFctPointer);
	int globalX2 = 0;
	int globalY2 = 0; 
	pragma::REAL globalVal2 = responseImage2->findGlobalMax(globalX2,globalY2);
	// Maximale Aehnlichkeit in Konsole ausgeben
    // TODO: done!
	std::cout << "Maximale Aehnlichkeit (test)    " << globalVal2 << " bei x: "<< globalX2 << " y: " << globalY2 << std::endl;



	// ========== Aufgabe 7.1.2 ==========
	// Gittergraph aus Galeriebild erstellen (an Position (posX, posY)), der alle FeaStPointer speichert, auf denen die Graphknoten liegen.
	// Zweidimensionalen Graphen erstellen

	pragma::Image::ByteImagePointer graphImg = SetGraph(galleryImage, posX, posY, stepSize, nodesInRow, nodesInColumn);
	graphImg->write("graphOnGalery.png");


	pragma::vector<pragma::vector<pragma::FeaSt::FeaStPointer>> graph(nodesInRow, pragma::vector<pragma::FeaSt::FeaStPointer>(nodesInColumn));

	// Ueber alle Knoten (Nodes) iterieren und entsprechende FeaStPointer eintragen
	// TODO: done
	for (unsigned int nX = 0; nX < nodesInRow; nX++)
	{
		for (unsigned int nY = 0; nY < nodesInColumn; nY++)
		{
			int xNode = (posX + nX * stepSize) % galleryFeastImagePointer->xResolution();
			int yNode = (posY + nY * stepSize) % galleryFeastImagePointer->yResolution();

			/*
			pragma_assert(xNode<galleryFeastImagePointer->xResolution() && xNode >= 0);
			pragma_assert(yNode<galleryFeastImagePointer->yResolution() && yNode >= 0);
			*/

			graph[nX][nY] = galleryFeastImagePointer->pixel(xNode, yNode);
		}
	}
	// Testbild mit dem Gittergraphen mit Schrittweite 5 Pixel an allen Stellen durchlaufen, an denen der Graph ins Bild passt (links oben anfangen), 
	// und durchschnittliche Aehnlichkeit berechnen (dabei jeweils Aehnlichkeitswert ins Ausgabebild schreiben)
	pragma::Image::RealImagePointer responseImageGraph = PlotSimilarity(testFeastImagePointer, graph, stepSize, simFctPointer);

	// Stelle mit maximaler Aehnlichkeit bestimmen
	// TODO: done
	int globalX3 = 0;
	int globalY3 = 0; 
	pragma::REAL globalVal3 = responseImageGraph->findGlobalMax(globalX3,globalY3);
	// Aehnlichkeit in Konsole ausgeben
    // TODO: done
	std::cout << "Maximale Aehnlichkeit (graph)   " << globalVal3 << " bei x: "<< globalX3 << " y: " << globalY3 << std::endl;


	// SetGraph() benutzen, um den Graphen an der entsprechenden Stelle im Testbild zu markieren und Ergebnis in Datei schreiben
	pragma::Image::ByteImagePointer outputImage = SetGraph(testImage, globalX3, globalY3, stepSize, nodesInRow, nodesInColumn);
	outputImage->write(outputImageName);

	std::getchar();

	return 0;
}