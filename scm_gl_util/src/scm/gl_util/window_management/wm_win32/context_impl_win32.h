
#ifndef SCM_GL_UTIL_WM_WIN32_CONTEXT_IMPL_WIN32_H_INCLUDED
#define SCM_GL_UTIL_WM_WIN32_CONTEXT_IMPL_WIN32_H_INCLUDED

#include <scm/core/platform/platform.h>

#if SCM_PLATFORM == SCM_PLATFORM_WINDOWS

#include <scm/core/platform/windows.h>

#include <scm/core/pointer_types.h>

#include <scm/gl_util/window_management/context.h>

namespace scm {
namespace gl {
namespace wm {

namespace util {

class wgl_extensions;

} // namespace util

struct context::context_impl
{
    context_impl(const surface_ptr&     in_surface,
                 const attribute_desc&  in_attributes,
                 const context_ptr&     in_share_ctx);
    virtual ~context_impl();

    bool                    make_current(const surface_ptr& in_surface, bool current) const;
    void                    cleanup();

    HGLRC                   _context_handle;
    bool                    _swap_control_supported;

    shared_ptr<util::wgl_extensions>  _wgl_extensions;

}; // class context_impl

} // namespace wm
} // namepspace gl
} // namepspace scm

#endif // SCM_PLATFORM == SCM_PLATFORM_WINDOWS
#endif // SCM_GL_UTIL_WM_WIN32_CONTEXT_IMPL_WIN32_H_INCLUDED
