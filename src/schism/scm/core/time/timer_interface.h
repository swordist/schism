
#ifndef TIMER_INTERFACE_H_INCLUDED
#define TIMER_INTERFACE_H_INCLUDED

#include <scm/core/time/time_types.h>

#include <scm/core/platform/platform.h>

#include <scm/core/utilities/platform_warning_disable.h>

namespace scm {
namespace time {

class __scm_export(core) timer_interface
{
public:
    timer_interface() : _duration(0, 0, 0, 0) {};
    virtual ~timer_interface() {};

    virtual void                start()             = 0;
    virtual void                stop()              = 0;

    time_duration               get_time() const { return (_duration); }

protected:
    time_duration               _duration;

private:

}; // class timer_interface

} // namespace core
} // namespace scm

#include <scm/core/utilities/platform_warning_enable.h>

#endif // TIMER_INTERFACE_H_INCLUDED