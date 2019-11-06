#include <dlib/opencv.h>
#include <opencv2/opencv.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <Windows.h>
#include<opencv2/highgui.hpp>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"


//using namespace dlib;
//using namespace std;
class 高精度计时
{
public:
	高精度计时(void);
	~高精度计时(void);//析构函数
private:
	LARGE_INTEGER 开始时间;
	LARGE_INTEGER 结束时间;
	LARGE_INTEGER CPU频率;
public:
	double 间隔;
public:
	void 开始();
	void 结束();
};
高精度计时::高精度计时(void)
{
	QueryPerformanceFrequency(&CPU频率);
}
高精度计时::~高精度计时(void)
{
}
void 高精度计时::开始()
{
	QueryPerformanceCounter(&开始时间);
}
void 高精度计时::结束()
{
	QueryPerformanceCounter(&结束时间);
	间隔 = ((double)结束时间.QuadPart - (double)开始时间.QuadPart) / (double)CPU频率.QuadPart;
}
int main()
{
	高精度计时 时间;
	try
	{
		时间.开始();
		cv::VideoCapture cap(0);
		if (!cap.isOpened())
		{
			std::cout << "Unable to connect to camera" << std::endl;
			return 1;
		}
		//image_window win;
		// Load face detection and pose estimation models.
		dlib::frontal_face_detector detector = dlib::get_frontal_face_detector();
		dlib::shape_predictor pose_model;
		dlib::deserialize("C:/Users/coy19/source/repos/testdlib/shape_predictor_68_face_landmarks.dat") >> pose_model;
		时间.结束();
		float 初始化时间 = 时间.间隔;
		DWORD k = ::GetTickCount(); //获取毫秒级数目
		// Grab and process frames until the main window is closed by the user.
		while (cv::waitKey(30) != 27)
		{
			// Grab a frame
			cv::Mat temp;
			cap >> temp;
			cv::putText(temp, "Initial Time/ms : " + std::to_string(初始化时间 * 1000), cv::Point(20, 380), 5, 2, cvScalar(0, 0, 255));
			dlib::cv_image<dlib::bgr_pixel> cimg(temp);
			// Detect faces
		//    DWORD k = ::GetTickCount(); //获取毫秒级数目
			std::vector<dlib::rectangle> faces = detector(cimg);
			时间.结束();
			std::cout << "检测人脸部分耗时:" << 时间.间隔 * 1000 << std::endl;
			cv::putText(temp, "Face Detector/ms : " + std::to_string(时间.间隔 * 1000), cv::Point(20, 420), 5, 2, cvScalar(0, 0, 255));
			//    std::cout << (::GetTickCount() - k) << std::endl;
				// Find the pose of each face.
			std::vector<dlib::full_object_detection> shapes;
			//    k = ::GetTickCount();
			时间.开始();
			for (unsigned long i = 0; i < faces.size(); ++i)
				shapes.push_back(pose_model(cimg, faces[i]));
			if (!shapes.empty()) {
				for (int j = 0; j < shapes.size(); j++) {
					for (int i = 0; i < 68; i++) {
						cv::circle(temp, cv::Point(shapes[j].part(i).x(), shapes[j].part(i).y()), 2, cv::Scalar(255, 0, 0), -1);
						//  shapes[0].part(i).x();//68个
					}
				}
			}
			时间.结束();
			std::cout << "剩余部分的总时间:" << 时间.间隔 * 1000 << std::endl << std::endl << std::endl;
			cv::putText(temp, "The rest/ms: " + std::to_string(时间.间隔 * 1000), cv::Point(20, 460), 5, 2, cv::Scalar(0, 0, 255));
			//Display it all on the screen
			cv::imshow("Dlib特征点", temp);
		}
	}
	catch (dlib::serialization_error& e)
	{
		std::cout << "You need dlib's default face landmarking model file to run this example." << std::endl;
		std::cout << "You can get it from the following URL: " << std::endl;
		std::cout << "   http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2" << std::endl;
		std::cout << std::endl << e.what() << std::endl;
		system("pause");
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}