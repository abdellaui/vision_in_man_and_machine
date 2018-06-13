/*
Abdullah Sahin
Tolga Akkiraz
*/
#include "Faltung.h"

// Main function
int pragma::main(int argc, char *argv[])
{
  std::cout << "Sehen in Mensch und Maschine Aufgabe 3\n\n";

  // Faltungsobjekt
  //Faltung faltung;

  // Name des Eingabebildes
  pragma::string bildNameL("../images/Kante_22.jpg");

  // Bild einlesen
  pragma::Image::ByteImagePointer bildL;
  bildL->read(bildNameL);

  // Bild anzeigen
  pragma::Graphics::WindowPointer fensterL;
  fensterL->image(bildL);

  //std::getchar();

  // Faltung durchführen
  Faltung faltungL;
  // TODO: done!

  pragma::Image::RealImagePointer sX = faltungL.falte(bildL, Filter::createSobelX(), 'u');
  pragma::Image::RealImagePointer sY = faltungL.falte(bildL, Filter::createSobelY(), 'u');

  pragma::Image::ByteImagePointer b_sX(sX);
  pragma::Image::ByteImagePointer b_sY(sY);
  b_sX->writeJPEG("b_sX.jpg");
  b_sY->writeJPEG("b_sY.jpg");

  // Absolute Kantenstärke ermitteln und in RealImage ausgeben
  // TODO: done!
  pragma::Image::RealImagePointer betrag = faltungL.betrag(sX, sY);
  pragma::Image::ByteImagePointer b_betrag(betrag);
  b_betrag->writeJPEG("b_betrag.jpg");

  // Richtungsabhängige Kantenstärke für bestimmte Richtung ermitteln und in RealImage ausgeben
  // TODO: done!
  pragma::REAL directionDegree = 90.0;
  pragma::REAL winkelL = pragma::PI * directionDegree / 180.0;
  pragma::Image::RealImagePointer richtung = faltungL.richtung(sX, sY, winkelL);
  pragma::Image::ByteImagePointer b_richtung(richtung);
  b_richtung->writeJPEG("b_richtung.jpg");

  // ====== Aufgabe 3.3 ======

  // Stoppuhr-Objekt anlegen um Faltungszeit zu messen
  Stopwatch stopperL(Stopwatch::REAL_TIME);
  const pragma::string pathToGabor = "../gabors/";
  const pragma::string prefixGabor = "kernel_l";
  const pragma::string middleGabor = "_d";
  const pragma::string suffixGabor = "_real";
  const pragma::string endGabor = ".txt";

  std::getchar();

  for (pragma::UINT32 gX = 0; gX <= 4; gX++)
  {

    for (pragma::UINT32 gY = 0; gY <= 7; gY++)
    {
      pragma::string gabor1NameL = prefixGabor + std::to_string(gX) + middleGabor + std::to_string(gY) + suffixGabor;
      Filter currFilter;
      currFilter.readGabor(pathToGabor + gabor1NameL + endGabor);
      stopperL.start(pragma::TRUE);
      pragma::Image::RealImagePointer currResult = faltungL.falte(bildL, currFilter, 'z');
      stopperL.stop();
      pragma::REAL executedTime = stopperL.executionTime();

      
        pragma::Image::ByteImagePointer currResultToSave(currResult);
        currResultToSave->writeJPEG("gabor_results/"+gabor1NameL + ".jpg");
        

      std::cout << gabor1NameL << "  [ " << currFilter.xRes() << " x " << currFilter.yRes() << " ]  ";
      std::cout << " : " << executedTime << "s" << std::endl;
    }
  }

  std::getchar();
  return 0;
}
