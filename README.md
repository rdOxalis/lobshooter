# LobShooter for Oracle

Command line tool for moving LOBs — BLOBs and CLOBs — in and out of an Oracle
database. Point it at a row, give it a file, and it streams the data either way.
No PL/SQL, no `dbms_lob` boilerplate, no intermediate directory objects.

Ships as a small shared library (`libloadutil`) plus the `lobshooter` binary, so
the LOB handling can be reused from other C++ code.

## Requirements

* An Oracle client installation — full client, Oracle XE, or
  [Instant Client](https://www.oracle.com/database/technologies/instant-client.html)
  (Basic + SDK). The SDK part is required, it contains `occi.h`.
* CMake 3.5 or newer, and a C++ compiler.
* `ORACLE_HOME` pointing at that installation. The build looks **only** below
  `ORACLE_HOME` — nothing is picked up from system paths.

Verified in August 2026 with CMake 4.2.3, g++ 15.2 and Instant Client 23.26
against an Oracle 26ai server. The code itself dates back to 2004 and still
speaks to current servers unchanged.

## Build and install

```bash
export ORACLE_HOME=/path/to/instantclient_23_26   # or your full client / XE home
cmake .                                           # in-source build, the dot matters
make all
sudo make install                                 # /usr/local/bin/lobshooter
                                                  # /usr/local/lib/libloadutil.*
```

With Instant Client the layout matches out of the box: headers are found in
`$ORACLE_HOME/sdk/include`, libraries in `$ORACLE_HOME` itself.

At runtime the Oracle libraries have to be on the loader path:

```bash
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$ORACLE_HOME:/usr/local/lib
```

For a full client or XE install use `$ORACLE_HOME/lib:$ORACLE_HOME/ctx/lib`
instead. To remove everything again, run `./uninstall.sh`.

## Usage

```
lobshooter [-v] user pass db DC|UC|DB|UB "SqlString" filename [options]
```

| Mode | Direction |
|------|-----------|
| `UB` | **U**pload **B**LOB — file into the database |
| `DB` | **D**ownload **B**LOB — database into a file |
| `UC` | **U**pload **C**LOB |
| `DC` | **D**ownload **C**LOB |

| Argument | Meaning |
|----------|---------|
| `user` `pass` | database credentials |
| `db` | TNS alias, or an EZConnect string like `//host:1521/service` |
| `SqlString` | a **SELECT** that yields the LOB column (see below) |
| `filename` | source file for uploads, target file for downloads |

Options:

| Option | Effect |
|--------|--------|
| `-l logfile` | where to write the log (default `lobshooter.log`) |
| `-c charset` | character set for CLOBs (default `UTF8`), see the Oracle Globalization Support Guide, Appendix A |
| `-v` | print version |

### Two rules that trip everyone up

**The SQL is always a `SELECT`, never an `INSERT`.** It selects the LOB column
you want to read from or write into — the tool obtains a LOB locator from it.

**Uploads need `for update`.** The row has to be locked, and the LOB column must
already hold a locator rather than NULL. Initialise it with `empty_blob()` or
`empty_clob()` first:

```sql
insert into music values (4711, empty_blob());
commit;
```

Downloads need neither the lock nor the initialisation.

### Examples

Upload an MP3 into a BLOB column:

```bash
lobshooter hr hr xe UB \
  "select blob_field from blob_table where pk_field = 4711 for update" music.mp3
```

Download it again, this time over EZConnect and with an explicit log file:

```bash
lobshooter hr hr //lynx:1521/xe DB \
  "select blob_field from blob_table where pk_field = 4711" music.mp3 -l ./log.txt
```

A CLOB in a specific character set:

```bash
lobshooter hr hr //lynx:1521/xe UC \
  "select doc from documents where id = 1 for update" letter.txt -c WE8ISO8859P15
```

### Exit codes

| Code | Meaning |
|------|---------|
| `0` | success |
| `-1` (255) | wrong arguments, or the file could not be opened |
| `-3` (253) | Oracle error — details in the log |
| `-9` (247) | could not connect |

Errors are also appended to the log file with a timestamp.

## Using the library

`libloadutil` exposes the same functionality to C++. The classes form one
inheritance chain — `Log` → `Conn` → `BasicDML` → `Lob` → `CharLob`/`BinLob` —
so every LOB object also knows how to connect, run statements and log:

```cpp
BinLob bl("hr", "hr", "//localhost:1521/freepdb1");
bl.setLogFile("transfer.log");
if (bl.connect() == 0) {
    bl.setFilename("music.mp3");
    bl.setSqlLocator("select blob_field from blob_table where pk_field = 4711 for update");
    bl.UploadBlobData();
}
```

Both LOB classes also accept an existing `oracle::occi::Connection*` and
`Environment*`, for embedding into an application that already has a session.

## Layout

```
lib/          libloadutil — connection, DML, LOB streaming, logging
lobshooter/   the command line program
tests/        CppUnit integration tests (not part of the build, needs a live DB)
lobwizard/    Qt4 GUI, unmaintained and not built
cmake/        FindOracle.cmake
html/         generated Doxygen documentation
```

Regenerate the API documentation with `doxygen Doxyfile`. This file doubles as
the documentation's start page, so keep usage changes here rather than in the
sources.

## History

Written in 2004 under the name **oraload**. That name turned out to belong to
another tool already, so the project became **LobShooter** — but the binary kept
the old name for years. Since 2026 everything is called `lobshooter`.

The original project still lives at
[sourceforge.net/projects/oraload](http://sourceforge.net/projects/oraload/)
under its first name, which is also why a SourceForge reference remains in
`tests/TestSuite.cpp`.

## License

GNU Lesser General Public License, version 2.1 or later.
Copyright © 2004–2026 Ralf Duenkelmann. See `LICENSE`.
