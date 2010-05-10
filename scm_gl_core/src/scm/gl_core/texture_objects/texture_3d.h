
#ifndef SCM_GL_CORE_TEXTURE_3D_H_INCLUDED
#define SCM_GL_CORE_TEXTURE_3D_H_INCLUDED

#include <vector>

#include <scm/core/math.h>

#include <scm/gl_core/data_formats.h>
#include <scm/gl_core/texture_objects/texture.h>

#include <scm/core/platform/platform.h>
#include <scm/core/utilities/platform_warning_disable.h>

namespace scm {
namespace gl {

struct __scm_export(gl_core) texture_3d_desc
{
    texture_3d_desc(const math::vec3ui& in_size,
                    const data_format   in_format,
                    const unsigned      in_mip_levels = 1);

    bool operator==(const texture_3d_desc& rhs) const;
    bool operator!=(const texture_3d_desc& rhs) const;

    math::vec3ui    _size;
    data_format     _format;
    unsigned        _mip_levels;
}; // struct texture_3d_desc

class __scm_export(gl_core) texture_3d : public texture
{
public:
    virtual ~texture_3d();

    const texture_3d_desc&  descriptor() const;
    void                    print(std::ostream& os) const {};

    data_format             format() const;
    math::vec2ui            dimensions() const;
    unsigned                array_layers() const;
    unsigned                mip_map_layers() const;
    unsigned                samples() const;

protected:
    texture_3d(render_device&            in_device,
               const texture_3d_desc&    in_desc);
    texture_3d(render_device&            in_device,
               const texture_3d_desc&    in_desc,
               const data_format         in_initial_data_format,
               const std::vector<void*>& in_initial_mip_level_data);

    void                    bind(const render_context& in_context, int in_unit) const;
    void                    unbind(const render_context& in_context) const;

    bool                    image_data(const render_device&      in_device,
                                       const texture_3d_desc&    in_desc,
                                       const data_format         in_initial_data_format,
                                       const std::vector<void*>& in_initial_mip_level_data);
    bool                    image_sub_data();

protected:
    texture_3d_desc         _descriptor;

private:

    friend class render_device;
    friend class render_context;
}; // class texture_3d

} // namespace gl
} // namespace scm

#include <scm/core/utilities/platform_warning_enable.h>

#endif // SCM_GL_CORE_TEXTURE_3D_H_INCLUDED