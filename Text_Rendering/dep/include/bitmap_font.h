#ifndef _BITMAP_FONTCLASS_H
#define _BITMAP_FONTCLASS_H

#include "OpenGL\renderer.h"

#define BFG_MAXSTRING 255     // Maximum string length

#define WIDTH_DATA_OFFSET  20 // Offset to width data with BFF file
#define MAP_DATA_OFFSET   276 // Offset to texture image data with BFF file

typedef struct
{
	unsigned char ID1, ID2;
	unsigned char BPP;
	int ImageWidth, ImageHeight, CellWidth, CellHeight;
	unsigned char StartPoint;
}FontFileHeader;

class BitmapFont
{
public:
	BitmapFont(Shader&, Texture&);

	bool Load(const char* fname);
	int  GetWidth(const char* Text);

	void SetCursor(const int& x, const int& y);
	void SetColor
		(const float& r, const float& g, const float& b, const float& a = 1.0f);
	void ReverseYAxis(const bool&);
	void Print(const char* txt);
	void Print(const char* txt, const int& x, const int& y);

private:
	int m_CellX, m_CellY, m_YOffset, m_RowPitch;
	char m_Base;
	char m_Width[256];
	int m_CurX, m_CurY;
	float m_RowFactor, m_ColFactor;
	float m_Red, m_Green, m_Blue, m_Alpha;
	bool m_InvertYAxis;

	Shader& m_Shader;
	Texture& m_Texture;

	void Bind();
	void Unbind();
};

#endif