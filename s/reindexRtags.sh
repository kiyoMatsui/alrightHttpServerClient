#!/bin/bash
rm -r rtagsDir
rdm &
mkdir rtagsDir && cd rtagsDir
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ..
rc -J
