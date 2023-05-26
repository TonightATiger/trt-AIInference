/*
 * Copyright (c) 1993-2021 NVIDIA Corporation. All rights reserved.
 *
 * NOTICE TO LICENSEE:
 *
 * This source code and/or documentation ("Licensed Deliverables") are
 * subject to NVIDIA intellectual property rights under U.S. and
 * international Copyright laws.
 *
 * These Licensed Deliverables contained herein is PROPRIETARY and
 * CONFIDENTIAL to NVIDIA and is being provided under the terms and
 * conditions of a form of NVIDIA software license agreement by and
 * between NVIDIA and Licensee ("License Agreement") or electronically
 * accepted by Licensee.  Notwithstanding any terms or conditions to
 * the contrary in the License Agreement, reproduction or disclosure
 * of the Licensed Deliverables to any third party without the express
 * written consent of NVIDIA is prohibited.
 *
 * NOTWITHSTANDING ANY TERMS OR CONDITIONS TO THE CONTRARY IN THE
 * LICENSE AGREEMENT, NVIDIA MAKES NO REPRESENTATION ABOUT THE
 * SUITABILITY OF THESE LICENSED DELIVERABLES FOR ANY PURPOSE.  IT IS
 * PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY OF ANY KIND.
 * NVIDIA DISCLAIMS ALL WARRANTIES WITH REGARD TO THESE LICENSED
 * DELIVERABLES, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY,
 * NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE.
 * NOTWITHSTANDING ANY TERMS OR CONDITIONS TO THE CONTRARY IN THE
 * LICENSE AGREEMENT, IN NO EVENT SHALL NVIDIA BE LIABLE FOR ANY
 * SPECIAL, INDIRECT, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, OR ANY
 * DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
 * ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
 * OF THESE LICENSED DELIVERABLES.
 *
 * U.S. Government End Users.  These Licensed Deliverables are a
 * "commercial item" as that term is defined at 48 C.F.R. 2.101 (OCT
 * 1995), consisting of "commercial computer software" and "commercial
 * computer software documentation" as such terms are used in 48
 * C.F.R. 12.212 (SEPT 1995) and is provided to the U.S. Government
 * only as a commercial end item.  Consistent with 48 C.F.R.12.212 and
 * 48 C.F.R. 227.7202-1 through 227.7202-4 (JUNE 1995), all
 * U.S. Government End Users acquire the Licensed Deliverables with
 * only those rights set forth herein.
 *
 * Any use of the Licensed Deliverables in individual and commercial
 * software must include, in the user documentation and internal
 * comments to the code, the above Disclaimer and U.S. Government End
 * Users Notice.
 */

#ifndef NV_INFER_CONSISTENCY_H
#define NV_INFER_CONSISTENCY_H

#include "NvInferConsistencyImpl.h"
#include "NvInferRuntimeCommon.h"

//!
//! \file NvInferConsistency.h
//!

namespace nvinfer1
{

namespace consistency
{

//!
//! \class IConsistencyChecker
//!
//! \brief Validates a serialized engine blob.
//!
//! \warning Do not inherit from this class, as doing so will break forward-compatibility of the API and ABI.
//!
class IConsistencyChecker
{
public:
    //!
    //! \brief Check that a blob that was input to createConsistencyChecker method represents a valid engine.
    //
    //! \return true if the original blob encoded an engine that belongs to valid engine domain with
    //! target capability EngineCapability::kSAFETY, false otherwise.
    //!
    bool validate() const noexcept
    {
        return mImpl->validate();
    }

    //!
    //! \brief De-allocates any internally allocated memory.
    //!
    virtual ~IConsistencyChecker() = default;

protected:
    apiv::VConsistencyChecker* mImpl;
    IConsistencyChecker() = default;
    IConsistencyChecker(const IConsistencyChecker& other) = delete;
    IConsistencyChecker& operator=(const IConsistencyChecker& other) = delete;
    IConsistencyChecker(IConsistencyChecker&& other) = delete;
    IConsistencyChecker& operator=(IConsistencyChecker&& other) = delete;
};

//!
//! \class IPluginChecker
//!
//! \brief Consistency Checker plugin class for user implemented Plugins.
//!
//! Plugins are a mechanism for applications to implement custom layers. It provides a
//! mechanism to register Consistency plugins and
//! look up the Plugin Registry during validate.
//!
//! Supported IPlugin inferfaces are limited to IPluginV2IOExt only.
//!
class IPluginChecker : public IPluginCreator
{
public:
    //!
    //! \brief Called during IConsistencyChecker::validate. Allows users to provide
    //! custom validation of serialized Plugin data. Returns boolean that indicates
    //! whether or not the Plugin passed validation.
    //!
    //! \param name The plugin name
    //! \param serialData The memory that holds the plugin serialized data.
    //! \param serialLength The size of the plugin serialized data.
    //! \param in The input tensors attributes.
    //! \param nbInputs The number of input tensors.
    //! \param out The output tensors attributes.
    //! \param nbOutputs The number of output tensors.
    //! \param workspaceSize The size of workspace provided during enqueue.
    //!
    virtual bool validate(const char* name, const void* serialData, size_t serialLength, const PluginTensorDesc* in,
        size_t nbInputs, const PluginTensorDesc* out, size_t nbOutputs, int64_t workspaceSize) const noexcept
        = 0;

    IPluginChecker() = default;
    virtual ~IPluginChecker() override = default;

protected:
    IPluginChecker(IPluginChecker const&) = default;
    IPluginChecker(IPluginChecker&&) = default;
    IPluginChecker& operator=(IPluginChecker const&) & = default;
    IPluginChecker& operator=(IPluginChecker&&) & = default;
};

} // namespace consistency

} // namespace nvinfer1

extern "C" TENSORRTAPI void* createConsistencyChecker_INTERNAL(void* logger, const void* blob, size_t size,
    int32_t version); //!< Internal C entry point for creating IConsistencyChecker.

namespace nvinfer1
{

namespace consistency
{
//!
//! \brief Create an instance of an IConsistencyChecker class.
//!
//! ILogger is the logging class for the consistency checker.
//!
//! anonymous namespace avoids linkage surprises when linking objects built with different versions of this header.
//!
namespace // anonymous
{

inline IConsistencyChecker* createConsistencyChecker(ILogger& logger, const void* blob, size_t size)
{
    return static_cast<IConsistencyChecker*>(
        createConsistencyChecker_INTERNAL(&logger, blob, size, NV_TENSORRT_VERSION));
}

} // namespace

} // namespace consistency

} // namespace nvinfer1

#endif // NV_INFER_CONSISTENCY_H
