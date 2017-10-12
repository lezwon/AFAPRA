#pragma once
#include "BufferAlogrithm.h"

typedef struct Header {
	int page_id;
	int frame_id;
	char dirty;
	char hot;
	char ghost;
	char eFactor;
	bFrame *data;
	Header *nextHash;
	Header *prevHash;
	Header *nextEl;
	Header *prevEl;
} Header;


class AFAPRA : public BMgr
{
public:
	AFAPRA();
	~AFAPRA();
	Header* request(int frame_id, int op);
	void adjustWindowSize();
	Header* alloc();
	int writeToDisk(Header *);
	void addToMixedListMRU(Header *);
	//void Init();
	//int FixPage(int page_id);
	//NewPage FixNewPage(LBA lba);
	//int UnFixPage(int page_id);
	//void ReadFrame(int frame_id, char* buffer);
	//void WriteFrame(int frame_id, const char*buffer);
	//int WriteDirty(void);
	//double HitRatio(void);
	//void RWInfo();
	//int IsLBAValid(LBA lba);
	//int LBAToPID(LBA lba);

private:
	//int hash(int page_id);
	//Header* pagetoheader(int page_id);
	//void removeheader(header* pb);
	//void insertlruele(int frame_id);
	//void removelruele(int frame_id);
	//void adjustlrulist(int frame_id);
	//void setdirty(int frame_id);
	//int selectvictim();
	//void registentry(lba lba, pid page_id);



private:
	Header* hashmap[DEFBUFSIZE / 20];
	Header* mixedlist_MRU;
	Header* mixedlist_LRU;
	Header* mixedlist_window;
	Header* cclist_MRU;
	Header* cclist_LRU;
	Header* ghostList_LRU;
	Header* freelist;
	Header* temp;


	int hit;
	int total;
	int ghostListSize;
	int windowSize;
	int ml_count;
};
