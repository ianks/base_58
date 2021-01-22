require "mkmf"

$CFLAGS << " -std=c99 -Wno-declaration-after-statement"

have_library('crypto')
have_library('base58')

have_header("stdbool.h")
have_header("unistd.h")
have_header('openssl/sha.h')

dir_config "base_58"

create_makefile("base_58/base_58")
