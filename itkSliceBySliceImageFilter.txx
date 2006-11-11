#ifndef __itkSliceBySliceImageFilter_txx
#define __itkSliceBySliceImageFilter_txx

#include "itkSliceBySliceImageFilter.h"
#include "itkProgressAccumulator.h"

namespace itk {

template <class TInputImage, class TOutputImage, class TFilter>
SliceBySliceImageFilter<TInputImage, TOutputImage, TFilter>
::SliceBySliceImageFilter()
{
  m_Filter = NULL;
  m_Dimension = ImageDimension - 1;
}


template<class TInputImage, class TOutputImage, class TFilter>
void
SliceBySliceImageFilter<TInputImage, TOutputImage, TFilter>
::GenerateInputRequestedRegion()
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();

  typename InputImageType::Pointer  inputPtr =
    const_cast< InputImageType * >( this->GetInput() );

  if ( !inputPtr )
    { return; }

  inputPtr->SetRequestedRegion(inputPtr->GetLargestPossibleRegion());
}


template<class TInputImage, class TOutputImage, class TFilter>
void
SliceBySliceImageFilter<TInputImage, TOutputImage, TFilter>
::EnlargeOutputRequestedRegion(DataObject *)
{
  this->GetOutput()->SetRequestedRegion( this->GetOutput()->GetLargestPossibleRegion() );
}


template <class TInputImage, class TOutputImage, class TFilter>
void
SliceBySliceImageFilter<TInputImage, TOutputImage, TFilter>
::GenerateData()
{
  if( !m_Filter )
    {
    itkExceptionMacro("Filter must be set.");
    }

  this->AllocateOutputs();

  RegionType requestedRegion = this->GetOutput()->GetRequestedRegion();
  IndexType requestedIndex = requestedRegion.GetIndex();
  SizeType requestedSize = requestedRegion.GetSize();

  InternalRegionType internalRegion;
  InternalSizeType internalSize;
  InternalIndexType internalIndex;

  for( int i=0; i<InternalImageDimension; i++ )
    {
    if(i!=m_Dimension)
      {
      internalSize[i] = requestedSize[i];
      internalIndex[i] = requestedIndex[i];
      }
    else
      {
      internalSize[i] = requestedSize[ ImageDimension - 1 ];
      internalIndex[i] = requestedIndex[ ImageDimension - 1 ];
      }
    }
  internalRegion.SetSize( internalSize );
  internalRegion.SetIndex( internalIndex );

  typename InternalInputImageType::Pointer internalInput = InternalInputImageType::New();
  internalInput->SetRegions( internalRegion );
  internalInput->Allocate();
  
  m_Filter->SetInput( internalInput );

  typename InputImageType::ConstPointer input = this->GetInput();
  typename OutputImageType::Pointer output = this->GetOutput();

  for( int slice=requestedIndex[m_Dimension]; slice < requestedSize[m_Dimension] - requestedIndex[m_Dimension]; slice++ )
    {
    // copy the current slice to the input image
    ImageRegionIterator< InternalInputImageType > iIt( internalInput, internalRegion );
    for( iIt.Begin(); iIt.IsAtEnd(); ++iIt )
      {
      IndexType idx;
      const InternalIndexType & iidx = iIt.GetIndex();
      for( int i=0; i<InternalImageDimension; i++ )
        {
        if( i >= m_Dimension )
          {
          idx[i+1] = iidx[i];
          }
        else
          {
          idx[i] = iidx[i];
          }
        }
        idx[ m_Dimension ] = slice;
      iIt.Set( input->GetPixel( idx ) );
      }

    // run the filter on the current slice
    m_Filter->UpdateLargestPossibleRegion();
    
    // and copy the output slice to the output image
    ImageRegionConstIterator< InternalOutputImageType > oIt( m_Filter->GetOutput(), internalRegion );
    for( oIt.Begin(); oIt.IsAtEnd(); ++oIt )
      {
      IndexType idx;
      const InternalIndexType & iidx = oIt.GetIndex();
      for( int i=0; i<InternalImageDimension; i++ )
        {
        if( i >= m_Dimension )
          {
          idx[i+1] = iidx[i];
          }
        else
          {
          idx[i] = iidx[i];
          }
        }
        idx[ m_Dimension ] = slice;
      output->SetPixel( idx, oIt.Get() );
      }
    }
}


template <class TInputImage, class TOutputImage, class TFilter>
void
SliceBySliceImageFilter<TInputImage, TOutputImage, TFilter>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Dimension: " << m_Dimension << std::endl;
}

}


#endif
