#pragma once
#include <opencascade/TopoDS_Shape.hxx>
#include <opencascade/XSControl_Reader.hxx>

class StepIgesModelReader
{
public:
	enum FileExtension
	{
		IGES, STEP
	};

public:
	static TopoDS_Shape ReadFile(Standard_CString file, FileExtension ext);

private:
	static XSControl_Reader GetFileReader(FileExtension ext);
};

