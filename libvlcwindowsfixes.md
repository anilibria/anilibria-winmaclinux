# Fixes for 3.0.20
## plugins/vlc_common.h
`#include <vlc_arrays.h>` -> `//#include <vlc_arrays.h>`
## plugins/vlc_threads.h
need add `#include "../../core/compat/poll.h"`
## libvlc_media.h
need add few lines  
```cpp
#if defined(_MSC_VER)
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#endif
```

## plugins/vlc_common.h
`#include "vlc_configuration.h"` -> `//#include "vlc_configuration.h"`
