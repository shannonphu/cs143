/*
 * Copyright (C) 2008 by The Regents of the University of California
 * Redistribution of this file is permitted under the terms of the GNU
 * Public License (GPL).
 *
 * @author Junghoo "John" Cho <cho AT cs.ucla.edu>
 * @date 3/24/2008
 */
 
#include "BTreeIndex.h"
#include "BTreeNode.h"
#include <iostream>

using namespace std;

/*
 * BTreeIndex constructor
 */
BTreeIndex::BTreeIndex()
{
    rootPid = -1;
    treeHeight = 0;

    // buffer contains [treeHeight|rootPid]
    memset(buffer, 0, PageFile::PAGE_SIZE);
}

/*
 * Open the index file in read or write mode.
 * Under 'w' mode, the index file should be created if it does not exist.
 * @param indexname[IN] the name of the index file
 * @param mode[IN] 'r' for read, 'w' for write
 * @return error code. 0 if no error
 */
RC BTreeIndex::open(const string& indexname, char mode)
{
	if (pf.open(indexname, mode) != 0)
	{
		return RC_FILE_OPEN_FAILED;
	}

	// New index file
	if (pf.endPid() == 0)
	{
		pf.write(0, buffer);
	}
	else
	{
		pf.read(0, buffer);
		memcpy(&treeHeight, buffer, sizeof(int));
		memcpy(&rootPid, buffer + sizeof(int), sizeof(PageId));
	}

    return 0;
}

/*
 * Close the index file.
 * @return error code. 0 if no error
 */
RC BTreeIndex::close()
{
	memcpy(buffer, &treeHeight, sizeof(int));
	memcpy(buffer + sizeof(int), &rootPid, sizeof(PageId));
	pf.write(0, buffer);
    return pf.close();
}

// Debug helper
void BTreeIndex::traverse()
{
	BTNonLeafNode root;
	root.read(rootPid, pf);
	root.print();
}

/*
 * Insert (key, RecordId) pair to the index.
 * @param key[IN] the key for the value inserted into the index
 * @param rid[IN] the RecordId for the record being inserted into the index
 * @return error code. 0 if no error
 */
RC BTreeIndex::insert(int key, const RecordId& rid)
{
	cout << "inserting " << key << endl;
	// Initialize root with first leaf
	cout << "treeheight before checks:" << treeHeight <<endl;
	if (treeHeight == 0)
	{
		cout << "Making new root at key:" << key << endl;
		BTLeafNode leaf;
		if (leaf.insert(key, rid) != 0)
		{
			return RC_NODE_FULL;
		}

		treeHeight = 1;
		rootPid = pf.endPid();
		RC err = leaf.write(rootPid, pf);
		if (err != 0)
		{
			return err;
		}
		cout << "treeheight after inserting:" << key << " is:" << treeHeight << endl;
		return 0;
	}

	// Search and insert (not first insertion)
	int movedKey;
	PageId movedPid;
	// Call recursive insertion function and if overflow occured
	insertHelper(key, rid, rootPid, 1, movedKey, movedPid);
    return 0;
}

// Recursive insertion helper
RC BTreeIndex::insertHelper(int key, RecordId rid, PageId &currNode, int currHeight, int &movedKey, PageId &movedPid)
{
	cout << "Inside insert helper with key:" << key << endl;
	// Reached leaf level
	if (currHeight == treeHeight)
	{
		cout << "Inserting into current level" << endl;
		BTLeafNode leaf;
		leaf.read(currNode, pf);
		cout << "read successful at key:" << key << endl;
		// Check for successful insertion
		if (leaf.insert(key, rid) == 0)
		{
			cout << "normal insert" << endl;
			return 0;
		}

		// Deal with overflow
		BTLeafNode sibling;
		leaf.insertAndSplit(key, rid, sibling, movedKey);
		movedPid = pf.endPid();

		// Connect leaf next pointers
		sibling.setNextNodePtr(leaf.getNextNodePtr());
		leaf.setNextNodePtr(pf.endPid());

		// Write leaves back to page file
		leaf.write(currNode, pf);
		sibling.write(pf.endPid(), pf);
		return RC_NODE_FULL;
	}

	//// Search through non-leaf nodes

	// Get non-leaf node contents
	BTNonLeafNode nl_node;
	nl_node.read(currNode, pf);

	// Get child node the non-leaf node points to for this key
	PageId childPid;
	nl_node.locateChildPtr(key, childPid);

	// Overflow at the leaf level
	if (insertHelper(key, rid, childPid, currHeight + 1, movedKey, movedPid) == RC_NODE_FULL)
	{
		// Try to insert into non-leaf node successfully
		if (nl_node.insert(movedKey, movedPid) == 0)
		{
			nl_node.write(childPid, pf);
			return 0;
		}

		// Deal with overflow in non-leaf node level
		BTNonLeafNode nl_sibling;
		int midKey;
		nl_node.insertAndSplit(movedKey, movedPid, nl_sibling, midKey);

		movedKey = midKey;
		movedPid = pf.endPid();

		nl_node.write(childPid, pf);
		nl_sibling.write(pf.endPid(), pf);
	}

	return 0;
}

/**
 * Run the standard B+Tree key search algorithm and identify the
 * leaf node where searchKey may exist. If an index entry with
 * searchKey exists in the leaf node, set IndexCursor to its location
 * (i.e., IndexCursor.pid = PageId of the leaf node, and
 * IndexCursor.eid = the searchKey index entry number.) and return 0.
 * If not, set IndexCursor.pid = PageId of the leaf node and
 * IndexCursor.eid = the index entry immediately after the largest
 * index key that is smaller than searchKey, and return the error
 * code RC_NO_SUCH_RECORD.
 * Using the returned "IndexCursor", you will have to call readForward()
 * to retrieve the actual (key, rid) pair from the index.
 * @param key[IN] the key to find
 * @param cursor[OUT] the cursor pointing to the index entry with
 *                    searchKey or immediately behind the largest key
 *                    smaller than searchKey.
 * @return 0 if searchKey is found. Othewise an error code
 */
RC BTreeIndex::locate(int searchKey, IndexCursor& cursor)
{
    return 0;
}

/*
 * Read the (key, rid) pair at the location specified by the index cursor,
 * and move foward the cursor to the next entry.
 * @param cursor[IN/OUT] the cursor pointing to an leaf-node index entry in the b+tree
 * @param key[OUT] the key stored at the index cursor location.
 * @param rid[OUT] the RecordId stored at the index cursor location.
 * @return error code. 0 if no error
 */
RC BTreeIndex::readForward(IndexCursor& cursor, int& key, RecordId& rid)
{
    return 0;
}
