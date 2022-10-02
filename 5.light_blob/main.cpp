#include "LightBlob.hpp"

#include <string>
#include <vector>

void imagePreProcess(cv::Mat & src)
{
    static cv::Mat kernelErode = getStructuringElement(cv::MORPH_RECT, cv::Size(3, 5));
    erode(src, src, kernelErode);

    static cv::Mat kernelDilate = getStructuringElement(cv::MORPH_RECT, cv::Size(3, 5));
    dilate(src, src, kernelDilate);

    static cv::Mat kernelDilate2 = getStructuringElement(cv::MORPH_RECT, cv::Size(3, 5));
    dilate(src, src, kernelDilate2);

    static cv::Mat kernelErode2 = getStructuringElement(cv::MORPH_RECT, cv::Size(3, 5));
    erode(src, src, kernelErode2);
}

static double lwRate(const cv::RotatedRect & rect)
{
    return rect.size.height > rect.size.width ?
           rect.size.height / rect.size.width :
           rect.size.width / rect.size.height;
}

// 轮廓面积和其最小外接矩形面积之比
static double areaRatio(const Vec<cv::Point> & contour, const cv::RotatedRect & rect)
{
    return cv::contourArea(contour) / rect.size.area();
}

static bool isValidLightBlob(const Vec<cv::Point> & contour, const cv::RotatedRect & rect)
{
    return (1.2 < lwRate(rect) && lwRate(rect) < 10) &&
           //           (rect.isPresent.area() < 3000) &&
           ((rect.size.area() < 50 && areaRatio(contour, rect) > 0.4) ||
            (rect.size.area() >= 50 && areaRatio(contour, rect) > 0.6));
}

static bool isSameBlob(const LightBlob & blob1, const LightBlob & blob2)
{
    auto dist = blob1.rect.center - blob2.rect.center;
    return (dist.x * dist.x + dist.y * dist.y) < 9;
}

// 绘制旋转矩形
void drawRotatedRect(cv::Mat mask, const cv::RotatedRect & rotatedRect,
    const cv::Scalar & color, int thickness, int lineType)
{
    // 提取旋转矩形的四个角点
    cv::Point2f points[4];
    rotatedRect.points(points);

    // 构建轮廓线
    Vec<Vec<cv::Point>> tmpContours;    // 创建一个InputArrayOfArrays 类型的点集
    Vec<cv::Point> contours;

    for (auto & point : points) {
        contours.emplace_back(cv::Point2i(point));
    }

    tmpContours.insert(tmpContours.end(), contours);

    // 绘制轮廓，即旋转矩形
    drawContours(mask, tmpContours, 0, color, thickness, lineType);  // 填充mask
}


int main()
{
    std::string imageName = "5_red_1_50cm46.png";
    cv::Mat src = cv::imread(imageName);

    Vec<LightBlob> lightBlobs;

    cv::Mat colorChannel;
    cv::Mat srcBinLight, srcBinDim;
    Vec<cv::Mat> channels;       

    /* YOUR CODE BEGIN */
        
        // 通道拆分，由channels存储

    /* YOUR CODE END */
    
    colorChannel = channels[2] - channels[0] / 2 - channels[1] / 2;


    /* YOUR CODE BEGIN */

        // 二值化对应通道，阈值70，srcBinLight存储结果
        // 二值化对应通道，阈值40，srcBinDim存储结果

    /* YOUR CODE END */
     
    /* YOUR CODE BEGIN */

    //找到本文件中设置好的开闭运算函数，对srcBinLight，srcBinDim变量进行开闭运算。
    
    /* YOUR CODE END */

    Vec<Vec<cv::Point>> lightContoursLight, lightContoursDim;
    Vec<LightBlob> lightBlobsLight, lightBlobsDim;
    Vec<cv::Vec4i> hierarchy_light, hierarchy_dim;
    
    /* YOUR CODE BEGIN */

    //寻找轮廓，存储在93行和94行定义的变量中。
  
    /* YOUR CODE END */

    
    for (int i = 0; i < lightContoursLight.size(); i++) {
        if (hierarchy_light[i][2] == -1) {
            cv::RotatedRect rect = cv::minAreaRect(lightContoursLight[i]);
            if (isValidLightBlob(lightContoursLight[i], rect)) {
                lightBlobsLight.emplace_back(
                    LightBlob(rect, areaRatio(lightContoursLight[i], rect), LightBlob::Color::BLUE)
                );
            }
        }
    }

    Vec<int> lightToRemove, dimToRemove;
    for (int l = 0; l != lightBlobsLight.size(); l++) {
        for (int d = 0; d != lightBlobsDim.size(); d++) {
            if (isSameBlob(lightBlobsLight[l], lightBlobsDim[d])) {
                if (lightBlobsLight[l].areaRatio > lightBlobsDim[d].areaRatio) {
                    dimToRemove.emplace_back(d);
                } else {
                    lightToRemove.emplace_back(l);
                }
            }
        }
    }

    sort(lightToRemove.begin(), lightToRemove.end(), [](int a, int b) { return a > b; });
    sort(dimToRemove.begin(), dimToRemove.end(), [](int a, int b) { return a > b; });

    for (auto x: lightToRemove) {
        lightBlobsLight.erase(lightBlobsLight.begin() + x);
    }

    for (auto x: dimToRemove) {
        lightBlobsDim.erase(lightBlobsDim.begin() + x);
    }

    for (const auto & light: lightBlobsLight) {
        lightBlobs.emplace_back(light);
    }

    for (const auto & dim: lightBlobsDim) {
        lightBlobs.emplace_back(dim);
    }

    for (auto lightBlob: lightBlobs) {
        drawRotatedRect(src, lightBlob.rect, cv::Scalar(0, 0, 255), 2, 0);
    }

    cv::imshow("image", src);
    cv::waitKey(0);

    return 0;

}