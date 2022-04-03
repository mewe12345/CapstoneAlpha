#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "iostream"

using namespace cv;
using namespace std;


std::string rgb2hex(int r, int g, int b, bool with_head)
{
    std::stringstream ss;
    if (with_head)
        ss << "#";
    ss << std::hex << (r << 16 | g << 8 | b);
    return ss.str();
}

Mat showCenters(const Mat& centers, int siz = 64) {

    Mat cent = centers.reshape(3, centers.rows);
    // make  a horizontal bar of K color patches:
    Mat draw(siz, siz * cent.rows, cent.type(), Scalar::all(0));
    for (int i = 0; i < cent.rows; i++) {
        // set the resp. ROI to that value (just fill it):
        draw(Rect(i * siz, 0, siz, siz)) = cent.at<Vec3f>(i, 0);
    }
    draw.convertTo(draw, CV_8U);

    // optional visualization:
    
    imshow("CENTERS", draw);
    waitKey();

    //imwrite("centers.png", draw);

    return draw;
}

int main()
{
    int x, y;
    int b, g, r;
    Mat src;
    std::string image_path = samples::findFile("D:/Documents/Francis Reyes - Assignments/Capstone Alpha/Resources/mariosprite.jpg");
    src = imread(image_path, IMREAD_COLOR);
    
    


    Mat data;
    src.convertTo(data, CV_32F);
    data = data.reshape(1, data.total());

    // do kmeans
    Mat labels, centers;
    int k = 8;
    kmeans(data, k, labels, TermCriteria(TermCriteria::MAX_ITER, 10, 1.0), 3,
        KMEANS_PP_CENTERS, centers);

    // reshape both to a single row of Vec3f pixels:
    centers = centers.reshape(3, centers.rows);
    data = data.reshape(3, data.rows);

    // replace pixel values with their center value:
    Vec3f* p = data.ptr<Vec3f>();
    for (size_t i = 0; i < data.rows; i++) {
        int center_id = labels.at<int>(i);
        p[i] = centers.at<Vec3f>(center_id);
    }

    // back to 2d, and uchar:
    src = data.reshape(3, src.rows);
    src.convertTo(src, CV_8U);



    //visualize data:
    imshow("Image", src);
    showCenters(centers);


    
  
}
