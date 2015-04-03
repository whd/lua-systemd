# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

# Define projects name
# The C tells CMake to check for C compiler
project ( systemd C )

# Define minimun version of CMAKE for compatibility
cmake_minimum_required ( VERSION 2.8 )

# Include dist.cmake - defines LuaDist related macros and install paths
# Also include lua macros
include ( cmake/dist.cmake )
include ( cmake/FindSystemd.cmake )
include ( lua )

if( UNIX AND NOT CYGWIN )
  add_definitions( -D_GNU_SOURCE )
endif( UNIX AND NOT CYGWIN )

# Find systemd
find_package ( Systemd REQUIRED ) # REQUIRED ensures that Systemd must be found
include_directories ( vendor/compat-5.3/c-api/ ) # We need Systemds's headers, so include them

# Build systemd module
install_lua_module ( systemd src/init.lua )
install_lua_module ( systemd.daemon src/daemon.lua )
install_lua_module ( systemd.daemon.core src/daemon.c LINK ${LIBSYSTEMD_DAEMON_LIBRARIES})
install_lua_module ( systemd.id128 src/id128.lua )
install_lua_module ( systemd.id128.core src/id128.c src/messages.c LINK ${LIBSYSTEMD_JOURNAL_LIBRARIES} ${LIBSYSTEMD_ID128_LIBRARIES})
install_lua_module ( systemd.journal src/journal.lua )
install_lua_module ( systemd.journal.core src/journal.c LINK ${LIBSYSTEMD_JOURNAL_LIBRARIES})
install_lua_module ( systemd.login src/login.lua )
install_lua_module ( systemd.login.core src/login.c LINK ${LIBSYSTEMD_LOGIN_LIBRARIES})
install_lua_module ( systemd.messages src/messages.lua )

# Install Data (points to root of the project specific data folder)
install_data ( README.md LICENSE )

# Install all files from examples directory into the default destination folder
install_example ( examples/ )
