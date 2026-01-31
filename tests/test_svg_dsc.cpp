#include <gtest/gtest.h>

#include "lvgl_cpp.h"

TEST(ImageDescriptorTest, SVGSource) {
  const char* svg = "<svg>...</svg>";
  auto dsc = lvgl::ImageDescriptor::from_svg(svg);

  EXPECT_TRUE(dsc.is_valid());
  EXPECT_EQ(dsc.raw()->header.cf, LV_COLOR_FORMAT_RAW);
  EXPECT_STREQ(reinterpret_cast<const char*>(dsc.raw()->data), svg);

  // Test move semantics
  lvgl::ImageDescriptor dsc2 = std::move(dsc);
  EXPECT_TRUE(dsc2.is_valid());
  EXPECT_FALSE(dsc.is_valid());
  EXPECT_STREQ(reinterpret_cast<const char*>(dsc2.raw()->data), svg);
}

TEST(ImageDescriptorTest, SVGStringSource) {
  std::string svg = "<svg>string</svg>";
  auto dsc = lvgl::ImageDescriptor::from_svg(svg);
  EXPECT_TRUE(dsc.is_valid());
  EXPECT_STREQ(reinterpret_cast<const char*>(dsc.raw()->data), svg.c_str());
}
