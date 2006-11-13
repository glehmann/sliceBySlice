WRAP_CLASS("itk::SliceBySliceImageFilter" POINTER)
  FOREACH(d ${WRAP_ITK_DIMS})
    MATH(EXPR d_1 "${d} - 1")
    FILTER_DIMS(has_d_1 ${d_1})
    IF(has_d_1)
      WRAP_IMAGE_FILTER_ALL_TYPES(2 ${d})
    ENDIF(has_d_1)
  ENDFOREACH(d)
END_WRAP_CLASS()
