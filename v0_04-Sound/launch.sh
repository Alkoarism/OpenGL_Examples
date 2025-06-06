#!/bin/bash
set -e #Exit on any error
make #Everything after this only runs if 'make' succeeded

PROJECTS_FOLDER="/mnt/c/Users/guilh/Documents/Guilherme/Projetos"
VERSION=v0_4
DEST_FOLDER="$PROJECTS_FOLDER/LittleBigEngine/$VERSION"

# Create destination folder if it doesn't exist
mkdir -p "$DEST_FOLDER/res"

echo "Deploying to destionation Folder: "$DEST_FOLDER
cp -r res/* "$DEST_FOLDER/res"
cp bin/*.exe "$DEST_FOLDER"
cp *.dll "$DEST_FOLDER"

echo
echo "Launching Application..."
"$DEST_FOLDER"/*.exe