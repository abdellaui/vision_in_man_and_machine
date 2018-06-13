// =============================================================================
//  PRaGMA is a library of Pattern Recognition and Graph Matching Algorithms.
// =============================================================================
//  Copyright 2007-2014 Institut fuer Neuroinformatik, Ruhr-Universitaet Bochum
//
//  This file is part of PRaGMA.
//  PRaGMA is free software: you can redistribute it and/or modify it under
//  the terms of the GNU Lesser General Public License as published by the
//  Free Software Foundation, either version 3 of the License, or (at your
//  option) any later version.
//  PRaGMA is distributed in the hope that it will be useful, but WITHOUT
//  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
//  License for more details.
//  You should have received a copy of the GNU Lesser General Public License
//  along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// =============================================================================
//
//  Author:   Andreas Nilkens
//
//  Email:    andreas.nilkens@ini.rub.de
//
//  Address:  Ruhr-Universitaet Bochum
//            Institut fuer Neuroinformatik
//            Universitaetsstr. 150
//            D-44801 Bochum, Germany
//
// ===========================================================================
//
//! \file     ./programs/task/Zettel8/main.cpp
//! \ingroup  ./programs/task
//
// ===========================================================================
//
//  $HeadURL$
//  $Revision$
//  $Date$
//  $Author$
//
// ===========================================================================
// ===========================================================================


// ---- local includes -------------------------------------------------------

#include "pragma.h"

// ---- global includes ------------------------------------------------------

// ---- constants ------------------------------------------------------------


// Funktion zum Einlesen einer Liste von Dateinamen aus einer txt-Datei. Die
// Dateinamen werden in vecA gespeichert
PRAGMA_VOID fileList(std::string dateiNameA, pragma::vector<std::string>& vecA)
{
  vecA.clear();
  FILE* f = fopen(dateiNameA.c_str(), "r");
  char bufferL[500];
  while (!feof(f))
  {
    if( NULL != fgets(bufferL, sizeof(bufferL), f) )
    {
      char* pCL = strchr(bufferL,'\n');
      if( NULL != pCL )
        *pCL = '\0';
      vecA.push_back(bufferL);
    }
  }
  fclose(f);
}


// Funktion zur Berechnung der Ähnlichkeit zweier Gesichtsgraphen
pragma::REAL modelGraphSimilarity
(
  PRAGMA_CONST pragma::GraphLab::ModelGraphPointer& mg1PointerA,
  PRAGMA_CONST pragma::GraphLab::ModelGraphPointer& mg2PointerA
 )
{
  //pragma::FeaSt::Cubic::SimFct::AbsPhaseSimFctPointer absSimFctPointerL;
  pragma::FeaSt::Cubic::SimFct::AbsSimFctPointer absSimFctPointerL;

  // iterieren ueber alle knoten
  pragma::REAL similarityL = 0.0;
  pragma::UINT16 numberOfUsedNodesL = 0;
  for (pragma::UINT16 nodeL = 0; nodeL < mg1PointerA -> numberOfNodes(); nodeL ++)
  {
    PRAGMA_CONST pragma::GraphLab::ModelNodeInfoPointer& nodeInfoPointer1L
      = mg1PointerA -> node(nodeL)->modelPayload();
    PRAGMA_CONST pragma::GraphLab::ModelNodeInfoPointer& nodeInfoPointer2L
      = mg2PointerA -> node(nodeL)->modelPayload();


    similarityL += (..)
    ++numberOfUsedNodesL;
  }

  return similarityL / static_cast<pragma::REAL>(numberOfUsedNodesL);
}


// Funktion zum Einlesen eines Sets von Gesichtsgraphen
PRAGMA_VOID createModelGraphSet
(
  const std::string fileNameA,
  pragma::GraphLab::ModelGraphSet& modelGraphSetA
)
{
  pragma::vector<std::string> fileListL;
  fileList(fileNameA, fileListL);

  for (pragma::vector<std::string>::const_iterator pFilenameL=fileListL.begin();
       pFilenameL != fileListL.end();
       pFilenameL ++)
  {
    pragma::GraphLab::GraphPointer graphPointerL;
    graphPointerL.read(*pFilenameL);
    pragma::GraphLab::ModelGraphPointer modelGraphPointerL(graphPointerL);
    modelGraphSetA.push_back(modelGraphPointerL);
  }
}


// Funktion zum Detektieren eines Gesichts auf imagePointerA mittels verschiedener Moves
// und zum Auslesen des entsprechenden Gesichtsgraphen an der bestimmten Stelle
PRAGMA_VOID matchOnImage(
  const pragma::Image::ByteImagePointer& imagePointerA,
  const pragma::GraphLab::ModelGraphSet& modelGraphSetA,
  pragma::Graphics::WindowPointer& matchWindowPointerA,
  pragma::GraphLab::ModelGraphPointer& matchGraphPointerA)
{

  pragma::GraphLab::BunchModelGraphPointer bunchGraphPointerL(modelGraphSetA);

  // FeaSt Image Erstellen -----------------------------------------------------

  pragma::Trafo::GaborTrafoPointer trafoPointerL;
  pragma::Trafo::ComplexTrafoImagePointer  complexTrafoImagePointerL;

  trafoPointerL->transform(imagePointerA, complexTrafoImagePointerL);
  pragma::FeaSt::FeaStImagePointer feaStImagePointerL(
    pragma::FeaSt::CUBIC_ABS_PHASE_FEAST, complexTrafoImagePointerL);

  // Matching ------------------------------------------------------------------

  pragma::EGM::MatchingSchedulePointer matchingSchedulePointerL;

  matchingSchedulePointerL->push_back
  (
    pragma::EGM::GlobalTranslationMovePointer
    (
      pragma::FeaSt::Cubic::SimFct::AbsSimFctPointer(),
      10, // step width
      pragma::EGM::COARSE_DISPLAY_MOVE
    )
  );
  matchingSchedulePointerL->push_back
  (
    pragma::EGM::GlobalTranslationMovePointer
    (
      pragma::FeaSt::Cubic::SimFct::AbsSimFctPointer(),
      2, // step width
      10, // half width of local search area
      pragma::EGM::COARSE_DISPLAY_MOVE
    )
  );
  matchingSchedulePointerL->push_back
  (
    pragma::EGM::ScalingMovePointer
    (
      pragma::FeaSt::Cubic::SimFct::AbsSimFctPointer(),
      0.8,
      5,
      0.1,
      pragma::EGM::COARSE_DISPLAY_MOVE
    )
  );
  matchingSchedulePointerL->push_back
  (
    pragma::EGM::ScalingMovePointer
    (
      pragma::FeaSt::Cubic::SimFct::AbsSimFctPointer(),
      0.95,
      3,
      0.05,
      pragma::EGM::COARSE_DISPLAY_MOVE
    )
  );
  matchingSchedulePointerL->push_back
  (
    pragma::EGM::LocalTranslationMovePointer
    (
      pragma::FeaSt::Cubic::SimFct::AbsSimFctPointer(),
      1, // Step width in both directions.
      1, // Half size of local search area in both directions.
      0.0, // Don't account for topology cost.
      pragma::FALSE, // Is irrelevant if topology cost is zero.
      pragma::EGM::COARSE_DISPLAY_MOVE
    )
  );


  pragma::EGM::MatcherPointer matcherPointerL;

  matcherPointerL->match
  ( bunchGraphPointerL,
    feaStImagePointerL,
    matchingSchedulePointerL,
    matchWindowPointerA,
    imagePointerA,
    pragma::EGM::MOVE_DISPLAY_MODE_UNDEFINED,
    pragma::TRUE);

  matchGraphPointerA = bunchGraphPointerL;

  //pragma::GraphLab::GraphPointer(matchGraphPointerA).write("test.xml");
}



// Funktion zum Einlesen mehrerer Bilder und Abspeichern derselben in vectorOfImagesA,
// wobei die Dateinamen der Bilder in der Datei mit Namen fileNameA stehen
PRAGMA_VOID readImages(
  const std::string& fileNameA,
  pragma::vector<pragma::Image::ByteImagePointer>& vectorOfImagesA)
{
  pragma::vector<std::string> fileListL;
  fileList(fileNameA, fileListL);
  for (pragma::vector<std::string>::const_iterator pFilenameL=fileListL.begin();
       pFilenameL != fileListL.end();
       pFilenameL ++)
  {
    pragma::Image::ByteImagePointer byteImagePointerL;
    byteImagePointerL -> read(*pFilenameL);
    vectorOfImagesA.push_back(byteImagePointerL);
  }
}


/*************************************************************************//*!

          PRAGMA main function.

\param    argc                  number of command line arguments.

\param    argv                  vector of command line arguments.

\retval   0                     on success.

\retval   -1                    on failure.

\author   Andreas Nilkens

\date     2017-04-26

*//**************************************************************************/
int pragma::main( int argc, char *argv[] )
//----------------------------------------------------------------------------
{
  std::cout << "Vorlesung Sehen in Mensch und Maschine Aufgabe 8\n\n";

  if (argc!=3)
  {
    std::cout << "Usage:\n" << argv[0] << " <gallery> <probe>\n";

    return 0;
  } // if

  std::string galleryNameL(argv[1]);
  std::string probeNameL(argv[2]);

  // Gallery Bilder einlesen ---------------------------------------------------

  pragma::vector<pragma::Image::ByteImagePointer> vectorOfGalleryImagesL;
  readImages(galleryNameL, vectorOfGalleryImagesL);
  // Proben Bilder einlesen ----------------------------------------------------

  pragma::vector<pragma::Image::ByteImagePointer> vectorOfProbeImagesL;
  readImages(probeNameL, vectorOfProbeImagesL);


  // ModelGraphSet erstellen ---------------------------------------------------

  pragma::GraphLab::ModelGraphSet modelGraphSetL;
  createModelGraphSet("bg2.txt", modelGraphSetL);


  // Auf Gallery Bilder matchen und Modelgraphen erstellen ---------------------

  pragma::vector<pragma::GraphLab::ModelGraphPointer> vectorOfGalleryGraphsL;
  pragma::vector<pragma::GraphLab::ModelGraphPointer> vectorOfProbeGraphsL;

  pragma::Graphics::WindowPointer matchWindowPointerL("Match Window",220,10);

  pragma::Graphics::WindowPointer imageWindowPointerL("Image Window",20,10);

  for (pragma::UINT32 iL = 0; iL < vectorOfGalleryImagesL.size(); iL++)
  {
    imageWindowPointerL->image(vectorOfGalleryImagesL[iL]);
    pragma::GraphLab::ModelGraphPointer modelGraphPointerL;
    matchOnImage(vectorOfGalleryImagesL[iL], modelGraphSetL,
                 matchWindowPointerL, modelGraphPointerL);
    vectorOfGalleryGraphsL.push_back(modelGraphPointerL);
  }

  // Auf Probe Bilder matchen und Modelgraphen erstellen -----------------------

  for (pragma::UINT32 iL = 0; iL < vectorOfProbeImagesL.size(); iL++)
  {
    imageWindowPointerL->image(vectorOfProbeImagesL[iL]);
    pragma::GraphLab::ModelGraphPointer modelGraphPointerL;
    matchOnImage(vectorOfProbeImagesL[iL], modelGraphSetL,
                 matchWindowPointerL, modelGraphPointerL);
    vectorOfProbeGraphsL.push_back(modelGraphPointerL);
  }

  // Cross Run durchführen (also jeden Modellgraphen mit jedem Probegraphen vergleichen) -------------------------------

  std::cout << "cross run" << std::endl;

  (...)

  for (pragma::UINT32 probeIndexL= 0;
       probeIndexL < vectorOfProbeGraphsL.size();
       probeIndexL ++)
  {



    (...)


    if (bestIndexL == probeIndexL)
      numberOfCorrectlyRecognisedL ++;

    std::cout << "Probenbild: " << probeIndexL + 1
              << "  aehnlichstes Galeriebild: " << bestIndexL + 1
              << "  erkannt?: " << (bestIndexL == probeIndexL)
              << "  Aehnlichkeit: " << bestSimL << std::endl;

  }

  std::cout << "Recognition Rate: "
            << numberOfCorrectlyRecognisedL << " / "
            << vectorOfProbeGraphsL.size() << " = "
            << (static_cast<pragma::REAL>(numberOfCorrectlyRecognisedL) /
                static_cast<pragma::REAL>(vectorOfProbeGraphsL.size())) * 100.0 << "%\n";


  return 0;
} // method


// ====== END OF FILE ========================================================
