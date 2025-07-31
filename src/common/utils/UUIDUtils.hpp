#pragma once

#include <string>
#include <boost/uuid/uuid_io.hpp>

namespace UUIDUtils {

    inline std::string UUIDtoString(boost::uuids::uuid value) {
        return boost::uuids::to_string(value);
    }

} // namespace NetUtils
