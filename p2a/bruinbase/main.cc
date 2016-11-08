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
#include <cassert>
#include "BTreeNode.h"
#include "RecordFile.h"

using namespace std;

int main()
{
	BTLeafNode node;
	RecordId rid;

	int count = 84;
	for (int i = count; i > 0; i--)
	{
		rid.pid = i;
		rid.sid = i;
		assert(node.insert(i, rid) == 0);
	}
	assert(node.insert(85, rid) == RC_NODE_FULL);

	PageFile pf = PageFile("movie.tbl", 'w');
	node.write(1, pf);
	node.read(1, pf);

	assert(node.getKeyCount() == count);

	for (int i = 1; i <= 84; i++)
	{
		int index;
		assert(node.locate(i, index) == 0 && index == i - 1);
	}

	// run the SQL engine taking user commands from standard input (console).
	// SqlEngine::run(stdin);

	return 0;
}
