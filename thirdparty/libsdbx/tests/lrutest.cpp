

#include <stdio.h>
#include <ctime>

#define LRUCACHE_DEBUG
#include <sdbx/lru.h>


typedef LRUCache<int, int, LRUDeleteNone> Cache;


int main()
{
	Cache cache(40);
	int ints[] = { 
		0,1,2,3,4,5,6,7,8,9,
		0,1,2,3,4,5,6,7,8,9,
		0,1,2,3,4,5,6,7,8,9,
		0,1,2,3,4,5,6,7,8,9,
		0,1,2,3,4,5,6,7,8,9,
		0,1,2,3,4,5,6,7,8,9 };
	int i;

	srand((unsigned)time(NULL));
	
	// random accesses
	for (i = 0; i < 1000; i++)
	{	
		int id = rand() % 60;
		int * item = cache.get(id);
		
		if (!item)
		{
			Cache::Node * nd = cache.add(id, &ints[id]);
			delete nd;
		}
	}
	
	printf("Hits: %d (%f%%), Misses: %d (%f%%)\n", 
		cache.m_hits, (double)cache.m_hits/(double)(cache.m_hits+cache.m_misses),
		cache.m_misses, (double)cache.m_misses/(double)(cache.m_hits+cache.m_misses));
	

	LRUCache<int, int, LRUDeleteArray> test1(5);

	int * tmp = new int;

	// when you add an item, you need to free anything returned
	if (test1.add(0, tmp))
		fprintf(stderr, "test1.add returned something.. that shouldn't have happened..\n");
		
	return 0;
}
