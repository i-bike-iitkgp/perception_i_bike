//#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

//  Global variables

/*  General variables */
Mat src, edges;
Mat src_gray;
Mat standard_hough, probabilistic_hough;

int min_threshold = 50;
int max_trackbar = 150;

const char* standard_name = "Standard Hough Lines Demo";
const char* probabilistic_name = "Probabilistic Hough Lines Demo";

int s_trackbar = max_trackbar;
int p_trackbar = max_trackbar;

/// Function Headers
void help();
void Standard_Hough( int, void* );
void Probabilistic_Hough( int, void* );

/**
 * @function main
 */
int main()
{
   /// Read the image
   VideoCapture v(2);

   /// Create Trackbars for Thresholds
   char thresh_label[50];
   sprintf( thresh_label, "Thres: %d + input", min_threshold );

   namedWindow( standard_name, WINDOW_AUTOSIZE );
   createTrackbar( thresh_label, standard_name, &s_trackbar, max_trackbar, Standard_Hough);

   namedWindow( probabilistic_name, WINDOW_AUTOSIZE );
   createTrackbar( thresh_label, probabilistic_name, &p_trackbar, max_trackbar, Probabilistic_Hough);

   while(1)
{
   v >> src;
    /// Pass the image to gray
   cvtColor( src, src_gray, COLOR_RGB2GRAY );

   /// Apply Canny edge detector
   Canny( src_gray, edges, 50, 200, 3 );

   
   /// Initialize
   Standard_Hough(0, 0);
   Probabilistic_Hough(0, 0);
   char a=waitKey(1);
   	if (a == 27)
		break;
}
	return(0);
}

/**
 * @function help
 * @brief Indications of how to run this program and why is it for
 */
void help()
{
  printf("\t Hough Transform to detect lines \n ");
  printf("\t---------------------------------\n ");
  printf(" Usage: ./HoughLines_Demo <image_name> \n");
}

/**
 * @function Standard_Hough
 */
void Standard_Hough( int, void* )
{
  vector<Vec2f> s_lines;
  cvtColor( edges, standard_hough, COLOR_GRAY2BGR );

  /// 1. Use Standard Hough Transform
  HoughLines( edges, s_lines, 1, CV_PI/180, min_threshold + s_trackbar, 0, 0 );

  /// Show the result
  for( size_t i = 0; i < s_lines.size(); i++ )
     {
      float r = s_lines[i][0], t = s_lines[i][1];
      double cos_t = cos(t), sin_t = sin(t);
      double x0 = r*cos_t, y0 = r*sin_t;
      double alpha = 1000;

       Point pt1( cvRound(x0 + alpha*(-sin_t)), cvRound(y0 + alpha*cos_t) );
       Point pt2( cvRound(x0 - alpha*(-sin_t)), cvRound(y0 - alpha*cos_t) );
       line( standard_hough, pt1, pt2, Scalar(255,0,0), 3, CV_AA);
     }

   imshow( standard_name, standard_hough );
}

/**
 * @function Probabilistic_Hough
 */
void Probabilistic_Hough( int, void* )
{
  
  Mat temp[6];
  vector<Vec4i> p_lines;
  cvtColor( edges, probabilistic_hough, COLOR_GRAY2BGR );
  
  for(int k=0;k<6;k++)
  {
  
   temp[k]=edges(Range(k*(edges.rows)/6,(k+1)*edges.rows/6),Range(0,edges.cols));
  /// 2. Use Probabilistic Hough Transform
  HoughLinesP( temp[k], p_lines, 1, CV_PI/180, min_threshold + p_trackbar, 30, 10 );

  /// Show the result
  for( size_t i = 0; i < p_lines.size(); i++ )
     {
       Vec4i l = p_lines[i];
       line( probabilistic_hough, Point(l[0], k*(edges.rows)/6+l[1]), Point(l[2], k*(edges.rows)/6+l[3]), Scalar(255,0,0), 3, CV_AA);
     }
   }

   imshow( probabilistic_name, probabilistic_hough );
}


