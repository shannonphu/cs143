/**
 * Copyright (C) 2008 by The Regents of the University of California
 * Redistribution of this file is permitted under the terms of the GNU
 * Public License (GPL).
 *
 * @author Junghoo "John" Cho <cho AT cs.ucla.edu>
 * @date 3/24/2008
 */
 
#include "Bruinbase.h"
#include "SqlEngine.h"
#include <cstdio>
#include <iostream>
// for debug running
#include "BTreeNode.h"
#include "RecordFile.h"

using namespace std;

int main()
{
	BTLeafNode node;
	RecordId rid;

	for (int i = 84; i >= 0; i--)
	{
		rid.pid = i;
		rid.sid = i;
		node.insert(i, rid);
	}

	PageFile pf = PageFile("movie.tbl", 'w');
	node.write(1, pf);
	node.read(1, pf);

	// run the SQL engine taking user commands from standard input (console).
	// SqlEngine::run(stdin);

	return 0;
}
