#!/bin/bash

dirName="322_framework" ;
origDir=`pwd` ;

# Compile all compilers
make -j ;

# Copy all files to an empty directory
mkdir ../$dirName ;
cd ../$dirName ;
cp -r "$origDir"/* ./ ;

# Remove compiler sources
rm */src/* ;

# Remove object files
rm -r */obj ;

# Create the package
cd ../ ;
tar cfj ${dirName}.tar.bz2 ${dirName} ;
mv ${dirName}.tar.bz2 src ;
rm -r ${dirName} ;
