#if defined(__GNUC__) && defined (__ELF__)
#include <charon-core/configVersion.h>
/// charon-core version information (invalid: all zero)
__attribute__ ((section (".charon-plugin"), used))
const unsigned char charon_core_version[] = {0, 0, 0};
#endif
