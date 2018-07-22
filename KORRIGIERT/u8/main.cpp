/*
Abdullah Sahin
Tolga Akkiraz
*/
// Bewertung:  5  +  3  +  3  +  3  =  14
// -----------------------------------------------------------------------------
// Maximum:    5  +  4  +  3  +  3  =  15
// -----------------------------------------------------------------------------
// @Error: Markiert Fehler mit Punktabzug.
// @Warning: Markiert Fehler, die zukünftig zu Punktabzug führen können.
// @Note: Markiert Anmerkungen und Hinweise.




#include "pragma.h"

/* Hinweis:
Die Dateinamen sind in den Listen als "Bilder/datei.jpg" angegeben. Die .exe muss sich daher im selben Ordner wie der Bilder-Ordner befinden.
Dasselbe gilt fuer die Listen und Gesichtsgraphen. Entweder also die .exe verschieben oder die ganzen Dateien in den Ordner mit der kompilierten .exe.

In dieser Aufgabe geht es hauptsaechlich darum, sich in den vorhandenen Code einzuarbeiten. Daher ist die einzige zu ergaenzende Funktion der Kreuzvergleich  
in der main() (5P). Fuer alle anderen Teilaufgaben sollen lediglich verschiedene Matching-Schedules ausprobiert und die Fragen beantwortet werden (4P + 3P + 3P).
Die dafuer relevanten Stellen sind durch Kommentare im Code angegeben. Bis auf den Schedule selber muss nichts mehr am Code geaendert werden.
*/

// Funktionen
PRAGMA_VOID readFileList(std::string fileName, pragma::vector<std::string> &vec);
PRAGMA_VOID readImages(const std::string &fileName, pragma::vector<pragma::Image::ByteImagePointer> &imageVector);
PRAGMA_VOID createModelGraphSet(const std::string fileName, pragma::GraphLab::ModelGraphSet &modelGraphSet);
PRAGMA_VOID matchOnImage(const pragma::Image::ByteImagePointer &imagePointer, const pragma::GraphLab::ModelGraphSet &modelGraphSet,
						 pragma::Graphics::WindowPointer &matchWindowPointer, pragma::GraphLab::ModelGraphPointer &matchGraphPointer);
pragma::REAL modelGraphSimilarity(PRAGMA_CONST pragma::GraphLab::ModelGraphPointer &mgPointer1, PRAGMA_CONST pragma::GraphLab::ModelGraphPointer &mgPointer2, int simFunction);

// ========== Main function ==========
int pragma::main(int argc, char *argv[])
{
	cout << "Vorlesung Sehen in Mensch und Maschine Aufgabe 8\n\n";

	// Parameterzahl ueberpruefen
	if (argc != 4)
	{
		std::cout << "Benutzung: <Liste mit Galleriebildern> <Liste mit Probebildern> <Aehnlichkeitsfunktion (0 oder 1)>\n";
		getchar();
		return 0;
	}

	// Galeriebilder einlesen
	std::string galleryName = argv[1];
	pragma::vector<pragma::Image::ByteImagePointer> galleryVector;
	readImages(galleryName, galleryVector);

	// Probebilder einlesen
	std::string probeName = argv[2];
	pragma::vector<pragma::Image::ByteImagePointer> probeVector;
	readImages(probeName, probeVector);

	// FIXME: Eingabe: Aehnlichkeitsfunktion
	pragma::UINT8 simFunction = atoi(argv[3]);

	// ModelGraphSet, ModelGraphPointer und entsprechende Fenster erstellen
	pragma::GraphLab::ModelGraphSet modelGraphSet;
	createModelGraphSet("bg2.txt", modelGraphSet);

	pragma::vector<pragma::GraphLab::ModelGraphPointer> galleryGraphVector;
	pragma::vector<pragma::GraphLab::ModelGraphPointer> probeGraphVector;

	pragma::Graphics::WindowPointer matchWindowPointer("Match Window", 220, 10);
	pragma::Graphics::WindowPointer imageWindowPointer("Image Window", 20, 10);

	// Auf Galeriebilder matchen und Modelgraphen erstellen
	cout << "Matching gallery... " << endl;
	for (pragma::UINT32 i = 0; i < galleryVector.size(); i++)
	{
		imageWindowPointer->image(galleryVector[i]);
		pragma::GraphLab::ModelGraphPointer modelGraphPointer;
		matchOnImage(galleryVector[i], modelGraphSet, matchWindowPointer, modelGraphPointer);
		galleryGraphVector.push_back(modelGraphPointer);
		cout << std::round((double)(i + 1) / galleryVector.size() * 100) << "% ";
	}
	cout << "\nDone." << endl;

	// Auf Probebilder matchen und Modelgraphen erstellen
	cout << "Matching probes... " << endl;
	for (pragma::UINT32 i = 0; i < probeVector.size(); i++)
	{
		imageWindowPointer->image(probeVector[i]);
		pragma::GraphLab::ModelGraphPointer modelGraphPointer;
		matchOnImage(probeVector[i], modelGraphSet, matchWindowPointer, modelGraphPointer);
		probeGraphVector.push_back(modelGraphPointer);
		cout << std::round((double)(i + 1) / probeVector.size() * 100) << "% ";
	}
	cout << "\nDone." << endl;

	// ========== Aufgabe 1 ==========
	// Kreuzvergleich durchfuehren (jeden Modellgraphen mit jedem Probegraphen vergleichen)
	cout << "Starting cross-run... " << endl;
	pragma::UINT32 numberOfCorrectRecognised = 0;

	// probeGraphVector durchlaufen
	for (pragma::UINT32 probeIndex = 0; probeIndex < probeGraphVector.size(); probeIndex++)
	{
		pragma::UINT32 bestIndex = 0;	  // Index des Bildes mit der hoechsten Aehnlichkeit
		pragma::REAL bestSimilarity = 0.0; // Wert der hoechsten Aehnlichkeit

		// Mit galleryGraphVector vergleichen (vorhandene Funktionen nutzen und darauf achten, welche Werte ausgegeben werden sollen)
		// TODO: done!
		for (unsigned int galleryIndex = 0; galleryIndex < galleryGraphVector.size(); galleryIndex++)
		{
			pragma::Real tempResult = modelGraphSimilarity(galleryGraphVector[galleryIndex], probeGraphVector[probeIndex], simFunction);

			if (tempResult > bestSimilarity)
			{
				bestSimilarity = tempResult;
				bestIndex = galleryIndex;
			}
		}

		// Counter erhoehen, wenn richtig erkannt
		if (bestIndex == probeIndex)
			numberOfCorrectRecognised++;

		// Ergebnis ausgeben
		std::cout << "Probenbild: " << probeIndex + 1
				  << "  aehnlichstes Galeriebild: " << bestIndex + 1
				  << "  erkannt?: " << (bestIndex == probeIndex)
				  << "  Aehnlichkeit: " << bestSimilarity << std::endl;
	}

	// Erkennungsrate ausgeben
	pragma::REAL recognitionRate = (static_cast<pragma::REAL>(numberOfCorrectRecognised) / static_cast<pragma::REAL>(probeGraphVector.size())) * 100.0;
	std::cout << "\nRecognition Rate: " << numberOfCorrectRecognised << " / " << probeGraphVector.size() << " = " << recognitionRate << "%" << endl
			  << endl;
	return 0;
}

// Einlesen der Liste von Dateinamen aus der txt-Datei und Speichern in Vector fileList
PRAGMA_VOID readFileList(std::string fileName, pragma::vector<std::string> &fileList)
{
	fileList.clear();

	FILE *f = fopen(fileName.c_str(), "r");
	char bufferL[500];

	while (!feof(f))
	{
		if (NULL != fgets(bufferL, sizeof(bufferL), f))
		{
			char *pCL = strchr(bufferL, '\n');
			if (NULL != pCL)
				*pCL = '\0';
			fileList.push_back(bufferL);
		}
	}
	fclose(f);
}

// Einlesen mehrerer Bilder aus Datei fileName und Abspeichern in imageVector
PRAGMA_VOID readImages(const std::string &fileName, pragma::vector<pragma::Image::ByteImagePointer> &imageVector)
{
	// Datei einlesen und Inhalt in fileList abspeichern
	pragma::vector<std::string> fileList;
	readFileList(fileName, fileList);

	// Zugehoerige Bilder einlesen und Speichern in Vector imageVector
	cout << "Reading " << fileList.size() << " images from " << fileName << "... ";
	for (pragma::vector<std::string>::const_iterator pFilename = fileList.begin(); pFilename != fileList.end(); pFilename++)
	{
		pragma::Image::ByteImagePointer byteImagePointer;
		byteImagePointer->read(*pFilename);
		imageVector.push_back(byteImagePointer);
	}
	cout << "done." << endl;
}

// Einlesen eines Sets von Gesichtsgraphen
PRAGMA_VOID createModelGraphSet(const std::string fileName, pragma::GraphLab::ModelGraphSet &modelGraphSet)
{
	// Datei einlesen und Inhalt in fileList abspeichern
	pragma::vector<std::string> fileList;
	readFileList(fileName, fileList);

	// Zugehoerige Gesichtsgraphen einlesen und in modelGraphSet speichern
	cout << "Reading " << fileList.size() << " graphs from " << fileName << "... ";
	for (pragma::vector<std::string>::const_iterator pFilename = fileList.begin(); pFilename != fileList.end(); pFilename++)
	{
		pragma::GraphLab::GraphPointer graphPointer;
		graphPointer.read(*pFilename);
		pragma::GraphLab::ModelGraphPointer modelGraphPointer(graphPointer);
		modelGraphSet.push_back(modelGraphPointer);
	}
	cout << "done." << endl;
}

// Detektieren eines Gesichts auf imagePointer mittels verschiedener Moves und Auslesen des entsprechenden Gesichtsgraphen an der entsprechenden Stelle
PRAGMA_VOID matchOnImage(const pragma::Image::ByteImagePointer &imagePointer, const pragma::GraphLab::ModelGraphSet &modelGraphSet,
						 pragma::Graphics::WindowPointer &matchWindowPointer, pragma::GraphLab::ModelGraphPointer &matchGraphPointer)
{
	// BunchModelGraphPointer erstellen
	pragma::GraphLab::BunchModelGraphPointer bunchGraphPointer(modelGraphSet);

	// FeaStImagePointer erstellen
	pragma::Trafo::GaborTrafoPointer trafoPointer;
	pragma::Trafo::ComplexTrafoImagePointer complexTrafoImagePointer;
	trafoPointer->transform(imagePointer, complexTrafoImagePointer);
	pragma::FeaSt::FeaStImagePointer feastImagePointer(pragma::FeaSt::CUBIC_ABS_PHASE_FEAST, complexTrafoImagePointer);

	// Matching
	pragma::EGM::MatchingSchedulePointer matchingSchedulePointer;

	// ========== Aufgabe 2 (Teil 1) ==========
	// Welche der beiden Aehnlichkeitsfunktionen (AbsSimFctPointer, AbsPhaseSimFctPointer) ist fuer die drei Moves besser geeignet?
	// TODO: done!
	/*
		GlobalMove = AbsSimFctPointer
		ScalingMove = AbsSimFctPointer
		LocalTranslationMove = AbsSimFctPointer
		
		laut Vorlesung 09 Seite 9			
	
	// @Error: Das Reicht hier als Erklärung nicht ganz aus... gefragt war entweder eine richtige Erklärung
	// - 1     oder einige Erkennungsraten, die zeigen wie diese Aussage zustande kommt 
		
	*/
	// ========== Aufgabe 3 ==========
	// Vorgegebenen Matching-Schedule und zwei weitere ausprobieren und Erkennungsraten fuer zwei oder mehr Kombinationen von Galerie- und Probe-Liste angeben
	// Alle benutzten Matching-Schedules bitte auskommentiert im Code lassen
	// TODO: done!

	/* Moves in der Dokumentation (enthaelt Erklaerungen zu den Argumenten):
	- GlobalTranslationMove: html/classpragma_1_1EGM_1_1GlobalTranslationMoveRep.html
	- ScalingMove: html/classpragma_1_1EGM_1_1ScalingMoveRep.html
	- LocalTranslationMove: html/classpragma_1_1EGM_1_1LocalTranslationMoveRep.html
	*/

	// Vorgegebener Matching-Schedule

	matchingSchedulePointer->push_back(pragma::EGM::GlobalTranslationMovePointer(pragma::FeaSt::Cubic::SimFct::AbsSimFctPointer(), 10, pragma::EGM::COARSE_DISPLAY_MOVE));
	matchingSchedulePointer->push_back(pragma::EGM::GlobalTranslationMovePointer(pragma::FeaSt::Cubic::SimFct::AbsSimFctPointer(), 2, 10, pragma::EGM::COARSE_DISPLAY_MOVE));
	matchingSchedulePointer->push_back(pragma::EGM::ScalingMovePointer(pragma::FeaSt::Cubic::SimFct::AbsSimFctPointer(), 0.8, 5, 0.1, pragma::TRUE, pragma::TRUE, pragma::FALSE, pragma::EGM::COARSE_DISPLAY_MOVE));
	matchingSchedulePointer->push_back(pragma::EGM::ScalingMovePointer(pragma::FeaSt::Cubic::SimFct::AbsSimFctPointer(), 0.95, 3, 0.05, pragma::TRUE, pragma::TRUE, pragma::FALSE, pragma::EGM::COARSE_DISPLAY_MOVE));
	matchingSchedulePointer->push_back(pragma::EGM::LocalTranslationMovePointer(pragma::FeaSt::Cubic::SimFct::AbsSimFctPointer(), 1, 1, 0.0, pragma::FALSE, pragma::EGM::COARSE_DISPLAY_MOVE));

	/*
	Erkennungsrate:

	./zettel8_checks_Linux 1.txt 2.txt 0 -nographics
	Recognition Rate: 15 / 30 = 50%

	./zettel8_checks_Linux 1.txt 2.txt 1 -nographics
	Recognition Rate: 28 / 30 = 93.3333%

	./zettel8_checks_Linux 3.txt 4.txt 0 -nographics
	Recognition Rate: 5 / 30 = 16.6667%

	./zettel8_checks_Linux 3.txt 4.txt 1 -nographics
	Recognition Rate: 19 / 30 = 63.3333%
	*/

	/*
	GlobalTranslationMoveRep (const pragma::FeaSt::SimFctPointer simFctPointerA, 
	const pragma::UINT16 stepWidthA, 
	const pragma::UINT16 halfSizeOfLocalSearchAreaA, 
	const pragma::EGM::MoveDisplayMode moveDisplayModeA=pragma::EGM::DISABLE_DISPLAY_MOVE)

	ScalingMoveRep (const pragma::FeaSt::SimFctPointer simFctPointerA, 
	const pragma::REAL scaleFactorBeginA, 
	const pragma::UINT8 numberOfScalesA, 
	const pragma::REAL scaleFactorStepA, 
	const pragma::BOOL scaleHorizontallyA=pragma::TRUE, 
	const pragma::BOOL scaleVerticallyA=pragma::TRUE, 
	const pragma::BOOL scaleLogarithmicallyA=pragma::FALSE, 
	const pragma::EGM::MoveDisplayMode moveDisplayModeA=pragma::EGM::DISABLE_DISPLAY_MOVE)


	LocalTranslationMoveRep (const pragma::FeaSt::SimFctPointer simFctPointerA, 
	const pragma::UINT16 stepWidthA, 
	const pragma::UINT16 halfSizeOfLocalSearchAreaA, 
	const pragma::REAL topologyWeightA=0., 
	const pragma::BOOL moveNodesAfterCalculatingAllDisplacementsA=pragma::FALSE, 
	const pragma::EGM::MoveDisplayMode moveDisplayModeA=pragma::EGM::DISABLE_DISPLAY_MOVE)
	*/

	/*
	// custom Matching-Schedule 1  (liefert die besten Ergebnisse, relativ zu den zwei Weiteren!)
	matchingSchedulePointer->push_back(pragma::EGM::GlobalTranslationMovePointer(pragma::FeaSt::Cubic::SimFct::AbsSimFctPointer(), 10, 10, pragma::EGM::COARSE_DISPLAY_MOVE));
	matchingSchedulePointer->push_back(pragma::EGM::ScalingMovePointer(pragma::FeaSt::Cubic::SimFct::AbsSimFctPointer(), 0.85, 4, 0.05, pragma::TRUE, pragma::TRUE, pragma::FALSE, pragma::EGM::COARSE_DISPLAY_MOVE));
	matchingSchedulePointer->push_back(pragma::EGM::LocalTranslationMovePointer(pragma::FeaSt::Cubic::SimFct::AbsSimFctPointer(), 10, 10, 0.0, pragma::FALSE, pragma::EGM::COARSE_DISPLAY_MOVE));
	*/

	/*
	Erkennungsrate:

	./zettel8_checks_Linux 1.txt 2.txt 0 -nographics
	Recognition Rate: 17 / 30 = 56.6667%

	./zettel8_checks_Linux 1.txt 2.txt 1 -nographics
	Recognition Rate: 30 / 30 = 100%

	./zettel8_checks_Linux 3.txt 4.txt 0 -nographics
	Recognition Rate: 9 / 30 = 30%

	./zettel8_checks_Linux 3.txt 4.txt 1 -nographics
	Recognition Rate: 21 / 30 = 70%
	*/

	/*
	// custom Matching-Schedule 2
	matchingSchedulePointer->push_back(pragma::EGM::GlobalTranslationMovePointer(pragma::FeaSt::Cubic::SimFct::AbsSimFctPointer(), 5, 20, pragma::EGM::COARSE_DISPLAY_MOVE));
	matchingSchedulePointer->push_back(pragma::EGM::ScalingMovePointer(pragma::FeaSt::Cubic::SimFct::AbsSimFctPointer(), 0.75, 10, 0.05, pragma::TRUE, pragma::TRUE, pragma::FALSE, pragma::EGM::COARSE_DISPLAY_MOVE));
	matchingSchedulePointer->push_back(pragma::EGM::LocalTranslationMovePointer(pragma::FeaSt::Cubic::SimFct::AbsSimFctPointer(), 5, 20, 0.0, pragma::FALSE, pragma::EGM::COARSE_DISPLAY_MOVE));
	*/
	/*
	Erkennungsrate:

	./zettel8_checks_Linux 1.txt 2.txt 0 -nographics
	Recognition Rate: 23 / 30 = 76.6667%

	./zettel8_checks_Linux 1.txt 2.txt 1 -nographics
	Recognition Rate: 30 / 30 = 100%

	./zettel8_checks_Linux 3.txt 4.txt 0 -nographics
	Recognition Rate: 7 / 30 = 23.3333%

	./zettel8_checks_Linux 3.txt 4.txt 1 -nographics
	Recognition Rate: 18 / 30 = 60%
	*/

	pragma::EGM::MatcherPointer matcherPointer;
	matcherPointer->match(bunchGraphPointer, feastImagePointer, matchingSchedulePointer, matchWindowPointer, imagePointer, pragma::EGM::MOVE_DISPLAY_MODE_UNDEFINED, pragma::TRUE);
	matchGraphPointer = bunchGraphPointer;
}

//  Berechnung der Aehnlichkeit zweier Gesichtsgraphen
pragma::REAL modelGraphSimilarity(PRAGMA_CONST pragma::GraphLab::ModelGraphPointer &mgPointer1, PRAGMA_CONST pragma::GraphLab::ModelGraphPointer &mgPointer2, int simFunction)
{
	// ========== Aufgabe 2 (Teil 2) ==========
	// Welche Aehnlichkeitsfunktion zum Vergleichen der Identitaeten ist die bessere Wahl?
	// TODO: done!
	/*
		Die AbsSimFctPointer Aehnlichkeitsfunktion ist besser geeignet, 
		da hier bei nur Absolutwerte betrachtet werden. 
		Bei der Phase kann es aufgrund der Phasenrotation dazu kommen,
		dass Jets die nah beinander liegen, 
		stark unterschiedliche Koeffizienten haben, 
		obwohl die Jets ungefaehr dasselbe Merkmal repraesentieren.
		Die Absolutwerte sind viel robuster gegen leichte geometrische Variationen.
		Deswegen ist es besser nur die Absolutwerte zu betrachten.
		Das kann man auch aus einigen Beispielen leicht erkennen.
		Falls man 1.txt als galleryBilder und 3.txt als probeBilder nimmt, 
		erhaelt man bei Phase eine Erkennungsrate von ca 47% und bei Absolutwerte 
		eine Erkennungsrate von 100%.
		In allen anderen Faellen hat die Absolute Aehnlichkeitsfunktion besser abgeschnitten.
	*/

	// Aehnlichkeitsfunktion auswaehlen (0 = Phase, 1 = Absolutwert)
	pragma::FeaSt::SimFctPointer simFctPointer;
	if (simFunction == 0)
		simFctPointer = pragma::FeaSt::Cubic::SimFct::AbsPhaseSimFctPointer();
	else
		simFctPointer = pragma::FeaSt::Cubic::SimFct::AbsSimFctPointer();

	pragma::REAL similarity = 0.0;
	pragma::UINT16 numberOfUsedNodes = 0;

	// Ueber alle Knoten iterieren
	for (pragma::UINT16 nodeL = 0; nodeL < mgPointer1->numberOfNodes(); nodeL++)
	{
		PRAGMA_CONST pragma::GraphLab::ModelNodeInfoPointer &nodeInfoPointer1 = mgPointer1->node(nodeL)->modelPayload();
		PRAGMA_CONST pragma::GraphLab::ModelNodeInfoPointer &nodeInfoPointer2 = mgPointer2->node(nodeL)->modelPayload();

		similarity += simFctPointer->similarity(nodeInfoPointer1->feaStPointer(), nodeInfoPointer2->feaStPointer());
		numberOfUsedNodes++;
	}
	return similarity / static_cast<pragma::REAL>(numberOfUsedNodes);
}

// ========== Aufgabe 4 ==========
// Eine der sechs Listen als Galerie waehlen (Wahl begruenden)
// TODO: done!
/*
	1.txt eignet sich als Galerie, aus folgenden Gruenden:
	- die Lichtverhaeltnisse sind ideal
	- das Gesicht ist von keinem Objekt bedeckt
	- Person traegt eine neutrale Geste
	- die Blickrichtung der Person ist gerade
	- Gesicht ueberwiegend im Zentrum des Bildes

*/
// Erkennungsraten auf den uebrigen funf Listen angeben (besten Matching-Schedule und zu bevorzugende Ahenlichkeitsfunktion verwenden)
// TODO: done!

/*
bevorzugende Ahenlichkeitsfunktion: 1  (AbsSimFctPointer)
besten Matching-Schedule: 			custom Matching-Schedule 1

./zettel8_checks_Linux 1.txt 2.txt 1 -nographics
Recognition Rate: 30 / 30 = 100%

./zettel8_checks_Linux 1.txt 3.txt 1 -nographics
Recognition Rate: 26 / 30 = 86.6667%

./zettel8_checks_Linux 1.txt 4.txt 1 -nographics
Recognition Rate: 19 / 30 = 63.3333%

./zettel8_checks_Linux 1.txt 5.txt 1 -nographics
Recognition Rate: 20 / 30 = 66.6667%

./zettel8_checks_Linux 1.txt 6.txt 1 -nographics
Recognition Rate: 22 / 30 = 73.3333%


*/
