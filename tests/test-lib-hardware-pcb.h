#ifndef TESTS_TEST_LIB_HARDWARE_PCB_H_
#define TESTS_TEST_LIB_HARDWARE_PCB_H_

#include <boost/shared_ptr.hpp>
 
#include "namespaces.h"
 
class HardwareTest : public CPPUNIT_NS :: TestFixture
{
  CPPUNIT_TEST_SUITE( HardwareTest );
  CPPUNIT_TEST( testV4 );
  CPPUNIT_TEST( testV6 );
  CPPUNIT_TEST( testFirmware27b );
  CPPUNIT_TEST_SUITE_END();

  private:

  void run(bo::hardware::comm::Base& comm, unsigned int excpectedSamples);

  public:
  void setUp();
  void tearDown();

  //! tests

  void testV4();
  void testV6();
  void testFirmware27b();
};

#endif
