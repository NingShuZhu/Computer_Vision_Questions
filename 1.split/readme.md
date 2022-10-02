# 三通道分离

## 题目背景


本代码可以直接尝试运行,尝试在终端运行如下代码:

```
cd Computer_Vision_Questions
cd 1.split 

cmake .
make 
./run

```


# 题目描述

彩色图片由三原色构成。一张普通的jpg或者是png图片由opencv中的 cv::imread 函数读取到程序中，每一个像素被记录为(a,b,c). abc都是[0,255]的整数，也就是8位二进制数。

灰度图，每一个像素只会用一个[0,255]的整数表示,所以整张图只会由黑，白和灰色构成。
# 输入格式

在填空部分输入正确的函数和输入变量。

# 输出格式

如图

![fig](https://github.com/UoN-Lancet/Computer_Vision_Questions/blob/master/1.split/sample.jpg) 
# 提示

上网搜索，研究哪个函数把记录彩色图的 ``cv::Mat`` 变量的数据记录到向量中。
