#include <cppunit/extensions/HelperMacros.h>
#include "PhysicsTools/Utilities/interface/Operations.h"
#include "PhysicsTools/Utilities/interface/Gaussian.h"
#include "PhysicsTools/Utilities/interface/Exponential.h"
#include "PhysicsTools/Utilities/interface/Identity.h"
#include "PhysicsTools/Utilities/interface/Composition.h"
#include "PhysicsTools/Utilities/interface/Convolution.h"
#include "PhysicsTools/Utilities/interface/Functions.h"
#include "PhysicsTools/Utilities/interface/Variables.h"
#include "PhysicsTools/Utilities/interface/Numerical.h"
#include "PhysicsTools/Utilities/interface/Fraction.h"

class testFunctions : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(testFunctions);
  CPPUNIT_TEST(checkAll);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp() {}
  void tearDown() {}
  void checkAll(); 
};

CPPUNIT_TEST_SUITE_REGISTRATION(testFunctions);

void testFunctions::checkAll() {
  using namespace funct;
  {
    Gaussian g1(0, 1);
    Gaussian g2(1, 1);
    Identity i;
    const double epsilon = 1.e-6;
    Sum<Gaussian, Gaussian>::type g1plus2 = g1 + g2;
    Product<Gaussian, Gaussian>::type g1times2 = g1 * g2; 
    Difference<Gaussian, Gaussian>::type g1minus2 = g1 - g2;
    Ratio<Gaussian, Gaussian>::type g1over2 = g1 / g2; 
    Minus<Gaussian>::type gm1 = - g1;
    Composition<Identity, Gaussian>::type gg1 = compose(i, g1);
    double x = 0.5;
    CPPUNIT_ASSERT(fabs(g1plus2(x) - (g1(x) + g2(x))) < epsilon);
    CPPUNIT_ASSERT(fabs(g1times2(x) - (g1(x) * g2(x))) < epsilon);
    CPPUNIT_ASSERT(fabs(g1minus2(x) - (g1(x) - g2(x))) < epsilon);
    CPPUNIT_ASSERT(fabs(g1over2(x) - (g1(x) / g2(x))) < epsilon);
    CPPUNIT_ASSERT(fabs(gm1(x) - (-g1(x)) < epsilon));
    Convolution<Gaussian, Gaussian>::type gg(g1, g1, -5, 5, 1000);
    CPPUNIT_ASSERT(fabs(gg(0) - g1(0)/sqrt(2.0))<epsilon);
    CPPUNIT_ASSERT(gg1(0) == g1(0));
  }
  {
    double value = 0.123;
    double epsilon = 1.e-8;
    X x;
    x = value;

    Exp<X>::type f_exp = exp(x);
    CPPUNIT_ASSERT(fabs(f_exp() - exp(value)) < epsilon);
    Sin<X>::type f_sin = sin(x);
    CPPUNIT_ASSERT(fabs(f_sin() - sin(value)) < epsilon);
    Cos<X>::type f_cos = cos(x);
    CPPUNIT_ASSERT(fabs(f_cos() - cos(value)) < epsilon);
    Log<X>::type f_log = log(x);
    CPPUNIT_ASSERT(fabs(f_log() - log(value)) < epsilon);
  }
  {
    Numerical<1> _1;
    CPPUNIT_ASSERT(_1 == 1);
    Numerical<2> _2;
    CPPUNIT_ASSERT(_2 == 2);
    Numerical<3> _3;
    CPPUNIT_ASSERT(_3 == 3);
    CPPUNIT_ASSERT(num<1>() == 1);
    CPPUNIT_ASSERT(num<2>() == 2);
    CPPUNIT_ASSERT(num<3>() == 3);
  }
  {
    Fraction<1,2>::type _1_2;
    CPPUNIT_ASSERT(_1_2 == 0.5);
  }
}
