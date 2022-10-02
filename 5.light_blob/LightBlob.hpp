
#ifndef LIGHTBLOB_HPP
#define LIGHTBLOB_HPP

#include <opencv2/opencv.hpp>

struct LightBlob
{
    // light blob color enum
    enum class Color { RED, BLUE };

    // use cv::RotatedRect to store the position of the light blob
    cv::RotatedRect rect;

    double areaRatio {};
    double length {};

    LightBlob::Color color {};

    LightBlob() = default;

    explicit LightBlob(cv::RotatedRect & rect, double ratio, Color color)
        : rect(rect), areaRatio(ratio), color(color)
    {
        length = std::max(rect.size.height, rect.size.width);
    };
};

#endif // LIGHTBLOB_HPP
