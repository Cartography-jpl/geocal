#ifndef _PICMTCH4_UTILS_H
#define _PICMTCH4_UTILS_H

void convertPoints(int geocord1, double tiePoints[6], float** lineSamp, 
		   int dim, int labnl, int labns, float** c_data = NULL, 
		   int RefImageLocCount = 0);
void convertTiePoints(float** fstImgCoeff, float** sndImgCoeff, float** c_data, 
		      int geocord1, double itie[6], double otie[6], 
		      int geocord2, int RefImageLocCount, int labnl1, int labns1, 
		      int labnl2, int labns2);
void convertGeoTIFFTiePoints(float** fstImgCoeff, float** sndImgCoeff, 
			     float** c_data, int geocord1, double itie[6], 
			     double otie[6], int geocord2, int RefImageLocCount, 
			     int labnl1, int labns1, int labnl2, int labns2, 
			     bool rotated = false); 
void getGeoTIFFLabels(double t[6], double tinv[6], const char* labelstr, 
		      int labnl, int labns, 
		      int gtgetlabval);
float getzvl(float* imgCoeff, int n, float coord[2], int nw, int nr);

#endif
