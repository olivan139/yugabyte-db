// Copyright (c) YugabyteDB, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
// in compliance with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed under the License
// is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
// or implied.  See the License for the specific language governing permissions and limitations
// under the License.
//

// #include <gtest/gtest.h>
#include "yb/util/test_util.h"
#include "yb/vector/coordinate_types.h"
#include "yb/vector/hnswlib_wrapper.h"
#include "yb/vector/index_wrapper_base.h"
#include "yb/util/status_log.h"
#include "yb/vector/vector_index_if.h"
#include "yb/vector/vectorann_util.h"
#include "yb/util/logging.h"


namespace yb::vectorindex {

// Test fixture class for Merge operation.
class HnswlibIndexMergeTest : public YBTest {
// class HnswlibIndexMergeTest : public ::testing::Test {

 protected:
  void SetUp() override {
    // HNSW options setup with 3 dimensions and L2 distance.

    // Create HnswlibIndex instances for index_a and index_b.
    index_a = HnswlibIndexFactory<FloatVector, float>::Create(hnsw_options);
    index_b = HnswlibIndexFactory<FloatVector, float>::Create(hnsw_options);
  
    // Reserve space for 10 elements in each index.
    ASSERT_OK(index_a->Reserve(10));
    ASSERT_OK(index_b->Reserve(10));
    
    // Insert vectors into index_a.
    ASSERT_OK(index_a->Insert(1, {0.1f, 0.2f, 0.3f}));
    ASSERT_OK(index_a->Insert(2, {0.4f, 0.5f, 0.6f}));
    
    // Insert vectors into index_b.
    ASSERT_OK(index_b->Insert(3, {0.7f, 0.8f, 0.9f}));
    ASSERT_OK(index_b->Insert(4, {1.0f, 1.1f, 1.2f}));
  }

  HNSWOptions hnsw_options = {
    .dimensions = 3,  
    .max_neighbors_per_vertex = 16,
    .ef_construction = 20,
    .distance_kind = DistanceKind::kL2Squared
  };
  
  VectorIndexIfPtr<FloatVector, float> index_a;
  VectorIndexIfPtr<FloatVector, float> index_b;
};

// Test case to verify the Merge method for HnswlibIndex.
TEST_F(HnswlibIndexMergeTest, TestMergeIndices) {
  // Perform merge operation.
  VectorIndexIfPtr<FloatVector, float> merged_index = Merge <FloatVector, float> (index_a, index_b);

  // Check that the merged index contains all entries.
  auto result_a = merged_index->Search(FloatVector({0.1f, 0.2f, 0.3f}), 1);
  ASSERT_EQ(result_a.size(), 1);
  EXPECT_EQ(result_a[0].vertex_id, 1);

  auto result_b = merged_index->Search({0.7f, 0.8f, 0.9f}, 1);
  ASSERT_EQ(result_b.size(), 1);
  EXPECT_EQ(result_b[0].vertex_id, 3);

  // Verify the size of the merged index.
  auto all_results = merged_index->Search({0.0f, 0.0f, 0.0f}, 10);  // Assuming a query that fetches all.
  EXPECT_EQ(all_results.size(), 4);  // Should contain all 4 entries.
}

// Test case to verify merging an empty index with a non-empty one.
TEST_F(HnswlibIndexMergeTest, TestMergeWithEmptyIndex) {
  // Create an empty index with the same options.    
  VectorIndexIfPtr<FloatVector, float> empty_index = HnswlibIndexFactory<FloatVector, float>::Create(hnsw_options);

  ASSERT_OK(empty_index->Reserve(10));

  // Merge empty_index into index_a.
  VectorIndexIfPtr<FloatVector, float> merged_index = Merge <FloatVector, float> (index_a, empty_index);

  // Check that the merged index contains only the entries from index_a.
  auto all_results = merged_index->Search({0.0f, 0.0f, 0.0f}, 10);  // Query that fetches all.
  EXPECT_EQ(all_results.size(), 2);  // Should contain only the 2 entries from index_a.
}

}  // namespace yb::vectorindex
