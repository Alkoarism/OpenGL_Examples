#include "bitmap_font.h"

using namespace std;

BitmapFont::BitmapFont(const char* vertexPath, const char* fragmentPath)
    : m_Shader(vertexPath, fragmentPath)
{
    m_CurX = m_CurY = 0;
    m_Red = m_Green = m_Blue = m_Alpha = 1.0f;
    m_InvertYAxis = false;
}

bool BitmapFont::Load(const char* fname)
{
    fstream in;
    char bpp;
    int ImgX, ImgY;

    in.open(fname, ios_base::binary | ios_base::in);

    if (in.fail())
        return false;

    // Get Filesize
    in.seekg(0, ios_base::end);
    auto fileSize = in.tellg();
    in.seekg(0, ios_base::beg);

    // allocate space for file data
    unique_ptr<char> dat(new char[fileSize]);

    // Read filedata
    if (!dat)
        return false;

    in.read(dat.get(), fileSize);

    if (in.fail())
    {
        in.close();
        return false;
    }

    in.close();

    // Check ID is 'BFF2'
    if ((unsigned char)dat.get()[0] != 0xBF || (unsigned char)dat.get()[1] != 0xF2)
        return false;

    // Grab the rest of the header
    memcpy(&ImgX, &dat.get()[2], sizeof(int));
    memcpy(&ImgY, &dat.get()[6], sizeof(int));
    memcpy(&m_CellX, &dat.get()[10], sizeof(int));
    memcpy(&m_CellY, &dat.get()[14], sizeof(int));
    bpp = dat.get()[18];
    m_Base = dat.get()[19];

    // Check filesize
    if (fileSize != ((MAP_DATA_OFFSET)+((ImgX * ImgY) * (bpp / 8))))
        return false;

    // Calculate font params
    m_RowPitch = ImgX / m_CellX;
    m_ColFactor = (float)m_CellX / (float)ImgX;
    m_RowFactor = (float)m_CellY / (float)ImgY;
    m_YOffset = m_CellY;

    // Determine blending options based on BPP
    switch (bpp)
    {
    case 8:
        //OpenGL supports single channel images through the RED channel
        m_Layout.SetFormat(GL_RED);
        break;

    case 24:
        m_Layout.SetFormat(GL_RGB);
        break;

    case 32:
        m_Layout.SetFormat(GL_RGBA);
        break;

    default: // Unsupported BPP
        return false;
        break;
    }

    // Allocate space for image
    unique_ptr<char> img(new char[(ImgX * ImgY) * (bpp / 8)]);

    if (!img)
        return false;

    // Grab char widths
    memcpy(m_Width, &dat.get()[WIDTH_DATA_OFFSET], 256);

    // Grab image data
    memcpy(img.get(), &dat.get()[MAP_DATA_OFFSET], (ImgX * ImgY) * (bpp / 8));

    m_Layout.SetType(GL_TEXTURE_2D);

    // Fonts should be rendered at native resolution so no need for texture filtering
    m_Layout.AddPar(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    m_Layout.AddPar(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // Stop chararcters from bleeding over edges
    m_Layout.AddPar(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    m_Layout.AddPar(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    m_Texture.SetLayout(m_Layout);
    m_Texture.DirectLoad(img.get(), ImgX, ImgY);

    Unbind();

    return true;
}

// Returns the width in pixels of the specified text
int BitmapFont::GetWidth(const char* Text)
{
    int size = 0;
    size_t sLen = strnlen(Text, BFG_MAXSTRING);

    for (size_t loop = 0; loop != sLen; loop++)
    {
        size += m_Width[Text[loop]];
    }

    return size;
}

// Set the position for text output, this will be updated as text is printed
void BitmapFont::SetCursor(const int& x, const int& y)
{
    m_CurX = x;
    m_CurY = y;
}

void BitmapFont::SetColor
    (const float& r, const float& g,
     const float& b, const float& a) {
    m_Red = r;
    m_Green = g;
    m_Blue = b;
    m_Alpha = a;
}

void BitmapFont::ReverseYAxis(const bool& State)
{
    if (State)
        m_YOffset = -m_CellY;
    else
        m_YOffset = m_CellY;

    m_InvertYAxis = State;
}

void BitmapFont::Print(const char* text) {
    //texture mapping, top and bottom
    float u, v, u1, v1;
    int row, col;
    size_t sLen = strnlen(text, BFG_MAXSTRING);

    Bind();

    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3
    };
    VertexBufferLayout vbl;
    vbl.Push<float>(3);
    vbl.Push<float>(2);
    vbl.Push<float>(4);

    IndexBuffer ib(indices, 6);

    for (size_t i = 0; i != sLen; i++) {
        row = (text[i] - m_Base) / m_RowPitch;
        col = (text[i] - m_Base) - (row * m_RowPitch);

        u = col * m_ColFactor;
        v = row * m_RowFactor;
        u1 = u + m_ColFactor;
        v1 = v + m_RowFactor;
        
        float coords[] = {
            //vertex coords			                       //texture	//colors	
           (m_CurX + m_CellX), (m_CurY + m_YOffset), 0.0f, u1, v,	    m_Red, m_Green, m_Blue, m_Alpha,    //top right
           (m_CurX + m_CellX),  m_CurY,              0.0f, u1, v1,	    m_Red, m_Green, m_Blue, m_Alpha,    //bottom right
            m_CurX,             m_CurY,              0.0f, u,  v1,	    m_Red, m_Green, m_Blue, m_Alpha,    //bottom left
            m_CurX,            (m_CurY + m_YOffset), 0.0f, u,  v,       m_Red, m_Green, m_Blue, m_Alpha     //top left
        };

        m_CurX += m_Width[text[i]];

        VertexArray va;

        VertexBuffer vb(coords, sizeof(coords));
        va.AddBuffer(vb, vbl);
        Renderer::Render(va, ib, m_Shader);
    }

    Unbind();
}

void BitmapFont::Print(const char* text, const int& x, const int& y) {
    SetCursor(x, y);
    Print(text);
}

void BitmapFont::Bind()
{
    m_Texture.Bind();
}

void BitmapFont::Unbind()
{
    m_Texture.Unbind();
}
