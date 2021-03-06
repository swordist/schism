
// Copyright (c) 2012 Christopher Lux <christopherlux@gmail.com>
// Distributed under the Modified BSD License, see license.txt.

#include "wgl.h"

#if SCM_PLATFORM == SCM_PLATFORM_WINDOWS

#include <boost/tokenizer.hpp>

#include <scm/core/pointer_types.h>

#include <scm/log.h>

#include "context_helper.h"

namespace scm {
namespace gl {
namespace test {

wgl::wgl()
{
    _initialized = false;

    // WGL_ARB_create_context
    wglCreateContextAttribsARB      = 0;

    // WGL_ARB_pixel_format
    wglGetPixelFormatAttribivARB    = 0;
    wglGetPixelFormatAttribfvARB    = 0;
    wglChoosePixelFormatARB         = 0;

    // WGL_ARB_extensions_string
    wglGetExtensionsStringARB       = 0;

    // WGL_EXT_swap_control
    wglSwapIntervalEXT              = 0;
    wglGetSwapIntervalEXT           = 0;

    // WGL_ARB_pbuffer
    wglCreatePbufferARB             = 0;
    wglGetPbufferDCARB              = 0;
    wglReleasePbufferDCARB          = 0;
    wglDestroyPbufferARB            = 0;
    wglQueryPbufferARB              = 0;
}

bool
wgl::initialize()
{
    if (is_initialized()) {
        return (true);
    }

    scm::scoped_ptr<test::classic_gl_context> dummy_context;
    if (wglGetCurrentContext() == 0) {
        dummy_context.reset(new test::classic_gl_context);
        if (!dummy_context->create()) {
            scm::err() << log::error
                    << "wgl::initialize(): unable to initialize dummy OpenGL context" << log::end;
            return (false);
        }
    }

    // WGL_ARB_extensions_string
    wglGetExtensionsStringARB       = (PFNWGLGETEXTENSIONSSTRINGARBPROC)wglGetProcAddress("wglGetExtensionsStringARB");

    if (wglGetExtensionsStringARB == 0) {
        scm::err() << log::error
                 << "wgl::initialize(): WGL_ARB_extensions_string not supported" << log::end;
        return (false);
    }

    // get and tokenize the extension strings
    HDC cur_hdc = wglGetCurrentDC();
    if (cur_hdc == 0) {
        scm::err() << log::error
                << "wgl::initialize(): unable to retrieve current HDC" << log::end;
        return (false);
    }
    std::string wgl_ext_string = reinterpret_cast<const char*>(wglGetExtensionsStringARB(cur_hdc));

    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
    boost::char_separator<char> space_separator(" ");
    tokenizer                   extension_strings(wgl_ext_string, space_separator);

    for (tokenizer::const_iterator i = extension_strings.begin(); i != extension_strings.end(); ++i) {
        _wgl_extensions.insert(std::string(*i));
    }

    // WGL_ARB_create_context
    wglCreateContextAttribsARB      = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

    if (wglCreateContextAttribsARB == 0) {
        scm::err() << log::error
                << "wgl::initialize(): WGL_ARB_create_context not supported" << log::end;
        return (false);
    }

    // WGL_ARB_pixel_format
    wglGetPixelFormatAttribivARB    = (PFNWGLGETPIXELFORMATATTRIBIVARBPROC)wglGetProcAddress("wglGetPixelFormatAttribivARB");
    wglGetPixelFormatAttribfvARB    = (PFNWGLGETPIXELFORMATATTRIBFVARBPROC)wglGetProcAddress("wglGetPixelFormatAttribfvARB");
    wglChoosePixelFormatARB         = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");

    if (   wglGetPixelFormatAttribivARB == 0
        || wglGetPixelFormatAttribfvARB == 0
        || wglChoosePixelFormatARB == 0) {
        scm::err() << log::error
                << "wgl::initialize(): WGL_ARB_pixel_format not supported" << log::end;
        return (false);
    }

    // WGL_EXT_swap_control
    wglSwapIntervalEXT              = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
    wglGetSwapIntervalEXT           = (PFNWGLGETSWAPINTERVALEXTPROC)wglGetProcAddress("wglGetSwapIntervalEXT");

    if (   wglSwapIntervalEXT == 0
        || wglGetSwapIntervalEXT == 0) {
        scm::err() << log::error
                << "wgl::initialize(): WGL_EXT_swap_control not supported" << log::end;
        return (false);
    }

    if (  !is_supported("WGL_ARB_framebuffer_sRGB")
        ||!is_supported("WGL_EXT_framebuffer_sRGB")) {
        out() << log::warning
                << "wgl::initialize(): WGL_ARB_framebuffer_sRGB not supported" << log::end;
    }
    if (!is_supported("WGL_ARB_multisample")) {
        out() << log::warning
                << "wgl::initialize(): WGL_ARB_multisample not supported" << log::end;
    }

    // WGL_ARB_pbuffer
    wglCreatePbufferARB             = (PFNWGLCREATEPBUFFERARBPROC)wglGetProcAddress("wglCreatePbufferARB");
    wglGetPbufferDCARB              = (PFNWGLGETPBUFFERDCARBPROC)wglGetProcAddress("wglGetPbufferDCARB");
    wglReleasePbufferDCARB          = (PFNWGLRELEASEPBUFFERDCARBPROC)wglGetProcAddress("wglReleasePbufferDCARB");
    wglDestroyPbufferARB            = (PFNWGLDESTROYPBUFFERARBPROC)wglGetProcAddress("wglDestroyPbufferARB");
    wglQueryPbufferARB              = (PFNWGLQUERYPBUFFERARBPROC)wglGetProcAddress("wglQueryPbufferARB");

    if (   wglCreatePbufferARB == 0
        || wglGetPbufferDCARB == 0
        || wglReleasePbufferDCARB == 0
        || wglDestroyPbufferARB == 0
        || wglQueryPbufferARB == 0) {
        scm::err() << log::error
                << "wgl::initialize(): WGL_ARB_pbuffer not supported" << log::end;
        return (false);
    }
    _initialized = true;

    return (true);
}

bool
wgl::is_initialized() const
{
    return (_initialized);
}

bool
wgl::is_supported(const std::string& ext) const
{
    if (_wgl_extensions.find(ext) != _wgl_extensions.end()) {
        return (true);
    }
    else {
        return (false);
    }
}

} //namespace test
} // namespace gl
} // namespace scm

#endif // SCM_PLATFORM == SCM_PLATFORM_WINDOWS
