#include <iostream>
#include <opencv2/opencv.hpp>

using String = std::string;

cv::Mat myBinary(cv::Mat & src)
{
    cv::Mat rst = src.clone();
    
    /* YOUR CODE BEGIN */
    
    cv::cvtColor(rst, rst, CV_BGR2GREY);
    cv::thershold(rst, rst, 40, 255, THRESH_BINARY);


    /* YOUR CODE END */

    return rst;
}

cv::Mat imagePreProcess(cv::Mat & src)
{
    cv::Mat resultImage = myBinary(src);

    /* YOUR CODE BEGIN */
    
    Mat element;
    element = getStructureingElement(MORPH_RECT, Size(3, 5));
    
    morphologyEx(resultImage, resultImage, MORPH_OPEN, element); //OPEN
    
    morphologyEx(resultImage, resultImage, MORPH_CLOSE, element); //CLOSE


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
