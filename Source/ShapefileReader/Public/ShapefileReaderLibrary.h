// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ogrsf_frmts.h"
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ShapefileReaderLibrary.generated.h"

class UShapefileReaderRecord;

UENUM(BlueprintType)
enum class EShapefileGeomType : uint8
{
	Unkonwn 				UMETA(DisplayName = " unknown type, non-standard "),         	
	Point 					UMETA(DisplayName = " 0-dimensional geometric object, standard WKB "),           
	LineString	 			UMETA(DisplayName = " 1-dimensional geometric object with linear interpolation between Points, standard WKB "),      		
	Polygon 				UMETA(DisplayName = " planar 2-dimensional geometric object defined by 1 exterior boundary and 0 or more interior boundaries, standard WKB "),         	
	MultiPoint 				UMETA(DisplayName = " GeometryCollection of Points, standard WKB "),      		
	MultiLineString 		UMETA(DisplayName = " GeometryCollection of LineStrings, standard WKB "), 			
	MultiPolygon 			UMETA(DisplayName = " GeometryCollection of Polygons, standard WKB "),    		
	GeometryCollection 		UMETA(DisplayName = " geometric object that is a collection of 1 or more geometric objects, standard WKB "), 			
		                    
	CircularString 			UMETA(DisplayName = " one or more circular arc segments connected end to end, ISO SQL/MM Part 3 "),  			
	CompoundCurve 			UMETA(DisplayName = " sequence of contiguous curves, ISO SQL/MM Part 3.  "),   		
	CurvePolygon 			UMETA(DisplayName = " planar surface, defined by 1 exterior boundary and zero or more interior boundaries, that are curves. ISO SQL/MM Part 3 "),   		
	MultiCurve 				UMETA(DisplayName = " GeometryCollection of Curves, ISO SQL/MM Part 3.  "),     		
	MultiSurface 			UMETA(DisplayName = " GeometryCollection of Surfaces, ISO SQL/MM Part 3.  "),   		
	Curve 					UMETA(DisplayName = " Curve (abstract type). ISO SQL/MM Part 3. GDAL &gt;= 2.1 "),          	
	Surface 				UMETA(DisplayName = " Surface (abstract type). ISO SQL/MM Part 3. GDAL &gt;= 2.1 "),        	
	PolyhedralSurface 		UMETA(DisplayName = " a contiguous collection of polygons, which share common boundary segments, ISO SQL/MM Part 3"),			
	TIN 					UMETA(DisplayName = " a PolyhedralSurface consisting only of Triangle patches ISO SQL/MM Part 3. GDAL &gt;= 2.3 "),           
	Triangle 				UMETA(DisplayName = " a Triangle. ISO SQL/MM Part 3. GDAL &gt;= 2.3 "),         	
	None 					UMETA(DisplayName = " non-standard, for pure attribute records "),          	
	LinearRing 				UMETA(DisplayName = " non-standard, just for createGeometry() "),    		
	CircularStringZ	 		UMETA(DisplayName = " wkbCircularString with Z component. ISO SQL/MM Part 3. "), 			
	CompoundCurveZ			UMETA(DisplayName = " wkbCompoundCurve with Z component. ISO SQL/MM Part 3. "),   		
	CurvePolygonZ 			UMETA(DisplayName = " wkbCurvePolygon with Z component. ISO SQL/MM Part 3. "),    		
	MultiCurveZ 			UMETA(DisplayName = " wkbMultiCurve with Z component. ISO SQL/MM Part 3. "),      		
	MultiSurfaceZ 			UMETA(DisplayName = " wkbMultiSurface with Z component. ISO SQL/MM Part 3. "),    		
	CurveZ					UMETA(DisplayName = " wkbCurve with Z component. ISO SQL/MM Part 3."),           
	SurfaceZ 				UMETA(DisplayName = " wkbSurface with Z component. ISO SQL/MM Part 3."),         	
	PolyhedralSurfaceZ 		UMETA(DisplayName = " ISO SQL/MM Part 3."),  			
	TINZ 					UMETA(DisplayName = " ISO SQL/MM Part 3."),           
	TriangleZ				UMETA(DisplayName = " ISO SQL/MM Part 3."),           

	PointM 					UMETA(DisplayName = " ISO SQL/MM Part 3."),           
	LineStringM 			UMETA(DisplayName = " ISO SQL/MM Part 3."),         	
	PolygonM 				UMETA(DisplayName = " ISO SQL/MM Part 3."),           
	MultiPointM				UMETA(DisplayName = " ISO SQL/MM Part 3."),         	
	MultiLineStringM 		UMETA(DisplayName = " ISO SQL/MM Part 3."),    		
	MultiPolygonM 			UMETA(DisplayName = " ISO SQL/MM Part 3."),      		
	GeometryCollectionM 	UMETA(DisplayName = " ISO SQL/MM Part 3."), 			
	CircularStringM 		UMETA(DisplayName = " ISO SQL/MM Part 3."),     		
	CompoundCurveM 			UMETA(DisplayName = " ISO SQL/MM Part 3."),      		
	CurvePolygonM 			UMETA(DisplayName = " ISO SQL/MM Part 3."),       	
	MultiCurveM 			UMETA(DisplayName = " ISO SQL/MM Part 3."),         	
	MultiSurfaceM 			UMETA(DisplayName = " ISO SQL/MM Part 3."),       	
	CurveM 					UMETA(DisplayName = " ISO SQL/MM Part 3."),           
	SurfaceM 				UMETA(DisplayName = " ISO SQL/MM Part 3."),           
	PolyhedralSurfaceM		UMETA(DisplayName = " ISO SQL/MM Part 3."),  			
	TINM 					UMETA(DisplayName = " ISO SQL/MM Part 3."),           
	TriangleM 				UMETA(DisplayName = " ISO SQL/MM Part 3."),           
	PointZM 				UMETA(DisplayName = " ISO SQL/MM Part 3."),           
	LineStringZM 			UMETA(DisplayName = " ISO SQL/MM Part 3."),         	
	PolygonZM 				UMETA(DisplayName = " ISO SQL/MM Part 3."),           
	MultiPointZM 			UMETA(DisplayName = " ISO SQL/MM Part 3."),         	
	MultiLineStringZM 		UMETA(DisplayName = " ISO SQL/MM Part 3."),    		
	MultiPolygonZM			UMETA(DisplayName = " ISO SQL/MM Part 3."),       	
	GeometryCollectionZM 	UMETA(DisplayName = " ISO SQL/MM Part 3."),	 		
	CircularStringZM 		UMETA(DisplayName = " ISO SQL/MM Part 3."),     		
	CompoundCurveZM 		UMETA(DisplayName = " ISO SQL/MM Part 3."),      		
	CurvePolygonZM 			UMETA(DisplayName = " ISO SQL/MM Part 3."),       	
	MultiCurveZM 			UMETA(DisplayName = " ISO SQL/MM Part 3."),         	
	MultiSurfaceZM 			UMETA(DisplayName = " ISO SQL/MM Part 3."),       	
	CurveZM 				UMETA(DisplayName = " ISO SQL/MM Part 3."),           
	SurfaceZM 				UMETA(DisplayName = " ISO SQL/MM Part 3."),           
	PolyhedralSurfaceZM		UMETA(DisplayName = " ISO SQL/MM Part 3."),  			
	TINZM 					UMETA(DisplayName = " ISO SQL/MM Part 3."),           
	TriangleZM				UMETA(DisplayName = " ISO SQL/MM Part 3."),           
	Point25D 				UMETA(DisplayName = " 2.5D extension as per 99-402 "), 			
	LineString25D			UMETA(DisplayName = " 2.5D extension as per 99-402 "), 			
	Polygon25D 				UMETA(DisplayName = " 2.5D extension as per 99-402 "), 			
	MultiPoint25D 			UMETA(DisplayName = " 2.5D extension as per 99-402 "), 			
	MultiLineString25D 		UMETA(DisplayName = " 2.5D extension as per 99-402 "), 			
	MultiPolygon25D			UMETA(DisplayName = " 2.5D extension as per 99-402 "), 			
	GeometryCollection25D	UMETA(DisplayName = " 2.5D extension as per 99-402 "),  			
};

UENUM(BlueprintType)
enum class EShapefileFieldType : uint8
{
	Integer = 0			UMETA(DisplayName = "Simple 32bit integer")					,
	IntegerList = 1		UMETA(DisplayName = "List of 32bit integers")				,
	Real = 2			UMETA(DisplayName = "Double Precision floating point")      ,
	RealList = 3		UMETA(DisplayName = "List of doubles")						,
	String = 4			UMETA(DisplayName = "String of ASCII chars")				,
	StringList = 5		UMETA(DisplayName = "Array of strings")						,
	WideString = 6		UMETA(DisplayName = "deprecated")							,
	WideStringList = 7	UMETA(DisplayName = "deprecated")							,
	Binary = 8			UMETA(DisplayName = "Raw Binary data")						,
	Date = 9			UMETA(DisplayName = "Date")									,
	Time = 10			UMETA(DisplayName = "Time")									,
	DateTime = 11		UMETA(DisplayName = "Date and Time")						,
	Integer64 = 12		UMETA(DisplayName = "Single 64bit integer")					,
	Integer64List = 13	UMETA(DisplayName = "List of 64bit integers")				,
	MaxType = 14		UMETA(Hidden, DisplayName = "None")									
};

UCLASS(BlueprintType)
class UShapefileFieldInfo: public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, Category = "ShapefileReader")
	FString Names;
	UPROPERTY(BlueprintReadOnly, Category = "ShapefileReader")
	EShapefileFieldType FieldType;
	UPROPERTY(BlueprintReadOnly, Category = "ShapefileReader")
	int Width;
	UPROPERTY(BlueprintReadOnly, Category = "ShapefileReader")
	int Precision;
};

UCLASS(BlueprintType)
class UShapefileFeature: public UObject
{
	GENERATED_BODY()
	friend class UShapefileReaderRecord;
public:
	UShapefileFeature();
	UPROPERTY(BlueprintReadOnly, Category = "ShapefileReader")
	FVector Center;
	UPROPERTY(BlueprintReadOnly, Category = "ShapefileReader")
	EShapefileGeomType GeomType;
	UPROPERTY(BlueprintReadOnly, Category = "ShapefileReader")
	FVector UpRight;
	UPROPERTY(BlueprintReadOnly, Category = "ShapefileReader")
	FVector LeftDown;
	
	UFUNCTION(BlueprintCallable, Category = "ShapefileReader")
	FString GetFieldAsString(const FString& FieldName) const;
	UFUNCTION(BlueprintCallable, Category = "ShapefileReader")
	float GetFieldAsDouble(const FString& FieldName) const;
	UFUNCTION(BlueprintCallable, Category = "ShapefileReader")
	int GetFieldAsInteger(const FString& FieldName) const;

	OGRGeometry* GetGeometryRef() const noexcept { return Feature_->GetGeometryRef(); }
	OGRFeature*  GetOGRFeature() const noexcept { return Feature_.Get(); }
private:
	void SetFeature(OGRFeature* Feature) noexcept;
	EShapefileGeomType ConvertOGRGeomType(OGRwkbGeometryType Type) const noexcept; 
	struct ogrfeature_deleter
	{
		void operator()(OGRFeature* p) const
		{
			OGRFeature::DestroyFeature(p); 
		}
	};
	
	TUniquePtr<OGRFeature, ogrfeature_deleter> Feature_;
};


UCLASS(BlueprintType)
class SHAPEFILEREADER_API UShapefileReaderRecord : public UObject
{
	GENERATED_BODY()
public:
	UShapefileReaderRecord();
	
	UPROPERTY(BlueprintReadOnly, Category = "ShapefileReader")
	int EntitiesCount = 0;

	UPROPERTY(BlueprintReadOnly, Category = "ShapefileReader")
	int FieldCount = 0;
	
	UPROPERTY(BlueprintReadOnly, Category = "ShapefileReader")
	FString LayerName;

	UFUNCTION(BlueprintCallable, Category = "ShapefileReader")
	TArray<UShapefileFieldInfo*> GetFieldInfos() const;

	UFUNCTION(BlueprintCallable, Category = "ShapefileReader")
	UShapefileFeature* GetShapefileFeature(int index) const;
	
	void SetDataset(GDALDataset* Dataset) noexcept;
private:
	struct gdaldataset_deleter
	{
		void operator()(GDALDataset* p) const
		{
			GDALClose(p);
		}
	};
	
	TUniquePtr<GDALDataset, gdaldataset_deleter> Dataset_;
	OGRLayer* Layer_;
};

/**
 * 
 */
UCLASS()
class SHAPEFILEREADER_API UShapefileReaderLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	UFUNCTION(BlueprintCallable,  Category = "ShapefileLib")
	static UShapefileReaderRecord* ReadShapefile(const FString& Path);
};
