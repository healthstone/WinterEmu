#include <cstdint>

enum ExpansionFlags
{
    POST_BC_EXP_FLAG                            = 0x2,
    PRE_BC_EXP_FLAG                             = 0x1,
    NO_VALID_EXP_FLAG                           = 0x0
};

namespace AuthHelper
{
    bool IsAcceptedClientBuild(uint32_t build);
    bool IsPostBCAcceptedClientBuild(uint32_t build);
    bool IsPreBCAcceptedClientBuild(uint32_t build);
}