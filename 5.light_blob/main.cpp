#include <iostream>
#include <opencv2/opencv.hpp>

class LightBlob
{
  public:

    cv::RotatedRect rect;   //灯条位置
    double area_ratio{};
    double length{};          //灯条长度
    uint8_t blob_color{};      //灯条颜色

    LightBlob(cv::RotatedRect & r, double ratio, uint8_t color) : rect(r), area_ratio(ratio), blob_color(color)
    {
        length = std::max(rect.size.height, rect.size.width);
    };

    LightBlob() = default;
};
typedef std::vector<LightBlob> LightBlobs;
std::vector<LightBlob> light_blobs;
void imagePreProcess(cv::Mat & src)
{
    static cv::Mat kernel_erode = getStructuringElement(cv::MORPH_RECT, cv::Size(3, 5));
    erode(src, src, kernel_erode);

    static cv::Mat kernel_dilate = getStructuringElement(cv::MORPH_RECT, cv::Size(3, 5));
    dilate(src, src, kernel_dilate);

    static cv::Mat kernel_dilate2 = getStructuringElement(cv::MORPH_RECT, cv::Size(3, 5));
    dilate(src, src, kernel_dilate2);

    static cv::Mat kernel_erode2 = getStructuringElement(cv::MORPH_RECT, cv::Size(3, 5));
    erode(src, src, kernel_erode2);
}

static double lw_rate(const cv::RotatedRect & rect)
{
    return rect.size.height > rect.size.width ?
           rect.size.height / rect.size.width :
           rect.size.width / rect.size.height;
}

// 轮廓面积和其最小外接矩形面积之比
static double areaRatio(const std::vector<cv::Point> & contour, const cv::RotatedRect & rect)
{
    return cv::contourArea(contour) / rect.size.area();
}

static bool isValidLightBlob(const std::vector<cv::Point> & contour, const cv::RotatedRect & rect)
{
    return (1.2 < lw_rate(rect) && lw_rate(rect) < 10) &&
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
void DrawRotatedRect(cv::Mat mask,const cv::RotatedRect &rotatedrect,const cv::Scalar &color,int thickness, int lineType)
{
    // 提取旋转矩形的四个角点
	cv::Point2f ps[4];
	rotatedrect.points(ps);

    // 构建轮廓线
	std::vector<std::vector<cv::Point>> tmpContours;    // 创建一个InputArrayOfArrays 类型的点集
	std::vector<cv::Point> contours;
	for (int i = 0; i != 4; ++i) {
		contours.emplace_back(cv::Point2i(ps[i]));
	}
	tmpContours.insert(tmpContours.end(), contours);

    // 绘制轮廓，即旋转矩形
	drawContours(mask, tmpContours, 0, color,thickness, lineType);  // 填充mask
}


int main(){
   
    
    std::string image_name = "5_red_1_50cm46.png";
    cv::Mat src    = cv::imread(image_name.c_str());
    
    cv::Mat color_channel;
    cv::Mat src_bin_light, src_bin_dim;
    std::vector<cv::Mat> channels;       // 通道拆分
    
    /* YOUR CODE BEGIN */
    
    cv::split(src, channels);    // 通道拆分，由channels存储

    /* YOUR CODE END */
    
    color_channel = channels[2] - channels[0] / 2 - channels[1] / 2;
    
    /* YOUR CODE BEGIN */

    cv::threshold(color_channel, src_bin_light, 70, 255, cv::THRESH_BINARY);    // 二值化对应通道，阈值70，srcBinLight存储结果
    cv::threshold(color_channel, src_bin_dim, 40, 255, cv::THRESH_BINARY);    // 二值化对应通道，阈值40，srcBinDim存储结果

    /* YOUR CODE END */
     
    /* YOUR CODE BEGIN */

    imagePreProcess(src_bin_light);//找到本文件中设置好的开闭运算函数，对srcBinLight，srcBinDim变量进行开闭运算。
    imagePreProcess(src_bin_dim);

    /* YOUR CODE END */
    
    
    
    std::vector<std::vector<cv::Point>> light_contours_light, light_contours_dim;
    LightBlobs light_blobs_light, light_blobs_dim;   
    std::vector<cv::Vec4i> hierarchy_light, hierarchy_dim;        
     /* YOUR CODE BEGIN */

    cv::findContours(src_bin_light, light_contours_light, hierarchy_light, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);//寻找轮廓，存储在118行和119行定义的变量中。
    cv::findContours(src_bin_dim, light_contours_dim, hierarchy_dim, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);//寻找轮廓，存储在118行和119行定义的变量中。

    /* YOUR CODE END */
    
    for (int i = 0; i < light_contours_light.size(); i++) {
        if (hierarchy_light[i][2] == -1) {
            cv::RotatedRect rect = cv::minAreaRect(light_contours_light[i]);
            if (isValidLightBlob(light_contours_light[i], rect)) {
                light_blobs_light.emplace_back(
                    rect, areaRatio(light_contours_light[i], rect), 0
                );
            }
        }
    }

    std::vector<int> light_to_remove, dim_to_remove;
    for (int l = 0; l != light_blobs_light.size(); l++) {
        for (int d = 0; d != light_blobs_dim.size(); d++) {
            if (isSameBlob(light_blobs_light[l], light_blobs_dim[d])) {
                if (light_blobs_light[l].area_ratio > light_blobs_dim[d].area_ratio) {
                    dim_to_remove.emplace_back(d);
                } else {
                    light_to_remove.emplace_back(l);
                }
            }
        }
    }
   
    sort(light_to_remove.begin(), light_to_remove.end(), [](int a, int b) { return a > b; });
    sort(dim_to_remove.begin(), dim_to_remove.end(), [](int a, int b) { return a > b; });

    for (auto x: light_to_remove) {
        light_blobs_light.erase(light_blobs_light.begin() + x);
    }

    for (auto x: dim_to_remove) {
        light_blobs_dim.erase(light_blobs_dim.begin() + x);
    }

    for (const auto & light: light_blobs_light) {
        light_blobs.emplace_back(light);
    }

    for (const auto & dim: light_blobs_dim) {
        light_blobs.emplace_back(dim);
    }

    for(auto i: light_blobs){
    	DrawRotatedRect(src,i.rect,cv::Scalar(0,0,255),2, 0);
    }
   
    cv::imshow("1",src); 
    cv::waitKey(0);
    
    return 0;

}


