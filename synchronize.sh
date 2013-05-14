#!/bin/bash

basepath="//dfs/"
baseserv=$1
unison="/usr/bin/unison-2.40"

cat mem_servers.txt | while read serv;do
    src="ssh://$baseserv$basepath"
    dest="ssh://$serv$basepath"
    echo "echo synchronizing $src with $dest"
    unison "$src" "$dest" -batch -auto -ui text
done;
