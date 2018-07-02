#include "pragma.h"

/* Hinweis:
Die Dateinamen sind in den Listen als "Bilder/datei.jpg" angegeben. Die .exe muss sich daher im selben Ordner wie der Bilder-Ordner befinden.
Dasselbe gilt fuer die Listen und Gesichtsgraphen. Entweder also die .exe verschieben oder die ganzen Dateien in den Ordner mit der kompilierten .exe.

In dieser Aufgabe geht es hauptsaechlich darum, sich in den vorhandenen Code einzuarbeiten. Daher ist die einzige zu ergaenzende Funktion der Kreuzvergleich  
in der main() (5P). Fuer alle anderen Teilaufgaben sollen lediglich verschiedene Matching-Schedules ausprobiert und die Fragen beantwortet werden (4P + 3P + 3P).
Die dafuer relevanten Stellen sind durch Kommentare im Code angegeben. Bis auf den Schedule selber muss nichts mehr am Code geaendert werden.
*/

// Funktionen
void readFileList(std::string fileName,
                  pragma::vector<std::string> &vec);
void readImages(const std::string &fileName,
                pragma::vector<pragma::Image::ByteImagePointer> &imageVector);
void createModelGraphSet(const std::string fileName,
                         pragma::GraphLab::ModelGraphSet &modelGraphSet);
void matchOnImage(const pragma::Image::ByteImagePointer &imagePointer,
                  const pragma::GraphLab::ModelGraphSet &modelGraphSet,
                  pragma::Graphics::WindowPointer &matchWindowPointer,
                  pragma::GraphLab::ModelGraphPointer &matchGraphPointer);
pragma::REAL modelGraphSimilarity(const pragma::GraphLab::ModelGraphPointer &mgPointer1,
                                  const pragma::GraphLab::ModelGraphPointer &mgPointer2,
                                  int simFunction);

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

  // Aehnlichkeitsfunktion
  int simBool = atoi(argv[3]);
  pragma_assert(simBool == 0 || simBool == 1);

  // ModelGraphSet, ModelGraphPointer und entsprechende Fenster erstellen
  pragma::GraphLab::ModelGraphSet modelGraphSet;
  createModelGraphSet("bg2.txt", modelGraphSet);

  pragma::vector<pragma::GraphLab::ModelGraphPointer> galleryGraphVector;
  pragma::vector<pragma::GraphLab::ModelGraphPointer> probeGraphVector;

  pragma::Graphics::WindowPointer matchWindowPointer("Match Window", 220, 10);
  pragma::Graphics::WindowPointer imageWindowPointer("Image Window", 20, 10);

  // Auf Galeriebilder matchen und Modelgraphen erstellen
  cout << "Matching gallery... " << endl;
  for (unsigned int i = 0; i < galleryVector.size(); i++)
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
  for (unsigned int i = 0; i < probeVector.size(); i++)
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

  unsigned int numberOfCorrectRecognised = 0;

  // probeGraphVector durchlaufen
  for (unsigned int probeIndex = 0; probeIndex < probeGraphVector.size(); probeIndex++)
  {
    unsigned int bestIndex = 0;                 // Index des Bildes mit der hoechsten Aehnlichkeit
    pragma::REAL bestSimilarity = 0.0; // Wert der hoechsten Aehnlichkeit

    // Mit galleryGraphVector vergleichen (vorhandene Funktionen nutzen und darauf achten, welche Werte ausgegeben werden sollen)
    // TODO: done!

    // currentProbe = probeGraphVector.at(probeIndex)
    for (unsigned int galleryIndex = 0; galleryIndex < galleryGraphVector.size(); galleryIndex++)
    {
      // currentGallery = galleryGraphVector.at(galleryIndex)
      pragma::REAL tempResult = modelGraphSimilarity(galleryGraphVector.at(galleryIndex), probeGraphVector.at(probeIndex), simBool);

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
void readFileList(std::string fileName, pragma::vector<std::string> &fileList)
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
void readImages(const std::string &fileName, pragma::vector<pragma::Image::ByteImagePointer> &imageVector)
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
void createModelGraphSet(const std::string fileName, pragma::GraphLab::ModelGraphSet &modelGraphSet)
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
void matchOnImage(const pragma::Image::ByteImagePointer &imagePointer,
                  const pragma::GraphLab::ModelGraphSet &modelGraphSet,
                  pragma::Graphics::WindowPointer &matchWindowPointer,
                  pragma::GraphLab::ModelGraphPointer &matchGraphPointer)
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
  // TODO:

  // ========== Aufgabe 3 ==========
  // Vorgegebenen Matching-Schedule und zwei weitere ausprobieren und Erkennungsraten fuer zwei oder mehr Kombinationen von Galerie- und Probe-Liste angeben
  // Alle benutzten Matching-Schedules bitte auskommentiert im Code lassen
  // TODO:

  /* Moves in der Dokumentation (enthaelt Erklaerungen zu den Argumenten):
	- GlobalTranslationMove: html/classpragma_1_1EGM_1_1GlobalTranslationMoveRep.html
	- ScalingMove: html/classpragma_1_1EGM_1_1ScalingMoveRep.html
	- LocalTranslationMove: html/classpragma_1_1EGM_1_1LocalTranslationMoveRep.html
	*/

  // Vorgegebener Matching-Schedule
  matchingSchedulePointer->push_back(pragma::EGM::GlobalTranslationMovePointer(pragma::FeaSt::Cubic::SimFct::AbsSimFctPointer(), 10, pragma::EGM::COARSE_DISPLAY_MOVE));
  matchingSchedulePointer->push_back(pragma::EGM::GlobalTranslationMovePointer(pragma::FeaSt::Cubic::SimFct::AbsSimFctPointer(), 2, 10, pragma::EGM::COARSE_DISPLAY_MOVE));
  matchingSchedulePointer->push_back(pragma::EGM::ScalingMovePointer(pragma::FeaSt::Cubic::SimFct::AbsSimFctPointer(), 0.8, 5, 0.1, pragma::EGM::COARSE_DISPLAY_MOVE));
  matchingSchedulePointer->push_back(pragma::EGM::ScalingMovePointer(pragma::FeaSt::Cubic::SimFct::AbsSimFctPointer(), 0.95, 3, 0.05, pragma::EGM::COARSE_DISPLAY_MOVE));
  matchingSchedulePointer->push_back(pragma::EGM::LocalTranslationMovePointer(pragma::FeaSt::Cubic::SimFct::AbsSimFctPointer(), 1, 1, 0.0, pragma::FALSE, pragma::EGM::COARSE_DISPLAY_MOVE));

  pragma::EGM::MatcherPointer matcherPointer;
  matcherPointer->match(bunchGraphPointer, feastImagePointer, matchingSchedulePointer, matchWindowPointer, imagePointer, pragma::EGM::MOVE_DISPLAY_MODE_UNDEFINED, pragma::TRUE);
  matchGraphPointer = bunchGraphPointer;
}

//  Berechnung der Aehnlichkeit zweier Gesichtsgraphen
pragma::REAL modelGraphSimilarity(const pragma::GraphLab::ModelGraphPointer &mgPointer1,
                                  const pragma::GraphLab::ModelGraphPointer &mgPointer2,
                                  int simFunction)
{
  // ========== Aufgabe 2 (Teil 2) ==========
  // Welche Aehnlichkeitsfunktion zum Vergleichen der Identitaeten ist die bessere Wahl?
  // TODO:

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
    const pragma::GraphLab::ModelNodeInfoPointer &nodeInfoPointer1 = mgPointer1->node(nodeL)->modelPayload();
    const pragma::GraphLab::ModelNodeInfoPointer &nodeInfoPointer2 = mgPointer2->node(nodeL)->modelPayload();

    similarity += simFctPointer->similarity(nodeInfoPointer1->feaStPointer(), nodeInfoPointer2->feaStPointer());
    numberOfUsedNodes++;
  }
  return similarity / static_cast<pragma::REAL>(numberOfUsedNodes);
}

// ========== Aufgabe 4 ==========
// Eine der sechs Listen als Galerie waehlen (Wahl begruenden)
// TODO:

// Erkennungsraten auf den uebrigen funf Listen angeben (besten Matching-Schedule und zu bevorzugende Ahenlichkeitsfunktion verwenden)
// TODO: