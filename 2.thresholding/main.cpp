#include <iostream>
#include <opencv2/opencv.hpp>

using String = std::string;

cv::Mat solve(const cv::Mat& sourceImage)
{
   

    cv::Mat result = sourceImage.clone();
    
    /* YOUR CODE BEGIN */

    cv::cvtColor(result, result, cv::COLOR_BGR2GRAY); //convert RGB to GREY
    cv::threshold(result, result, 40, 255, cv::THRESH_BINARY); //thresh

    /* YOUR CODE END */
    
    
    return result;

  
}

int main()
{
    // set file path
    String imageName = "lena.jpg";

    // read image from file
    cv::Mat image = cv::imread(imageName);
    
    // show the original image
    cv::imshow("original", image);
   
    // call the solve function
    cv::Mat result = solve(image);

    // show the result image
    cv::imshow("result", result);
   
    // wait for the user to press a key
    cv::waitKey(0);
   
    return 0;
}
