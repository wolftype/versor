#!/bin/bash

echo "Removing all files in build/install_manifest.txt"
xargs rm < build/install_manifest.txt
echo "Removing /usr/local/include/vsr itself"
rm -rf /usr/local/include/vsr
