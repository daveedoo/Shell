#include "StepIgesModelReader.h"
#include <opencascade/IGESControl_Reader.hxx>
#include <opencascade/STEPControl_Reader.hxx>

TopoDS_Shape StepIgesModelReader::ReadFile(Standard_CString file, FileExtension ext)
{
    XSControl_Reader reader = GetFileReader(ext);

    // read the file
    IFSelect_ReturnStatus stat = reader.ReadFile(file);
    std::cout << "=== Load Check ===" << std::endl;
    reader.PrintCheckLoad(Standard_False, IFSelect_ItemsByEntity);

    // translate
    Standard_Integer nbTransfers = reader.TransferRoots();
    std::cout << "=== Transfer Check ===" << std::endl;
    reader.PrintCheckTransfer(Standard_False, IFSelect_ItemsByEntity);
}

XSControl_Reader StepIgesModelReader::GetFileReader(FileExtension ext)
{
    switch (ext)
    {
    case StepIgesModelReader::IGES:
        return IGESControl_Reader();
    case StepIgesModelReader::STEP:
        return STEPControl_Reader();
    default:
        throw std::exception("StepIgesModelReader::GetFileReader : Unknown file type");
    }
    return XSControl_Reader();
}
