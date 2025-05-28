#ifndef TEXTURE_LAYOUT_H
#define TEXTURE_LAYOUT_H

#include "headers.h"

class TextureLayout {
public:
	TextureLayout() = default;
	TextureLayout(GLenum type, GLenum format) 
		: m_Type(type), m_Format(format) { }

	inline GLenum GetType() const { return m_Type; }
	inline GLenum GetFormat() const { return m_Format; }

	inline void SetType(const GLenum& type) { m_Type = type; }
	inline void SetFormat(const GLenum& format) { m_Format = format; }

	void AddPar(const GLenum& first, const GLenum& second)
	{ m_Layout.insert({ first, second });	}
	void RemovePar() { m_Layout.erase(--m_Layout.end()); }
	void RemovePar(const GLenum& par) { m_Layout.erase(par); }

	void Run() const {
		for (auto a : m_Layout) {
			glTexParameteri(m_Type, a.first, a.second);
		}
	}

private:
	GLenum m_Type;
	GLenum m_Format;
	std::map<GLenum, GLenum> m_Layout;
};

#endif