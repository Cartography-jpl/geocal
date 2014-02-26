

#include <sdbx/StringTable.h>
#include <sdbx/BTree/BTreePrint.h>

#include <stdio.h>

#ifdef USE_WX_MSVC_MEMORY_TRACING
	// this enables advanced memory leak detection
	#include <wx/msw/msvcrt.h>
#endif


typedef StringTable<3, 4> Table;

#define INSERT(x) \
	result = table.InsertString(x, id); \
	if (result) \
	printf("Inserted %s, Id: %" PRINTF_INT64_MODIFIER "u\n", x, (uint64_t)id); \
	else printf("Could not insert, error code %d returned\n", table.GetErrorCode())

#define FIND_STRING(x) \
	result = table.FindStringId(x, id); \
	printf("Searching for %s, %s", x, result ? "found it" : "not found"); \
	if (result) printf(" at id %" PRINTF_INT64_MODIFIER "u\n",(uint64_t)id); \
	else printf(", error code %d\n", table.GetErrorCode())


int main()
{
	Table table;

	if (!table.Initialize(wxFileName(wxT("StringTable.str")), 3, 4))
	{
		fprintf(stderr, "Error initializing tree: code %d\n", table.GetErrorCode());
		return 1;
	}
	
	Table::Iterator * i = table.Begin();

	UniqueId id;
	bool result;

	// insert a lot of strings
	INSERT("st");
	//i->m_tree->Print("t01");
	INSERT("st0");
	//i->m_tree->Print("t02");
	INSERT("test_string");
	//i->m_tree->Print("t03");
	INSERT("str0");
	//i->m_tree->Print("t04");
	INSERT("str1");
	//i->m_tree->Print("t05");
	INSERT("str2");
	//i->m_tree->Print("t06");
	INSERT("str3");
	//i->m_tree->Print("t07");
	INSERT("str4");
	//i->m_tree->Print("t08");
	INSERT("str5");
	//i->m_tree->Print("t09");
	INSERT("str6");
	//i->m_tree->Print("t10");
	INSERT("str7");
	//i->m_tree->Print("t11");
	INSERT("str8");
	//i->m_tree->Print("t12");
	INSERT("str9");
	//i->m_tree->Print("t13");

	INSERT(
		"0000000000"
		"1234567890"
		"1234567890"
		"1234567890"
		"1234567890"
		"1234567890"
		"1234567890"
		"1234567890"
		"1234567890"
		"1234567890"
		"1234567890"
		"1234567890"
		"1234567890"
		"0000000000"
	);
	

	// make some queries, make sure duplicate keys are handled correctly
	FIND_STRING("st");
	FIND_STRING("st0");
	FIND_STRING("test_string");
	FIND_STRING("str0");
	FIND_STRING("str1");
	FIND_STRING("str2");
	FIND_STRING("str3");
	FIND_STRING("str4");
	FIND_STRING("does not exist");
	FIND_STRING("str5");
	FIND_STRING("str6");
	FIND_STRING("str7");
	FIND_STRING("str8");
	FIND_STRING("str9");

	// insert some more strings

	INSERT("another one");
	//i->m_tree->Print("t14");
	INSERT("another two");
	//i->m_tree->Print("t15");
	INSERT("ano");
	//i->m_tree->Print("t16");
	INSERT("anpd");
	//i->m_tree->Print("t17");
	INSERT("ana");
	//i->m_tree->Print("t18");
	INSERT("what are you looking at?");
	//i->m_tree->Print("t19");
	INSERT("ano2");
	//i->m_tree->Print("t20");
	INSERT("st0");
	//i->m_tree->Print("t21");
	INSERT("st0");
	//i->m_tree->Print("t22");
	

	// do some more queries
	FIND_STRING("str0");
	FIND_STRING("str1");
	FIND_STRING("str2");
	FIND_STRING("str3");
	FIND_STRING("str4");

	FIND_STRING("another one");
	FIND_STRING("another two");
	FIND_STRING("ano");
	FIND_STRING("what are you looking at?");

	// print out the contents of the table
	if (!i->begin())
	{
		fprintf(stderr, "Error getting iterator to tree: code %d\n", table.GetErrorCode());
		delete i;
		return 1;
	}
	
	
	do 
	{
		char key[Table::key_length + 16];
		memset(key, 0, Table::key_length + 16);

		strncpy(key, i->current().key.keyval, Table::key_length);
		id = i->current().value;

		const char * str = table.GetStringById(id);

		printf("Key: %s, Id: %" PRINTF_INT64_MODIFIER "u, str: %s\n", key, (uint64_t)id, str ? str : "ERROR");

	} while (i->next());

	if (!table.VerifyIndex())
		printf("Invalid tree!!!\n");

	delete i;
		
	return 0;
}

