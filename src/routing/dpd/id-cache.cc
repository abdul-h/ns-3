/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 IITP RAS
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
 * Based on
 *      NS-2 AODV model developed by the CMU/MONARCH group and optimized and
 *      tuned by Samir Das and Mahesh Marina, University of Cincinnati;
 *
 *      AODV-UU implementation by Erik Nordström of Uppsala University
 *      http://core.it.uu.se/core/index.php/AODV-UU
 *
 * Authors: Elena Borovkova <borovkovaes@iitp.ru>
 *          Pavel Boyko <boyko@iitp.ru>
 */
#include "id-cache.h"
#include "ns3/test.h"
#include <algorithm>

namespace ns3
{
namespace dpd
{
bool
IdCache::IsDuplicated (Ipv4Address addr, uint32_t id)
{
  Purge ();
  for (std::vector<UniqueId>::const_iterator i = m_idCache.begin ();
      i != m_idCache.end (); ++i)
    if (i->m_context == addr && i->m_id == id)
      return true;
  struct UniqueId uniqueId =
   { addr, id, m_lifetime + Simulator::Now () };
   m_idCache.push_back (uniqueId);
  return false;
}
void
IdCache::Purge ()
{
  m_idCache.erase (remove_if (m_idCache.begin (), m_idCache.end (),
      IsExpired ()), m_idCache.end ());
}

uint32_t
IdCache::GetSize ()
{
  Purge ();
  return m_idCache.size ();
}

//-----------------------------------------------------------------------------
// Tests
//-----------------------------------------------------------------------------
/// Unit test for id cache
struct IdCacheTest : public TestCase
{
  IdCacheTest () : TestCase ("Id Cache"), cache (Seconds(10))
  {}
  virtual bool DoRun();
  void CheckTimeout1 ();
  void CheckTimeout2 ();
  void CheckTimeout3 ();

  IdCache cache;
};

bool
IdCacheTest::DoRun ()
{
  NS_TEST_EXPECT_MSG_EQ (cache.GetLifeTime(), Seconds(10), "Lifetime");
  NS_TEST_EXPECT_MSG_EQ (cache.IsDuplicated (Ipv4Address ("1.2.3.4"), 3), false, "Unknown ID & address");
  NS_TEST_EXPECT_MSG_EQ (cache.IsDuplicated (Ipv4Address ("1.2.3.4"), 4), false, "Unknown ID");
  NS_TEST_EXPECT_MSG_EQ (cache.IsDuplicated (Ipv4Address ("4.3.2.1"), 3), false, "Unknown address");
  NS_TEST_EXPECT_MSG_EQ (cache.IsDuplicated (Ipv4Address ("1.2.3.4"), 3), true, "Known address & ID");
  cache.SetLifetime(Seconds(15));
  NS_TEST_EXPECT_MSG_EQ (cache.GetLifeTime(), Seconds(15), "New lifetime");
  cache.IsDuplicated (Ipv4Address ("1.1.1.1"), 4);
  cache.IsDuplicated (Ipv4Address ("1.1.1.1"), 4);
  cache.IsDuplicated (Ipv4Address ("2.2.2.2"), 5);
  cache.IsDuplicated (Ipv4Address ("3.3.3.3"), 6);
  NS_TEST_EXPECT_MSG_EQ (cache.GetSize (), 6, "trivial");

  Simulator::Schedule (Seconds(5), &IdCacheTest::CheckTimeout1, this);
  Simulator::Schedule (Seconds(11), &IdCacheTest::CheckTimeout2, this);
  Simulator::Schedule (Seconds(30), &IdCacheTest::CheckTimeout3, this);
  Simulator::Run ();
  Simulator::Destroy ();
  
  return GetErrorStatus ();
}

void
IdCacheTest::CheckTimeout1 ()
{
  NS_TEST_EXPECT_MSG_EQ (cache.GetSize (), 6, "Nothing expire");
}

void
IdCacheTest::CheckTimeout2 ()
{
  NS_TEST_EXPECT_MSG_EQ (cache.GetSize (), 3, "3 records left");
}

void
IdCacheTest::CheckTimeout3 ()
{
  NS_TEST_EXPECT_MSG_EQ (cache.GetSize (), 0, "All records expire");
}
//-----------------------------------------------------------------------------
class IdCacheTestSuite : public TestSuite
{
public:
  IdCacheTestSuite () : TestSuite ("routing-id-cache", UNIT)
  {
    AddTestCase (new IdCacheTest);
  }
} g_idCacheTestSuite;

}}
