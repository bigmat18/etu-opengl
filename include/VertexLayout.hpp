#pragma once

#include <cstdint>
#include <glad/glad.h>
#include <Types.hpp>
#include <utils.hpp>

namespace etugl {

struct LayoutData {
    enum class Type {
        None = 0, 
        Float, Float2, Float3, Float4, 
        Int, Int2, Int3, Int4, 
        Mat2, Mat3, Mat4, 
        Bool
    };

    constexpr LayoutData() : m_Type(Type::None) {}
    constexpr LayoutData(Type type) : m_Type(type) {}

    [[nodiscard]] constexpr u32 size() const {
        switch (m_Type){
            case Type::Float:     return 4;
            case Type::Float2:    return 4 * 2;
            case Type::Float3:    return 4 * 3;
            case Type::Float4:    return 4 * 4;
            case Type::Mat2:      return 4 * 2 * 2;
            case Type::Mat3:      return 4 * 3 * 3;
            case Type::Mat4:      return 4 * 4 * 4;
            case Type::Int:       return 4;
            case Type::Int2:      return 4 * 2;
            case Type::Int3:      return 4 * 3;
            case Type::Int4:      return 4 * 4;
            case Type::Bool:      return 1;
            default:
                LOG_WARN("GLType None not supported");
                return 0;
        }
        LOG_WARN("Unknown LayoutData type in size()");
        __builtin_unreachable();
        return 0; 
    };

    [[nodiscard]] constexpr u32 componets() const {
        switch (m_Type){
            case Type::Float:     return 1;
            case Type::Float2:    return 2;
            case Type::Float3:    return 3;
            case Type::Float4:    return 4;
            case Type::Mat2:      return 2 * 2;
            case Type::Mat3:      return 3 * 3;
            case Type::Mat4:      return 4 * 4;
            case Type::Int:       return 1;
            case Type::Int2:      return 2;
            case Type::Int3:      return 3;
            case Type::Int4:      return 4;
            case Type::Bool:      return 1;
            default:
                LOG_WARN("GLType None not supported");
                return 0;
        }
        LOG_WARN("Unknown LayoutData type in size()");
        __builtin_unreachable();
        return 0; 
    };

    [[nodiscard]] constexpr GLenum gl_type() const {
        switch (m_Type) {
            case Type::Float:   return GL_FLOAT;
            case Type::Float2:  return GL_FLOAT;
            case Type::Float3:  return GL_FLOAT;
            case Type::Float4:  return GL_FLOAT;
            case Type::Mat2:    return GL_FLOAT; 
            case Type::Mat3:    return GL_FLOAT;
            case Type::Mat4:    return GL_FLOAT;
            case Type::Int:     return GL_INT;
            case Type::Int2:    return GL_INT;
            case Type::Int3:    return GL_INT;
            case Type::Int4:    return GL_INT;
            case Type::Bool:    return GL_BOOL;
            default:            
                LOG_WARN("GLType None not supported");
                return 0;
        }
        LOG_WARN("Unknown LayoutData type in size()");
        __builtin_unreachable();
        return 0;
    };

    [[nodiscard]] constexpr bool is_integer() const {
        return m_Type == Type::Int || m_Type == Type::Int2 || 
               m_Type == Type::Int3 || m_Type == Type::Int4;
    }
    
private:
    Type m_Type;
};


class VertexLayout {

public:
    struct VertexElement {
        u32 m_Location;     // Index (position in shader)
        LayoutData m_Data;  // Type of data 
        bool m_Normalized;  // If data should be normalized
        u32 m_Offset;       // Local stride  
    }; 
 
    VertexLayout() = default; 
    ~VertexLayout() = default; 
 
    VertexLayout& add(const u32 location,
                      const LayoutData::Type type, 
                      const bool normalized = false);
 

    void bind() const;

private:
    std::vector<VertexElement> m_Elements;
    u32 m_Stride = 0;
};

}
