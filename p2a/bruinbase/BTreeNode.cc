#include "BTreeNode.h"
#include <iostream>
#include <cmath>
using namespace std;

#define PAIR_SIZE (sizeof(int) + sizeof(RecordId))
#define LAST_ENTRY_ADDR (PageFile::PAGE_SIZE - PAIR_SIZE - sizeof(PageId))
#define MAX_KEYS (LAST_ENTRY_ADDR / PAIR_SIZE)

// Init node with -1's and set number of keys to 0
BTLeafNode::BTLeafNode()
{
	numKeys = 0;
	memset(buffer, 0, PageFile::PAGE_SIZE);
}

/*
 * Read the content of the node from the page pid in the PageFile pf.
 * @param pid[IN] the PageId to read
 * @param pf[IN] PageFile to read from
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::read(PageId pid, const PageFile& pf)
{ 
	cout << "Form: (key, { rid.pid, rid.sid })" << endl;
	for (int i = 0; i < getKeyCount(); i++)
	{
		int k;
		RecordId rid;
		readEntry(i, k, rid);
		cout << "read: (" << k << ", { " << rid.pid << ", " << rid.sid << " })" << endl;
	}

	return pf.read(pid, buffer);
}
    
/*
 * Write the content of the node to the page pid in the PageFile pf.
 * @param pid[IN] the PageId to write to
 * @param pf[IN] PageFile to write to
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::write(PageId pid, PageFile& pf)
{ 
	return pf.write(pid, buffer);
}

/*
 * Return the number of keys stored in the node.
 * @return the number of keys in the node
 */
int BTLeafNode::getKeyCount()
{
	return numKeys; 
}

int BTLeafNode::getInsertAddress(int key)
{
	int indexToInsert;
	for (indexToInsert = 0; indexToInsert < LAST_ENTRY_ADDR; indexToInsert += PAIR_SIZE)
	{
		int currKey;
		memcpy(&currKey, buffer + indexToInsert, sizeof(int));
		if (currKey == 0 || currKey >= key)
			break;
	}
	return indexToInsert;
}

void BTLeafNode::insertIntoTempBuffer(char *temp, int indexToInsert, int size, int key, const RecordId &rid)
{
	// prefill temp with 0's
	memset(temp, 0, size);
	// copy buffer elements up till what we want to insert
	memcpy(temp, buffer, indexToInsert);
	// copy key
	memcpy(temp + indexToInsert, &key, sizeof(int));
	// copy rid
	memcpy(temp + indexToInsert + sizeof(int), &rid, sizeof(RecordId));
	// copy rest of buffer elements after inserted area
	memcpy(temp + indexToInsert + PAIR_SIZE, buffer + indexToInsert, getKeyCount() * PAIR_SIZE - indexToInsert);
}

/*
 * Insert a (key, rid) pair to the node.
 * @param key[IN] the key to insert
 * @param rid[IN] the RecordId to insert
 * @return 0 if successful. Return an error code if the node is full.
 */
RC BTLeafNode::insert(int key, const RecordId& rid)
{ 
	if (getKeyCount() >= MAX_KEYS)
		return RC_NODE_FULL;

	int addressToInsert = getInsertAddress(key);

	char *temp = (char *)malloc(PageFile::PAGE_SIZE);
	insertIntoTempBuffer(temp, addressToInsert, PageFile::PAGE_SIZE, key, rid);
	memcpy(buffer, temp, PageFile::PAGE_SIZE);

	// debug cout
	int kk;
	RecordId t_rid;
	int eid = addressToInsert / PAIR_SIZE;
	readEntry(eid, kk, t_rid);
	cout << "inserted: (" << kk << ", { " << t_rid.pid << ", " << t_rid.sid << " })" << endl;

	free(temp);
	numKeys++;
	return 0; 
}

/*
 * Insert the (key, rid) pair to the node
 * and split the node half and half with sibling.
 * The first key of the sibling node is returned in siblingKey.
 * @param key[IN] the key to insert.
 * @param rid[IN] the RecordId to insert.
 * @param sibling[IN] the sibling node to split with. This node MUST be EMPTY when this function is called.
 * @param siblingKey[OUT] the first key in the sibling node after split.
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::insertAndSplit(int key, const RecordId& rid, 
                              BTLeafNode& sibling, int& siblingKey)
{ 
	if (sibling.getKeyCount() != 0 || getKeyCount() < MAX_KEYS)
		return RC_INVALID_ATTRIBUTE;
	
	int leftSideEntries = ceil(getKeyCount() / 2);
	int midptAddr = leftSideEntries * PAIR_SIZE;
	int rightSideEntries = getKeyCount() - leftSideEntries;

	memcpy(sibling.buffer, buffer + midptAddr, rightSideEntries * PAIR_SIZE);
	sibling.setNextNodePtr(getNextNodePtr());
	
	memset(buffer + midptAddr, 0, PageFile::PAGE_SIZE - midptAddr - sizeof(PageId));

	memcpy(&siblingKey, sibling.buffer, sizeof(int));
	if (key < siblingKey)
		insert(key, rid);
	else
		sibling.insert(key, rid);

	return 0; 
}

/**
 * If searchKey exists in the node, set eid to the index entry
 * with searchKey and return 0. If not, set eid to the index entry
 * immediately after the largest index key that is smaller than searchKey,
 * and return the error code RC_NO_SUCH_RECORD.
 * Remember that keys inside a B+tree node are always kept sorted.
 * @param searchKey[IN] the key to search for.
 * @param eid[OUT] the index entry number with searchKey or immediately
                   behind the largest key smaller than searchKey.
 * @return 0 if searchKey is found. Otherwise return an error code.
 */
RC BTLeafNode::locate(int searchKey, int& eid)
{
	for (int i = 0; i < getKeyCount(); i++)
	{
		int key;
		memcpy(&key, buffer + i * PAIR_SIZE, sizeof(int));
		if (key >= searchKey)
		{
			eid = i;
			return 0;
		}
	} 
	return RC_NO_SUCH_RECORD; 
}

/*
 * Read the (key, rid) pair from the eid entry.
 * @param eid[IN] the entry number to read the (key, rid) pair from
 * @param key[OUT] the key from the entry
 * @param rid[OUT] the RecordId from the entry
 * @return 0 if successful. Return an error code if there is an error.
 */
// TODO WRONG WHEN DEBUGGING
RC BTLeafNode::readEntry(int eid, int& key, RecordId& rid)
{
	memcpy(&key, buffer + (eid * PAIR_SIZE), sizeof(int));
	memcpy(&rid, buffer + (eid * PAIR_SIZE) + sizeof(int), sizeof(RecordId));
	return 0; 
}

/*
 * Return the pid of the next slibling node.
 * @return the PageId of the next sibling node 
 */
PageId BTLeafNode::getNextNodePtr()
{
	PageId lastId;
	memcpy(&lastId, buffer + PageFile::PAGE_SIZE - sizeof(PageId), sizeof(PageId)); 
	return lastId; 
}

/*
 * Set the pid of the next slibling node.
 * @param pid[IN] the PageId of the next sibling node 
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::setNextNodePtr(PageId pid)
{
	memcpy(buffer + PageFile::PAGE_SIZE - sizeof(PageId), &pid, sizeof(PageId)); 
	return 0; 
}

/*
 * Read the content of the node from the page pid in the PageFile pf.
 * @param pid[IN] the PageId to read
 * @param pf[IN] PageFile to read from
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::read(PageId pid, const PageFile& pf)
{ return 0; }
    
/*
 * Write the content of the node to the page pid in the PageFile pf.
 * @param pid[IN] the PageId to write to
 * @param pf[IN] PageFile to write to
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::write(PageId pid, PageFile& pf)
{ return 0; }

/*
 * Return the number of keys stored in the node.
 * @return the number of keys in the node
 */
int BTNonLeafNode::getKeyCount()
{ return 0; }


/*
 * Insert a (key, pid) pair to the node.
 * @param key[IN] the key to insert
 * @param pid[IN] the PageId to insert
 * @return 0 if successful. Return an error code if the node is full.
 */
RC BTNonLeafNode::insert(int key, PageId pid)
{ return 0; }

/*
 * Insert the (key, pid) pair to the node
 * and split the node half and half with sibling.
 * The middle key after the split is returned in midKey.
 * @param key[IN] the key to insert
 * @param pid[IN] the PageId to insert
 * @param sibling[IN] the sibling node to split with. This node MUST be empty when this function is called.
 * @param midKey[OUT] the key in the middle after the split. This key should be inserted to the parent node.
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::insertAndSplit(int key, PageId pid, BTNonLeafNode& sibling, int& midKey)
{ return 0; }

/*
 * Given the searchKey, find the child-node pointer to follow and
 * output it in pid.
 * @param searchKey[IN] the searchKey that is being looked up.
 * @param pid[OUT] the pointer to the child node to follow.
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::locateChildPtr(int searchKey, PageId& pid)
{ return 0; }

/*
 * Initialize the root node with (pid1, key, pid2).
 * @param pid1[IN] the first PageId to insert
 * @param key[IN] the key that should be inserted between the two PageIds
 * @param pid2[IN] the PageId to insert behind the key
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::initializeRoot(PageId pid1, int key, PageId pid2)
{ return 0; }
