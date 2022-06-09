#include "StdAfx.h"
#include "HalconCpp.h"
#include "HDevThread.h"
using namespace HalconCpp;


void GenRegionCross (HObject *ho_RegionUnion,HTuple hv_row, HTuple hv_col, HTuple hv_size);
void MeanReducedImage (HObject ho_ImageReduced, HObject *ho_ImageMean, HTuple hv_MaskWidth,HTuple hv_MaskHeight);
void WidthLine(HObject ho_Rectangle, HTuple *hv_Width, HTuple* hvLength);
void FoilDetect (HObject ho_Image, HObject ho_ImageMean, HObject ho_ImageRegion, 
	HObject ho_hoSumRegion, HObject *ho_ErrorRegion, HTupleVector/*{eTupleVector,Dim=1}*/ hvec_Param, 
	HTuple hv_PeakGray);
void MostGrayValue (HObject ho_Region, HObject ho_Image, HTuple *hv_PeakGray);
void SegmentRegion(HObject& ho_Coat, HObject& ho_Foil, HObject ho_AT9Region, HObject *ho_Mid,
	HObject *ho_Border,HObject* ho_EdgeDrop,HObject* ho_Corner, HTuple hv_dBorderWidth,HTuple hv_dEdgeWidth,HTuple hv_dCornerWidth);
void SelectAdjacentLine (HObject ho_Region, HObject ho_ObjectsConcat, HObject *ho_SelectedRegions);
void SelectLine (HObject ho_Rectangle, HObject *ho_ErrorRegion, HTuple hv_StandarWidth, 
	HTuple hv_StandarLength, HTuple *hv_hvWidth, HTuple *hv_hvLength);
void SelectSuitableLine (HObject ho_Rectangle, HObject *ho_ErrorRegion, HTuple hv_StandarWidthMin, 
	HTuple hv_StandarWidthMax, HTuple hv_StandarLength, HTuple *hv_hvWidth, HTuple *hv_hvLength);
