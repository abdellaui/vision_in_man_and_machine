#ifndef FALTUNG_H
#define FALTUNG_H


//Tolga Akkiraz
//Abdullah Sahin

#include <pragma.h>

// ========== Filter ==========
class Filter
{
private:
	// ===== Variables =====
	pragma::Image::RealImagePointer filterE;

	// Aufsatzpunkt
	pragma::UINT32 xOffsetE; 
	pragma::UINT32 yOffsetE; 

protected:

public:
	// ===== Constructor =====
	Filter(pragma::Image::RealImagePointer filterA,
		pragma::UINT32 xOffsetA,
		pragma::UINT32 yOffsetA);

	// ===== Destructor =====
	~Filter();

	// ===== Functions =====
	pragma::REAL getValueAt(pragma::INT32 xA, pragma::INT32 yA) const;

	// Auflösung
	pragma::UINT32 getWidth() const;
	pragma::UINT32 getHeight() const;

	// Aufsatzpunkt
	pragma::UINT32 getXOffset() const;
	pragma::UINT32 getYOffset() const;

	// Sobelfilter
	static Filter getSobelX();
	static Filter getSobelY();
};


// ========== Faltung ==========
class Faltung
{
private:

protected:

public:
	// ===== Constructor =====
	Faltung();

	// ===== Destructor =====
	~Faltung();

	// ===== Functions =====
	// Faltet ein Bild
	pragma::Image::RealImagePointer	falte(
		const pragma::Image::ByteImagePointer& imageA,
		const Filter& filterA,
		pragma::CHAR modeA) const;

	// Berechnet die richtungsunabhängige Kantenstärke
	pragma::Image::RealImagePointer getKSUngerichtet(
		const pragma::Image::RealImagePointer& sxResultatA,
		const pragma::Image::RealImagePointer& syResultatA) const;

	// Berechnet die richtungsabhängige Kantenstärke bezüglich winkelA
	pragma::Image::RealImagePointer getKSGerichtet(
		const pragma::Image::RealImagePointer& sxResultatA,
		const pragma::Image::RealImagePointer& syResultatA,
		pragma::REAL winkelA) const;
};

#endif // FALTUNG_H
