#include "Modules/imgLoader/imgLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <Modules/imgLoader/stb_image.h>
#define STBI_FAILURE_USERMSG
#define STBI_ONLYJPEG

ImgLoader::ImgLoader(const char* location, bool flipImage){
    stbi_set_flip_vertically_on_load(flipImage);
	m_data = stbi_load(location, &m_imgWidth, &m_imgHeight, &m_nrChannels, 0);
    if (m_data == nullptr){
        m_failLog = stbi_failure_reason();
        std::cout << "ERROR::IMGLOAD::FAILED_TO_LOAD\n" << m_failLog << std::endl;
    }
    stbi_image_free(m_data);
}
