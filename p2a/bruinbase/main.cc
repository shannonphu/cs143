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
#include <cassert>
#include "BTreeNode.h"
#include "RecordFile.h"
#include "BTreeIndex.h"

using namespace std;

int main()
{
	// Test BTLeafNodes

	// BTLeafNode node_l;
	// RecordId rid;

	// int count = 84;
	// for (int i = count; i > 0; i--)
	// {
	// 	rid.pid = i;
	// 	rid.sid = i;
	// 	assert(node_l.insert(i, rid) == 0);
	// }
	// assert(node_l.insert(85, rid) == RC_NODE_FULL);

	// PageFile pf = PageFile("movie.tbl", 'w');
	// node_l.write(1, pf);
	// node_l.read(1, pf);

	// assert(node_l.getKeyCount() == count);

	// for (int i = 1; i <= 84; i++)
	// {
	// 	int index;
	// 	assert(node_l.locate(i, index) == 0 && index == i - 1);
	// }

	// BTLeafNode node_l2;
	// int midkey;
	// rid.pid = 222;
	// rid.sid = 444;
	// node_l.insertAndSplit(1, rid, node_l2, midkey);
	// assert(node_l.getKeyCount() == 43);
	// assert(node_l2.getKeyCount() == 42);
	// cout << node_l.getNextNodePtr() << endl;

	// // cout << "First leaf" << endl;
	// // node_l.print();
	// // cout << "Second leaf" << endl;
	// // node_l2.print();

	// Test BTNonLeafNode

	// BTNonLeafNode node_nl;
	// for (int i = 1; i < 128; i++)
	// {
	// 	assert(node_nl.insert(i, 555) == 0);
	// }

	// BTNonLeafNode node_nl2;
	// int midkey;
	// node_nl.insertAndSplit(500, 500, node_nl2, midkey);

	// cout << "First non-leaf" << endl;
	// node_nl.print();
	// cout << "Second non-leaf" << endl;
	// node_nl2.print();


	// Test index
	// BTreeIndex index;
	// index.open("btree.tbl", 'w');
	// rid.pid = 5;
	// rid.sid = 6;
	// int num = 3000;
	// // int num = 5419 * 128;
	// for (int i = 1; i < num; i++){
	// 	rid.pid = i;
	// 	rid.sid = i;
	// 	index.insert(i, rid);
	// }
	// index.insert(num, rid);
	// index.traverse();

	// cout << "testing readForward" << endl;
	// IndexCursor ic;
	// RecordId r_test;
	// int key = 5291;
	// int test_eid = 0;
	// ic.pid = 5;
	// ic.eid = 0;
	// index.locate(key, ic);
	// index.readForward(ic,key,r_test);
	// cout << "key is now:"<< key << endl;
	// cout << "IndexCursor eid is now:" << ic.eid << endl;
	// index.readForward(ic,key,r_test);
	// cout << "IndexCursor eid is now:" << ic.eid << endl;
	// // index.locate(key, ic);
	// cout << "key is now:"<< key << endl;
	// // cout << "rid is:" << r_test.pid << endl;
	// // cout << (int) r_test << endl;
	// index.close();

	// run the SQL engine taking user commands from standard input (console).
	SqlEngine::run(stdin);

	return 0;
}
