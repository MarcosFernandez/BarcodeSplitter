/*
 * support.h
 *
 *  Created on: 8 Set, 2015
 *      Author: marcos
 */

#ifndef SUPPORT_H_
#define SUPPORT_H_

#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <map>

using namespace std;

struct userInput
{
	string barcodeFile;
    bool barcodes_at_eol;
    bool barcodes_at_bol;
	bool exact_match;
    string newfile_prefix;
    string newfile_suffix;
	bool quiet;
	unsigned int allow_partial_overlap;
	bool debug;
	unsigned int allowed_mismatches;
    bool help;
};

struct barCode
{
	string id;
	string barcode;
};

struct record
{
	string seq_name;
	string seq_bases;
	string seq_name2;
	string seq_qualities;
};

void usage();



#endif /* SUPPORT_H_ */
