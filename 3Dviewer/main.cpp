// 2017-09-29 by LSS
// 2017-10-01 edited

#pragma region
#include <vtkVRMLImporter.h>
#include <vtk3DSImporter.h>
#include <vtkSTLReader.h>
#include <vtkOBJReader.h>
#include <vtkPLYReader.h>
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#pragma comment( lib, "OpenGL32.lib" )
#pragma comment( lib, "vtkCommon.lib" )
#pragma comment( lib, "vtkIO.lib" )
#pragma comment( lib, "vtkRendering.lib" )
#pragma comment( lib, "vtkGraphics.lib" )
#pragma comment( lib, "vtkFiltering.lib" )
#pragma comment( lib, "vtkHybrid.lib" )
#pragma comment( lib, "vtksys.lib" )
#pragma comment( lib, "vtkexpat.lib" )
#pragma comment( lib, "vtkpng.lib" )
#pragma comment( lib, "vtkjpeg.lib" )
#pragma comment( lib, "vtktiff.lib" )
#pragma comment( lib, "vtkzlib.lib" )
#pragma comment( linker, "/subsystem:windows /ENTRY:mainCRTStartup" )
#pragma endregion

class Viewer {
private:
	char *file;
	vtkSmartPointer<vtkActor> actor ;
	vtkSmartPointer<vtkPolyDataMapper> mapper;
	vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkRenderWindow> renderWindow;
    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor;

public:
	Viewer( char *File ) {
		file = File;
		actor = vtkSmartPointer<vtkActor>::New();
		mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		renderer = vtkSmartPointer<vtkRenderer>::New();
		renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
		renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
		renderWindow->Render();
		renderWindow->SetWindowName( "3D viewer by LSS" );
		renderWindow->SetSize( 600, 600 );
		renderWindow->AddRenderer( renderer );
		renderWindowInteractor->SetRenderWindow( renderWindow );
		renderWindowInteractor->SetInteractorStyle( vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New() );
	}
	
	void viewWRL() {
		vtkSmartPointer<vtkVRMLImporter> importer = vtkSmartPointer<vtkVRMLImporter>::New();
		importer->SetFileName( file );
		importer->Read();
		renderer = importer->GetRenderer();
	}
	
	void view3DS() {
		vtkSmartPointer<vtk3DSImporter> importer = vtkSmartPointer<vtk3DSImporter>::New();
		importer->SetFileName( file );
		importer->Read();
		renderer = importer->GetRenderer();
	}
	
	void viewSTL() {
		vtkSmartPointer<vtkSTLReader> reader = vtkSmartPointer<vtkSTLReader>::New();
		reader->SetFileName( file );
		reader->Update();
		mapper->SetInputConnection( reader->GetOutputPort() );
	}
	
	void viewOBJ() {
		vtkSmartPointer<vtkOBJReader> reader = vtkSmartPointer<vtkOBJReader>::New();
		reader->SetFileName( file );
		reader->Update();
		mapper->SetInputConnection( reader->GetOutputPort() );
	}

	void view() {
		char *ExName = toLower( file+strlen( file )-4 );
		if( !strcmp( ExName, ".wrl" ) ) viewWRL();
		else if( !strcmp( ExName, ".3ds" ) ) view3DS();
		else if( !strcmp( ExName, ".stl" ) ) viewSTL();
		else if( !strcmp( ExName, ".obj" ) ) viewOBJ();
		else {
			system( "mshta vbscript:msgbox(\"Supported format:  .wrl .3ds .stl .obj\",64,\"unknown file format\")(window.close)" );
			return;
		}
		actor->SetMapper( mapper );
		renderer->AddActor( actor );
		renderWindow->AddRenderer( renderer );
		renderer->SetBackground( 0, 0, 0 );
		renderer->SetBackground2( 0.5, 0.5, 1.0 );
		renderer->SetGradientBackground( 1 );
		renderWindow->Render();
		renderWindowInteractor->Start();
	}

	char *toLower( char *s ) {
		char *p = s;
		while( *p ) {
			if( *p >= 'A' && *p <= 'Z' ) *p += 32;
			p++;
		}
		return s;
	}
};

int main( int argc, char* argv[] ) {
	if( argc != 2 ) {
		system( "mshta vbscript:msgbox(\"Do not open directly. Drag only one file to this exe instead. Please close and retry.\",64,\"error\")(window.close)" );
		return 1;
	}
	Viewer viewer( argv[1] );
	viewer.view();
	return 0;
}
