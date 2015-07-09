#include <gtest/gtest.h>
#include "bson/model.hpp"

TEST(TypeTest,Mapping)
{
    bson::double_type b1;
    ASSERT_EQ(bson::type(b1), 0x01);

    bson::string_type b2;
    ASSERT_EQ(bson::type(b2), 0x02);

    bson::document_type b3;
    ASSERT_EQ(bson::type(b3), 0x03);

    bson::array_type b4;
    ASSERT_EQ(bson::type(b4), 0x04);

    bson::boolean_type b8;
    ASSERT_EQ(bson::type(b8), 0x08);

    bson::date_type b9;
    ASSERT_EQ(bson::type(b9), 0x09);

    bson::null_type b10;
    ASSERT_EQ(bson::type(b10), 0x0A);

    bson::int32_type b16;
    ASSERT_EQ(bson::type(b16), 0x10);

    bson::int64_type b18;
    ASSERT_EQ(bson::type(b18), 0x12);
}
