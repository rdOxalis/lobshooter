# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project

LobShooter for Oracle (`lobshooter`) — a C++ command-line tool plus shared library (`libloadutil`) that uploads and downloads LOBs (CLOB/BLOB) to and from an Oracle database via OCCI. LGPL 2.1, originally hosted on SourceForge SVN (the checked-in `html/` Doxygen output still contains SVN conflict leftovers: `*.mine`, `*.r34`, `*.r35`).

## Build

Requires `ORACLE_HOME` to be set before configuring — `cmake/modules/FindOracle.cmake` searches **only** under `$ORACLE_HOME` (`NO_DEFAULT_PATH`), and both `OCCI` and `CLNTSH` are `REQUIRED`.

```bash
export ORACLE_HOME=/path/to/instantclient_or_server   # e.g. /usr/lib/oracle/xe/app/oracle/product/10.2.0/server
cmake .          # in-source build (the dot is intentional; INSTALL documents this)
make all
sudo make install    # -> /usr/local/lib/libloadutil.*, /usr/local/bin/lobshooter
./uninstall.sh       # removes both
```

In-source builds drop `CMakeCache.txt`, `CMakeFiles/`, `Makefile` and `cmake_install.cmake` into the repo root and into `lib/` and `lobshooter/`; `.gitignore` covers all of them.

Verified working combination (August 2026): CMake 4.2.3, g++ 15.2, Oracle Instant Client 23.26 against an Oracle 26ai server. The Instant Client layout fits `FindOracle.cmake` as-is — headers in `sdk/include`, libraries in the top-level directory — so `ORACLE_HOME=/path/to/instantclient_23_26` is all it needs. Two things that bit before and are now fixed in-tree: Oracle's own `occiControl.h` needs `<cstdint>` included ahead of `<occi.h>`, and OCCI must be linked before CLNTSH.

At runtime the Oracle client libs must be on the loader path:

```bash
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$ORACLE_HOME/lib:$ORACLE_HOME/ctx/lib:/usr/local/lib:/usr/lib
```

The root `CMakeLists.txt` only adds `lib/` and `lobshooter/`. **`tests/` and `lobwizard/` are not part of the build** — see below.

## Running

```
lobshooter [-v] user pass db DC|UC|DB|UB "SqlString" filename [-l logfile] [-c charset]
```

`DC`/`UC` = download/upload CLOB, `DB`/`UB` = download/upload BLOB. Argument parsing in `lobshooter/main.cpp` is strictly positional (`argv[1..6]`), options start at `argv[7]`. Defaults: charset `UTF8`, logfile `lobshooter.log`.

Two rules the API enforces implicitly and callers get wrong:
- The SQL string is always a **SELECT** returning the LOB column, never an INSERT.
- For uploads it must end in `for update` (row lock), and the LOB column must already be non-NULL — initialize it with `empty_clob()`/`empty_blob()` in SQL or via `Lob::InitLob()`.

Exit codes: `-1` usage problem, `-9` connection failure, otherwise the transfer method's own return value — `0` ok, `-1` file open problem, `-3` Oracle error (visible in the shell as 255/253).

## Architecture

Single linear inheritance chain, one class per file in `lib/`:

```
Log        (Log.hpp)          file logging: setLogFile/FlushLogFile/WriteLogFile, timestamps
 └ Conn    (Connection.hpp)   owns occi Environment* env and Connection* conn; connect() from
                              username/password/url; ConnectorType ("Oracle") gates every DB method
   └ BasicDML (BasicDML.hpp)  statements, positional binds (setBind), displayRows/getString/getInt,
                              InsertRow (any non-returning DML), Commit/Rollback
     └ Lob   (Lob.hpp)        filename + sqlLocator state, InitLob()
       ├ CharLob (CharLob.hpp)  Upload/DownloadClobData, setCharSet (applied via Clob::setCharSetId)
       └ BinLob  (BinLob.hpp)   Upload/DownloadBlobData
```

Consequences worth knowing before editing:
- Every LOB object *is* a connection, a statement runner, and a logger. Adding logging or SQL helpers means touching `Log`/`BasicDML`, and everything downstream inherits it.
- The classes are pure setter/state objects: construct with `(user, pass, db)`, then `connect()`, `setLogFile()`, `setFilename()`, `setSqlLocator()`, then call the transfer method. Nothing validates that the state was set.
- Transfers stream through OCCI `Stream*` with a 200-byte buffer (`readBuffer`/`writeBuffer` loops in `CharLob.cpp` / `BinLob.cpp`); this is the shape to preserve for large LOBs.
- Both LOB classes also have `(Connection*, Environment*)` constructors for reusing a caller-owned connection (used by the Qt wizard).
- Headers are not self-contained by convention: each pulls `<occi.h>` and does `using namespace oracle::occi; using namespace std;` at file scope before its include guard. Match that style rather than fixing it piecemeal.

## Tests

`tests/` is **not** wired into the build and does not compile as-is:
- `tests/CMakeLists.txt` hardcodes Oracle 10 sonames (`libnnz10.so`, `libocci.so.10.1`, `libclntsh.so.10.1`), `/usr/lib/libcppunit.so`, and links the *installed* `/usr/local/lib/libloadutil.so` rather than the build-tree target — so `make install` must run first.
- `tests/TestSuite.sh` hardcodes an Oracle XE 10.2 `ORACLE_HOME` and runs `./TestSuite.x`, which the CMake target (`loadutiltests`) does not produce.

These are CppUnit **integration** tests, not unit tests: they connect as `hr/hr@//lynx:1521/xe` and create/drop real tables (`TestSuiteCL`, `TestSuiteBL`). Running them needs a live Oracle instance. Most assertions in `runCharLobTest`/`runBinLobTest` are commented out, and only those two of the six fixtures are registered in `CPPUNIT_TEST_SUITE`. To run anything, build the directory manually and expect to fix the include paths and lib names first.

## Qt wizard (dormant)

`lobwizard/` builds a Qt4 GUI (`LobWizard`, generated from `LobWizard.ui`) that inherits `BasicDML` alongside `QWizard`. It is not in the root `CMakeLists.txt`, and its entry point in `lobshooter/main.cpp` (the `-gui` branch and the `LobWizard.h` include) is commented out. Treat it as unmaintained; don't assume it compiles against current Qt.

Similarly, the multi-file option (`-ml`, `filename,file2,...`) is documented in the Doxygen mainpage and there is an unused `int multifile` in `main.cpp`, but no implementation exists.

## Documentation

The Doxygen `\mainpage` — usage, options, install instructions, examples — lives in the header comment block of `lobshooter/main.cpp`. Update it there when CLI behavior changes, then regenerate:

```bash
doxygen Doxyfile     # INPUT=., RECURSIVE=YES, writes into html/
```

`html/` is checked in (184 files). Regenerating it produces a very large diff, so only do so when explicitly asked.
