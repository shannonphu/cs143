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

// for debug running
#include "BTreeNode.h"
#include "RecordFile.h"

int main()
{
	BTLeafNode node;
	RecordId rid;
	rid.pid = 1;
	rid.sid = 3;
	node.insert(5, rid);
	PageFile pf = PageFile("movie.tbl", 'w');
	node.write(1, pf);
	node.read(1, pf);

	// run the SQL engine taking user commands from standard input (console).
	SqlEngine::run(stdin);

	return 0;
}
