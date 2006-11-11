#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkCommand.h"
#include "itkSimpleFilterWatcher.h"
#include "itkGrayscaleFillholeImageFilter.h"
#include "itkMedianImageFilter.h"
#include "itkSliceBySliceImageFilter.h"


int main(int argc, char * argv[])
{

  if( argc != 3 )
    {
    std::cerr << "usage: " << argv[0] << " " << std::endl;
    // std::cerr << "  : " << std::endl;
    exit(1);
    }

  const int dim = 3;
  
  typedef unsigned char PType;
  typedef itk::Image< PType, dim > IType;
  typedef itk::Image< PType, dim-1 > IType2;

  typedef itk::ImageFileReader< IType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );

  typedef itk::MedianImageFilter< IType2, IType2 > FillHoleType;
//  typedef itk::GrayscaleFillHoleImageFilter< IType2, IType2 > FillHoleType;
  FillHoleType::Pointer fillHole = FillHoleType::New();

  typedef itk::SliceBySliceImageFilter< IType, IType, FillHoleType > FilterType;
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput( reader->GetOutput() );
  filter->SetFilter( fillHole );

  itk::SimpleFilterWatcher watcher(filter, "filter");

  typedef itk::ImageFileWriter< IType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( filter->GetOutput() );
  writer->SetFileName( argv[2] );
  writer->Update();

  return 0;
}

