#include "BTreeNode.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <cstring>
using namespace std;

#define PAIR_SIZE (sizeof(int) + sizeof(RecordId))
#define PAIR_SIZE_NL (sizeof(int) + sizeof(PageId))
#define LAST_ENTRY_ADDR (PageFile::PAGE_SIZE - PAIR_SIZE - sizeof(PageId))
#define LAST_ENTRY_ADDR_NL (PageFile::PAGE_SIZE - sizeof(PageId))
#define MAX_KEYS (LAST_ENTRY_ADDR / PAIR_SIZE)
// #define MAX_KEYS ((PageFile::PAGE_SIZE - sizeof(PageId))/PAIR_SIZE)
#define MAX_KEYS_NL (PageFile::PAGE_SIZE / PAIR_SIZE_NL)

// Init node with -1's and set number of keys to 0
BTLeafNode::BTLeafNode()
{
	numKeys = 0;
	memset(buffer, 0, PageFile::PAGE_SIZE);
}

void BTLeafNode::print()
{
	cout << "In node buffer: (key, { rid.pid, rid.sid })" << endl;
	for (int i = 0; i < getKeyCount(); i++)
	{
		int k;
		RecordId rid;
		readEntry(i, k, rid);
		cout << "(" << k << ", { " << rid.pid << ", " << rid.sid << " })" << endl;
	}
	cout << "=====" << endl;
}

/*
 * Read the content of the node from the page pid in the PageFile pf.
 * @param pid[IN] the PageId to read
 * @param pf[IN] PageFile to read from
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::read(PageId pid, const PageFile& pf)
{ 
	// print();
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
	//cout << "writing " << endl;
	//print();
	return pf.write(pid, buffer);
}

/*
 * Return the number of keys stored in the node.
 * @return the number of keys in the node
 */
int BTLeafNode::getKeyCount()
{	
	int count = 0;
	char* temp = buffer;
	int i;
	for (i = 0; i<=1008; i+= PAIR_SIZE){
		int insideKey;
		memcpy(&insideKey, temp, sizeof(int));
		if (insideKey == 0)
			break;

		count++;
		temp+= PAIR_SIZE;

	}

	return count;
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
	//cout << "size of pageid:" << sizeof(RecordId)<<endl;
	//cout << "inserting key:" << key << " with insertAddress:" << indexToInsert << endl;
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
	//cout << "Copy ok up to this point" << endl;
	int copy = getKeyCount() * PAIR_SIZE - indexToInsert;
	//cout << "bytes copied:" << copy << endl;
	memcpy(temp + indexToInsert + PAIR_SIZE, buffer + indexToInsert, getKeyCount() * PAIR_SIZE - indexToInsert);
	//cout << "Made it boys" << endl;
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
	int keyCount = getKeyCount();
<<<<<<< HEAD

	cout << "inserting:" << key << " with " << keyCount << " keys before" << endl;
	cout << "printing before inserting" << endl;
	print();
=======
	
	//cout << "inserting:" << key << " with " << keyCount << " keys before" << endl;
	//cout << "printing before inserting" << endl;
	//print();
>>>>>>> 80149535947bf57d9090d954bdb3a392f4bc8c83
	int addressToInsert = getInsertAddress(key);

	char *temp = (char *)malloc(PageFile::PAGE_SIZE);
	insertIntoTempBuffer(temp, addressToInsert, PageFile::PAGE_SIZE, key, rid);
	memcpy(buffer, temp, PageFile::PAGE_SIZE);

	// debug cout
	int kk;
	RecordId t_rid;
	int eid = addressToInsert / PAIR_SIZE;
	readEntry(eid, kk, t_rid);
	// cout << "inserted: (" << kk << ", { " << t_rid.pid << ", " << t_rid.sid << " })" << endl;
	//print();
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
	sibling.numKeys = rightSideEntries;

	memset(buffer + midptAddr, 0, PageFile::PAGE_SIZE - midptAddr - sizeof(PageId));
	numKeys = leftSideEntries;

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

BTNonLeafNode::BTNonLeafNode()
{
	numKeys = 0;
	memset(buffer, 0, PageFile::PAGE_SIZE);
}

RC BTNonLeafNode::readEntry(int eid, int& key, PageId &pid)
{
	memcpy(&key, buffer + (eid * PAIR_SIZE_NL) + sizeof(PageId), sizeof(int));
	memcpy(&pid, buffer + (eid * PAIR_SIZE_NL) + sizeof(PageId) + sizeof(int), sizeof(PageId));	
	return 0; 
}

void BTNonLeafNode::print()
{
	cout << "Form: (key, pid )" << endl;
	for (int i = 0; i < getKeyCount(); i++)
	{
		int k;
		PageId pid;
		readEntry(i, k, pid);
		cout << "read: (" << k << ", " << pid << ")" << endl;
	}
}

/*
 * Read the content of the node from the page pid in the PageFile pf.
 * @param pid[IN] the PageId to read
 * @param pf[IN] PageFile to read from
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::read(PageId pid, const PageFile& pf)
{ 

	// print();
	return pf.read(pid, buffer);
}
    
/*
 * Write the content of the node to the page pid in the PageFile pf.
 * @param pid[IN] the PageId to write to
 * @param pf[IN] PageFile to write to
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::write(PageId pid, PageFile& pf)
{ return pf.write(pid, buffer); }

/*
 * Return the number of keys stored in the node.
 * @return the number of keys in the node
 */
int BTNonLeafNode::getKeyCount()
{ 
	int count = 0;
	int i;
	for (i = sizeof(PageId); i <= LAST_ENTRY_ADDR_NL; i += PAIR_SIZE_NL)
	{
		int insideKey;
		memcpy(&insideKey, buffer + i, sizeof(int));
		if (insideKey == 0)
			break;

		count++;
	}

	return count;
}

int BTNonLeafNode::getInsertAddress(int key) 
{
	int indexToInsert, currKey;
	for (indexToInsert = sizeof(PageId); indexToInsert <= LAST_ENTRY_ADDR_NL; indexToInsert += PAIR_SIZE_NL) {
		memcpy(&currKey, buffer + indexToInsert, sizeof(int));

		if (currKey == 0 || currKey >= key)
			break;
	}
	// Insert address is after last entry
	// if (currKey < key && indexToInsert == LAST_ENTRY_ADDR_NL + PAIR_SIZE_NL)
	// 	return indexToInsert;

	return indexToInsert;
}

void BTNonLeafNode::insertIntoTempBuffer(char *temp, int indexToInsert, int size, int key, const PageId &pid)
{
	// prefill temp with 0's
	memset(temp, 0, size);
	// copy buffer elements up till what we want to insert
	memcpy(temp, buffer, indexToInsert);
	// copy key
	memcpy(temp + indexToInsert, &key, sizeof(int));
	// copy pid
	memcpy(temp + indexToInsert + sizeof(int), &pid, sizeof(PageId));
	// copy rest of buffer elements after inserted area
	int remainingSize = PageFile::PAGE_SIZE - indexToInsert - PAIR_SIZE_NL;
	memcpy(temp + indexToInsert + PAIR_SIZE_NL, buffer + indexToInsert, remainingSize);
}
/*
 * Insert a (key, pid) pair to the node.
 * @param key[IN] the key to insert
 * @param pid[IN] the PageId to insert
 * @return 0 if successful. Return an error code if the node is full.
 */
RC BTNonLeafNode::insert(int key, PageId pid)
{ 
	if (getKeyCount() >= MAX_KEYS_NL)
			return RC_NODE_FULL;

	int addressToInsert = getInsertAddress(key);
	char *temp = (char*)malloc(PageFile::PAGE_SIZE);
	insertIntoTempBuffer(temp, addressToInsert, PageFile::PAGE_SIZE, key, pid);
	memcpy(buffer, temp, PageFile::PAGE_SIZE);
	
	free(temp);
	numKeys++;

	// debug cout
	// int kk;
	// PageId t_pid;
	// int eid = addressToInsert / PAIR_SIZE_NL;
	// readEntry(eid, kk, t_pid);
	// cout << "inserted: (" << kk << ", " << t_pid << ")" << endl;

	return 0;
}

void BTNonLeafNode::setKeyCount(int val) {
	numKeys = val;
}

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
{ 
	if (getKeyCount() < MAX_KEYS_NL || sibling.getKeyCount() != 0) 
			return RC_INVALID_ATTRIBUTE;
	memset(sibling.buffer, 0, PageFile::PAGE_SIZE);

	int half = ceil(getKeyCount()/2);
	int mid_node_pos = ((half+1) * PAIR_SIZE_NL);

	int last_left;
	int first_right;

	memcpy(&last_left, buffer + mid_node_pos - PAIR_SIZE_NL-8, sizeof(int));
	memcpy(&first_right, buffer + mid_node_pos-8, sizeof(int));

	if (key < last_left) {
		midKey = last_left;

		//copy right side to sibling 
		memcpy(sibling.buffer + PAIR_SIZE_NL, buffer + mid_node_pos, PageFile::PAGE_SIZE - mid_node_pos);
		memcpy(sibling.buffer, buffer + mid_node_pos -sizeof(PAIR_SIZE_NL), sizeof(PAIR_SIZE_NL));

		// delete everything from mid_node_pos and after
		memset(buffer + mid_node_pos, 0, PageFile::PAGE_SIZE - mid_node_pos - PAIR_SIZE_NL);

		int sibling_keys = getKeyCount() - half;
		sibling.setKeyCount(sibling_keys);
		setKeyCount(half-1);

		insert(key, pid);
	}
	else if (key > first_right) {

		midKey = first_right;

		//copy right side to sibling 
		memcpy(sibling.buffer + PAIR_SIZE_NL, buffer + mid_node_pos + PAIR_SIZE_NL, PageFile::PAGE_SIZE - mid_node_pos - PAIR_SIZE_NL );
		memcpy(sibling.buffer, buffer + mid_node_pos,sizeof(PAIR_SIZE_NL));

		// delete everything from mid_node_pos and after
		memset(buffer + mid_node_pos, 0 , PageFile::PAGE_SIZE - mid_node_pos);

		//update keycounts 
		int sibling_keys = getKeyCount() - half - 1;
		sibling.setKeyCount(sibling_keys);
		setKeyCount(half);

		sibling.insert(key,pid);
	}
	else {
		cout << "inserting duplicate key in non-leaf" << endl;
		midKey = key;

		// copy to right side 
		memcpy(sibling.buffer + PAIR_SIZE_NL, buffer + mid_node_pos, PageFile::PAGE_SIZE - mid_node_pos);

		// the first pageid of thesibling node is the neweset inserted pageid
		memcpy(sibling.buffer, &pid, sizeof(PageId));

		// delete from older buffer 
		memset(buffer + mid_node_pos, 0 , PageFile::PAGE_SIZE);

		int sibling_keys = getKeyCount() - half;
		sibling.setKeyCount(sibling_keys);
		setKeyCount(half);
	}
	return 0; 
}

/*
 * Given the searchKey, find the child-node pointer to follow and
 * output it in pid.
 * @param searchKey[IN] the searchKey that is being looked up.
 * @param pid[OUT] the pointer to the child node to follow.
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::locateChildPtr(int searchKey, PageId& pid)
{ 
	if (getKeyCount() == 0)
		return RC_NO_SUCH_RECORD;

	for (int i = sizeof(PageId); i < getKeyCount() * PAIR_SIZE_NL; i += PAIR_SIZE_NL){
	 	int currKey;
	 	memcpy(&currKey, buffer + i, sizeof(int));

	 	// Get last pid because the searchKey is larger than all keys in non-leaf
	 	if (ceil(i / PAIR_SIZE_NL) == getKeyCount() - 1 && currKey < searchKey) {
	 		memcpy(&pid, buffer + i + sizeof(int), sizeof(PageId));
	 		return 0;
	 	}

	 	// Found position in middle of non-leaf
	 	if (currKey > searchKey) {
	 		memcpy(&pid, buffer + i - sizeof(PageId), sizeof(PageId));
	 		return 0;
	 	}
	}
	return RC_NO_SUCH_RECORD;
}

/*
 * Initialize the initializeRoot node with (pid1, key, pid2).
 * @param pid1[IN] the first PageId to insert
 * @param key[IN] the key that should be inserted between the two PageIds
 * @param pid2[IN] the PageId to insert behind the key
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::initializeRoot(PageId pid1, int key, PageId pid2)
{ 
	if (getKeyCount() != 0)
		return RC_INVALID_ATTRIBUTE;
	memset(buffer,0, PageFile::PAGE_SIZE);

	memcpy(buffer, &pid1, sizeof(PageId));
	memcpy(buffer + sizeof(PageId), &key, sizeof(int));
	memcpy(buffer + PAIR_SIZE_NL, &pid2, sizeof(PageId));
	numKeys++;
	return 0; 
}
