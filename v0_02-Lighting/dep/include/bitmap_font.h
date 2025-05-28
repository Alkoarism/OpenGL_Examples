#ifndef _BITMAP_FONTCLASS_H
#define _BITMAP_FONTCLASS_H

#include "OpenGL\renderer.h"
#include "OpenGL\texture.h"

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
	BitmapFont(const char*, const char*);

	bool Load(const char* fname);
	int  GetWidth(const char* Text);

	void SetCursor(const int&, const int&);
	void SetColor
		(const float&, const float&, const float&, const float& a = 1.0f);
	void ReverseYAxis(const bool&);
	void Print(const char*);
	void Print(const char*, const int&, const int&);

private:
	int m_CellX, m_CellY, m_YOffset, m_RowPitch;
	char m_Base;
	char m_Width[256];
	int m_CurX, m_CurY;
	float m_RowFactor, m_ColFactor;
	float m_Red, m_Green, m_Blue, m_Alpha;
	bool m_InvertYAxis;

	Shader m_Shader;
	Texture m_Texture;
	TextureLayout m_Layout;

	void Bind();
	void Unbind();
};

#endif