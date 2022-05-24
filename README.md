# libsnowflakeid

[![Latest release](https://img.shields.io/badge/latest_release-1.2.0-orange.svg)](https://github.com/thibaultmeyer/libsnowflakeid/releases)
[![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/thibaultmeyer/libsnowflakeid/blob/master/LICENSE)
[![CodeFactor](https://www.codefactor.io/repository/github/thibaultmeyer/libsnowflakeid/badge)](https://www.codefactor.io/repository/github/thibaultmeyer/libsnowflakeid)
[![Repository size](https://img.shields.io/github/repo-size/thibaultmeyer/libsnowflakeid.svg?logo=git)](https://github.com/thibaultmeyer/libsnowflakeid)

A tiny and portable library to generate Snowflake IDs.
*****

## Build & install from sources
To compile and install this project, you must ensure that make, cmake, gcc or
clang or MSVC are being correctly installed.

```bash
#> mkdir cmake-build-release
#> cd cmake-build-release
#> cmake -DCMAKE_BUILD_TYPE=Release ..
#> make
#> make install
```

## Usage
```c
#include <snowflakeid.h>

int main(const int argc, const char *const *argv) {
    enum e_snowflakeid_init_status     status_out;
    struct s_snowflakeid_generator_ctx *ctx = snowflakeid_initialize(
            datacenter_id,
            worker_id,
            offset_time_ms,
            &status_out);
    
    if (status_out == SNOWFLAKEID_INIT_SUCCESS) {
        uint64_t id = snowflakeid_next_value(ctx);
        
        snowflakeid_destroy(ctx);
    }
    
    return (0);
}
```

```bash
#> cc example.c -lsnowflakeid
```


## License
This project is released under terms of the [MIT license](https://github.com/thibaultmeyer/libsnowflakeid/blob/master/LICENSE).
