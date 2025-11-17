#include <VertexLayout.hpp>

namespace etugl {

VertexLayout& VertexLayout::add(const u32 location,
                                const LayoutData::Type type, 
                                const bool normalized) {
 
    const LayoutData layout_data(type); 
    const u32 size = layout_data.size();
  
    m_Elements.push_back({  
        .m_Location = location,  
        .m_Data = layout_data,
        .m_Normalized = normalized,
        .m_Offset = m_Stride
    });

    m_Stride += size;
    return *this;
}

void VertexLayout::bind() const {
    if (m_Elements.empty()) {
        LOG_WARN("VertexLayout bind not permitted for empty layour");
        return;
    }

    for (const auto& element : m_Elements) {
        glEnableVertexAttribArray(element.m_Location);

        if (!element.m_Data.is_integer()) {
            glVertexAttribPointer(
                element.m_Location, 
                element.m_Data.componets(),
                element.m_Data.gl_type(),
                element.m_Normalized ? GL_TRUE : GL_FALSE,
                m_Stride,
                (const void*)static_cast<uintptr_t>(element.m_Offset)
            );
        } else {
            glVertexAttribIPointer(
                element.m_Location, 
                element.m_Data.componets(),
                element.m_Data.gl_type(),
                m_Stride,
                (const void*)static_cast<uintptr_t>(element.m_Offset)
            );
        }
    }
}

}
