#ifndef TESTS_TEST_LIB_DATA_SAMPLES_H_
#define TESTS_TEST_LIB_DATA_SAMPLES_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <boost/shared_ptr.hpp>

#include "namespaces.h"
#include "data/Events.h"
#include "Logger.h"

class EventTest : public CPPUNIT_NS :: TestFixture
{
  //  CPPUNIT_TEST_SUITE( EventTest );
  //  CPPUNIT_TEST( testAdd );
  //  CPPUNIT_TEST( testWrite );
  //  CPPUNIT_TEST( testAppend );
  //  CPPUNIT_TEST( testSize );
  //  CPPUNIT_TEST_SUITE_END();

  private:

    bo::data::Events::P createEvents1();
    bo::data::Events::P createEvents2();
    bo::data::Event::AP createEvent(const pt::ptime&);

  protected:

    bo::data::Format::CP dataFormat_;

    virtual unsigned int getDataSize() const = 0;

  public:

    virtual void setUp() = 0;
    virtual void tearDown();

    //! tests

    void testAdd();

    void testWrite();

    void testAppend();

    void testSize();
};

#endif

