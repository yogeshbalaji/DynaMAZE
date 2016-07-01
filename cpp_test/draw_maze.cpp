// Code for drawing the generated mazes
// Graphic simulation is done in OpenCV

#include <iostream>
#include <highgui.h>
#include <cv.h>

using namespace std;
using namespace cv;

struct node
{
	int row;
	int col;
};

Mat draw_maze(int Grid_length, int Grid_width, vector< vector<int> >Adjacency_list, int Num_pixels_bwn_nodes, int Border)
{
	
	int Img_height = 2*Border + Num_pixels_bwn_nodes*(Grid_length-1);
	int Img_width = 2*Border + Num_pixels_bwn_nodes*(Grid_width-1);
	Mat img(Img_height, Img_width, CV_8UC3, Scalar(0,0,0));
	
	// Index to node mapping
	node Nlist[Grid_length*Grid_width+1];
	int K = 1;
	for(int i=1;i<=Grid_length;i++)
		for(int j=1;j<=Grid_width;j++)
		{
			node Tmp;
			Tmp.row = i;
			Tmp.col = j;
			Nlist[K] = Tmp;
			K++;
		}
		
	// Simulating the graph
	for(int i=1;i<Adjacency_list.size();i++)
	{
		node Tmp;
		Tmp = Nlist[i];
		int pt1row = Border + (Tmp.row-1)*Num_pixels_bwn_nodes;
		int pt1col = Border + (Tmp.col-1)*Num_pixels_bwn_nodes;
		circle(img, Point(pt1col, pt1row), 3, Scalar(255,0,0));
		for(int j=0;j<Adjacency_list[i].size();j++)
		{
			node Tmp2;
			Tmp2 = Nlist[Adjacency_list[i][j]];
			int pt2row = Border + (Tmp2.row-1)*Num_pixels_bwn_nodes;
			int pt2col = Border + (Tmp2.col-1)*Num_pixels_bwn_nodes;
			line(img, Point(pt1col, pt1row), Point(pt2col, pt2row), Scalar(0,255,0));
		}
	}
	
	return img;
	
}


void move_ball(int Grid_length, int Grid_width, vector< vector<int> > Adjacency_list, Mat img, int Num_pixels_bwn_nodes, int Border, int Speed)
{
	node CurLocation;		// Indicates the current location of the ball
	int Node_Index[Grid_length+1][Grid_width+1];
	int Img_height = 2*Border + Num_pixels_bwn_nodes*(Grid_length-1);
	int Img_width = 2*Border + Num_pixels_bwn_nodes*(Grid_width-1);
		
	// Node to index mapping
	int K = 1;
	for(int i=1;i<=Grid_length;i++)
		for(int j=1;j<=Grid_width;j++)
		{
			Node_Index[i][j] = K;
			K++;
		}
	
	// Index to node mapping
	node Nlist[Grid_length*Grid_width+1];
	K = 1;
	for(int i=1;i<=Grid_length;i++)
		for(int j=1;j<=Grid_width;j++)
		{
			node Tmp;
			Tmp.row = i;
			Tmp.col = j;
			Nlist[K] = Tmp;
			K++;
		}
	
	CurLocation.row = Border;
	CurLocation.col = Border;
	
	while(1)
	{
		// Rendering
		Mat Img_new = img.clone();
		circle(Img_new, Point(CurLocation.col, CurLocation.row), 5, Scalar(0,0,255), -1);
		//destroyWindow("Game");
		imshow("Game",Img_new);
		int Response = waitKey(0);
		char Move;
		
		if(Response == 65362)
			Move = 'u';
		else if(Response == 65364)
			Move = 'd';
		else if(Response == 65363)
			Move = 'r';
		else if(Response == 65361)
			Move = 'l';
		else if(Response == 27)
			Move = 'q';
		if(Move == 'q')
		{
			cout<<"TERMINATED\n";
			break;
		}
			
		// Checking the current location to determine valid moves
		
		int Val1 = (CurLocation.row-Border)/Num_pixels_bwn_nodes+1;
		int Val2 = (CurLocation.col-Border)/Num_pixels_bwn_nodes+1;
		int Val3 = (CurLocation.row-Border)%Num_pixels_bwn_nodes;
		int Val4 = (CurLocation.col-Border)%Num_pixels_bwn_nodes;
		
		if(Val1 == Grid_length && Val2 == Grid_width)		// Reached the terminal state
		{
			cout<<"YOU WIN!!!\n\n";
			break;
		}
		
		vector<char> Possible_moves;
		
		if(Val3 == 0 && Val4 == 0)	// Reached a vertex
		{
			int Present_vertex = Node_Index[Val1][Val2];
			for(int i=0;i<Adjacency_list[Present_vertex].size();i++)
			{
				node Connected_vertex = Nlist[Adjacency_list[Present_vertex][i]];
				if(Connected_vertex.row == Val1)
				{
					if(Connected_vertex.col == Val2+1)
						Possible_moves.push_back('r');
					else if(Connected_vertex.col == Val2-1)
						Possible_moves.push_back('l');
				}
				else if(Connected_vertex.col == Val2)
				{
					if(Connected_vertex.row == Val1-1)
						Possible_moves.push_back('u');
					else if(Connected_vertex.row == Val1+1)
						Possible_moves.push_back('d');
				}
				
			}
		}
		else if(Val3 == 0)			// Horizontal edge
		{
			Possible_moves.push_back('l');
			Possible_moves.push_back('r');
		}
		else if(Val4 == 0)			// Vertical edge
		{
			Possible_moves.push_back('u');
			Possible_moves.push_back('d');
		}
		
		for(int i=0;i<Possible_moves.size();i++)
		{
			if(Move == Possible_moves[i])
			{
				if(Move == 'u')
					CurLocation.row = CurLocation.row - Speed;
				else if(Move == 'd')
					CurLocation.row = CurLocation.row + Speed;
				else if(Move == 'l')
					CurLocation.col = CurLocation.col - Speed;
				else if(Move == 'r')
					CurLocation.col = CurLocation.col + Speed;
				
				// Boundary Conditions
				
				if(CurLocation.row > Img_height-Border)
					CurLocation.row = Img_height-Border;
				if(CurLocation.col > Img_width-Border)
					CurLocation.col = Img_width-Border;
				if(CurLocation.row < Border)
					CurLocation.row = Border;
				if(CurLocation.col<Border)
					CurLocation.col = Border;
				
				break;
			}
		}
		
	}
}

