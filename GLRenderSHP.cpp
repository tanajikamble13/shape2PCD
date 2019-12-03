 
//Program to Parse ESRI Shapefile and render it in OpenGL and convert it into Point cloud library friendly PCD format.
//Author : Tanaji Umaji Kamble
//Contact: tanajikamble13@gmail.com
//Libraries Used: Shapelib ,OpenGL


#include <GL/glut.h>
#include <stdlib.h>
 
#include "shapelib/shapefil.h"

 
#include <vector>
#include <iostream>
#include <fstream>
#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>

using namespace std;
 
 typedef pcl::PointXYZ pointT;
 pcl::PointCloud<pointT> cloudPt,cloudArc,cloudPoly,cloudShape;
 pcl::PointXYZ point;

typedef struct MyPoint2D
{
 double dX;
 double dY;
 
}MyPoint2D;
 
//Holds Coordinates of Point Shapefile
vector<MyPoint2D> vPoints;
 
typedef struct MyLineString2D
{
 vector<MyPoint2D> vPointList;
 
}MyLineString2D;

//Holds Coordinates of Line Shapefile
vector<MyLineString2D> vLines;
 
 
typedef struct MyPolygon2D
{
 vector<MyPoint2D> vPointList;
 
}MyPolygon2D;
//Holds Coordinates of Polygon Shapefile 
vector<MyPolygon2D> vPolygons;
 
 

typedef struct SBoundingBox
{
 float fMaxX;
 float fMaxY;
 float fMinX;
 float fMinY;
 
}SBoundingBox;
//Bounding Box of Shapefile 
SBoundingBox sBoundingBox;
 
//Function to Open Shapefile and parse the info 
void OpenShapeFile(char* fileName)
{
           
    SHPHandle hSHP=SHPOpen(fileName, "rb" );


	//Read Bounding Box of Shapefile
    sBoundingBox.fMaxX=hSHP->adBoundsMax[0];
    sBoundingBox.fMaxY=hSHP->adBoundsMax[1];

    sBoundingBox.fMinX=hSHP->adBoundsMin[0];
    sBoundingBox.fMinY=hSHP->adBoundsMin[1];


    if(hSHP == NULL) return;

	//Point Shapefile
    if(hSHP->nShapeType == SHPT_POINT)
    {
        SHPObject *psShape;

        cloudPt.width    = hSHP->nRecords;				// This means that the point cloud is "unorganized"
	cloudPt.height   = 1;						// (i.e. not a depth map)
	cloudPt.is_dense = false;
	cloudPt.points.resize (cloudPt.width * cloudPt.height);

        for(int i=0;i<hSHP->nRecords;i++)
        {
            psShape = SHPReadObject(hSHP, i);

            double fX = psShape->padfX[0];
            double fY = -psShape->padfY[0];

             point.x=fX ;
             point.x=-fY;
             point.z=0;

            cloudPt.points[i].x = float (fX);
            cloudPt.points[i].y = -float (fY);
            cloudPt.points[i].z = 0.0;
            //point.PointXYZ (1, 2, 0)
            //cloud.push_back(point);
            //Plot these points
            MyPoint2D pt;
            pt.dX=fX;
            pt.dY=-fY;
            vPoints.push_back(pt);
        }
    }


	//Line Shapefile
    else if(hSHP->nShapeType == SHPT_ARC)
    {
        SHPObject *psShape;

        cloudArc.width    = hSHP->nRecords;				// This means that the point cloud is "unorganized"
	cloudArc.height   = 1;						// (i.e. not a depth map)
	cloudArc.is_dense = false;
	cloudArc.points.resize (cloudArc.width * cloudArc.height);
	    for(int i=0;i<hSHP->nRecords;i++)
        {
            psShape = SHPReadObject(hSHP, i);
            vector<MyPoint2D> tempPointArray;

            for(int j=0;j<psShape->nVertices;j++)
            {
				double fX = psShape->padfX[j];
				double fY = psShape->padfY[j];

                                 point.x=fX ;
                                 point.x=fY;
                                 point.z=0;

                                 cloudArc.points[i].x = float (fX);
                                 cloudArc.points[i].y = float (fY);
                                 cloudArc.points[i].z = 0.0;

				MyPoint2D pt;
				pt.dX=fX;
				pt.dY=fY;
				tempPointArray.push_back(pt);
            }
           
			MyLineString2D linestring;
            linestring.vPointList=tempPointArray;
            vLines.push_back(linestring);
           
        }
    }

 	//Polygon Shapefile
    if(hSHP->nShapeType == SHPT_POLYGON)
    {
		SHPObject *psShape;
                cloudPoly.width    = hSHP->nRecords;				// This means that the point cloud is "unorganized"
	        cloudPoly.height   = 1;						// (i.e. not a depth map)
	        cloudPoly.is_dense = false;
	        cloudPoly.points.resize (cloudPoly.width * cloudPoly.height);

		for(int i=0;i<hSHP->nRecords;i++)
		{
			psShape = SHPReadObject(hSHP, i);
      		vector<MyPoint2D> tempPointArray;
     
			for(int j=0;j<psShape->nVertices;j++)
			{
				double fX = psShape->padfX[j];
				double fY = psShape->padfY[j];
                                
                                point.x=fX ;
                                point.x=fY;
                                point.z=0;

                                cloudPoly.points[i].x = float (fX);
                                cloudPoly.points[i].y = float (fY);
                                cloudPoly.points[i].z = 0.0;


				MyPoint2D pt;
				pt.dX=fX;
				pt.dY=fY;
      			tempPointArray.push_back(pt);
			}
			MyPolygon2D polygon;
			polygon.vPointList=tempPointArray;
			vPolygons.push_back(polygon);
		}

    }
  
}
 
 
void initializeGL()
{
	glClearColor (0.0, 0.0, 0.0, 0.0);
//	glClearColor (1.0, 1.0, 1.0, 1.0);
	glShadeModel (GL_FLAT);
	glEnable (GL_LINE_SMOOTH);
	glHint (GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
	
	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);  



	//Assign Default Map Bounds to glOrtho
	sBoundingBox.fMaxX=180.0f;
	sBoundingBox.fMaxY=90.0f;
	sBoundingBox.fMinX=-180.0f;
	sBoundingBox.fMinY=-90.0f;
}
 
 
void resizeGL(int w, int h)
{
	if(h<=0) h=1 ;
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	//Assign Bounding Box Coordinates of Shapefile to glOrtho()
	glOrtho(sBoundingBox.fMinX, sBoundingBox.fMaxX,sBoundingBox.fMinY,sBoundingBox.fMaxY,-1,1);
	glMatrixMode(GL_MODELVIEW);
}
 
 
void draw()
{
  
	glClear (GL_COLOR_BUFFER_BIT);
	glColor3f (0.0, 0.0, 1.0);
	glLoadIdentity ();
	
	//Render Point Shapefile
	glColor3f (0.0, 0.0, 1.0);
	glEnable(GL_POINT_SMOOTH) ;
	glPointSize(5.0);
	glBegin(GL_POINTS);
	
	for(int i=0;i<vPoints.size();i++)
	{
		glVertex2f(vPoints[i].dX,vPoints[i].dY);
	}
	
	glEnd();
	
	//Render Line Shapefile
	glColor3f (0.0, 1.0, 0.0);
	for( int i=0;i<vLines.size();i++)
	{
		
		glBegin(GL_LINE_STRIP);
		for(int j=0;j<vLines[i].vPointList.size();j++)
		{
		  glVertex2f(vLines[i].vPointList[j].dX,vLines[i].vPointList[j].dY);
	
		}
		
		glEnd();
	}
	
	//Render Polygon Shapefile
	glColor3f(1.0,0.0, 0.0);
	for(int i=0;i<vPolygons.size();i++)
	{
		glBegin(GL_LINE_LOOP);
		for(int j=0;j<vPolygons[i].vPointList.size();j++)
		{
			glVertex2f(vPolygons[i].vPointList[j].dX,vPolygons[i].vPointList[j].dY);
		}
		
		glEnd();
	}
 
   glFlush();
}
 
void paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    draw();
}
 
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE| GLUT_RGB);
	glutInitWindowSize (600,600);
	glutCreateWindow ("Render Shapefiles");
	initializeGL();
	
        //std::string filePath;
        //filePath=argv[1];
        //OpenShapeFile(&filePath[0]);//Line Shapefile
        //OpenShapeFile(argv[1]);//Line Shapefile //transfer of all supporting files needed
	OpenShapeFile("../Shapefiles/strassen.shp");//Line Shapefile
	OpenShapeFile("../Shapefiles/poi.shp");//Point Shapefile
	OpenShapeFile("../Shapefiles/gruenflaechen.shp");//Polygon Shapefile
        cout << "saving into pcd format.." << endl;
        //pcl::io::savePCDFileASCII ("convertedPt.pcd", cloudPt);
        //pcl::io::savePCDFileASCII ("convertedArc.pcd", cloudArc);
        //pcl::io::savePCDFileASCII ("convertedPoly.pcd", cloudPoly);
	cloudShape=cloudPt + cloudArc;
        cloudShape=cloudPt + cloudPoly;
        pcl::io::savePCDFileASCII ("cloudShape.pcd", cloudShape);
	glutReshapeFunc (resizeGL);
	glutDisplayFunc (paintGL);
	glutMainLoop();
        
   return 0;
}
