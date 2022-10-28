#include <time.h> //for time info
#include <iostream> // for standard I/O
#include <opencv2/core.hpp>     // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2/highgui.hpp>  // OpenCV window I/O, works with capturing from a cam
#include <opencv2/imgproc.hpp>  // Works with images, e.g. Gaussian Blur.

using namespace std;
using namespace cv;

int main(int argc, char **argv) {
    int frameCounter = 0;
    double totalRead = 0;
    double totalProcess = 0;
    double totalWrite = 0;
    int tick = 0; //counts how many milliseconds last from the last call
    int totalFPS = 0;

    time_t time_begin = time(nullptr);
    clock_t time_begin_clock = clock(); 
    clock_t tmpReadTime, tmpProcessTime, tmpWriteTime;

    VideoCapture cap(0); //open the default camera using default API
    if (!cap.isOpened()) {
        cout << "Can not open webcam" << endl;
        return 0;
    }

    Mat input; //itit video capture
    while (true) {
        // reading
        tmpReadTime = clock();

        // wait for a new frame from camera and store it into 'input'
        cap.read(input);
        if (input.empty()) {
            cout << "ERROR! blank frame grabbed" << endl;
            break;
        }
        totalRead += clock() - tmpReadTime;

        // proccessing
        tmpProcessTime = clock();
        imshow("No Filters Image", input); //display an image in window

        char key = waitKey(1);
        if (key == 27) {
            break;
        }
        Mat outFlippered,outErode, outBorder;

        //flip the capture
        flip(input, outFlippered, 1); // 1 means flip in Oy

        /*  About kernel
            OpenCV blurs an image by applying what's called a Kernel. 
            A Kernel tells you how to change the value of any given pixel 
            by combining it with different amounts of the neighboring pixels. 
            The kernel is applied to every pixel in the image one-by-one to 
            produce the final image (this operation known as a convolution).
            erode(outFlippered, outErode, kernel);
        */
        Mat kernel = getStructuringElement(MORPH_RECT, Size(7, 7));

        /*      
            @param outFlippered - input iamge
            @param outErode - output iamge
            @param kernel - structuring element used for erosion
        */
        erode(outFlippered, outErode, kernel);        

        //make a border
        int top = (int)(0.15 * input.rows); //input.rows =  number of rows in a matrix
        int bottom = top;
        int left = (int)(0.05 * input.cols); //input.cols =  number of columns in a matrix
        int right = left;
        int borderType = BORDER_CONSTANT;
        Scalar color(186, 85, 211); // RGB for purple color     

        /*
            @param outErode - source image
            @param outBorder - destination image
            @params top, bottom, left, right - length in pixels of the borders at each side of the image
            @param BORDER_CONSTANT - type of the border
            @param color - color of the border
        */
        copyMakeBorder(outErode, outBorder, top, bottom, left, right, BORDER_CONSTANT, color);   

        double alpha = 1.1; // argument for contrast control, [1.0-3.0]
        int beta = 5;       // for brightness control, [0-100]

        /*  We perform the operation: g(y, x) = alfa * f(y, x) + beta
            alfa is gain (усиления) parameter to control contrast
            beta is bias (смещения) parameter to control brightness
            f(y, x) - src image, g(y, x) - dst image.

            To access each pixel in the images we are using this syntax: 
            outBorder.at<Vec3b>(y,x)[c] where y is the row, x is the column and c is B, G or R (0, 1 or 2).
            We use cv::saturate_cast to make sure the values are valid
            chanels() - number of matrix chanels
        */

        for (int y = 10; y < input.rows / 1.5; y++) {
            for (int x = input.cols / 1.5 - 30; x > 0 ; x--) {
                for (int c = 0; c < input.channels(); c++) {
                    outBorder.at<Vec3b>(y, x)[c] =
                        saturate_cast<uchar>(alpha * input.at<Vec3b>(y, x)[c] + beta);
                }
            }
        }

        Mat res = outBorder;

        //include a text on the image
        String text = "Sharp Image";
        int fontFace = FONT_HERSHEY_PLAIN;
        int fontScale = 3;
        Scalar colorText = (128, 0, 128);
        int thickness = 3;

        putText(res, text, Point(10, 50), fontFace, fontScale, colorText, thickness);

        totalProcess += clock() - tmpProcessTime;

        // writing
        tmpWriteTime = clock();
        imshow("With Filters", res);
        totalWrite += clock() - tmpWriteTime;

        // FPS counter
        frameCounter++;
        auto time_now = time(nullptr) - time_begin;
        if (time_now - tick >= 1) {
            tick++;
            totalFPS += frameCounter;
            cout << "FPS: " << frameCounter << endl;
            frameCounter = 0;
        }
    }

    clock_t total_time_clock = clock() - time_begin_clock;
    cout << "Average FPS: " << totalFPS * 1.0 / tick << endl;
    cout << "Time for reading: " << totalRead * 100 / total_time_clock << "%" << endl;
    cout << "Time for processing: " << totalProcess * 100 / total_time_clock << "%" << endl;
    cout << "Time for writing: " << totalWrite * 100 / total_time_clock << "%" << endl;
    return 0;
}
