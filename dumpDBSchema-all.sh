#!/bin/bash

cd App
printf '================ dumping App\n'
./dumpDBSchema.sh
cd ..

cd Modules
find -path './[^.]*' -prune -type d -print0 | xargs -0 -I '{}' sh -c \
"printf '================ dumping %s\n' '{}'; cd {}/moduleSrc; ./dumpDBSchema.sh; cd ../.."
