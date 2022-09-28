// Fill out your copyright notice in the Description page of Project Settings.


#include "ShapefileReaderLibrary.h"

UShapefileReaderRecord::UShapefileReaderRecord(): Dataset_(nullptr), Layer_(nullptr)
{
}

void UShapefileReaderRecord::SetDataset(GDALDataset* Dataset) noexcept
{
	Dataset_.Reset(Dataset);
	Layer_ = Dataset_->GetLayer(0);
	OGRFeatureDefn* LayerDef = Layer_->GetLayerDefn();
	FieldCount = LayerDef->GetFieldCount();
}

TArray<UShapefileFieldInfo*> UShapefileReaderRecord::GetFieldInfos() const
{
	TArray<UShapefileFieldInfo*> FieldInfos;
	FieldInfos.Empty();
	for(int i = 0; i < FieldCount; i++)
	{
		OGRFieldDefn* FieldDef = Layer_->GetLayerDefn()->GetFieldDefn(i);
		UShapefileFieldInfo* FieldInfo = NewObject<UShapefileFieldInfo>();
		FieldInfo->Names = UTF8_TO_TCHAR(FieldDef->GetNameRef());
		FieldInfo->Precision = FieldDef->GetPrecision();
		FieldInfo->Width = FieldDef->GetWidth();
		FieldInfo->FieldType = StaticCast<EShapefileFieldType>(static_cast<int>(FieldDef->GetType()));
		FieldInfos.Add(FieldInfo);
	}
	return FieldInfos;
}

UShapefileFeature* UShapefileReaderRecord::GetShapefileFeature(int index) const
{
	OGRFeature* feature = Layer_->GetFeature(index);
	UShapefileFeature* record = NewObject<UShapefileFeature>();
	record->SetFeature(feature);
	return record;
}

UShapefileFeature::UShapefileFeature(): Feature_(nullptr)
{
}

FString UShapefileFeature::GetFieldAsString(const FString& FieldName) const
{
	return UTF8_TO_TCHAR(
		Feature_->GetFieldAsString(
			TCHAR_TO_UTF8(*FieldName))
		);
}

float UShapefileFeature::GetFieldAsDouble(const FString& FieldName) const{
	return Feature_->GetFieldAsDouble(TCHAR_TO_UTF8(*FieldName));
}

int UShapefileFeature::GetFieldAsInteger(const FString& FieldName) const{
	return Feature_->GetFieldAsInteger(TCHAR_TO_UTF8(*FieldName));
}

void UShapefileFeature::SetFeature(OGRFeature* Feature) noexcept
{
	Feature_.Reset(Feature);

	OGRGeometry* geom = Feature_->GetGeometryRef();
	// update geom type
	GeomType = ConvertOGRGeomType(geom->getGeometryType());

	// update center
	OGRPoint center;
	geom->Centroid(&center);
	Center.X = center.getX();
	Center.Y = center.getY();
	Center.Z = center.getZ();

	// update envelope
	OGREnvelope  envelope;
	geom->getEnvelope(&envelope);
	UpRight.X = envelope.MaxX;
	UpRight.Y = envelope.MaxY;
	UpRight.Z = 0;
	LeftDown.X = envelope.MinX;
	LeftDown.Y = envelope.MinY;
	LeftDown.Z = 0;
}

EShapefileGeomType UShapefileFeature::ConvertOGRGeomType(OGRwkbGeometryType Type) const noexcept
{
	EShapefileGeomType geomtype = EShapefileGeomType::Unkonwn;
	switch (Type)
	{
	case wkbPoint 					: geomtype = EShapefileGeomType::Point 					;break;
	case wkbLineString	 			: geomtype = EShapefileGeomType::LineString	 			;break;
	case wkbPolygon 				: geomtype = EShapefileGeomType::Polygon 				;break;
	case wkbMultiPoint 				: geomtype = EShapefileGeomType::MultiPoint 			;break;
	case wkbMultiLineString 		: geomtype = EShapefileGeomType::MultiLineString		;break;
	case wkbMultiPolygon 			: geomtype = EShapefileGeomType::MultiPolygon 	 		;break;
	case wkbGeometryCollection 		: geomtype = EShapefileGeomType::GeometryCollection 	;break;
	case wkbCircularString 			: geomtype = EShapefileGeomType::CircularString 		;break;
	case wkbCompoundCurve 			: geomtype = EShapefileGeomType::CompoundCurve 			;break;
	case wkbCurvePolygon 			: geomtype = EShapefileGeomType::CurvePolygon 			;break;
	case wkbMultiCurve 				: geomtype = EShapefileGeomType::MultiCurve 			;break;
	case wkbMultiSurface 			: geomtype = EShapefileGeomType::MultiSurface 			;break;
	case wkbCurve 					: geomtype = EShapefileGeomType::Curve 					;break;
	case wkbSurface 				: geomtype = EShapefileGeomType::Surface 				;break;
	case wkbPolyhedralSurface 		: geomtype = EShapefileGeomType::PolyhedralSurface 		;break;
	case wkbTIN 					: geomtype = EShapefileGeomType::TIN 					;break;
	case wkbTriangle 				: geomtype = EShapefileGeomType::Triangle 				;break;
	case wkbNone 					: geomtype = EShapefileGeomType::None 					;break;
	case wkbLinearRing 				: geomtype = EShapefileGeomType::LinearRing 			;break;
	case wkbCircularStringZ	 		: geomtype = EShapefileGeomType::CircularStringZ	 	;break;
	case wkbCompoundCurveZ			: geomtype = EShapefileGeomType::CompoundCurveZ			;break;
	case wkbCurvePolygonZ 			: geomtype = EShapefileGeomType::CurvePolygonZ 			;break;
	case wkbMultiCurveZ 			: geomtype = EShapefileGeomType::MultiCurveZ 			;break;
	case wkbMultiSurfaceZ 			: geomtype = EShapefileGeomType::MultiSurfaceZ 			;break;
	case wkbCurveZ					: geomtype = EShapefileGeomType::CurveZ					;break;
	case wkbSurfaceZ 				: geomtype = EShapefileGeomType::SurfaceZ 				;break;
	case wkbPolyhedralSurfaceZ 		: geomtype = EShapefileGeomType::PolyhedralSurfaceZ 	;break;
	case wkbTINZ 					: geomtype = EShapefileGeomType::TINZ 					;break;
	case wkbTriangleZ				: geomtype = EShapefileGeomType::TriangleZ				;break;
	case wkbPointM 					: geomtype = EShapefileGeomType::PointM 				;break;
	case wkbLineStringM 			: geomtype = EShapefileGeomType::LineStringM 			;break;
	case wkbPolygonM 				: geomtype = EShapefileGeomType::PolygonM 				;break;
	case wkbMultiPointM				: geomtype = EShapefileGeomType::MultiPointM			;break;
	case wkbMultiLineStringM 		: geomtype = EShapefileGeomType::MultiLineStringM 		;break;
	case wkbMultiPolygonM 			: geomtype = EShapefileGeomType::MultiPolygonM 			;break;
	case wkbGeometryCollectionM 	: geomtype = EShapefileGeomType::GeometryCollectionM 	;break;
	case wkbCircularStringM 		: geomtype = EShapefileGeomType::CircularStringM 		;break;
	case wkbCompoundCurveM 			: geomtype = EShapefileGeomType::CompoundCurveM 		;break;
	case wkbCurvePolygonM 			: geomtype = EShapefileGeomType::CurvePolygonM 			;break;
	case wkbMultiCurveM 			: geomtype = EShapefileGeomType::MultiCurveM 			;break;
	case wkbMultiSurfaceM 			: geomtype = EShapefileGeomType::MultiSurfaceM 			;break;
	case wkbCurveM 					: geomtype = EShapefileGeomType::CurveM 				;break;
	case wkbSurfaceM 				: geomtype = EShapefileGeomType::SurfaceM 				;break;
	case wkbPolyhedralSurfaceM		: geomtype = EShapefileGeomType::PolyhedralSurfaceM		;break;
	case wkbTINM 					: geomtype = EShapefileGeomType::TINM 					;break;  
	case wkbTriangleM 				: geomtype = EShapefileGeomType::TriangleM 				;break;  
	case wkbPointZM 				: geomtype = EShapefileGeomType::PointZM 				;break;  
	case wkbLineStringZM 			: geomtype = EShapefileGeomType::LineStringZM 			;break;	
	case wkbPolygonZM 				: geomtype = EShapefileGeomType::PolygonZM 				;break;  
	case wkbMultiPointZM 			: geomtype = EShapefileGeomType::MultiPointZM 			;break;	
	case wkbMultiLineStringZM 		: geomtype = EShapefileGeomType::MultiLineStringZM 		;break;
	case wkbMultiPolygonZM			: geomtype = EShapefileGeomType::MultiPolygonZM			;break;
	case wkbGeometryCollectionZM 	: geomtype = EShapefileGeomType::GeometryCollectionZM 	;break;
	case wkbCircularStringZM 		: geomtype = EShapefileGeomType::CircularStringZM 		;break;	
	case wkbCompoundCurveZM 		: geomtype = EShapefileGeomType::CompoundCurveZM 		;break;	
	case wkbCurvePolygonZM 			: geomtype = EShapefileGeomType::CurvePolygonZM 		;break;
	case wkbMultiCurveZM 			: geomtype = EShapefileGeomType::MultiCurveZM 			;break;	
	case wkbMultiSurfaceZM 			: geomtype = EShapefileGeomType::MultiSurfaceZM 		;break;
	case wkbCurveZM 				: geomtype = EShapefileGeomType::CurveZM 				;break;  
	case wkbSurfaceZM 				: geomtype = EShapefileGeomType::SurfaceZM 				;break;  
	case wkbPolyhedralSurfaceZM		: geomtype = EShapefileGeomType::PolyhedralSurfaceZM	;break;	
	case wkbTINZM 					: geomtype = EShapefileGeomType::TINZM 					;break;  
	case wkbTriangleZM				: geomtype = EShapefileGeomType::TriangleZM				;break;  
	case wkbPoint25D 				: geomtype = EShapefileGeomType::Point25D 				;break;
	case wkbLineString25D			: geomtype = EShapefileGeomType::LineString25D			;break;
	case wkbPolygon25D 				: geomtype = EShapefileGeomType::Polygon25D 			;break;
	case wkbMultiPoint25D 			: geomtype = EShapefileGeomType::MultiPoint25D 			;break;
	case wkbMultiLineString25D 		: geomtype = EShapefileGeomType::MultiLineString25D 	;break;
	case wkbMultiPolygon25D			: geomtype = EShapefileGeomType::MultiPolygon25D		;break;
	case wkbGeometryCollection25D	: geomtype = EShapefileGeomType::GeometryCollection25D	;break;
	default							: geomtype = EShapefileGeomType::Unkonwn				;break;	
	}
	return geomtype;
}
UShapefileReaderRecord* UShapefileReaderLibrary::ReadShapefile(const FString& Path)
{
	GDALDataset* dataset = static_cast<GDALDataset*>(GDALOpenEx(TCHAR_TO_UTF8(*Path), GDAL_OF_VECTOR,
		nullptr, nullptr, nullptr));
	if(dataset == nullptr)
		return nullptr;

	UShapefileReaderRecord* record = NewObject<UShapefileReaderRecord>();
	record->SetDataset(dataset);
	return record;
}