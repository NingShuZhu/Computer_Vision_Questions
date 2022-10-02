#include <iostream>
#include <opencv2/opencv.hpp>

using String = std::string;

cv::Mat myBinary(cv::Mat & src)
{
    cv::Mat rst = src.clone();
    
    /* YOUR CODE BEGIN */


    /* YOUR CODE END */

    return rst;
}

cv::Mat imagePreProcess(cv::Mat & src)
{
    cv::Mat resultImage = myBinary(src);

    /* YOUR CODE BEGIN */


    /* YOUR CODE END */

    return resultImage;
}


int main()
{
    // set file path
    String imageName = "lena.jpg";

    // read image from file
    cv::Mat image = cv::imread(imageName);

    // show the original image
    cv::imshow("original", image);

    image = imagePreProcess(image);



    // show the result image
    cv::imshow("result", image);
    
    // wait for the user to press a key    
    cv::waitKey(0);

    return 0;
}
