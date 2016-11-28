/**
 * Copyright (C) 2008 by The Regents of the University of California
 * Redistribution of this file is permitted under the terms of the GNU
 * Public License (GPL).
 *
 * @author Junghoo "John" Cho <cho AT cs.ucla.edu>
 * @date 3/24/2008
 */

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <string.h>
#include <iostream>
#include <fstream>
#include "Bruinbase.h"
#include "SqlEngine.h"
#include "RecordFile.h"
#include "BTreeIndex.h"

using namespace std;

// external functions and variables for load file and sql command parsing 
extern FILE* sqlin;
int sqlparse(void);


RC SqlEngine::run(FILE* commandline)
{
	fprintf(stdout, "Bruinbase> ");

	// set the command line input and start parsing user input
	sqlin = commandline;
	sqlparse();  // sqlparse() is defined in SqlParser.tab.c generated from
							 // SqlParser.y by bison (bison is GNU equivalent of yacc)

	return 0;
}

RC SqlEngine::select(int attr, const string& table, const vector<SelCond>& cond)
{
	RecordFile rf;   // RecordFile containing the table
	RecordId   rid;  // record cursor for table scanning
	BTreeIndex btree;
	IndexCursor ic;

	RC     rc;
	int    key;     
	string value;
	int    count = 0;
	int    diff;

	// open the table file
	if ((rc = rf.open(table + ".tbl", 'r')) < 0) {
		fprintf(stderr, "Error: table %s does not exist\n", table.c_str());
		return rc;
	}

	// scan the table file from the beginning
	rid.pid = rid.sid = 0;
	count = 0;
	int min = -1;
	int max = -1;
	int eq_val = -1;
	bool equal_cond = false;

	rc = btree.open(table + ".idx", 'r');

	// Use index file if exists
	if (rc == 0)
	{
		for (unsigned i = 0; i < cond.size(); i++) 
		{
			SelCond cur_cond = cond[i];
			if (cur_cond.attr == 1 && !equal_cond) 
			{
				switch (cur_cond.comp) 
				{
					case SelCond::EQ:
						max = atoi(cur_cond.value);
						min = max;
						equal_cond = true;
						break;
					case SelCond::LT:
						if (max == -1) 
						{
							max = atoi(cur_cond.value) - 1;
						}
						else 
						{
							int temp = atoi(cur_cond.value) -1;
							if (temp < max)
								max = temp;
						}
						break;
					case SelCond::GT:
						if (min == -1)
						{
							min = atoi(cur_cond.value) + 1;
						}
						else 
						{
							int temp = atoi(cur_cond.value) + 1;
							if (max < temp)
								max = temp;
						}
						break;
					case SelCond::LE:
						if (max == -1) 
						{
							max = atoi(cur_cond.value);
						}
						else 
						{
							int temp = atoi(cur_cond.value);
							if (temp < max)
								max = temp;
						}
						break;
					case SelCond::GE:
						if (min == -1)
						{
							min = atoi(cur_cond.value);
						}
						else 
						{
							int temp = atoi(cur_cond.value);
							if (max < temp)
								max = temp;
						}
						break;
				}
			}
		}

		cout << "min " << min << endl;
		btree.locate(min, ic);
	    int count2 = 0;

		while (btree.readForward(ic, key, rid) == 0)
		{
			cout << "ic.pid: " << ic.pid << " ic.eid: " << ic.eid << endl;
			count2++;
			// SELECT (*) case
			if (attr == 4) {
				if (max != -1 && key > max)
					goto btree_early_end;
				if (min != -1 && key < min)
					goto btree_early_end;
				if (equal_cond && key != min)
					goto btree_early_end;
				count++;
				// cout << "check_next_tuple goto hit" << endl;
				goto check_next_tuple; //can this be replaced by a continue?
			}

			rc = rf.read(rid, key, value);
			if (rc < 0) {
				fprintf(stderr, "Error: when reading a tuple from table %s\n", table.c_str());
				goto exit_select;
			}

			for (unsigned i = 0; i < cond.size(); i++) {
				SelCond cur_cond = cond[i];
				if (cur_cond.attr == 1) 
					diff = key - atoi(cur_cond.value);
				if (cur_cond.attr == 2)
					diff = strcmp(value.c_str(), cur_cond.value); 

				switch(cur_cond.comp) {
					case SelCond::GE:
						if (diff < 0)
							goto check_next_tuple;
						break;
					case SelCond::LE:
						if (diff > 0)
							if (cur_cond.attr == 1)
									goto exit_select;
							goto check_next_tuple;
						break;
					case SelCond::LT:
						if (diff >=0)
							if (cur_cond.attr == 1)
								goto exit_select;
							goto next_tuple;
						break;
					case SelCond::GT:
						if (diff <= 0)
							goto check_next_tuple;
						break;
					case SelCond::NE:
						if (diff == 0) 
							goto check_next_tuple;
						break;
					case SelCond::EQ:
						if (diff != 0) {
							if (cur_cond.attr == 1)
								goto exit_select;
							goto next_tuple;
						} 
						break;
				}
			}

			switch (attr) {
			case 1:  // SELECT key
				fprintf(stdout, "%d\n", key);
				break;
			case 2:  // SELECT value
				fprintf(stdout, "%s\n", value.c_str());
				break;
			case 3:  // SELECT *
				fprintf(stdout, "%d '%s'\n", key, value.c_str());
				break;
			};
			check_next_tuple:
			;
		}
    cout << "count2 is:" << count2 << endl;
	}
	else {
		while (rid < rf.endRid()){
					// read the tuple
					if ((rc = rf.read(rid, key, value)) < 0) {
						fprintf(stderr, "Error: while reading a tuple from table %s\n", table.c_str());
						goto exit_select;
					}

					// check the conditions on the tuple
					for (unsigned i = 0; i < cond.size(); i++) {
						// compute the difference between the tuple value and the condition value
						switch (cond[i].attr) {
						case 1:
							diff = key - atoi(cond[i].value);
							break;
						case 2:
							diff = strcmp(value.c_str(), cond[i].value);
							break;
						}

						// skip the tuple if any condition is not met
						switch (cond[i].comp) {
						case SelCond::EQ:
							if (diff != 0) goto next_tuple;
								break;
						case SelCond::NE:
							if (diff == 0) goto next_tuple;
								break;
						case SelCond::GT:
							if (diff <= 0) goto next_tuple;
								break;
						case SelCond::LT:
							if (diff >= 0) goto next_tuple;
								break;
						case SelCond::GE:
							if (diff < 0) goto next_tuple;
								break;
						case SelCond::LE:
							if (diff > 0) goto next_tuple;
								break;
						}
					}

					// the condition is met for the tuple. 
					// increase matching tuple counter
					count++;

					// print the tuple 
					switch (attr) {
					case 1:  // SELECT key
						fprintf(stdout, "%d\n", key);
						break;
					case 2:  // SELECT value
						fprintf(stdout, "%s\n", value.c_str());
						break;
					case 3:  // SELECT *
						fprintf(stdout, "%d '%s'\n", key, value.c_str());
						break;
					}

					// move to the next tuple
					next_tuple:
					++rid;
		}
	}

	btree_early_end:
	// print matching tuple count if "select count(*)"
	if (attr == 4) {
		fprintf(stdout, "%d\n", count);
	}
	rc = 0;

	// close the table file and return
	exit_select:
	rf.close();
	return rc;
}

RC SqlEngine::load(const string& table, const string& loadfile, bool index)
{

	RecordFile rf;
	RecordId rid;
	RC err;
	BTreeIndex btree;

	ifstream infile(loadfile.c_str());

	err = rf.open(table + ".tbl", 'w');
	if (err != 0)
		return err;

	if (index) 
	{
		err = btree.open(table + ".idx", 'w');
		if (err != 0)
			return err;
	}

	int key;
	string value;
	for(string line; getline( infile, line ); )
	{
			// int key;
			// string value;
			parseLoadLine(line, key, value);
			// cout << "key: " << key << " | " << "value: " << value << endl;

			err = rf.append(key, value, rid);

			if (err != 0)
				return err;

			if (index) 
			{
				err = btree.insert(key, rid);
				if (err != 0)
					return err;
			}

			// Prints out stored results
			// int i;
			// string s;
			// rf.read(id, i, s);
			// cout << "key: " << i << " | " << "value: " << s << endl;
	}

	if (index) {
		btree.traverse();
		err = btree.close();
		if (err != 0) 
			return err;

	}
	else
	{
		rf.close();
	}

	infile.close();

	return 0;
}

RC SqlEngine::parseLoadLine(const string& line, int& key, string& value)
{
		const char *s;
		char        c;
		string::size_type loc;
		
		// ignore beginning white spaces
		c = *(s = line.c_str());
		while (c == ' ' || c == '\t') { c = *++s; }

		// get the integer key value
		key = atoi(s);

		// look for comma
		s = strchr(s, ',');
		if (s == NULL) { return RC_INVALID_FILE_FORMAT; }

		// ignore white spaces
		do { c = *++s; } while (c == ' ' || c == '\t');
		
		// if there is nothing left, set the value to empty string
		if (c == 0) { 
				value.erase();
				return 0;
		}

		// is the value field delimited by ' or "?
		if (c == '\'' || c == '"') {
				s++;
		} else {
				c = '\n';
		}

		// get the value string
		value.assign(s);
		loc = value.find(c, 0);
		if (loc != string::npos) { value.erase(loc); }

		return 0;
}
