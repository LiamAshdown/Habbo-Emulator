#pragma once
#include <plog/Record.h>
#include <plog/Util.h>
#include <iomanip>

namespace plog
{
    template<bool useUtcTime>
    class TxtFormatterImpl
    {
    public:
        static util::nstring header()
        {
            return util::nstring();
        }

        static util::nstring format(const Record& record)
        {
            tm t;
            (useUtcTime ? util::gmtime_s : util::localtime_s)(&t, &record.getTime().time);

            util::nostringstream ss;

            ss << PLOG_NSTR("[");
            ss << severityToString(record.getSeverity());
            ss << PLOG_NSTR("]");
            ss << PLOG_NSTR(" ");
            ss << PLOG_NSTR(">>");
            ss << PLOG_NSTR(" ");
            ss << record.getMessage();
            ss << PLOG_NSTR("\n");

            return ss.str();
        }
    };

    class TxtFormatter : public TxtFormatterImpl<false> {};
    class TxtFormatterUtcTime : public TxtFormatterImpl<true> {};
}
