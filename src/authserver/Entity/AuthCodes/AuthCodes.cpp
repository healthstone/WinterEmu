#include "AuthCodes.hpp"
#include "src/authserver/Entity/ClientBuildInfo/ClientBuildInfo.hpp"

namespace AuthHelper
{
    constexpr static uint32_t MAX_PRE_BC_CLIENT_BUILD = 6141;

    bool IsPreBCAcceptedClientBuild(uint32_t build)
    {
        return build <= MAX_PRE_BC_CLIENT_BUILD && ClientBuild::GetBuildInfo(build);
    }

    bool IsPostBCAcceptedClientBuild(uint32_t build)
    {
        return build > MAX_PRE_BC_CLIENT_BUILD && ClientBuild::GetBuildInfo(build);
    }

    bool IsAcceptedClientBuild(uint32_t build)
    {
        return ClientBuild::GetBuildInfo(build) != nullptr;
    }
}
