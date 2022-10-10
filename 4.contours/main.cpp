#include <iostream>
#include <opencv2/opencv.hpp>

using String = std::string;

cv::Mat myBinary(cv::Mat& src)
{
    cv::Mat resultImage = src.clone();

    /* YOUR CODE BEGIN */

    cv::cvtColor(resultImage, resultImage, cv::COLOR_BGR2GRAY);
    cv::threshold(resultImage, resultImage, 40, 255, cv::THRESH_BINARY);

    /* YOUR CODE END */

    return resultImage;
}

cv::Mat imagePreProcess(cv::Mat& src)
{
    cv::Mat resultImage = myBinary(src);

    /* YOUR CODE BEGIN */

    cv::Mat element;
    element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 5));

    cv::morphologyEx(resultImage, resultImage, cv::MORPH_OPEN, element);
    cv::morphologyEx(resultImage, resultImage, cv::MORPH_CLOSE, element);

    /* YOUR CODE END */

    return resultImage;
}

std::vector<std::vector<cv::Point>> myFindContour(cv::Mat& src)
{
    cv::Mat rst = imagePreProcess(src);

    std::vector<cv::Vec4i> hierarchy;
    std::vector<std::vector<cv::Point>> contours;

    /* YOUR CODE BEGIN */
    
    cv::findContours(rst, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

    /* YOUR CODE END */

    return contours;
}

int main()
{
    // set file path
    String imageName = "shape.jpg";

    // read image from file
    cv::Mat image = cv::imread(imageName);

    // show the original image
    cv::imshow("original", image);

    cv::drawContours(image, myFindContour(image), -1, cv::Scalar(0, 255, 0), 2);

    // show the result image
    cv::imshow("result", image);
    
    // wait for the user to press a key
    cv::waitKey(0);

    return 0;
}


