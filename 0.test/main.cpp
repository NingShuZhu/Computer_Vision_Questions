// OpenCV header file
#include <opencv2/opencv.hpp>

// C++ standard library header file
#include <string>

int main()
{
    // set file path
    std::string imageName = "lena.jpg";

    // read image from file
    cv::Mat image = cv::imread(imageName);
    
    // show the image
    cv::imshow("result", image);

    // wait for the user to press a key
    cv::waitKey(0);

    return 0;
}
