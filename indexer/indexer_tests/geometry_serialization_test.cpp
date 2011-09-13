#include "../../testing/testing.hpp"

#include "test_polylines.hpp"

#include "../geometry_serialization.hpp"
#include "../coding_params.hpp"
#include "../mercator.hpp"

#include "../../coding/reader.hpp"
#include "../../coding/byte_stream.hpp"

#include "../../base/logging.hpp"


// Copy-Paste from feature_builder.cpp
namespace
{
  bool is_equal(double d1, double d2)
  {
    //return my::AlmostEqual(d1, d2, 100000000);
    return (fabs(d1 - d2) < MercatorBounds::GetCellID2PointAbsEpsilon());
  }

  bool is_equal(m2::PointD const & p1, m2::PointD const & p2)
  {
    return p1.EqualDxDy(p2, MercatorBounds::GetCellID2PointAbsEpsilon());
  }

  bool is_equal(m2::RectD const & r1, m2::RectD const & r2)
  {
    return (is_equal(r1.minX(), r2.minX()) &&
            is_equal(r1.minY(), r2.minY()) &&
            is_equal(r1.maxX(), r2.maxX()) &&
            is_equal(r1.maxY(), r2.maxY()));
  }
}


UNIT_TEST(SaveLoadPolyline_DataSet1)
{
  using namespace index_test;

  vector<m2::PointD> data1(arr1, arr1 + ARRAY_SIZE(arr1));

  vector<char> buffer;
  PushBackByteSink<vector<char> > w(buffer);

  serial::CodingParams cp;
  serial::SaveOuterPath(data1, cp, w);

  vector<m2::PointD> data2;
  ArrayByteSource r(&buffer[0]);
  serial::LoadOuterPath(r, cp, data2);

  TEST_EQUAL(data1.size(), data2.size(), ());

  m2::RectD r1, r2;
  for (size_t i = 0; i < data1.size(); ++i)
  {
    r1.Add(data1[i]);
    r2.Add(data2[i]);

    TEST(is_equal(data1[i], data2[i]), (data1[i], data2[i]));
  }

  LOG(LINFO, (data2));

  TEST(is_equal(r1, r2), (r1, r2));
}
