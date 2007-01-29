#!/bin/bash
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/lib/oracle/xe/app/oracle/product/10.2.0/server/lib:/usr/lib/oracle/xe/app/oracle/product/10.2.0/server/ctx/lib
#echo $LD_LIBRARY_PATH
export ORACLE_HOME=/usr/lib/oracle/xe/app/oracle/product/10.2.0/server/
#./load.x "$1" "$2" "$3" "$4" "$5" "$6" "$7" "$8"
./TestSuite.x $1 $2 $3 $4 "$5" $6 $7 $8