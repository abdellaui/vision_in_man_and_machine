#include "Faltung.h"

// Constructor
Faltung::Faltung()
{
}

// Destructor
Faltung::~Faltung()
{
}

// Diese Funktion faltet ein Bild bei gegebenem Filter und festgelegten Randbedingungen.
// R�ckgabewert ist das gefaltete Bild.
pragma::Image::RealImagePointer Faltung::falte(const pragma::Image::ByteImagePointer image,
											   const Filter &filter,
											   const pragma::CHAR &mode) const
{
	// Das gefaltete Bild
	pragma::Image::RealImagePointer resultL(image->xResolution(), image->yResolution());
	resultL->init(0);

	for (pragma::UINT32 iX = 0; iX < image->xResolution(); iX++)
	{
		for (pragma::UINT32 iY = 0; iY < image->yResolution(); iY++)
		{
			pragma::BOOL ignoring = false;
			pragma::REAL currValue = 0;
			for (pragma::UINT32 fX = 0; fX < filter.xRes(); fX++)
			{
				for (pragma::UINT32 fY = 0; fY < filter.yRes(); fY++)
				{
					pragma::UINT32 tempX = filter.absoluteCoardX(fX, iX);
					pragma::UINT32 tempY = filter.absoluteCoardY(fY, iY);

					pragma::REAL tempValue = 0;
					// ignore undefinied pixel
					if (mode == 'u')
					{
						if (tempX < 0 || tempY < 0 || tempX >= image->xResolution() || tempY >= image->yResolution())
						{
							ignoring = true;
							break;
						}

						
						tempValue = image->pixel(tempX, tempY);
					}
					// warp-around
					else if (mode == 'w')
					{
						// add image resolution to prevent negative values
						tempX = (tempX + image->xResolution()) % image->xResolution();
						tempY = (tempY + image->yResolution()) % image->yResolution();
						tempValue = image->pixel(tempX, tempY);
					}
					// zero-padding
					else if (mode == 'z')
					{
						if (tempX >= 0 && tempY >= 0 && tempX < image->xResolution() && tempY < image->yResolution())
							tempValue = image->pixel(tempX, tempY);
					}
					else
					{
						std::cout << "aviable modes are u, w, z" << std::endl;
						exit(EXIT_FAILURE);
					}
					if (!ignoring)
						currValue += tempValue * filter.pixel(fX, fY);
				}
				if (ignoring)
					break;
			}
			if (!ignoring)
				resultL->pixel(iX, iY, currValue);
		}
	}
	if (mode == 'u')
	{
		// make a smaller image
		// remove offset


		/*
		i know its not the best way to do that,
		but i go a different way to handle mode u at the beginning,
		so now i have to adapt it on my existing solution
		*/


		pragma::UINT32 minX = filter.xHotspot();
		pragma::UINT32 maxX = resultL->xResolution() - 1 - filter.xHotspot();
		pragma::UINT32 minY = filter.yHotspot();
		pragma::UINT32 maxY = resultL->yResolution() - 1 - filter.yHotspot();

		pragma::Image::RealImagePointer tempCopy(resultL, pragma::TRUE);
		resultL->resize(tempCopy, pragma::IntRect(minX, maxX, minY, maxY), maxX - minX + 1, maxY - minY + 1);
	}
	return resultL;
}

// Funktionen f�r richtungs(un)abh�ngige Kantenst�rke, Sobelfilter usw.
// TODO: done!

pragma::Image::RealImagePointer Faltung::betrag(const pragma::Image::RealImagePointer SxResultatA,
												const pragma::Image::RealImagePointer SyResultatA) const
{

	if(SxResultatA->xResolution() != SyResultatA->xResolution() ||
	   SxResultatA->yResolution() != SyResultatA->yResolution() )
	{
		std::cout << "read following exception:";
		std::getchar();
	}
	// TODO: done!
	//pragma::Image::RealImageRep tmpResult = SxResultatA*SxResultatA;
	pragma::Image::RealImagePointer temp_sX(SxResultatA);
	pragma::Image::RealImagePointer temp_sY(SyResultatA);


	(*temp_sX) *= (*temp_sX); // (G_x)^2
	(*temp_sY) *= (*temp_sY); // (G_y)^2

	pragma::Image::RealImagePointer tempResult(temp_sX);
	(*tempResult) += (*temp_sY); // (G_x)^2 + (G_y)^2
	tempResult->sqrValue();		 // sqrt( (G_x)^2 + (G_y)^2 )
	return tempResult;
}
pragma::Image::RealImagePointer Faltung::richtung(const pragma::Image::RealImagePointer SxResultatA,
												  const pragma::Image::RealImagePointer SyResultatA,
												  const pragma::REAL &winkelA) const
{
	if(SxResultatA->xResolution() != SyResultatA->xResolution() ||
	   SxResultatA->yResolution() != SyResultatA->yResolution() )
	{
		std::cout << "read following exception:";
		std::getchar();
	}
	// TODO: done!

	pragma::Image::RealImagePointer temp_sX(SxResultatA);
	pragma::Image::RealImagePointer temp_sY(SyResultatA);

	(*temp_sX) *= cos(winkelA);   // sX * cos(a)
	(*temp_sY) *= sin(winkelA);	  // sY * sin(a)

	(*temp_sX) += (*temp_sY);	  // sX * cos(a) + sY * sin(a)
	return temp_sX;
}

// FILTER

Filter::Filter() : m_xHotspot(0), m_yHotspot(0) {}
Filter::Filter(const pragma::Image::RealImagePointer filter) : m_filter(filter)
{
	// setting default hotspot
	setHotspot(xRes() / 2, yRes() / 2);
}

const Filter Filter::createSobelX(void)
{
	pragma::Image::RealImagePointer tempSobel(3, 3);
	tempSobel->init(0);

	// col 1
	tempSobel->pixel(0, 0, 1);
	tempSobel->pixel(0, 1, 2);
	tempSobel->pixel(0, 2, 1);

	// col 3
	tempSobel->pixel(2, 0, -1);
	tempSobel->pixel(2, 1, -2);
	tempSobel->pixel(2, 2, -1);

	(*tempSobel) /= 8.0;
	return Filter(tempSobel);
}
const Filter Filter::createSobelY(void)
{
	pragma::Image::RealImagePointer tempSobel(3, 3);
	tempSobel->init(0);

	// row 1
	tempSobel->pixel(0, 0, 1);
	tempSobel->pixel(1, 0, 2);
	tempSobel->pixel(2, 0, 1);

	// row 3
	tempSobel->pixel(0, 2, -1);
	tempSobel->pixel(1, 2, -2);
	tempSobel->pixel(2, 2, -1);

	(*tempSobel) /= 8.0;
	return Filter(tempSobel);
}
void Filter::readGabor(pragma::string pathToGabor)
{
	m_filter->read(pathToGabor);
	setHotspot(xRes() / 2, yRes() / 2);
}

const pragma::UINT32 Filter::xRes(void) const
{
	return m_filter->xResolution();
}
const pragma::UINT32 Filter::yRes(void) const
{
	return m_filter->yResolution();
}

const pragma::UINT32 Filter::absoluteCoardX(const pragma::UINT32 &relX,
											const pragma::UINT32 &imgX) const
{
	return imgX - xHotspot() + relX;
}

const pragma::UINT32 Filter::absoluteCoardY(const pragma::UINT32 &relY,
											const pragma::UINT32 &imgY) const
{
	return imgY - yHotspot() + relY;
}

const pragma::REAL Filter::pixel(const pragma::UINT32 &x, const pragma::UINT32 &y) const
{
	return m_filter->pixel(x, y);
}

void Filter::setHotspot(const pragma::UINT32 &x, const pragma::UINT32 &y)
{
	// x, y are uint so dont need to check if they are smaller than 0
	if (x < xRes() && y < yRes())
	{
		m_xHotspot = x;
		m_yHotspot = y;
	}
}
const pragma::UINT32 Filter::xHotspot(void) const
{
	// default hotspot is on the center

	return m_xHotspot;
}
const pragma::UINT32 Filter::yHotspot(void) const
{
	// default hotspot is on the center
	return m_yHotspot;
}
