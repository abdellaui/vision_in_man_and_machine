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
//! \file     ./programs/task/Zettel7/main.cpp
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

// ---- typedefs -------------------------------------------------------------

// ---- external functions ---------------------------------------------------


// Diese Funktion kopiert das übergebene Bild und markiert darin jede Stelle
// des Gitters, dessen LINKE OBERE ECKE (nicht der Mittelpunkt) bei xA,yA
// liegt, mit einem 5*5-Pixel großen Quadrat.
pragma::Image::ByteImagePointer
setGraph
(
  const pragma::Image::ByteImagePointer& byteImagePointerA,
  const pragma::UINT32& xA,
  const pragma::UINT32& yA,
  const pragma::UINT32& stepsizeA,
  const pragma::UINT32& nodesInRowA,
  const pragma::UINT32& nodesInColumnA
)
{
  pragma::Image::ByteImagePointer byteImagePointerL(byteImagePointerA,pragma::TRUE);
  for (pragma::UINT32 nXL = 0; nXL < nodesInRowA; nXL++)
  {
    for (pragma::UINT32 nYL = 0; nYL < nodesInColumnA; nYL++)
    {
      pragma::UINT32 xNodeL = xA + nXL * stepsizeA;
      pragma::UINT32 yNodeL = yA + nYL * stepsizeA;
      for (pragma::INT32 xL = -5; xL < 5; xL++)
      {
        for (pragma::INT32 yL = -5; yL < 5; yL++)
        {
          if ((xNodeL+xL >= 0) && (xNodeL+xL < byteImagePointerL->xResolution())
              && (yNodeL+yL >= 0) && (yNodeL+yL < byteImagePointerL->yResolution()))
          {
            if (std::sqrt(static_cast<pragma::REAL>(xL*xL + yL*yL)) < 2)
              byteImagePointerL->pixel(xNodeL+xL, yNodeL+yL) = 255;
            if (std::sqrt(static_cast<pragma::REAL>(xL*xL + yL*yL)) < 1)
              byteImagePointerL->pixel(xNodeL+xL, yNodeL+yL) = 0;
          }
        }
      }
    }
  }

  return byteImagePointerL;
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
  std::cout
    << "Vorlesung Sehen in Mensch und Maschine Aufgabe 7\n\n";

  if (argc != 9)
  {
    std::cout
      << "Usage:\n"
      << argv[0]
      << " <InputImage1> <InputImage2> <OutputImage> <X> <Y> <Stepsize> <nodes per row> <nodes per column>\n";

    return -1;
  } // if

  pragma::string inputImage1NameL(argv[1]);
  pragma::string inputImage2NameL(argv[2]);
  pramga::string outputImageNameL(argv[3]);
  pragma::INT32 xGraphL = atoi(argv[4]);
  pragma::INT32 yGraphL = atoi(argv[5]);
  pragma::UINT32 stepsizeL = atoi(argv[6]);
  pragma::UINT32 nodesInRowL = atoi(argv[7]);
  pragma::UINT32 nodesInColumnL = atoi(argv[8]);

  // Bilder einlesen

  pragma::Image::ByteImagePointer byteImagePointer1L;
  byteImagePointer1L->read(inputImage1NameL);

  pragma::Image::ByteImagePointer byteImagePointer2L;
  byteImagePointer2L->read(inputImage2NameL);

  // Bilder anzeigen

  pragma::Graphics::WindowPointer imageWindowPointer1L("Image Window",20,10);
  imageWindowPointer1L->image(byteImagePointer1L);

  pragma::Graphics::WindowPointer imageWindowPointer2L("Image Window2",220,10);
  imageWindowPointer2L->image(byteImagePointer2L);

  // FeaStImages erzeugen, jeder Pixel eines Feasts ist ein pragma::FeaSt::FeaStPointer
  pragma::Trafo::ComplexTrafoImagePointer complexTrafoImagePointer1L;
  pragma::Trafo::ComplexTrafoImagePointer complexTrafoImagePointer2L;
  pragma::Trafo::GaborTrafoPointer trafoPointer1L;
  pragma::Trafo::GaborTrafoPointer trafoPointer2L;
  trafoPointer1L->transform(byteImagePointer1L, complexTrafoImagePointer1L);
  trafoPointer2L->transform(byteImagePointer2L, complexTrafoImagePointer2L);
  pragma::FeaSt::FeaStImagePointer feaStImagePointer1L(pragma::FeaSt::CUBIC_ABS_PHASE_FEAST, complexTrafoImagePointer1L);
  pragma::FeaSt::FeaStImagePointer feaStImagePointer2L(pragma::FeaSt::CUBIC_ABS_PHASE_FEAST, complexTrafoImagePointer2L);

  // Ein einzelnes FeaSt kann dann über die Pixelfunktion ausgelesen werden (um
  // es zum Beispiel im Gittergraphen zu speichern).
  pragma::FeaSt::FeaStPointer feaStL=feaStImagePointer1L->pixel(xL,yL);


  // Gittergraph an Position xGraph yGraph aus Galeriebild erstellen; dieser
  // speichert alle FeaStPointer, auf denen die Graphknoten liegen.


  // Testbild mit dem Gittergraph mit Schrittweite 5 Pixel an allen Stellen
  // durchlaufen, an denen der Graph ins Bild passt (links oben anfangen), und
  // durchschnittliche Ähnlichkeit berechnen
  // Dabei jeweils Ähnlichkeitswert ins Ausgabebild schreiben

  // Zum Vergleichen der FeaSts eine dieser beiden Ähnlichkeitsfunktionen benutzen
  pragma::FeaSt::Cubic::SimFct::AbsPhaseSimFctPointer simFctPointerL;
  //pragma::FeaSt::Cubic::SimFct::AbsSimFctPointer simFctPointerL;

  // Zwei FeaSts werden dann wie folgt verglichen      
  pragma::REAL simL = simFctPointerL->similarity(feaStImagePointer1A->pixel(xL,yL),feaStImagePointer2A->pixel(xL,yL));

  // Stelle mit maximaler Ähnlichkeit bestimmen, Ähnlichkeit ausgeben und Graph
  // an entsprechender Stelle mit setGraph im Testbild markieren



  std::getchar();

  return 0;
} // method


// ====== END OF FILE ========================================================
