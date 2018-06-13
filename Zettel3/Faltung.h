#ifndef FALTUNG_H
#define FALTUNG_H

#include <pragma.h>

class Filter
{
  public:
	Filter();
	Filter(const pragma::Image::RealImagePointer filter);

	static const Filter createSobelX(void);
	static const Filter createSobelY(void);
	void readGabor(pragma::string pathToGabor);

	const pragma::UINT32 xRes(void) const;
	const pragma::UINT32 yRes(void) const;
	const pragma::UINT32 absoluteCoardX(const pragma::UINT32 &relX,
										const pragma::UINT32 &imgX) const;
	const pragma::UINT32 absoluteCoardY(const pragma::UINT32 &relY,
										const pragma::UINT32 &imgY) const;
	const pragma::REAL pixel(const pragma::UINT32 &x, const pragma::UINT32 &y) const;

	void setHotspot(const pragma::UINT32 &x, const pragma::UINT32 &y);
	const pragma::UINT32 xHotspot(void) const;
	const pragma::UINT32 yHotspot(void) const;

  private:
	pragma::Image::RealImagePointer m_filter;
	pragma::UINT32 m_xHotspot;
	pragma::UINT32 m_yHotspot;
};

class Faltung
{
  private:
  protected:
  public:
	// Constructor
	Faltung();

	// Destructor
	~Faltung();

	// Faltet ein Bild
	// TODO: done!
	pragma::Image::RealImagePointer falte(const pragma::Image::ByteImagePointer image,
										  const Filter &filter,
										  const pragma::CHAR &mode) const;

	// Funktionen für richtungs(un)abhängige Kantenstärke, Sobelfilter usw.
	// TODO: done!
	pragma::Image::RealImagePointer betrag(const pragma::Image::RealImagePointer SxResultatA,
										   const pragma::Image::RealImagePointer SyResultatA) const;
	pragma::Image::RealImagePointer richtung(const pragma::Image::RealImagePointer SxResultatA,
											 const pragma::Image::RealImagePointer SyResultatA,
											 const pragma::REAL &winkelA) const;
};

#endif // FALTUNG_H
