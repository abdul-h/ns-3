/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006 INRIA
 * Copyright (c) 2007 Emmanuelle Laprise
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 * TimeStep support by Emmanuelle Laprise <emmanuelle.laprise@bluekazoo.ca>
 */
#include "ns3/nstime.h"
#include "ns3/test.h"

namespace ns3 {

class Bug863TestCase : public TestCase
{
public:
  Bug863TestCase ();
  virtual void DoRun (void);
};

Bug863TestCase::Bug863TestCase ()
  : TestCase ("Bug 863")
{
}

void Bug863TestCase::DoRun (void)
{
  Scalar result = Scalar (0.9) / Scalar (1.0);
  NS_TEST_ASSERT_MSG_EQ ((result == Scalar (0.9)), true, "Invalid arithmetic result");
}

class TimeSimpleTestCase : public TestCase
{
public:
  TimeSimpleTestCase (enum Time::Unit resolution);
private:
  virtual void DoRun (void);
  virtual void DoTearDown (void);
  enum Time::Unit m_originalResolution;
  enum Time::Unit m_resolution;
};

TimeSimpleTestCase::TimeSimpleTestCase (enum Time::Unit resolution)
  : TestCase ("Sanity check of common time operations"),
    m_resolution (resolution)
{}
void
TimeSimpleTestCase::DoRun (void)
{
  m_originalResolution = Time::GetResolution ();
  Time::SetResolution (m_resolution);
  NS_TEST_ASSERT_MSG_EQ_TOL (Seconds (1.0).GetSeconds (), 1.0, TimeStep (1).GetSeconds (), 
                             "is 1 really 1 ?");
  NS_TEST_ASSERT_MSG_EQ_TOL (Seconds (10.0).GetSeconds (), 10.0, TimeStep (1).GetSeconds (), 
                             "is 10 really 10 ?");
  NS_TEST_ASSERT_MSG_EQ (MilliSeconds (1).GetMilliSeconds (), 1, 
                         "is 1ms really 1ms ?");
  NS_TEST_ASSERT_MSG_EQ (MicroSeconds (1).GetMicroSeconds (), 1, 
                         "is 1us really 1us ?");
#if 0
  Time ns = NanoSeconds (1);
  ns.GetNanoSeconds ();
  NS_TEST_ASSERT_MSG_EQ (NanoSeconds (1).GetNanoSeconds (), 1, 
                         "is 1ns really 1ns ?");
  NS_TEST_ASSERT_MSG_EQ (PicoSeconds (1).GetPicoSeconds (), 1, 
                         "is 1ps really 1ps ?");
  NS_TEST_ASSERT_MSG_EQ (FemtoSeconds (1).GetFemtoSeconds (), 1, 
                         "is 1fs really 1fs ?");
#endif
}

void 
TimeSimpleTestCase::DoTearDown (void)
{
  Time::SetResolution (m_originalResolution);
}

class ArithTestCase : public TestCase
{
public:
  ArithTestCase ();
private:
  virtual void DoRun (void);
};

ArithTestCase::ArithTestCase ()
  : TestCase ("check arithmetic operators")
{
}
void
ArithTestCase::DoRun (void)
{
  Time a, b, c;
  c = a + b;
  c = a * b;
  c = a / Seconds (1.0);
  c = a - b;
  c += a;
  c -= a;
  c /= Seconds (1.0);
  c *= a;
  bool x;
  x = a < b;
  x = a > b;
  x = a <= b;
  x = a >= b;
  x = a == b;
  x = a != b;
  //a = 1.0;
  //a = 1;
}



static class TimeTestSuite : public TestSuite
{
public:
  TimeTestSuite ()
    : TestSuite ("time", UNIT)
  {
    AddTestCase (new Bug863TestCase ());
    AddTestCase (new TimeSimpleTestCase (Time::US));
    AddTestCase (new ArithTestCase ());
  }
} g_timeTestSuite;

} // namespace ns3
