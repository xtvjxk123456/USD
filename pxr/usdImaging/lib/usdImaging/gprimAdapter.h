//
// Copyright 2016 Pixar
//
// Licensed under the Apache License, Version 2.0 (the "Apache License")
// with the following modification; you may not use this file except in
// compliance with the Apache License and the following modification to it:
// Section 6. Trademarks. is deleted and replaced with:
//
// 6. Trademarks. This License does not grant permission to use the trade
//    names, trademarks, service marks, or product names of the Licensor
//    and its affiliates, except as required to comply with Section 4(c) of
//    the License and to reproduce the content of the NOTICE file.
//
// You may obtain a copy of the Apache License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the Apache License with the above modification is
// distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied. See the Apache License for the specific
// language governing permissions and limitations under the Apache License.
//
#ifndef USDIMAGING_GPRIM_ADAPTER_H
#define USDIMAGING_GPRIM_ADAPTER_H

/// \file usdImaging/gprimAdapter.h

#include "pxr/pxr.h"
#include "pxr/usdImaging/usdImaging/api.h"
#include "pxr/usdImaging/usdImaging/primAdapter.h"

#include "pxr/usd/usdGeom/xformCache.h"

PXR_NAMESPACE_OPEN_SCOPE


class UsdGeomGprim;

/// \class UsdImagingGprimAdapter
///
/// Delegate support for UsdGeomGrims.
///
/// This adapter is provided as a base class for all adapters that want basic
/// Gprim data support, such as visibility, doubleSided, extent, displayColor,
/// purpose, and transform.
///
class UsdImagingGprimAdapter : public UsdImagingPrimAdapter {
public:
    typedef UsdImagingPrimAdapter BaseAdapter;

    UsdImagingGprimAdapter()
        : UsdImagingPrimAdapter()
    {}

    USDIMAGING_API
    virtual ~UsdImagingGprimAdapter();

    // ---------------------------------------------------------------------- //
    /// \name Parallel Setup and Resolve
    // ---------------------------------------------------------------------- //

    /// Thread Safe.
    USDIMAGING_API
    virtual void TrackVariability(UsdPrim const& prim,
                                  SdfPath const& cachePath,
                                  HdDirtyBits* timeVaryingBits,
                                  UsdImagingInstancerContext const* 
                                      instancerContext = NULL) const override;

    /// Thread Safe.
    USDIMAGING_API
    virtual void UpdateForTime(UsdPrim const& prim,
                               SdfPath const& cachePath, 
                               UsdTimeCode time,
                               HdDirtyBits requestedBits,
                               UsdImagingInstancerContext const* 
                                   instancerContext = NULL) const override;

    // ---------------------------------------------------------------------- //
    /// \name Change Processing 
    // ---------------------------------------------------------------------- //

    USDIMAGING_API
    virtual HdDirtyBits ProcessPropertyChange(UsdPrim const& prim,
                                              SdfPath const& cachePath,
                                              TfToken const& property) override;


    virtual void MarkDirty(UsdPrim const& prim,
                           SdfPath const& cachePath,
                           HdDirtyBits dirty,
                           UsdImagingIndexProxy* index) override;

    virtual void MarkRefineLevelDirty(UsdPrim const& prim,
                                      SdfPath const& cachePath,
                                      UsdImagingIndexProxy* index) override;

    virtual void MarkReprDirty(UsdPrim const& prim,
                               SdfPath const& cachePath,
                               UsdImagingIndexProxy* index) override;

    virtual void MarkCullStyleDirty(UsdPrim const& prim,
                                    SdfPath const& cachePath,
                                    UsdImagingIndexProxy* index) override;

    virtual void MarkTransformDirty(UsdPrim const& prim,
                                    SdfPath const& cachePath,
                                    UsdImagingIndexProxy* index) override;

    virtual void MarkVisibilityDirty(UsdPrim const& prim,
                                     SdfPath const& cachePath,
                                     UsdImagingIndexProxy* index) override;


    /// Returns color, opacity, and Usd interpolation token for a given
    /// prim, taking into account surface shader colors and explicitly
    /// authored color on the prim.
    USDIMAGING_API
    static VtValue GetColorAndOpacity(UsdPrim const& prim, 
                                      UsdTimeCode time,
                                      TfToken *interpolation);

    // Helper function: add a given type of rprim, potentially with instancer
    // name mangling, and add any bound shader.
    USDIMAGING_API
    static SdfPath _AddRprim(TfToken const& primType,
                      UsdPrim const& usdPrim,
                      UsdImagingIndexProxy* index,
                      SdfPath const& materialId,
                      UsdImagingInstancerContext const* instancerContext);

    // Helper function: apply gprim name mangling.
    USDIMAGING_API
    static SdfPath _ResolveCachePath(SdfPath const& cachePath,
            UsdImagingInstancerContext const* instancerContext);

protected:

    USDIMAGING_API
    virtual void _RemovePrim(SdfPath const& cachePath,
                             UsdImagingIndexProxy* index) override;

    // Give derived classes an opportunity to block GprimAdapter processing
    // of certain primvars.
    USDIMAGING_API
    virtual bool _IsBuiltinPrimvar(TfToken const& primvarName) const;

    // Helper method for the _DiscoverPrimvars methods above.
    void _ComputeAndMergePrimvar(UsdGeomGprim const& gprim,
                           SdfPath const& cachePath,
                           UsdGeomPrimvar const& primvar,
                           UsdTimeCode time,
                           UsdImagingValueCache* valueCache) const;

    // Conversion functions between usd and hydra enums.
    USDIMAGING_API
    static HdInterpolation _UsdToHdInterpolation(TfToken const& usdInterp);
    USDIMAGING_API
    static TfToken _UsdToHdRole(TfToken const& usdRole);

private:

    /// Reads the extent from the given prim. If the extent is not authored,
    /// an empty GfRange3d is returned, the extent will not be computed.
    GfRange3d _GetExtent(UsdPrim const& prim, UsdTimeCode time) const;

    /// Returns the doubleSided state for a given prim.
    bool _GetDoubleSided(UsdPrim const& prim) const;

    /// Returns the UsdGeomImagable "purpose" for this prim, including any
    /// inherited purpose. Inherited values are strongest.
    TfToken _GetPurpose(UsdPrim const & prim, UsdTimeCode time) const;
};


PXR_NAMESPACE_CLOSE_SCOPE

#endif //USDIMAGING_GPRIM_ADAPTER_H
