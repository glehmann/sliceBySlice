#ifndef __itkSliceBySliceImageFilter_h
#define __itkSliceBySliceImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkCastImageFilter.h"


namespace itk {

/**
 * \class SliceBySliceImageFilter
 * \brief
 *
 *
 * \author Gaetan Lehmann
 */

template<class TInputImage, class TOutputImage, class TFilter>
//=ImageToImageFilter< Image< typename TInputImage::PixelType, TInputImage::ImageDimension - 1 >,  Image< typename TOutputImage::PixelType, TOutputImage::ImageDimension - 1 > > >
class ITK_EXPORT SliceBySliceImageFilter : 
public ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef SliceBySliceImageFilter Self;
  typedef ImageToImageFilter<TInputImage,TOutputImage>  Superclass;
  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;
  
  /** Standard New method. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(SliceBySliceImageFilter,
               ImageToImageFilter);
 
  /** Image related typedefs. */
  typedef TInputImage InputImageType;
  typedef typename TInputImage::RegionType RegionType ;
  typedef typename TInputImage::SizeType SizeType ;
  typedef typename TInputImage::IndexType IndexType ;
  typedef typename TInputImage::PixelType PixelType ;
  typedef typename TInputImage::OffsetType OffsetType ;
  
  typedef TOutputImage OutputImageType;
  typedef typename TOutputImage::PixelType OutputPixelType ;

  typedef TFilter FilterType ;
  
  typedef typename FilterType::InputImageType InternalInputImageType;
  typedef typename InternalInputImageType::RegionType InternalRegionType ;
  typedef typename InternalInputImageType::SizeType InternalSizeType ;
  typedef typename InternalInputImageType::IndexType InternalIndexType ;
  typedef typename InternalInputImageType::OffsetType InternalOffsetType ;
  typedef typename InternalInputImageType::PixelType InternalInputPixelType ;

  typedef typename FilterType::OutputImageType InternalOutputImageType;
  typedef typename InternalOutputImageType::PixelType InternalOutputPixelType ;

  /** Image related typedefs. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TInputImage::ImageDimension);

  itkStaticConstMacro(InternalImageDimension, unsigned int,
                      InternalInputImageType::ImageDimension);

  itkSetMacro(Dimension, unsigned int);
  itkGetMacro(Dimension, unsigned int);

  itkSetObjectMacro(Filter, FilterType);
  itkGetObjectMacro(Filter, FilterType);

  void GenerateInputRequestedRegion() ;
  void EnlargeOutputRequestedRegion(DataObject *itkNotUsed(output));

  virtual void Modified() const;

protected:
  SliceBySliceImageFilter();
  ~SliceBySliceImageFilter() {};

  void GenerateData();

  void PrintSelf(std::ostream& os, Indent indent) const;

  typename FilterType::Pointer m_Filter;

private:
  SliceBySliceImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  unsigned int m_Dimension;
};

}

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkSliceBySliceImageFilter.txx"
#endif

#endif
