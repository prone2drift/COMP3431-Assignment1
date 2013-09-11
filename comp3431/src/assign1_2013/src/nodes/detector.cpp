#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cv.h>
namespace enc = sensor_msgs::image_encodings;

static const char WINDOW[] = "Image window";

class ImageConverter
{
  ros::NodeHandle nh_;
  image_transport::ImageTransport it_;
  image_transport::Subscriber image_sub_;
  image_transport::Publisher image_pub_;
  cv::Mat src, proc, out;

public:
  ImageConverter()
    : it_(nh_)
  {
    image_pub_ = it_.advertise("out", 1);
    image_sub_ = it_.subscribe("in", 1, &ImageConverter::imageCb, this);

    cv::namedWindow(WINDOW);
  }

  ~ImageConverter()
  {
    cv::destroyWindow(WINDOW);
  }

  void imageCb(const sensor_msgs::ImageConstPtr& msg)
  {
    cv_bridge::CvImagePtr cv_ptr;
    try
    {
      cv_ptr = cv_bridge::toCvCopy(msg, enc::BGR8);
      //IplImage *cvImage = cv_ptr;
      src = cv_ptr->image.clone();

    }
    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return;
    }

    cv::cvtColor(src, proc, CV_BGR2HSV);
    cv::blur(proc, proc, cv::Size(3, 3));

    cv::inRange(proc, cv::Scalar(160, 100, 100), cv::Scalar(170, 255, 255), out);

    //cv::Mat threshold_output;
    //vector<vector<cv::Point> > contours;



    /*IplImage cvImage = src;
    IplImage* cvImagePtr = &cvImage;
    /*IplImage cvImageHsv;
    cvCvtColor(cvImage, cvImageHsv, CV_BGR2HSV);
    IplImage* imgYellow = cvCreateImage(cvGetSize(&cvImage), IPL_DEPTH_8U, 1);
    cvInRangeS(&cvImage, cvScalar(22, 100, 100), cvScalar(30, 255, 255), imgYellow);
    /*IplImage* imgBlue = cvCreateImage(cvGetSize(&cvImage), IPL_DEPTH_8U, 1);
    cvInRangeS(&cvImage, cvScalar(90, 100, 100), cvScalar(125, 255, 255), imgBlue);
    IplImage* imgGreen = cvCreateImage(cvGetSize(&cvImage), IPL_DEPTH_8U, 1);
    cvInRangeS(&cvImage, cvScalar(44, 100, 100), cvScalar(65, 255, 255), imgGreen);
    IplImage* imgPink = cvCreateImage(cvGetSize(&cvImage), IPL_DEPTH_8U, 1);
    cvInRangeS(&cvImage, cvScalar(160, 100, 100), cvScalar(170, 255, 255), imgPink);*/
    //detect_beacons(cvImagePtr);

    cv::imshow(WINDOW, out);
    cv::waitKey(3);

    image_pub_.publish(cv_ptr->toImageMsg());
  }
	/*void detect_beacons(IplImage* cvImage){
		//color  detection
		IplImage* frame = 0;
		IplImage* imgThresh = 0;
		frame = cvCloneImage(cvImage);
		cvSmooth(frame, frame, CV_GAUSSIAN,3,3);

		IplImage* imgHSV = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 3);
		cvCvtColor(frame, imgHSV, CV_BGR2HSV);

		imgThresh=cvCreateImage(cvGetSize(imgHSV),IPL_DEPTH_8U, 1);
		cvInRangeS(imgHSV, cvScalar(160,100,100), cvScalar(170,255,255), imgThresh);
		//cvSmooth(imgThresh, imgThresh, CV_GAUSSIAN,3,3);
		cvShowImage("Found beacons", imgThresh);

	}*/

};

int main(int argc, char** argv)
{
  cvNamedWindow("Found beacons");
  ros::init(argc, argv, "image_converter");
  ImageConverter ic;
  ros::spin();
  cvDestroyWindow("Found beacons");
  return 0;
}
