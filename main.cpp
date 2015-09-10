/*
 * main.cpp
 *
 *  Created on: 8 Set, 2015
 *      Author: marcos
 */

#include <stdio.h>
#include <iostream>
#include "support.h"

using namespace std;


/**
 * \brief parse command line
 * \param argc total number of arguments
 * \param argv vector of arguments
 * \param user set of configuration parameters
 * \returns false if the program must be closed
 */
bool parse_command_line (int argc, char *argv[],userInput * user)
{
	user->barcodeFile = "";
	user->barcodes_at_eol = false;
	user->barcodes_at_bol = false;
	user->exact_match = false;
	user->allow_partial_overlap = 0;
	user->allowed_mismatches = 1;
	user->newfile_suffix = "";
	user->newfile_prefix = "";
	user->quiet = false;
	user->debug = false;

	//Arguments checking
	if (argc == 1)
	{
		usage();
		return false;
	}

	//Arguments processing
	for (int i = 1; i < argc; i++)
    {
        if (string(argv[i]).compare("--bcfile") == 0)
	    {
        	user->barcodeFile = string(argv[i + 1]);
	    }
        else if (string(argv[i]).compare("--eol") == 0)
        {
        	user->barcodes_at_eol = true;
	    }
        else if (string(argv[i]).compare("--bol") == 0)
        {
        	user->barcodes_at_bol = true;
        }
        else if (string(argv[i]).compare("--exact") == 0)
        {
        	user->exact_match = true;
        }
        else if (string(argv[i]).compare("--prefix")== 0)
        {
        	user->newfile_prefix = string(argv[i + 1]);
	    }
        else if (string(argv[i]).compare("--suffix")== 0)
        {
           	user->newfile_suffix = string(argv[i + 1]);
   	    }
        else if (string(argv[i]).compare("--quiet") == 0)
        {
        	user->quiet = true;
        }
        else if (string(argv[i]).compare("--partial") == 0)
        {
           	user->allow_partial_overlap = atoi(argv[i + 1]);
        }
        else if (string(argv[i]).compare("--debug") == 0)
        {
        	user->debug = true;
        }
        else if (string(argv[i]).compare("--mismatches") == 0)
        {
           	user->allowed_mismatches = atoi(argv[i + 1]);
        }
        else if (string(argv[i]).compare("--help") == 0)
		{
        	user->help = true;
		}
    }

	//Does the user call for help
	if (user->help)
	{
		usage();
		return false;
	}

	//Check parameters
	if(user->barcodeFile.empty())
	{
		cout << "Error: barcode file not specified (use '--bcfile [FILENAME]')" << endl;
		return false;
	}

	if(user->newfile_prefix.empty())
	{
		cout << "Error: prefix path/filename not specified (use '--prefix [PATH]')" << endl;
		return false;
	}

	if (user->barcodes_at_bol == user->barcodes_at_eol)
	{
		if (user->barcodes_at_eol)
		{
			cout << "Error: can't specify both --eol & --bol" << endl;
			return false;
		}
		else
		{
			cout << "Error: must specify either --eol or --bol" << endl;
			return false;
		}
	}

	if (user->allow_partial_overlap < 0)
	{
		cout << "Error: invalid for value partial matches (valid values are 0 or greater)" << endl;
		return false;
	}

	if(user->exact_match)
	{
		user->allowed_mismatches = 0;
		return false;
	}

	if(user->allowed_mismatches < 0)
	{
		cout << "Error: invalid value for mismatches (valid values are 0 or more)" << endl;
		return false;
	}

	if(user->allow_partial_overlap > user->allowed_mismatches)
	{
		cout << "Error: partial overlap value ($allow_partial_overlap) bigger than max. allowed mismatches ($allowed_mismatches)" << endl;
		return false;
	}

    return true;
}


/**
 * \brief Read BARCODE file
 * \param barcodeFile barcode file name
 * \param user structure of user arguments
 * \param barcodes_length barcodes length
 * \returns false if something wrong
 */
bool load_barcode_file (const userInput * user, vector <barCode> & barCodeList,int & barcodes_length)
{
	FILE * barcode;
	char line[4 * 256];
	char * token = "";

	barcode = fopen(user->barcodeFile.c_str(), "r");

	if (barcode == NULL)
	{
		cout <<  "Sorry!! Unable to open barcode file " << user->barcodeFile.c_str() << endl;
		return false;
	}

	while (1)
	{
		token = fgets(line, (4 * 256), barcode);
		if (feof(barcode)) break;

		//Chomp lines
		if (line[strlen(line) - 1] == '\n') line[strlen(line) - 1] = '\0';

		if (line [0] == '#')
		{
			continue;
		}

        //Split line
		char *id;
		char *barcode;

		id = strtok(line, "\t");
        barcode = strtok(NULL, "\t");

        if (sizeof(barcode) <= user->allowed_mismatches)
        {
        	cout << "Error: barcode($ident, $barcode) is shorter or equal to maximum number of mismatches ($allowed_mismatches). This makes no sense. Specify fewer  mismatches." << endl;
        	return false;
        }

        barCode newBarcode;
        newBarcode.id = id;
        newBarcode.barcode = barcode;
        barcodes_length = newBarcode.barcode.length();
        barCodeList.push_back(newBarcode);
	}

	fclose(barcode);

	if (user->debug)
	{
	    cerr << "barcode" << "\t" << "sequence" << endl;

	    for (std::vector <barCode>::iterator it=barCodeList.begin(); it!=barCodeList.end(); ++it)
	    {
	    	cerr << it->barcode << "\t" << it->id << endl;
	    }
	}

	return true;
}

/**
 * \brief Check input format
 * \param fastq_format determination
 * \returns false if something wrong
 */
bool open_and_detect_input_format(bool & fastq_format,const userInput * user)
{
	//Get the first character, and push it back
	int first_char = getc(stdin);
	ungetc(first_char,stdin);

	if (first_char == '>')
	{
		// FASTA format
		fastq_format = false ;
		if(user->debug) cout << "Detected FASTA format" << endl;
	}
	else if (first_char == '@')
	{
		// FASTQ format
		fastq_format = true;
		if(user->debug) cout << "Detected FASTQ format" << endl;
	}
	else
	{
		cout << "Error: unknown file format. First character = '$first_char' (expecting > or \@)"<< endl;
		return false;
	}

	return true;
}



/**
 * \brief Create one output file for each barcode. Also create a file for the dummy 'unmatched' barcode
 * \param barcodes map of id barcodes
 * \param user user input parameters
 * \param filenames file name per barcode
 * \param files map of file descriptors per barcode
 */
void create_output_files(const map<string,unsigned int> & barcodes,const userInput * user,map<string,string> & filenames,map<string,FILE*> & files)
{
	for (map<string,unsigned int>::const_iterator it=barcodes.begin(); it!=barcodes.end(); ++it)
	{
		string new_filename = user->newfile_prefix + it->first + user->newfile_suffix;
		filenames[it->first] = new_filename;
		FILE * fout;
		fout = fopen(new_filename.c_str(), "w");
		files[it->first] = fout;
	}
}

/**
 * \brief Read sequence record
 * \param seq sequence record
 * \param fastq_format true if input file is fastq
 * \returns false if nothing was read
 */
bool read_record(record & seq,bool fastq_format)
{
	if (getline (cin,seq.seq_name,'\n').eof())
	{
		return false;
	}

	if (getline (cin,seq.seq_bases,'\n').eof())
	{
		cout << "Error: bad input file, expecting line with sequences" << endl;
		return false;
	}

	if(fastq_format)
	{
		if (getline (cin,seq.seq_name2,'\n').eof())
		{
			cout << "Error: bad input file, expecting line with sequence name2" << endl;
			return false;
		}

		if (getline (cin,seq.seq_qualities,'\n').eof())
		{
			cout << "Error: bad input file, expecting line with quality scores" << endl;
			return false;
		}
	}

	return true;
}

/**
 * \brief Hamming distance calculation between two strings
 * \param fs first string to check
 * \param ss second string to check
 * \return number of mismatches or edit operations to be equal
 */
int hamming_distance(string& fs, string& ss)
{
    int hm_distance = 0;

    if((fs.length() == ss.length()))
    {
      for(unsigned int i = 0; i < fs.length(); i++)
      {
        if(!(fs[i] == ss[i]))
        {
          hm_distance++;
        }
      }
    }
    else
    {
      hm_distance = -1;
    }
    return hm_distance;
}

/**
 * \brief Write sequencing record to a given file
 * \param file file descriptor
 * \param record sequencing fields
 * \param fastq_format is record a FASTQ file
 */
void write_record(FILE * file, const record & sequence,bool fastq_format)
{
	fwrite (sequence.seq_name.c_str() , sizeof(char), sequence.seq_name.length(), file);
	fwrite ("\n" , sizeof(char), 1, file);
	fwrite (sequence.seq_bases.c_str() , sizeof(char), sequence.seq_bases.length(), file);
	fwrite ("\n" , sizeof(char), 1, file);

	//if using FASTQ format, write two more lines
	if (fastq_format)
	{
		fwrite (sequence.seq_name2.c_str(), sizeof(char), sequence.seq_name2.length(), file);
		fwrite ("\n" , sizeof(char), 1, file);
		fwrite (sequence.seq_qualities.c_str() , sizeof(char), sequence.seq_qualities.length(), file);
		fwrite ("\n" , sizeof(char), 1, file);
	}
}

/**
 * \brief match sequences barcodes fastq files
 * \param barCodeList vector of barcodes
 * \param barcodes map of barcodes
 * \param counts count for each type of barcode
 * \param user user inserted arguments
 * \param filenames files to store each barcoded sequences
 * \param files  map of file descriptors
 * \param fastq_format The input is fastq or fasta
 * \param barcodes_length length of the barcodes
 */
void match_sequences(vector <barCode> & barCodeList, map<string,unsigned int> & barcodes, map<string,unsigned int> & counts,
		             const userInput * user,map<string,string> & filenames,map<string,FILE*> & files,bool fastq_format,int barcodes_length)
{

    //Generate a uniq list of barcode identifiers
	for (std::vector <barCode>::iterator it=barCodeList.begin(); it!=barCodeList.end(); ++it)
	{
		barcodes[it->id] = 1;
	}
	barcodes["unmatched"] = 1 ;

    //Reset counters
	for (map<string,unsigned int>::iterator it=barcodes.begin(); it!=barcodes.end(); ++it)
	{
		counts [it->first] = 0;
	}

	create_output_files(barcodes,user,filenames,files);

	//Read file FASTQ file split according to barcodes
	record sequence;
	while (read_record(sequence,fastq_format))
	{
		if (user->debug) cerr << "sequence " << sequence.seq_bases;

		unsigned int best_barcode_mismatches_count = barcodes_length;
		string best_barcode_ident = "";

        //Try all barcodes, find the one with the lowest mismatch count
		for (std::vector <barCode>::iterator it=barCodeList.begin(); it!=barCodeList.end(); ++it)
		{
			string id = it->id;
			string barcode = it->barcode;

            // Get DNA fragment (in the length of the barcodes)
			// The barcode will be tested only against this fragment
			// (no point in testing the barcode against the whole sequence)
			string sequence_fragment = "";
			if(user->barcodes_at_bol)
			{
				sequence_fragment = sequence.seq_bases.substr(0,barcodes_length);
			}
			else
			{
				sequence_fragment = sequence.seq_bases.substr(sequence.seq_bases.length()-barcodes_length-1,sequence.seq_bases.length()-1);
			}

			unsigned int mm = hamming_distance(sequence_fragment,barcode);

            // if this is a partial match, add the non-overlap as a mismatch (partial barcodes are shorter than the length of the original barcodes)
			mm += (barcodes_length - barcode.length());

			if ( mm < best_barcode_mismatches_count )
			{
				best_barcode_mismatches_count = mm;
				best_barcode_ident = id;
			}
		}

		if ( (best_barcode_ident.compare("") == 0) || best_barcode_mismatches_count > user->allowed_mismatches)
		{
			best_barcode_ident = "unmatched";
		}

		if (user->debug) cerr << "sequence " << sequence.seq_bases << " matched barcode: " << best_barcode_ident << endl;

		counts[best_barcode_ident] ++;

        //get the file associated with the matched barcode. (note: there's also a file associated with 'unmatched' barcode)
		FILE * file = files[best_barcode_ident];

		write_record(file, sequence,fastq_format);
	}
}


/**
 * \brief Print results and close file descriptors
 * \param counts map of counts per each barcode file
 * \param filename map of filenames and barcodes
 * \param map of file descriptors
 */
void print_results(const map<string,unsigned int> & counts, const map<string,string> & filenames,map<string,FILE*> & files)
{
	cout << "Barcode" << "\t" << "Count" << "\t" << "Location" << endl;
	unsigned int total = 0;

	for (std::map<string, unsigned int>::const_iterator i = counts.begin(); i != counts.end(); i++)
	{
	    cout << i->first << "\t" << i->second << "\t" << filenames.at(i->first) << endl;
	    fclose(files.at(i->first));
	    total = total + counts.at(i->first);
	}

	cout << "total" << "\t" << total << endl;
}




/**
 * \brief main function
 */
int main(int argc, char *argv[])
{
	userInput * user = new userInput;
	vector <barCode> barCodeList;
	map<string,unsigned int> barcodes;
	map<string,unsigned int> counts;
	map<string,string> filenames;
	map<string,FILE*> files;
	bool fastq_format = true;
	int barcodes_length = 0;

	// Start of Program
	if (parse_command_line (argc,argv,user))
	{
		if(load_barcode_file(user, barCodeList,barcodes_length))
		{
			if(open_and_detect_input_format(fastq_format,user))
			{
				match_sequences(barCodeList,barcodes,counts,user,filenames,files,fastq_format,barcodes_length);
				print_results(counts,filenames,files);
			}
		}
	}
	// End of program
}
