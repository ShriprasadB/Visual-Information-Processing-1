#include <opencv2/opencv.hpp>
#include <iostream>
#include<math.h>

 

using namespace cv;
using namespace std;

int visited[1200][1200];
int visited1[1200][1200];
int visited2[1200][1200]; 
int original_image_histogram_values[255];

int area;


// A function to check if a given cell (row, col) can be included in DFS
int isSafe(Mat img, int row, int col)
{
    // row number is in range, column number is in range and value is 1 
    // and not yet visited
    return (row >= 0) && (row < img.rows) &&     
           (col >= 0) && (col < img.cols) &&      
           (int(img.at<uchar>(row, col))==255 && !visited[row][col]); 
}

int isSafeone(Mat img, int row, int col)
{
    // row number is in range, column number is in range and value is 1 
    // and not yet visited
    return (row >= 0) && (row < img.rows) &&     
           (col >= 0) && (col < img.cols) &&      
           (int(img.at<uchar>(row, col))==120 && !visited1[row][col]); 
}

int isSafetwo(Mat img, int row, int col)
{
    // row number is in range, column number is in range and value is 1 
    // and not yet visited
    return (row >= 0) && (row < img.rows) &&     
           (col >= 0) && (col < img.cols) &&      
           (int(img.at<uchar>(row, col))==120 && !visited2[row][col]); 
}

int getAverageIntensityValue(int hist[], int a, int b)
//Function to find average_intensity_value of histogram based on algorithm provided in the README file
{
	int sum_of_pixels=0;
	sum_of_pixels = sum_of_pixels + hist[0];
	int sum_product_of_pixels = (hist[0] * 0);

	for (int i = a; i < b; i++)
	{
		sum_of_pixels = sum_of_pixels + hist[i];
	}

	for (int i = a; i < b; i++)
	{
		sum_product_of_pixels = sum_product_of_pixels + (hist[i] * i) ;
	}

	int average_intensity_value = sum_product_of_pixels / sum_of_pixels;//average_intensity_value calculation code ends here
	return average_intensity_value;
}

Mat getBinaryImage(Mat img, int threshold)
{
	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			if (int(img.at<uchar>(i, j)) <= threshold)
			{
				img.at<uchar>(i, j) = 0;		//If pixel intensity is greater than threshold
			}
			else
			{
				img.at<uchar>(i, j) = 255;//If pixel intensity is less than threshold
			}
		}
	}

	return img;
}
void DFS(Mat img, int row, int col)
{
    // These arrays are used to get row and column numbers of 8 neighbours 
    // of a given cell
    static int rowNbr[] = {-1,  0, 0, 1};
    static int colNbr[] = { 0, -1, 1, 0};
 	area++;
    // Mark this cell as visited
    visited[row][col] = 1;
    img.at<uchar>(row,col)=120;

    // Recur for all connected neighbours
    for (int k = 0; k < 4; ++k)
        if (isSafe(img, row + rowNbr[k], col + colNbr[k]) )
            DFS(img, row + rowNbr[k], col + colNbr[k]);
}
void smallest(Mat img, int row, int col)
{
    // These arrays are used to get row and column numbers of 8 neighbours 
    // of a given cell
    static int rowNbr[] = {-1, 0, 0, 1};
    static int colNbr[] = {0, -1, 1, 0};
 	area++;
    // Mark this cell as visited
    visited2[row][col] = 1;
    img.at<uchar>(row,col)=60;

    // Recur for all connected neighbours
    for (int k = 0; k < 4; ++k)
        if (isSafetwo(img, row + rowNbr[k], col + colNbr[k]) )
            smallest(img, row + rowNbr[k], col + colNbr[k]);
}
void largest(Mat img, int row, int col)
{
    // These arrays are used to get row and column numbers of 8 neighbours 
    // of a given cell
    static int rowNbr[] = {-1, 0, 0, 1};
    static int colNbr[] = { 0, -1, 1, 0};
 	area++;
    // Mark this cell as visited
    visited1[row][col] = 1;
    img.at<uchar>(row,col)=200;

    // Recur for all connected neighbours
    for (int k = 0; k < 4; ++k)
        if (isSafeone(img, row + rowNbr[k], col + colNbr[k]) )
            largest(img, row + rowNbr[k], col + colNbr[k]);
}

void initializeVisited()
{
   memset(visited, 0, sizeof(visited));
}

void printComponents(int count)
{
     cout<<"number of connected components: "<<count<<endl;
}
void components(Mat img)
{
	   // Initialize count as 0 and travese through the all cells of
 	   // given matrix
    int count = 0;
	int k = 0;
	int a[100];
    int x[1000],y[1000];	
	//area = 0;
    for (int i = 0; i < img.rows; ++i)
        for (int j = 0; j < img.cols; ++j)
	if (int(img.at<uchar>(i, j))==255 && !visited[i][j]) // If a cell with value 1 is not
            {	
		
		area = 0;	                         // visited yet, then new island found
                DFS(img, i, j);
	     // Visit all cells in this island.
		if(area!=1){
		 x[k]=i;
		 y[k]=j;
		 k++;
                 a[count] = area;
      		 ++count;
		}
//		cout<<a[count-1]<<endl;
		
		                  // and increment island count
//		cout << "area: " <<area<<endl; 
            }

	
	
	//cout << "area: " <<area<<endl; 
	
	int m;
	for(m = 0;m<count;m++)
	{
		cout << "area of region: " <<a[m]<<" starting pixel row i: "<<x[m]<<" starting pixel column j "<<y[m] <<endl;
 
	}
	printComponents(count);
	int large = a[0];
	int small = a[0];
	for(m = 0;m<count;m++)
	{
		if(large<a[m])
			large = a[m];
		if(small > a[m])
			small = a[m];

	}

	cout<<"largest region "<<large<<"   smallest region "<<small<<endl;

	for(m = 0;m<count;m++)
	{
		if(large == a[m])
			largest(img,x[m],y[m]);
		if(small == a[m])
			smallest(img,x[m],y[m]);
	}	
	
}
void traverseBinary(Mat img)
{
    // Make a bool array to mark visited cells.
    // Initially all cells are unvisited
    initializeVisited();
    components(img);
 	 	

}
void setHistValues()
{
	for (int a = 0; a < 256; a++)				//Setting histogram Y axis values to zero
	{
		original_image_histogram_values[a] = 0;
	}

}
int main(int argc, char** argv)
{
	string filename;
	cout << "Enter image name with full path " << endl;
	cin >> filename;
	Mat img = imread(filename, CV_LOAD_IMAGE_GRAYSCALE);	//Load the image
	namedWindow("Original Image", WINDOW_AUTOSIZE);
	imshow("Original Image", img);
	int rows = img.rows, cols = img.cols;
	
	setHistValues();

	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			original_image_histogram_values[int(img.at<uchar>(i, j))]++;	//Generate histogram for original image
		}
	}

	int old_threshold = 128,new_threshold = 128,delta_threshold = 2 ,m1, m2;
	
	do
	{
		old_threshold = new_threshold;
		m1 = getAverageIntensityValue(original_image_histogram_values, 1, old_threshold);
		m2 = getAverageIntensityValue(original_image_histogram_values, old_threshold + 1, 256);
		new_threshold = ((m1 + m2) / 2)+11;
		
	} 
	while ((old_threshold-new_threshold) > delta_threshold);
	img = getBinaryImage(img, new_threshold);
	
	cout << "Threshold value T: " << new_threshold << endl;
	namedWindow("Binary Image", WINDOW_AUTOSIZE);
	imshow("Binary Image", img);
	traverseBinary(img);
	
	//namedWindow("detected shapes", WINDOW_AUTOSIZE);
	imshow("detetcted shapes", img);
	waitKey(0);
	cvDestroyWindow("Binary Image");

	return 0;
}

