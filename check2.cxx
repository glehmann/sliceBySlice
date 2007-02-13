#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkSimpleFilterWatcher.h"

#include "itkAddImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkSliceBySliceImageFilter.h"


int main(int argc, char * argv[])
{

  if( argc != 3 )
    {
    std::cerr << "usage: " << argv[0] << " input output" << std::endl;
    exit(1);
    }

  const int dim = 3;
  
  typedef unsigned char PType;
  typedef itk::Image< PType, dim > IType;

  typedef itk::Image< unsigned short, dim-1 > US2Type;
  typedef itk::Image< unsigned char, dim-1 > UC2Type;

  typedef itk::ImageFileReader< IType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );

  typedef itk::AddImageFilter< UC2Type, UC2Type, US2Type > AddType;
  AddType::Pointer add = AddType::New();
  
  typedef itk::RescaleIntensityImageFilter< US2Type, UC2Type > RescaleType;
  RescaleType::Pointer rescale = RescaleType::New();
  rescale->SetInput( add->GetOutput() );

  typedef itk::SliceBySliceImageFilter< IType, IType, AddType, RescaleType > FilterType;
  FilterType::Pointer filter = FilterType::New();

  filter->SetInput( 0, reader->GetOutput() );
  filter->SetInput( 1, reader->GetOutput() );

  try
    {
    filter->Update();
    // an exception must be sent
    std::cerr << "No exception about missing input or output filter" << std::endl;
    return 1;
    }
  catch(...)
    {
    }

  try
    {
    filter->SetFilter( add );
    // an exception must be sent
    std::cerr << "No exception about wrong output filter type" << std::endl;
    return 1;
    }
  catch(...)
    {
    }

  filter->SetInputFilter( add );
  filter->SetOutputFilter( rescale );

  itk::SimpleFilterWatcher watcher(filter, "filter");

  typedef itk::ImageFileWriter< IType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( filter->GetOutput() );
  writer->SetFileName( argv[2] );
  writer->Update();

  return 0;
}

