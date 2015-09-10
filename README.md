===============
BarcodeSplitter
===============

C++ Interpretation of [FASTX Barcode Splitter] (http://hannonlab.cshl.edu/fastx_toolkit/commandline.html#fastx_barcode_splitter_usage) originally written in perl by Assaf Gordon (gordon@cshl.edu).

Adaptation of fastx_barcode_splitter in C++ to speed up the process of BARCODE splitting.
  
This program reads FASTA/FASTQ file and splits it into several smaller files, Based on barcode matching.
FASTA/FASTQ data is read from STDIN (format is auto-detected.)
	
Output files will be writen to disk. Summary will be printed to STDOUT.
 
Licensing
=========

BarcodeSplitter is licensed under GPL.

Installation
============

Compile the application just running:
	make all

To do:
======

Partial option from fastx_barcode_splitter is not implemented.


Changelog:
==========

    0.0 Initial Release  

