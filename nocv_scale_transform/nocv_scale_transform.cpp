// nocv_scale_transform.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

#include <opencv2/opencv.hpp>

using namespace cv;


bool getPixleColor(cv::Mat mat, int x, int y, cv::Vec3b& col) {
	if (x < 0 || mat.cols <= x) return false;
	if (y < 0 || mat.rows <= y) return false;

	cv::Vec3b *p = mat.ptr<cv::Vec3b>(y);
	col = p[x];
	return true;
}

void setPixleColor(cv::Mat mat, int x, int y, cv::Vec3b col) {
	if (x < 0 || mat.cols <= x) return ;
	if (y < 0 || mat.rows <= y) return ;

	Vec3b *p;
	p = mat.ptr<cv::Vec3b>(y);
	p[x] = col;
}

int transform(Mat src, Mat dst) {

	float fx, fy;
	for (int y = 0; y < dst.rows; y++) {
		for (int x = 0; x < dst.cols; x++) {
			Vec3b col, out_col;
			fx = 0.75*x;
			fy = 0.75*y;


			if (getPixleColor(src, fx, fy, col)) {
				out_col = col;

				setPixleColor(dst, x, y, out_col);
			}
			
		}
	}
	return 0;
}


int toSmall(cv::Mat &img, double x1, double y1, double x2, double y2, cv::Vec3b &res)
{
	int sx, sy, ex, ey;
	int x, y;
	int val;
	double e1, e2;
	double ph, pw;
	double rr, gg, bb, oo;
	double s;			// 縮小元領域の面積の総和
	double ss;
	cv::Vec3b from;

	// 色を取得すべき範囲を求める
	sx = (int)x1;
	sy = (int)y1;
	ex = (int)x2;
	ey = (int)y2;
	if ((double)ex != x2) ex++;
	if ((double)ey != y2) ey++;
	s = 0.0;
	rr = gg = bb = oo = 0.0;
	for (y = sy; y<ey; y++) {
		// 重なっている矩形領域の高さを求める
		e1 = (double)y;
		e2 = (double)(y + 1);
		if (e1<y1) e1 = y1;	// 上端
		if (e2>y2) e2 = y2;	// 下端
		ph = e2 - e1;
		for (x = sx; x<ex; x++) {
			val = getPixleColor(img, x, y, from);	//画像上の画素情報を取得
			e1 = (double)x;
			e2 = (double)(x + 1);
			if (e1<x1) e1 = x1;	// 左端
			if (e2>x2) e2 = x2;	// 右端
			pw = e2 - e1;
			ss = pw*ph;
			s += ss;
			rr += (double)(from[2])*ss;
			gg += (double)(from[1])*ss;
			bb += (double)(from[0])*ss;
		}
	}
	if (s == 0.0) {
		return false;
	}
	rr /= s;
	gg /= s;
	bb /= s;
	oo /= s;
	res[2] = (unsigned char)(rr + .5);
	res[1] = (unsigned char)(gg + .5);
	res[0] = (unsigned char)(bb + .5);
	return true;
}
// 変形処理の本体
int transform2(cv::Mat &img, cv::Mat &outimg)
{
	int x, y;
	int x1, y1;
	int x2, y2;
	int ox, oy;
	double fx1, fy1, fx2, fy2;
	int rx1, ry1, rx2, ry2;
	cv::Vec3b col;
	int h1, w1;
	int h2, w2;
	double fx, fy;

	x1 = 0;
	y1 = 0;
	x2 = outimg.cols - 1;
	y2 = outimg.rows - 1;

	rx1 = 0;
	ry1 = 0;
	rx2 = img.cols - 1;
	ry2 = img.rows - 1;


	w1 = rx2 - rx1 + 1;
	h1 = ry2 - ry1 + 1;
	w2 = x2 - x1 + 1;
	h2 = y2 - y1 + 1;
	ox = 0;
	oy = 0;
	for (y = y1; y <= y2; y++) {
		for (x = x1; x <= x2; x++) {
			fx1 = (x - x1)*w1 / (double)w2 + rx1;//座標変換
			fy1 = (y - y1)*h1 / (double)h2 + ry1;
			fx2 = (x + 1 - x1)*w1 / (double)w2 + rx1;//座標変換
			fy2 = (y + 1 - y1)*h1 / (double)h2 + ry1;
			toSmall(img, fx1, fy1, fx2, fy2, col);		//縮小後の色を求める
			setPixleColor(outimg, x, y, col);	// 画像に値をセットする
		}
	}
	return 0;
}


int main()
{
	cv::Mat mat, dst;

	//mat = imread("D:\\scan5\\chk.png", 1);
	mat = imread("D:\\scan5\\75393983_p0.jpg", 1);

	dst.create(cv::Size(mat.size().width * 2.5, mat.size().height * 2.5), mat.type());

	transform2(mat, dst);
	
	//transform(mat, dst);
	//setPixleColor(mat, 1, 1, Vec3b(255, 0, 0));
	//setPixleColor(mat, 2, 1, Vec3b(255, 0, 0));


	imshow("dst", dst);
	imshow("sc", mat);
	waitKey(0);
    return 0;
}

