#include <VertexLayout.hpp>

namespace etugl {

void VertexLayout::bind() const {
    if (m_Elements.empty()) {
        LOG_WARN("VertexLayout bind not permitted for empty layout");
        return;
    }

    for (const auto& element : m_Elements) {
        glEnableVertexAttribArray(element.m_Location);
        const void* ptr = (const void*)(uintptr_t)element.m_Offset;

        if (!element.m_IsInteger) {
          glVertexAttribPointer(
              element.m_Location,
              (GLint)element.m_Components,
              element.m_GLType,
              element.m_Normalized ? GL_TRUE : GL_FALSE,
              (GLsizei)m_Stride,
              ptr);
        } else {
          glVertexAttribIPointer(
              element.m_Location,
              (GLint)element.m_Components,
              element.m_GLType,
              (GLsizei)m_Stride,
              ptr);
        }
    }
}

}
