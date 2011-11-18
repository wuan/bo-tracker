/*  

    Copyright (C) 2010-2011  Andreas Würl <blitzortung@tryb.de>

*/

#include <boost/program_options.hpp>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/XmlOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>

#include "namespaces.h"
#include "Logger.h"

int main (int argc, char* argv[])
{
  // programm arguments/options
  boost::program_options::options_description desc("program options");
  desc.add_options()
    ("help,h", "show program help")
    ("verbose,v", "verbose mode")
    ("debug,d", "debug mode")
    ;


  // parse command line options
  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(desc).run(), vm);
  po::notify(vm); 

  // help output or no 'sql-statement' given
  if (vm.count("help")) {
    std::cout << argv[0] << " [options]" << std::endl << std::endl;
    std::cout << desc << std::endl;
    std::cout << std::endl;

    return 1;
  }

  // Informiert Test-Listener ueber Testresultate
  bo::Logger logger;

  logger.setPriority(log4cpp::Priority::NOTICE);

  if (vm.count("verbose")) {
    logger.setPriority(log4cpp::Priority::INFO);
  }

  if (vm.count("debug")) {
    logger.setPriority(log4cpp::Priority::DEBUG);
  }


  // create test result object
  CPPUNIT_NS::TestResult testResult;

  // Listener zum Sammeln der Testergebnisse registrieren
  CPPUNIT_NS::TestResultCollector resultCollector;
  testResult.addListener(&resultCollector);

  // Listener zur Ausgabe der Ergebnisse einzelner Tests
  CPPUNIT_NS::BriefTestProgressListener progress;
  testResult.addListener(&progress);

  // Test-Suite ueber die Registry im Test-Runner einfuegen
  CPPUNIT_NS::TestRunner testRunner;
  testRunner.addTest (CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest());
  testRunner.run(testResult);

  // Resultate im Compiler-Format ausgeben
  CPPUNIT_NS::CompilerOutputter compileroutputter (&resultCollector, std::cerr);
  compileroutputter.write();

  std::ofstream xmlFileOut("cppunit-lib-results.xml");
  CPPUNIT_NS::XmlOutputter xmlOut(&resultCollector, xmlFileOut);
  xmlOut.write();

  // Returncode je nach Erfolg der Tests
  return resultCollector.wasSuccessful () ? 0 : 1;
}
