#pragma once

#include <concepts>
#include <cstddef>
#include <glad/glad.h>
#include <Types.hpp>
#include <utils.hpp>

namespace etugl {

class LayoutType {

    template <GLenum type, size_t bytes, u32 num>
    struct Type {
        [[nodiscard]] static constexpr u32 size() {return bytes*num; }
        
        [[nodiscard]] static constexpr u32 components() {return num;}
       
        [[nodiscard]] static constexpr u32 gl_type() {return type;}
    
        [[nodiscard]] static constexpr bool is_integer() {
            return type == GL_INT || type == GL_BOOL;
        }
    };

public:
    using Float  = Type<GL_FLOAT, sizeof(float), 1>;
    using Float2 = Type<GL_FLOAT, sizeof(float), 2>;
    using Float3 = Type<GL_FLOAT, sizeof(float), 3>;
    using Float4 = Type<GL_FLOAT, sizeof(float), 4>;


    using Int  = Type<GL_INT, sizeof(int), 1>;
    using Int2 = Type<GL_INT, sizeof(int), 2>;
    using Int3 = Type<GL_INT, sizeof(int), 3>;
    using Int4 = Type<GL_INT, sizeof(int), 4>;

    using Bool  = Type<GL_BOOL, sizeof(bool), 1>;
};

class VertexLayout {

public:
    struct VertexElement {
        u32 m_Location;    // shader location
        u32 m_Components;  // number of components (1..4)
        GLenum m_GLType;   // GL_FLOAT, GL_INT, ...
        bool m_Normalized; // only meaningful for integer->float via glVertexAttribPointer
        u32 m_Offset;      // byte offset within vertex
        bool m_IsInteger;  // use glVertexAttribIPointer when true
    }; 
 
    VertexLayout() = default; 
    ~VertexLayout() = default; 

    template <typename T> requires requires {
        { T::size() } -> std::convertible_to<u32>;
        { T::components() } -> std::convertible_to<u32>;
        { T::gl_type() } -> std::convertible_to<GLenum>;
        { T::is_integer() } -> std::same_as<bool>;
    }
    VertexLayout& add(const u32 location,
                      const bool normalized = false) 
    {
        constexpr u32 comps = T::components();
        constexpr u32 size = T::size();
        constexpr GLenum type = T::gl_type();
        constexpr bool is_integer = T::is_integer();

        const u32 offset = m_Stride;

        m_Elements.push_back(VertexElement{
            .m_Location = location,
            .m_Components = comps,
            .m_GLType = type,
            .m_Normalized = normalized,
            .m_Offset = offset,
            .m_IsInteger = is_integer,
        });

        m_Stride += size;
        return *this;
    }
 
    void bind() const;

private:
    std::vector<VertexElement> m_Elements;
    u32 m_Stride = 0;
};

}
