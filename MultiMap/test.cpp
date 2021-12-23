#include <gtest/gtest.h>
#include "multimap.h"

TEST(MultiMapTest, SetSizeTest) {
    LinkedSet TestSet;
    EXPECT_EQ(0, TestSet.num_of_mem);
    for (int i = 0; i < 10000; i++)
        TestSet.linked_set_put(to_string(i));
    EXPECT_EQ(10000, TestSet.num_of_mem);
    TestSet.linked_set_deleteall();
    EXPECT_EQ(0, TestSet.num_of_mem);
}

TEST(MultiMapTest, SetDeletAndPutTest) {
    LinkedSet TestSet;
    TestSet.linked_set_put("1");
    TestSet.linked_set_put("2");
    TestSet.linked_set_put("3");
    EXPECT_EQ("none", TestSet.linked_set_prev("1"));
    EXPECT_EQ("2", TestSet.linked_set_next("1"));
    EXPECT_EQ("1", TestSet.linked_set_prev("2"));
    EXPECT_EQ("3", TestSet.linked_set_next("2"));
    EXPECT_EQ("none", TestSet.linked_set_next("3"));
    EXPECT_EQ("2", TestSet.linked_set_prev("3"));
    TestSet.linked_set_delete("1");
    TestSet.linked_set_delete("2");
    TestSet.linked_set_delete("3");
    EXPECT_EQ("none", TestSet.linked_set_prev("1"));
    EXPECT_EQ("none", TestSet.linked_set_next("1"));
    EXPECT_EQ("none", TestSet.linked_set_prev("2"));
    EXPECT_EQ("none", TestSet.linked_set_next("2"));
    EXPECT_EQ("none", TestSet.linked_set_next("3"));
    EXPECT_EQ("none", TestSet.linked_set_prev("3"));
    TestSet.linked_set_put("1");
    TestSet.linked_set_put("2");
    TestSet.linked_set_put("3");
    EXPECT_EQ("none", TestSet.linked_set_prev("1"));
    EXPECT_EQ("2", TestSet.linked_set_next("1"));
    EXPECT_EQ("1", TestSet.linked_set_prev("2"));
    EXPECT_EQ("3", TestSet.linked_set_next("2"));
    EXPECT_EQ("none", TestSet.linked_set_next("3"));
    EXPECT_EQ("2", TestSet.linked_set_prev("3"));
}


TEST(MultiMapTest, MapSizeTest) {
    MultiMap TestMap;
    EXPECT_EQ(0, TestMap.num_of_members);
    for (int i = 0; i < 10000; i++)
        TestMap.multi_map_put("I'm just an unnecessary string to anyone", to_string(i));
    EXPECT_EQ(10000, TestMap.num_of_members);
    for (int i = 0; i < 10000; i++)
        TestMap.multi_map_put(to_string(i), "I'm just an unnecessary string to anyone too");
    EXPECT_EQ(20000, TestMap.num_of_members);
    TestMap.multi_map_deleteall("I'm just an unnecessary string to anyone");
    EXPECT_EQ(10000, TestMap.num_of_members);
    for (int i = 0; i < 10000; i++)
        TestMap.multi_map_deleteall(to_string(i));
    EXPECT_EQ(0, TestMap.num_of_members);
}

TEST(MultiMapTest, MapDeletAndPutTest) {
    MultiMap TestMap;
    EXPECT_EQ("0", TestMap.multi_map_get("0"));
    for (int i = 1; i <= 5; i++)
        for (int j = 1; j <= i; j++)
            TestMap.multi_map_put(to_string(i), to_string(j));
    EXPECT_EQ("0", TestMap.multi_map_get("0"));
    EXPECT_EQ("1 1", TestMap.multi_map_get("1"));
    EXPECT_EQ("2 2 1", TestMap.multi_map_get("2"));
    EXPECT_EQ("3 3 2 1", TestMap.multi_map_get("3"));
    EXPECT_EQ("4 4 3 2 1", TestMap.multi_map_get("4"));
    EXPECT_EQ("5 5 4 3 2 1", TestMap.multi_map_get("5"));
    for (int i = 1; i <= 5; i++)
        TestMap.multi_map_deleteall(to_string(i));
    EXPECT_EQ("0", TestMap.multi_map_get("0"));
    EXPECT_EQ("0", TestMap.multi_map_get("1"));
    EXPECT_EQ("0", TestMap.multi_map_get("2"));
    EXPECT_EQ("0", TestMap.multi_map_get("3"));
    EXPECT_EQ("0", TestMap.multi_map_get("4"));
    EXPECT_EQ("0", TestMap.multi_map_get("5"));
    for (int i = 1; i <= 5; i++)
        for (int j = 1; j <= i; j++)
            TestMap.multi_map_put(to_string(i), to_string(j));
    EXPECT_EQ("0", TestMap.multi_map_get("0"));
    EXPECT_EQ("1 1", TestMap.multi_map_get("1"));
    EXPECT_EQ("2 2 1", TestMap.multi_map_get("2"));
    EXPECT_EQ("3 3 2 1", TestMap.multi_map_get("3"));
    EXPECT_EQ("4 4 3 2 1", TestMap.multi_map_get("4"));
    EXPECT_EQ("5 5 4 3 2 1", TestMap.multi_map_get("5"));
    TestMap.multi_map_delete("1", "1");
    TestMap.multi_map_delete("2", "1");
    TestMap.multi_map_delete("3", "2");
    TestMap.multi_map_delete("4", "3");
    TestMap.multi_map_delete("5", "4");
    EXPECT_EQ("0", TestMap.multi_map_get("0"));
    EXPECT_EQ("0", TestMap.multi_map_get("1"));
    EXPECT_EQ("1 2", TestMap.multi_map_get("2"));
    EXPECT_EQ("2 3 1", TestMap.multi_map_get("3"));
    EXPECT_EQ("3 4 2 1", TestMap.multi_map_get("4"));
    EXPECT_EQ("4 5 3 2 1", TestMap.multi_map_get("5"));
}

int main() {
	testing::InitGoogleTest();
	return RUN_ALL_TESTS();
}