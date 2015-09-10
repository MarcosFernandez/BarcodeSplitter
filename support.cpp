/*
 * support.cpp
 *
 *  Created on: 8 Set, 2015
 *      Author: marcos
 */
#include "support.h"

#include <stdio.h>
#include <iostream>



/**
 * \brief Application Usage
 */
void usage()
{
	cout << "Barcode Splitter" << endl;
	cout << "This program reads FASTA/FASTQ file and splits it into several smaller files," << endl;
	cout << "Based on barcode matching." << endl;
	cout << "FASTA/FASTQ data is read from STDIN (format is auto-detected.)" << endl;
	cout << "Output files will be written to disk." << endl;
	cout << "Summary will be printed to STDOUT." << endl;
	cout << "usage: BarcodeSplitter --bcfile FILE --prefix PREFIX [--suffix SUFFIX] [--bol|--eol]" << endl;
	cout << "         [--mismatches N] [--exact] [--help] [--quiet] [--debug]" << endl;
	cout << "Arguments:" << endl;
	cout << "--bcfile FILE	- Barcodes file name. (see explanation below.)" << endl;
	cout << "--prefix PREFIX	- File prefix. will be added to the output files. Can be used" << endl;
	cout << "		  to specify output directories." << endl;
	cout << "--suffix SUFFIX	- File suffix (optional). Can be used to specify file" << endl;
	cout << "		  extensions." << endl;
	cout << "--bol		- Try to match barcodes at the BEGINNING of sequences." << endl;
	cout << "		  (What biologists would call the 5' end, and programmers" << endl;
	cout << "		  would call index 0.)" << endl;
	cout << "--eol		- Try to match barcodes at the END of sequences." << endl;
	cout << "		  (What biologists would call the 3' end, and programmers" << endl;
	cout << "		  would call the end of the string.)" << endl;
	cout << "		  NOTE: one of --bol, --eol must be specified, but not both." << endl;
	cout << "--mismatches N	- Max. number of mismatches allowed. default is 1." << endl;
	cout << "--exact		- Same as '--mismatches 0'. If both --exact and --mismatches" << endl;
	cout << "		  are specified, '--exact' takes precedence." << endl;
	cout << "--quiet		- Don't print counts and summary at the end of the run." << endl;
	cout << "		  (Default is to print.)" << endl;
	cout << "--debug		- Print lots of useless debug information to STDERR." << endl;
	cout << "--help		- This helpful help screen." << endl;
	cout << "Example (Assuming 's_2_100.txt' is a FASTQ file, 'mybarcodes.txt' is" << endl;
	cout << "the barcodes file):" << endl;
	cout << "   \$ cat s_2_100.txt | $0 --bcfile mybarcodes.txt --bol --mismatches 2 \\" << endl;
	cout << "   	--prefix /tmp/bla_ --suffix \".txt\"" << endl;
	cout << "Barcode file format" << endl;
	cout << "-------------------" << endl;
	cout << "Barcode files are simple text files. Each line should contain an identifier" << endl;
	cout << "(descriptive name for the barcode), and the barcode itself (A/C/G/T)," << endl;
	cout << "separated by a TAB character. Example:" << endl;
	cout << "    #This line is a comment (starts with a 'number' sign)" << endl;
	cout << "    BC1 GATCT" << endl;
	cout << "    BC2 ATCGT" << endl;
	cout << "    BC3 GTGAT" << endl;
	cout << "    BC4 TGTCT" << endl;
	cout << "For each barcode, a new FASTQ file will be created (with the barcode's" << endl;
	cout << "identifier as part of the file name). Sequences matching the barcode" << endl;
	cout << "will be stored in the appropriate file." << endl;
	cout << "Running the above example (assuming \"mybarcodes.txt\" contains the above" << endl;
	cout << "barcodes), will create the following files:" << endl;
	cout << "	/tmp/bla_BC1.txt" << endl;
	cout << "	/tmp/bla_BC2.txt" << endl;
	cout << "	/tmp/bla_BC3.txt" << endl;
	cout << "	/tmp/bla_BC4.txt" << endl;
	cout << "	/tmp/bla_unmatched.txt" << endl;
	cout << "The 'unmatched' file will contain all sequences that didn't match any barcode." << endl;
	cout << "Barcode matching" << endl;
	cout << "----------------" << endl;
	cout << "Count mismatches between the FASTA/Q sequences and the barcodes." << endl;
	cout << "The barcode which matched with the lowest mismatches count (providing the" << endl;
	cout << "count is small or equal to '--mismatches N') 'gets' the sequences." << endl;
	cout << "Example (using the above barcodes):" << endl;
	cout << "Input Sequence:" << endl;
	cout << "    GATTTACTATGTAAAGATAGAAGGAATAAGGTGAAG" << endl;
	cout << "Matching with '--bol --mismatches 1':" << endl;
	cout << "   GATTTACTATGTAAAGATAGAAGGAATAAGGTGAAG" << endl;
	cout << "   GATCT (1 mismatch, BC1)" << endl;
	cout << "   ATCGT (4 mismatches, BC2)" << endl;
	cout << "   GTGAT (3 mismatches, BC3)" << endl;
	cout << "   TGTCT (3 mismatches, BC4)" << endl;
	cout << "This sequence will be classified as 'BC1' (it has the lowest mismatch count)." << endl;
	cout << "If '--exact' or '--mismatches 0' were specified, this sequence would be" << endl;
	cout << "classified as 'unmatched' (because, although BC1 had the lowest mismatch count," << endl;
	cout << "it is above the maximum allowed mismatches)." << endl;
	cout << "Matching with '--eol' (end of line) does the same, but from the other side" << endl;
	cout << "of the sequence." << endl;
	cout << "Note: scoring counts a missing base as a mismatch, so the final" << endl;
	cout << "mismatch count is 2 (1 'real' mismatch, 1 'missing base' mismatch)." << endl;
	cout << "If running with '--mismatches 2' (meaning allowing upto 2 mismatches) - this" << endl;
	cout << "sequence will be classified as BC1." << endl;

}

