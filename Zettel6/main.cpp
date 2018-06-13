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
//! \file     ./programs/task/Zettel6/main.cpp
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
#include "ImageManipulator.h"

// ---- global includes ------------------------------------------------------
#include <pragma.h>
#include <iostream>

// ---- constants ------------------------------------------------------------

// ---- typedefs -------------------------------------------------------------

// ---- external functions ---------------------------------------------------


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
  std::cout << "Vorlesung Sehen in Mensch und Maschine Aufgabe 6\n\n";

  //Parameter von Kommandozeile einlesen, benötigt werden Name vom Bild aus dem
  //das Template erzeugt wird, Name des Testbildes,
  //Position des Auschnitts im ersten Bild, Breite des Ausschnitts, Skalierung
  //und Rauschstärke für Testbild

  //Eingabebild einlesen
  pragma::Image::ByteImagePointer imageL;
  imageL->read(imageNameL);

  //Bildausschnitt ausschneiden
  ImageManipulator imageManipulatorL(..);
  //Filter aus Bildausschnitt erzeugen
  pragma::Image::RealImagePointer filterL(..);


  //Testbild einlesen und gegebenenfalls mit ImageManipulator abändern
  imageL->read(testImageL);
  ImageManipulator imageManipulator2L(..);
  

  //Filter mit Bild falten (inklusive Normierung)
  pragma::Image::RealImagePointer convolvedImageL = convolve(..);

  //Position maximaler Antwort bestimmen und Filter in Testbild reinkopieren


  //Faltungsbild und Testbild auf Festplatte speichern


  return 0;
}
