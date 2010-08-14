#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/XmlOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>

int main (int argc, char* argv[])
{
  // Informiert Test-Listener ueber Testresultate
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
