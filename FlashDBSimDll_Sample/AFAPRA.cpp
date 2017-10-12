#include "AFAPRA.h"

// intilize free list
// initialize ml,ccl,gl

AFAPRA::AFAPRA() {

	windowSize = DEFBUFSIZE/3;
	ghostListSize = DEFBUFSIZE / 3;
	freelist = new Header();
	temp = freelist;

	for (int i = 0; i > DEFBUFSIZE; i++) {
		temp->nextEl = new Header();
		temp->nextEl->prevEl = temp;
		temp->dirty = 0;
		temp->eFactor = 0;
		temp->hot = 0;
		temp->ghost = 0;
		temp = temp->nextEl;
	}
}
	

AFAPRA::~AFAPRA() {
	Header *temp = freelist;

	while(temp != NULL) {
		freelist = freelist->nextEl;
		delete temp;
		temp = freelist;
	}
	temp = NULL;
}

void AFAPRA::addToMixedListMRU(Header* temp){
	temp->prevEl->nextEl = temp->nextEl;
	temp->nextEl->prevEl = temp->prevEl;
	mixedlist_MRU->prevEl = temp;
	temp->nextEl = mixedlist_MRU;
	mixedlist_MRU = temp;
}

Header* AFAPRA::request(int frame_id, int op){
	//check in ml
	Header* temp;

	//mixed list
	temp = mixedlist_MRU;
	while (temp != NULL) {
		if (temp->frame_id == frame_id) {
			//move to MRU
			if (temp != mixedlist_MRU) {
				addToMixedListMRU(temp);
			}

			//make hot etc
			temp->hot = 1;

			//increment eFactor
			temp->eFactor++;
			// if ghost flag set -> size++ , clear flag
			if (temp->ghost) {
				temp->ghost--;
				ghostListSize++;
			}

			return temp;
		}
	}

	//cold clean list
	temp = cclist_LRU;
	while (temp != NULL) {
		if (temp->frame_id == frame_id) {
			if (op == 1)
				temp->eFactor = 8;
			else
				temp->eFactor++;

			//	move page to MRU of ML
			addToMixedListMRU(temp);
			return temp;
	}

	//Ghost List
	temp = ghostList_LRU;
	while (temp != NULL) {
		if (temp->frame_id == frame_id) {
			temp->ghost = 1;

			if (temp->dirty) {
				windowSize++;
				writeToDisk(temp);
				temp->dirty = 0;
			}
			else
				windowSize--;

			// put to MRU of ML
			if (temp->hot == 1) {
				temp->eFactor = 4;
				addToMixedListMRU(temp);
			}
			else {
				// put to MRU of CL
				temp->prevEl->nextEl = temp->nextEl;
				temp->nextEl->prevEl = temp->prevEl;
				temp->nextEl = cclist_MRU;
				temp->prevEl = NULL;
				cclist_MRU = temp;
			}

			return temp;
		}

		temp= temp->nextEl;
	}

	temp = alloc();
	if (op = 1)
		temp->eFactor = 1;
	else
		temp->eFactor = 4;
	

		//		if (page is 32 sectors away)
		//			eFactor += 4

		//			move page to MRU of CCL

	//readfromdisk


		adjustWindowSize();

		addToMixedListMRU(temp);
		return temp;
	
	}
}


Header* AFAPRA::alloc() {
	Header *victim;

	if (freelist != NULL) {
		victim = freelist;
		freelist = freelist->nextEl;
		freelist->prevEl = NULL;
		victim->nextEl = NULL;
		victim->prevEl = NULL;
	}
	else if (cclist_LRU != NULL) {
		while (cclist_LRU != NULL) {
			victim = cclist_LRU;
			cclist_LRU = cclist_LRU->prevEl;
			cclist_LRU->nextEl = NULL;
			victim->prevEl = victim->nextEl = NULL;
			return victim;
		}
	}
	else {
		Header* temp = mixedlist_LRU;

		while (victim == NULL) {
			while (temp->eFactor != 0) {
				temp->eFactor--;
				temp = temp->prevEl;
			}

			victim = temp;
			if (victim->ghost)
				ghostListSize--;

			if (victim->dirty)
				writeToDisk(victim);
			
			victim->data = NULL;
			victim->nextEl = ghostList_LRU;
			ghostList_LRU->prevEl = victim;
			ghostList_LRU = victim;
			victim->prevEl = NULL;
		}
		
		return new Header();
	}
}

int AFAPRA::writeToDisk(Header* victim) {
	if (victim->dirty)
	{
		//程序结束时，我们应该将脏页写回二级存储器
		int rv = f_write_page(victim->page_id, (BYTE *)(victim->data), 0, FRAMESIZE);
		if (rv == RV_ERROR_FLASH_NO_MEMORY)
		{
			printf("no more flash memory \n");
			return rv;
		}

		flashwritecount++;
		
	}
}

void AFAPRA::adjustWindowSize() {
	
}

//window size managae
//ghostlist size manage
//stats manage
//hashmap