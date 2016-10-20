/*
* IMPORTS: none
* EXPORTS: none
* PURPOSE: Used as a function pointer to serialize library.
*/
void Serialize()
{
}

/*
* IMPORTS: none
* EXPORTS: none
* PURPOSE: Used as a function pointer to deserialize library.
*/
void Deserialize()
{
}

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>


// use these
m_out.open("");
m_out.close();

void save_schedule(const bus_schedule &s, const char * filename) {
	// make an archive
	std::ofstream ofs(filename);
	boost::archive::text_oarchive oa(ofs);
	oa << s;
}

void
restore_schedule(bus_schedule &s, const char * filename)
{
	// open the archive
	std::ifstream ifs(filename);
	boost::archive::text_iarchive ia(ifs);

	// restore the schedule from the archive
	ia >> s;
}