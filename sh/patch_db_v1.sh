#!/bin/sh

echo "change directory '..'"
cd .. 
echo "find and replace in the file 'DB.sql'"
sed -i 's/启动,结束/启动/g' DB.sql
